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

        return request<std::vector<Bar>, AlpacaMarketData>(url.str(), headers_.c_str(), nullptr);
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

            LOG_DEBUG("--> %s\n", url.str().c_str());
            retrieved = request<std::vector<Bar>, AlpacaMarketData>(url.str(), headers_.c_str(), nullptr);
            LOG_DEBUG("%d bars downloaded\n", retrieved.content().size());
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

            LOG_DEBUG("--> %s\n", url.str().c_str());
            retrieved = request<std::vector<Bar>, AlpacaMarketData>(url.str(), headers_.c_str(), nullptr, alpaca::L_TRACE);
            LOG_DEBUG("%d bars downloaded\n", retrieved.content().size());
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

    // TODO: fix tick data download when start is not mendatory or desendent data supported
    //if (!nTickMinutes) {
    //    // Tick data
    //    if (price_type == 1) {
    //        return downloadQuotes(symbol, start, end, nTickMinutes, limit);
    //    }
    //    else {
    //        return downloadTrades(symbol, start, end, nTickMinutes, limit);
    //    }
    //}
    return downloadBars(symbol, start, end, nTickMinutes, limit);
}

Response<std::vector<Bar>> AlpacaMarketData::downloadBars(
    const std::string& symbol,
    __time32_t start,
    __time32_t end,
    int nTickMinutes,
    uint32_t limit) const {

    // Alpaca only suport 1Min, 1Hour, 1D bars
    static std::vector<std::pair<uint32_t, const char*>> supported_timeframe = { {1, "1Min"}, {60, "1Hour"}, {1440, "1D"} };

    std::string timeframe;
    uint32_t n = 1;
    uint32_t granularity = nTickMinutes;
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
        for (auto it = supported_timeframe.rbegin(); it != supported_timeframe.rend(); ++it) {
            if ((nTickMinutes % it->first) == 0) {
                granularity = it->first;
                n = (uint32_t)(nTickMinutes / it->first);
                timeframe = it->second;
                break;
            }
        }
    }

    uint32_t i = 0;
    Response<std::vector<Bar>> response;
    auto& bars = response.content();
    bars.reserve(limit);
    std::vector<Bar> cached;
    Bar rtBar;
    __time32_t e = end;
    __time32_t s;
    std::string page_token;
    bool bFirstBar = true;
    uint32_t bar_end_time;

    auto buildBar = [&limit, start, &rtBar, &bFirstBar, n, &i, nTickMinutes, &bars, &bar_end_time, granularity](std::vector<Bar>& dowloadedBars) {
        int residual = 0;
        while (!dowloadedBars.empty() && limit) {
            const auto& bar = dowloadedBars.back();
            if (bar.time < (uint32_t)start) {
                break;
            }

            if (bar.time_string == "2021-05-11T23:59:00Z" || bar.time_string == "2021-05-12T08:01:00Z") {
                printf("\n");
            }

            if (n == 1) {
                bars.emplace_back(std::move(bar));
                --limit;
            }
            else {
                if (i == 0) {
                    if (bFirstBar) {
                        if ((bar.time + granularity * 60) % (nTickMinutes * 60) != 0) {
                            // drop incompleted bar data
                            LOG_DIAG("Drop bar at %s\n", bar.time_string.c_str());
                            dowloadedBars.pop_back();
                            continue;
                        }
                        bFirstBar = false;
                    }
                    rtBar = bar;
                    bar_end_time = rtBar.time;
                    LOG_DIAG("Bar end at %s\n", rtBar.time_string.c_str());
                }
                else {
                    if ((bar_end_time - bar.time) > ((nTickMinutes - 1) * 60))  { 
                        LOG_DIAG("Some data is missing, the data at %s belongs to different bar.\n", bar.time_string.c_str());
                        bars.emplace_back(std::move(rtBar));
                        rtBar = bar;
                        bar_end_time = bar.time;
                        i = 0;
                        --limit;
                    }
                    else {
                        LOG_DIAG("Bar at %s\n", bar.time_string.c_str());
                        rtBar.high_price = std::max<double>(rtBar.high_price, bar.high_price);
                        rtBar.low_price = std::min<double>(rtBar.low_price, bar.low_price);
                        rtBar.open_price = bar.open_price;
                        rtBar.time = bar.time;
                        rtBar.time_string = bar.time_string;
                        rtBar.volume += bar.volume;
                    }
                }

                if ((bar.time % (nTickMinutes * 60)) == 0) {
                    bars.emplace_back(std::move(rtBar));
                    i = 0;
                    --limit;
                }
                else {
                    ++i;
                    assert(i < n);
                }
            }
            dowloadedBars.pop_back();
        }
    };

    auto lmt = 5000 * granularity;

    do {
        s = std::max<__time32_t>(start, e - lmt * 60);
        std::string sStart = timeToString(s);
        std::string sEnd = timeToString(e);
        std::stringstream url;
        url << baseUrl_ << "/v2/stocks/" << symbol << "/bars?start=" << sStart
            << "&end=" << sEnd << "&limit=" << 5000 << "&timeframe=" << timeframe;

        if (!page_token.empty()) {
            url << "&page_token=" << page_token;
        }

        auto retrieved = request<Bars, AlpacaMarketData>(url.str(), headers_.c_str());
        if (!retrieved) {
            if (retrieved.getCode() == 40010001 && retrieved.what() == "end is too late for subscription") {
                // Basic Plan has 15min delay
                LOG_ERROR("%s(%d)\n", retrieved.what().c_str(), retrieved.getCode());
                response.onError(retrieved.getCode(), retrieved.what());
            }
            else {
                BrokerError(("Failed to get bars. " + retrieved.what()).c_str());
            }
            break;
        }

        auto& retrvievedBars = retrieved.content().bars;
        if (retrvievedBars.empty()) {
            LOG_DEBUG("0 bars downloaded.\n");
            break;
        }
        LOG_DEBUG("%d bars downloaded. %s-%s\n", retrvievedBars.size(), retrvievedBars.front().time_string.c_str(), retrvievedBars.back().time_string.c_str());

        if (!retrieved.content().next_page_token.empty()) {
            LOG_DIAG("data pagenated\n");
            // need to get more data entil reach the end;
            cached.insert(cached.end(), retrvievedBars.begin(), retrvievedBars.end());
            page_token = retrieved.content().next_page_token;
            continue;
        }
        else {
            page_token = "";
        }

        e = cached.empty() ? retrvievedBars.front().time - 30 : cached[0].time - 30;
        buildBar(retrvievedBars);
        if (!cached.empty()) {
            buildBar(cached);
        }
    } while (limit && ((s > start) || !page_token.empty()));

    std::reverse(bars.begin(), bars.end());
    return response;
}

void AlpacaMarketData::getFromDownloadedBars(std::vector<Bar>& bars, __time32_t& end, uint32_t& limit) const {
    if (!downloaded_bars_.empty()) {
        // found in bars we already downloaded
        LOG_DIAG("check dowloaded bars. returned_index_=%d, bar=%s\n", returned_index_, timeToString(downloaded_bars_[returned_index_].time).c_str());

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
            LOG_DIAG("have enough data in downloaded. returned_index_=%d, upperbound=%d, first bar=%s, last bar=%s\n", returned_index_, upperbound, timeToString(downloaded_bars_[returned_index_].time).c_str(), timeToString(downloaded_bars_[upperbound].time).c_str());
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
            LOG_DIAG("%d bars already downloaded, first bar %s, last bar %s\n", leftover_bars_.size(), timeToString(leftover_bars_[0].time).c_str(), timeToString(leftover_bars_.back().time).c_str());
            end = leftover_bars_[0].time;
        }
    }
}
