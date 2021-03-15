#include "stdafx.h"
#include "market_data/alpaca_market_data.h"
#include "date/date.h"
#include "trade.h"

using namespace alpaca;

Response<std::vector<Bar>> AlpacaMarketData::getBarsV1(
    const std::string& symbol,
    const __time32_t start,
    const __time32_t end,
    const int nTickMinutes,
    const uint32_t limit) const {

    // Alpaca only suport 1Min, 5Min, 15Min and 1D bars

    std::string timeframe = "1Min";
    bool standard = false;
    switch (nTickMinutes) {
    case 1:
        standard = true;
        break;
    case 5:
        timeframe = "5Min";
        standard = true;
        break;
    case 15:
        timeframe = "15Min";
        standard = true;
        break;
    case 1440:
        timeframe = "1D";
        standard = true;
        break;
    default:
        break;
    }

    std::string sStart;
    std::string sEnd;
    {
        using namespace date;
        try {
            if (start) {
                sStart = format("%FT%T", date::sys_seconds{ std::chrono::seconds{start} });
                sStart.append("-00:00");  // add UTC timezone offset
            }

            if (end) {
                sEnd = format("%FT%T", date::sys_seconds{ std::chrono::seconds{end} });
                sEnd.append("-00:00");  // add UTC timezone offset
            }
        }
        catch (const std::exception& e) {
            assert(false);
        }
    }

    if (standard) {

        std::stringstream url;
        url << baseUrl_ << "/v1/bars/" << timeframe << "?symbols=" << symbol
            << "&limit=" << limit << (sStart.empty() ? "" : "&start=" + sStart)
            << (sEnd.empty() ? "" : "&end=" + sEnd);

        return request<std::vector<Bar>, AlpacaMarketData>(url.str(), headers_.c_str(), nullptr, &logger_);
    }


    uint32_t n = 0;
    uint32_t span = 1;
    if ((nTickMinutes % 1440) == 0) {
        n = nTickMinutes / 1440;
        timeframe = "1D";
        span = 1440;
    }
    else if ((nTickMinutes % 15) == 0) {
        n = nTickMinutes / 15;
        timeframe = "15Min";
        span = 15;
    }
    else if ((nTickMinutes % 5) == 0) {
        n = nTickMinutes / 5;
        timeframe = "5Min";
        span = 5;
    }
    else {
        n = nTickMinutes;
        timeframe + "1Min";
    }

    uint32_t nLimit = n * limit;
    uint32_t i = 0;
    Response<std::vector<Bar>> response;
    Response<std::vector<Bar>> retrieved;
    Bar rtBar;
    while (nLimit) {
        if (nLimit <= 1000) {
            std::stringstream url;
            url << baseUrl_ << "/v1/bars/" << timeframe << "?symbols=" << symbol
                << "&limit=" << nLimit << (sStart.empty() ? "" : "&start=" + sStart)
                << (sEnd.empty() ? "" : "&end=" + sEnd);

            logger_.logDebug("--> %s\n", url.str().c_str());
            retrieved = request<std::vector<Bar>, AlpacaMarketData>(url.str(), headers_.c_str(), nullptr, &logger_);
            logger_.logDebug("%d bars downloaded\n", retrieved.content().size());
            if (!retrieved) {
                retrieved.content().swap(response.content());
                return retrieved;
            }
            nLimit = 0;
        }
        else {
            // limit exceeds 1000, need to download multiple times

            std::stringstream url;
            url << baseUrl_ << "/v1/bars/" << timeframe << "?symbols=" << symbol
                << "&limit=" << 1000 << (sStart.empty() ? "" : "&start=" + sStart)
                << (sEnd.empty() ? "" : "&end=" + sEnd);

            logger_.logDebug("--> %s\n", url.str().c_str());
            retrieved = request<std::vector<Bar>, AlpacaMarketData>(url.str(), headers_.c_str(), nullptr, &logger_);
            logger_.logDebug("%d bars downloaded\n", retrieved.content().size());
            if (!retrieved) {
                retrieved.content().swap(response.content());
                return retrieved;
            }
            if (retrieved.content().empty()) {
                // no more data
                break;
            }
            nLimit -= retrieved.content().size();

            sStart = date::format("%FT%T", date::sys_seconds{ std::chrono::seconds{retrieved.content().back().time + 60 * span} });
            sStart.append("-00:00");  // add UTC timezone offset
        }

        bool exceeded_end = false;
        for (auto& bar : retrieved.content()) {
            if ((i % n) == 0) {
                if (i != 0) {
                    response.content().push_back(rtBar);
                }
                if (bar.time > end) {
                    exceeded_end = true;
                }
                rtBar.time = bar.time;
                rtBar.open_price = bar.open_price;
                rtBar.high_price = bar.high_price;
                rtBar.low_price = bar.high_price;
                rtBar.volume = bar.volume;
            }
            else {
                rtBar.high_price = std::max<double>(rtBar.high_price, bar.high_price);
                rtBar.low_price = std::min<double>(rtBar.low_price, bar.low_price);
                rtBar.close_price = bar.close_price;
                rtBar.volume += bar.volume;
            }
            ++i;
        }

        if (!exceeded_end && (i % n) == 0) {
            response.content().push_back(rtBar);
        }
        else if (exceeded_end) {
            break;
        }
    }
    return response;
}

Response<std::vector<Bar>> AlpacaMarketData::getBars(
    const std::string& symbol,
    __time32_t start,
    __time32_t end,
    int nTickMinutes,
    uint32_t limit,
    int32_t price_type) const {

    return getBarsV1(symbol, start, end, nTickMinutes, limit);

    //switch (price_type) {
    //    // V2 bars not ready yet, for now use trade instead
    ////case 0:
    ////    return downloadBars(symbol, start, end, nTickMinutes, limit);
    ////    break;
    //case 0:
    //case 1:
    //    return downloadQuotes(symbol, start, end, nTickMinutes, limit);
    //case 2:
    //    return downloadTrades(symbol, start, end, nTickMinutes, limit);

    //default:
    //    return downloadBars(symbol, start, end, nTickMinutes, limit);
    //}
}

Response<std::vector<Bar>> AlpacaMarketData::downloadBars(
    const std::string& symbol,
    __time32_t start,
    __time32_t end,
    int nTickMinutes,
    uint32_t limit) const {

    // Alpaca only suport 1Sec, 1Min, 1Hour, 1D bars
    static std::vector<uint32_t> supported_fimeframe = { 1, 60, 1440 };

    std::string timeframe;
    uint32_t n = 1;
    switch (nTickMinutes) {
    case 1:
        timeframe = "1Min";
        break;
    case 60:
        timeframe = "1Hour";
        break;
    case 1440:
        timeframe = "1D";
        break;
    default:
        break;
    }

    if (timeframe.empty()) {
        for (auto it = supported_fimeframe.rbegin(); it != supported_fimeframe.rend(); ++it) {
            if ((nTickMinutes % *it) == 0) {
                n = (uint32_t)(nTickMinutes / *it);
            }
        }
    }

    uint32_t nLimit = limit * n;
    uint32_t i = 0;
    Response<std::vector<Bar>> response;
    auto& bars = response.content();
    bars.reserve(400000);
    Bar rtBar;
    __time32_t e = end;
    __time32_t s = start;

    auto _downloadBars = [this, &i, &response, &bars, &rtBar, &e, &nLimit, &timeframe, start, &symbol, n]() {
        std::string sStart = timeToString(start);
        std::string page_token;
        do {

            std::string sEnd = timeToString(e);
            std::stringstream url;
            url << baseUrl_ << "/v2/stocks/" << symbol << "/bars?start=" << sStart
                << "&end=" << sEnd << "&limit=" << 10000 << "&timeframe=" << timeframe;

            if (!page_token.empty()) {
                url << "&page_token=" << page_token;
            }

            auto retrieved = request<Bars, AlpacaMarketData>(url.str(), headers_.c_str(), nullptr, &logger_);
            if (!retrieved) {
                if (retrieved.getCode() == 40010001 && retrieved.what() == "end is too late for subscription") {
                    // Basic Plan has 15min delay
                    // TODO: switch to Polygon?
                    logger_.logError("%s(%d)\n", retrieved.what().c_str(), retrieved.getCode());
                    response.onError(retrieved.getCode(), retrieved.what());
                }
                else {
                    BrokerError(("Failed to get bars. " + retrieved.what()).c_str());
                }
                return;
            }

            auto& retrvievedBars = retrieved.content().bars;
            logger_.logDebug("%d bars downloaded\n", retrvievedBars.size());
            if (retrvievedBars.empty()) {
                return;
            }

            e = retrvievedBars.back().time - 30;
            while (!retrvievedBars.empty() && nLimit) {
                const auto& bar = retrvievedBars.back();
                if (bar.time < (uint32_t)start) {
                    break;
                }

                if (n == 1) {
                    bars.emplace_back(std::move(bar));
                }
                else {
                    if (i == 0) {
                        rtBar = bar;
                    }
                    else {
                        rtBar.high_price = std::max<double>(rtBar.high_price, bar.high_price);
                        rtBar.low_price = std::min<double>(rtBar.low_price, bar.low_price);
                        rtBar.close_price = bar.close_price;
                        rtBar.volume += bar.volume;
                    }

                    if (++i == n) {
                        bars.emplace_back(std::move(rtBar));
                        i = 0;
                    }
                }
                --nLimit;
                retrvievedBars.pop_back();
            }
            page_token = retrieved.content().next_page_token;
        } while (nLimit && !page_token.empty());
    };

    //if (nLimit <= 10000) {
    //    _downloadBars(nLimit);
    //}
    //else {
    //    // limit exceeds 10000, need to download multiple times
    //    while (nLimit) {
    //        _downloadBars(10000);
    //    }
    //}

    _downloadBars();

    return response;
}

//Response<std::vector<Bar>> AlpacaMarketData::downloadTrades(
//    const std::string& symbol,
//    __time32_t start,
//    __time32_t end,
//    int nTickMinutes,
//    uint32_t limit) const {
//
//    if (symbol != previous_symbol_ || (last_price_type_ != -1 && last_price_type_ != 2)) {
//        if (!previous_symbol_.empty()) {
//            downloaded_bars_.clear();
//        }
//        previous_symbol_ = symbol;
//    }
//
//    last_price_type_ = 2;
//
//    Response<std::vector<Bar>> response;
//    auto& bars = response.content();
//    bars.reserve(limit);
//
//    getFromDownloadedBars(bars, end, limit);
//
//    if (!limit) {
//        return response;
//    }
//
//
//    Bar rtBar;
//    rtBar.time = 0;
//
//    std::string sEnd = toTimeString(end);
//    std::string sStart = toTimeString(std::max<__time32_t>(start, end - 345600));
//    std::string page_token;
//    do {
//        std::stringstream url;
//        url << baseUrl_ << "/v2/stocks/" << symbol << "/trades?start=" << sStart
//            << "&end=" << sEnd << "&limit=" << 10000;
//
//        if (!page_token.empty()) {
//            url << "&page_token=" << page_token;
//        }
//
//        auto retrieved = request<Trades, AlpacaMarketData>(url.str(), headers_.c_str(), nullptr, &logger_);
//        if (!retrieved) {
//            if (retrieved.getCode() == 40010001 && retrieved.what() == "end is too late for subscription") {
//                // Basic Plan has 15min delay
//                // TODO: switch to Polygon?
//                logger_.logError("%s(%d)\n", retrieved.what().c_str(), retrieved.getCode());
//                response.onError(retrieved.getCode(), retrieved.what());
//            }
//            else if (retrieved.getCode() == 50010000 && retrieved.what() == "internal server error occurred") {
//                logger_.logError("%s(%d)\n", retrieved.what().c_str(), retrieved.getCode());
//                BrokerProgress(1);
//                std::this_thread::sleep_for(std::chrono::seconds(1));
//                continue;
//            }
//            else {
//                BrokerError(("Failed to get bars. " + retrieved.what()).c_str());
//            }
//            break;
//        }
//
//        auto& trades = retrieved.content().trades;
//        logger_.logDebug("%d trades downloaded\n", trades.size());
//        if (trades.empty()) {
//            return response;
//        }
//
//        for (size_t i = 0; limit && i < trades.size(); ++i) {
//            const auto& trade = trades[i];
//            if (trade.timestamp < (uint32_t)start) {
//                continue;
//            }
//
//            if (trade.timestamp > end) {
//                break;
//            }
//
//            if (rtBar.time && (trade.timestamp - rtBar.time) > 60) {
//                if (!downloaded_bars_.empty() && rtBar.time <= downloaded_bars_.back().time) {
//                    logger_.logWarning("Drop invalid bar time=%s, lastKnowBar time=%s\n", toTimeString(rtBar.time).c_str(), toTimeString(downloaded_bars_.back().time).c_str());
//                    rtBar.time = 0;
//                    continue;
//                }
//                downloaded_bars_.push_back(rtBar);
//                rtBar.time = 0;
//            }
//
//            if (rtBar.time == 0) {
//                rtBar.time = (uint32_t(trade.timestamp / 60)) * 60;
//                rtBar.open_price = trade.price;
//                rtBar.high_price = trade.price;
//                rtBar.low_price = trade.price;
//                rtBar.volume = trade.size;
//            }
//            else {
//                rtBar.high_price = std::max<double>(rtBar.high_price, trade.price);
//                rtBar.low_price = std::min<double>(rtBar.low_price, trade.price);
//                rtBar.volume += trade.size;
//            }
//            rtBar.close_price = trade.price;
//        }
//        page_token = retrieved.content().next_page_token;
//        BrokerProgress(1);
//        logger_.logDebug("%d bars built. last bar at %s\n", downloaded_bars_.size(), downloaded_bars_.empty() ? "" : toTimeString(downloaded_bars_.back().time).c_str());
//    } while (!page_token.empty());
//
//    returned_index_ = std::max<int>(downloaded_bars_.size() - limit - 1, 0);
//    for (size_t i = returned_index_ + 1; i < downloaded_bars_.size(); i++) {
//        bars.push_back(downloaded_bars_[i]);
//    }
//
//    logger_.logDebug("returned_index_=%d\n", returned_index_);
//    if (!bars.empty()) {
//        logger_.logDebug("first bar returned %s\n", toTimeString(bars[0].time).c_str());
//    }
//    return response;
//}

//Response<std::vector<Bar>> AlpacaMarketData::downloadQuotes(
//    const std::string& symbol,
//    __time32_t start,
//    __time32_t end,
//    int nTickMinutes,
//    uint32_t limit) const {
//
//    if (symbol != previous_symbol_ || (last_price_type_ != -1 && last_price_type_ != 1)) {
//        if (!previous_symbol_.empty()) {
//            downloaded_bars_.clear();
//        }
//        previous_symbol_ = symbol;
//    }
//
//    last_price_type_ = 1;
//
//    Response<std::vector<Bar>> response;
//    auto& bars = response.content();
//    bars.reserve(limit);
//
//    getFromDownloadedBars(bars, end, limit);
//
//
//    if (!limit) {
//        return response;
//    }
//
//
//    Bar rtBar;
//    rtBar.time = 0;
//
//    std::string sEnd = toTimeString(end);
//    std::string sStart = toTimeString(std::max<__time32_t>(start, end - 345600));
//    std::string page_token;
//    do {
//        std::stringstream url;
//        url << baseUrl_ << "/v2/stocks/" << symbol << "/quotes?start=" << sStart
//            << "&end=" << sEnd << "&limit=" << 10000;
//
//        if (!page_token.empty()) {
//            url << "&page_token=" << page_token;
//        }
//
//        auto retrieved = request<Quotes, AlpacaMarketData>(url.str(), headers_.c_str(), nullptr, &logger_);
//        if (!retrieved) {
//            if (retrieved.getCode() == 40010001 && retrieved.what() == "end is too late for subscription") {
//                // Basic Plan has 15min delay
//                // TODO: switch to Polygon?
//                logger_.logError("%s(%d)\n", retrieved.what().c_str(), retrieved.getCode());
//                response.onError(retrieved.getCode(), retrieved.what());
//            }
//            else if (retrieved.getCode() == 50010000 && retrieved.what() == "internal server error occurred") {
//                logger_.logError("%s(%d)\n", retrieved.what().c_str(), retrieved.getCode());
//                BrokerProgress(1);
//                std::this_thread::sleep_for(std::chrono::seconds(1));
//                continue;
//            }
//            else {
//                BrokerError(("Failed to get bars. " + retrieved.what()).c_str());
//            }
//            break;
//        }
//
//        auto& quotes = retrieved.content().quotes;
//        logger_.logDebug("%d quotes downloaded\n", quotes.size());
//        if (quotes.empty()) {
//            return response;
//        }
//
//        for (size_t i = 0; limit && i < quotes.size(); ++i) {
//            const auto& quote = quotes[i];
//            if (quote.timestamp < (uint32_t)start) {
//                continue;
//            }
//
//            if (quote.timestamp > end) {
//                break;
//            }
//
//            if (rtBar.time && (quote.timestamp - rtBar.time) > 60) {
//                if (!downloaded_bars_.empty() && rtBar.time <= downloaded_bars_.back().time) {
//                    logger_.logWarning("Drop invalid bar time=%s, lastKnowBar time=%s\n", toTimeString(rtBar.time).c_str(), toTimeString(downloaded_bars_.back().time).c_str());
//                    rtBar.time = 0;
//                    continue;
//                }
//                downloaded_bars_.push_back(rtBar);
//                rtBar.time = 0;
//            }
//
//            if (rtBar.time == 0) {
//                rtBar.time = (uint32_t(quote.timestamp / 60)) * 60;
//                rtBar.open_price = quote.ask_price;
//                rtBar.high_price = quote.ask_price;
//                rtBar.low_price = quote.ask_price;
//                rtBar.volume = quote.ask_size;
//            }
//            else {
//                rtBar.high_price = std::max<double>(rtBar.high_price, quote.ask_price);
//                rtBar.low_price = std::min<double>(rtBar.low_price, quote.ask_price);
//                rtBar.volume += quote.ask_size;
//            }
//            rtBar.close_price = quote.ask_price;
//        }
//        page_token = retrieved.content().next_page_token;
//        BrokerProgress(1);
//        logger_.logDebug("%d bars built. last bar at %s\n", downloaded_bars_.size(), downloaded_bars_.empty() ? "" : toTimeString(downloaded_bars_.back().time).c_str());
//    } while (!page_token.empty());
//
//    returned_index_ = std::max<int>(downloaded_bars_.size() - limit - 1, 0);
//    for (size_t i = returned_index_ + 1; i < downloaded_bars_.size(); i++) {
//        bars.push_back(downloaded_bars_[i]);
//    }
//
//    logger_.logDebug("returned_index_=%d\n", returned_index_);
//    if (!bars.empty()) {
//        logger_.logDebug("first bar returned %s\n", toTimeString(bars[0].time).c_str());
//    }
//    return response;
//}

void AlpacaMarketData::getFromDownloadedBars(std::vector<Bar>& bars, __time32_t& end, uint32_t& limit) const {
    if (!downloaded_bars_.empty()) {
        // found in bars we already downloaded
        logger_.logDiag("check dowloaded bars. returned_index_=%d, bar=%s\n", returned_index_, timeToString(downloaded_bars_[returned_index_].time).c_str());

        for (returned_index_; returned_index_ >= 0 && returned_index_ < downloaded_bars_.size();) {
            auto& bar = downloaded_bars_[returned_index_];
            if (bar.time > (uint32_t)end) {
                break;
            }
            else if (bar.time == end || returned_index_ == downloaded_bars_.size() - 1) {
                break;
            }
            ++returned_index_;
        }

        auto upperbound = returned_index_;
        returned_index_ = std::max<int>(returned_index_ - limit + 1, 0);
        if (returned_index_ > 0 || ((upperbound - returned_index_ + 1) == limit)) {
            logger_.logDiag("have enough data in downloaded. returned_index_=%d, upperbound=%d, first bar=%s, last bar=%s\n", returned_index_, upperbound, timeToString(downloaded_bars_[returned_index_].time).c_str(), timeToString(downloaded_bars_[upperbound].time).c_str());
            // dwonloaded bars contains enough bars
            for (auto i = returned_index_; i <= upperbound; ++i, --limit) {
                bars.emplace_back(std::move(downloaded_bars_[i]));
            }
            assert(limit == 0);
        }

        leftover_bars_.clear();

        if (!returned_index_ && limit) {
            // don't have enough data in downloaded bars
            leftover_bars_.resize(upperbound);
            memcpy(&leftover_bars_[0], &downloaded_bars_[0], sizeof(Bar) * upperbound);
            //for (size_t i = 0; i < upperbound; ++i) {
            //    leftover_bars_.emplace_back(std::move(downloaded_bars_[i]));
            //}
            downloaded_bars_.clear();
        }

        if (!leftover_bars_.empty()) {
            logger_.logDiag("%d bars already downloaded, first bar %s, last bar %s\n", leftover_bars_.size(), timeToString(leftover_bars_[0].time).c_str(), timeToString(leftover_bars_.back().time).c_str());
            end = leftover_bars_[0].time;
        }
    }
}
