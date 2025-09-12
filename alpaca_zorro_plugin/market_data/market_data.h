#pragma once

#include <string>
#include <functional>
#include "request.h"
#include "market_data/snapshot.h"
#include "market_data/bars.h"
#include "alpaca/asset.h"
#include "../logger.h"

namespace alpaca {

    class MarketData {

    public:
        MarketData(const char* base_url, std::string headers)
            : base_url_(base_url)
            , headers_(std::move(headers))
        {}

        virtual ~MarketData() = default;

        virtual AssetClass type() const noexcept = 0;

        virtual Response<LastQuotes> getLastQuotes(const std::string& symbols) const {
            return request<LastQuotes>(base_url_ + "quotes/latest?symbols=" + symbols, headers_.c_str(), nullptr, slick_logger::LogLevel::L_DEBUG);
        }

        virtual Response<LastTrades> getLastTrades(const std::string& symbols) const {
            return request<LastTrades>(base_url_ + "trades/latest?symbols=" + symbols, headers_.c_str(), nullptr, slick_logger::LogLevel::L_DEBUG);
        }

        virtual Response<Snapshot> getSnapshots(const std::string& symbols) const {
            return request<Snapshot>(base_url_ + "snapshots?symbols=" + symbols, headers_.c_str(), nullptr, slick_logger::LogLevel::L_DEBUG);
        }

        virtual Response<LastQuote> getLastQuote(const std::string& symbol) const {
            return request<LastQuote>(base_url_ + symbol + "/quotes/latest", headers_.c_str(), nullptr, slick_logger::LogLevel::L_DEBUG);
        }

        virtual Response<LastTrade> getLastTrade(const std::string& symbol) const {
            return request<LastTrade>(base_url_ + symbol + "/trades/latest", headers_.c_str(), nullptr, slick_logger::LogLevel::L_DEBUG);
        }

        virtual Response<Snapshot> getSnapshot(const std::string& symbol) const {
            return request<Snapshot>(base_url_ + symbol + "/snapshot", headers_.c_str(), nullptr, slick_logger::LogLevel::L_DEBUG);
        }

        Response<std::vector<Bar>> getBars(
            const std::string& symbol,
            __time32_t start,
            __time32_t end,
            int nTickMinutes = 1,
            uint32_t limit = 10000,
            int32_t price_type = 0) const;

    protected:
        virtual std::string downloadBarBaseUri(const std::string& symbol) const noexcept;

        virtual Response<std::vector<Bar>> downloadBars(
            const std::string& symbol,
            __time32_t start,
            __time32_t end,
            int nTickMinutes = 1,
            uint32_t limit = 100) const;

        Response<std::vector<Bar>> downloadTrades(
            const std::string& symbol,
            __time32_t start,
            __time32_t end,
            int nTickMinutes = 1,
            uint32_t limit = 100) const {
            return buildBarsFromTickData<Trades>(1, symbol, start, end, nTickMinutes, limit, "trades");
        }

        Response<std::vector<Bar>> downloadQuotes(
            const std::string& symbol,
            __time32_t start,
            __time32_t end,
            int nTickMinutes = 1,
            uint32_t limit = 100) const {
            return buildBarsFromTickData<Quotes>(1, symbol, start, end, nTickMinutes, limit, "quotes");
        }

        void getFromDownloadedBars(std::vector<Bar>& bars, __time32_t& end, uint32_t& limit) const;

        template<typename TickTypeT>
        inline Response<std::vector<Bar>> buildBarsFromTickData(int price_type, const std::string& symbol, __time32_t start, __time32_t end, uint32_t nTickMinutes, uint32_t limit, const std::string& tickType) const;

        template<typename TickTypeT>
        inline bool buildBar(TickTypeT& retrieved, Response<std::vector<Bar>>& response, Bar& rtBar, __time32_t start, __time32_t end, uint32_t nTickMinutes) const;

    protected:
        std::string base_url_;
        std::string headers_;

        // to facilitate historical data download 
        mutable std::vector<Bar> downloaded_bars_;
        mutable std::vector<Bar> leftover_bars_;
        mutable std::string previous_symbol_;
        mutable uint32_t returned_index_ = 0;
        mutable int32_t last_price_type_ = -1;
    };


    //template<typename TickTypeT>
    //inline Response<std::vector<Bar>> StockMarketData::buildBarsFromTickData(int price_type, const std::string& symbol, __time32_t start, __time32_t end, uint32_t nTickMinutes, uint32_t limit, const std::string& tickType) const {
    //    if (symbol != previous_symbol_ || (last_price_type_ != -1 && last_price_type_ != price_type)) {
    //        if (!previous_symbol_.empty()) {
    //            downloaded_bars_.clear();
    //        }
    //        previous_symbol_ = symbol;
    //    }

    //    last_price_type_ = price_type;

    //    // BrokerHistory2 function might drop bars, adding extra bars to insure enough bars are returned 
    //    limit += 5;

    //    Response<std::vector<Bar>> response;
    //    auto& bars = response.content();
    //    bars.reserve(limit);

    //    getFromDownloadedBars(bars, end, limit);

    //    if (!limit) {
    //        return response;
    //    }


    //    Bar rtBar;
    //    rtBar.time = 0;

    //   
    //    __time32_t e = end;
    //   
    //    std::string page_token;
    //    uint32_t nForceRetry = 6;
    //    do {
    //        std::string sEnd = timeToString(e);
    //        __time32_t s = std::max<__time32_t>(start, e - 14400);
    //        std::string sStart = timeToString(s);
    //        std::stringstream url;
    //        url << baseUrl_ << symbol << "/" << tickType << "?start=" << sStart
    //            << "&end=" << sEnd << "&limit=" << 10000;

    //        if (!page_token.empty()) {
    //            url << "&page_token=" << page_token;
    //        }

    //        auto retrieved = request<TickTypeT, StockMarketData>(url.str(), headers_.c_str(), nullptr/*, &logger_*/);
    //        if (!retrieved) {
    //            if (retrieved.getCode() == 40010001 && retrieved.what() == "end is too late for subscription") {
    //                // Basic Plan has 15min delay
    //                // TODO: switch to Polygon?
    //                LOG_ERROR("%s(%d)\n", retrieved.what().c_str(), retrieved.getCode());
    //                response.onError(retrieved.what(), retrieved.getCode());
    //            }
    //            else if (retrieved.getCode() == 50010000 && retrieved.what() == "internal server error occurred") {
    //                LOG_ERROR("%s(%d)\n", retrieved.what().c_str(), retrieved.getCode());
    //                BrokerProgress(1);
    //                std::this_thread::sleep_for(std::chrono::seconds(1));
    //                if (--nForceRetry == 0) {
    //                    break;
    //                }
    //                continue;
    //            }
    //            else {
    //                BrokerError(("Failed to get bars. " + retrieved.what()).c_str());
    //            }
    //            break;
    //        }

    //        nForceRetry = 6;

    //        if (!buildBar(retrieved.content(), response, rtBar, start, e, nTickMinutes)) {
    //            if (e < start) {
    //                break;
    //            }
    //            page_token = "";
    //            e = s - 30;
    //            continue;
    //        }

    //        page_token = retrieved.content().next_page_token;
    //        BrokerProgress(1);
    //        LOG_DIAG("%d bars built. last bar at %s\n", downloaded_bars_.size(), downloaded_bars_.empty() ? "" : timeToString(downloaded_bars_.back().time).c_str());

    //        if (page_token.empty()) {
    //            e = s - 30;
    //        }
    //    } while (e >= start);

    //    if (!leftover_bars_.empty()) {
    //        limit -= leftover_bars_.size();
    //    }

    //    auto ds = downloaded_bars_.size();
    //    returned_index_ = std::max<int>(ds - limit, 0);
    //    auto n = ds - returned_index_;
    //    bars.resize(n + leftover_bars_.size());
    //    memcpy(&bars[0], &downloaded_bars_[returned_index_], sizeof(Bar) * n);
    //    //for (size_t i = returned_index_; i < ds; i++) {
    //    //    bars.push_back(downloaded_bars_[i]);
    //    //}

    //    if (!leftover_bars_.empty()) {
    //        memcpy(&bars[n], &leftover_bars_[0], sizeof(Bar) * leftover_bars_.size());
    //    }
    //    //for (auto& bar : leftover_bars_) {
    //    //    bars.emplace_back(std::move(bar));
    //    //}
    //    leftover_bars_.clear();

    //    LOG_DIAG("returned_index_=%d, downloaded size=%d\n", returned_index_, ds);
    //    if (!bars.empty()) {
    //        LOG_DIAG("first bar returned %s, last bar %s\n", timeToString(bars[0].time).c_str(), timeToString(bars.back().time).c_str());
    //    }
    //    return response;
    //}

    //template<>
    //inline bool StockMarketData::buildBar(Trades& retrieved, Response<std::vector<Bar>>& response, Bar& rtBar, __time32_t start, __time32_t end, uint32_t nTickMinutes) const {
    //    auto& trades = retrieved.trades;
    //    LOG_DIAG("%d trades downloaded\n", trades.size());
    //    if (trades.empty()) {
    //        return false;
    //    }

    //    auto timeFrame = nTickMinutes * 60;
    //    for (size_t i = 0; i < trades.size(); ++i) {
    //        const auto& trade = trades[i];
    //        if (trade.timestamp < (uint32_t)start) {
    //            continue;
    //        }

    //        if (trade.timestamp > end) {
    //            break;
    //        }

    //        if (!timeFrame) {
    //            downloaded_bars_.push_back({trade.sTime, static_cast<uint32_t>(trade.timestamp), trade.price, trade.price, trade.price, trade.price, static_cast<uint32_t>(trade.size)});
    //        }
    //        else {
    //            if (rtBar.time && (trade.timestamp - rtBar.time) > timeFrame) {
    //                if (!downloaded_bars_.empty() && rtBar.time <= downloaded_bars_.rbegin()->time) {
    //                    LOG_WARNING("Drop invalid bar time=%s, lastKnowBar time=%s\n", timeToString(rtBar.time).c_str(), timeToString(downloaded_bars_.rbegin()->time).c_str());
    //                    rtBar.time = 0;
    //                    continue;
    //                }
    //                downloaded_bars_.push_back(rtBar);
    //                rtBar.time = 0;
    //            }

    //            if (rtBar.time == 0) {
    //                rtBar.time = (uint32_t(trade.timestamp / timeFrame)) * timeFrame;
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
    //    }
    //    return true;
    //}

    //template<>
    //inline bool StockMarketData::buildBar(Quotes& retrieved, Response<std::vector<Bar>>& response, Bar& rtBar, __time32_t start, __time32_t end, uint32_t nTickMinutes) const {
    //    auto& quotes = retrieved.quotes;
    //    if (quotes.size() < 1000) {
    //        LOG_DIAG("%d quotes downloaded\n", quotes.size());
    //    }
    //    if (quotes.empty()) {
    //        return false;
    //    }

    //    auto timeFrame = nTickMinutes * 60;
    //    for (size_t i = 0; i < quotes.size(); ++i) {
    //        const auto& quote = quotes[i];
    //        if (quote.timestamp < (uint32_t)start) {
    //            continue;
    //        }

    //        if (quote.timestamp > end) {
    //            break;
    //        }

    //        if (!timeFrame) {
    //            downloaded_bars_.push_back({ quote.sTime, static_cast<uint32_t>(quote.timestamp), quote.ask_price, quote.ask_price, quote.ask_price, quote.ask_price, static_cast<uint32_t>(quote.ask_size) });
    //        }
    //        else {
    //            if (rtBar.time && (quote.timestamp - rtBar.time) > timeFrame) {
    //                if (!downloaded_bars_.empty() && rtBar.time <= downloaded_bars_.rbegin()->time) {
    //                    LOG_WARNING("Drop invalid bar time=%s, lastKnowBar time=%s\n", timeToString(rtBar.time).c_str(), timeToString(downloaded_bars_.rbegin()->time).c_str());
    //                    rtBar.time = 0;
    //                    continue;
    //                }
    //                downloaded_bars_.push_back(rtBar);
    //                rtBar.time = 0;
    //            }

    //            if (rtBar.time == 0) {
    //                rtBar.time = (uint32_t(quote.timestamp / timeFrame)) * timeFrame;
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
    //    }
    //    return true;
    //}
}