#include "stdafx.h"
#include "market_data/market_data.h"
#include "date/date.h"
#include "trade.h"
#include "config.h"

using namespace alpaca;

Response<std::vector<Bar>> MarketData::getBars(
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

std::string MarketData::downloadBarBaseUri(const std::string& symbol) const noexcept
{
    return base_url_ + symbol + "/bars?";
}

Response<std::vector<Bar>> MarketData::downloadBars(
    const std::string& symbol,
    __time32_t start,
    __time32_t end,
    int nTickMinutes,
    uint32_t limit) const {

    // Alpaca only suport 1Min, 1Hour, 1D bars
    static std::vector<std::pair<uint32_t, const char*>> supported_timeframe = { {1440, "1Day"}, {10080, "1Week"} };

    std::string timeframe;
    uint32_t n = 1;
    uint32_t granularity = nTickMinutes;

    if (nTickMinutes < 60)
    {
        timeframe = std::to_string(nTickMinutes) + "Min";
    }
    else if (nTickMinutes < 1440)
    {
        if ((nTickMinutes % 60) == 0)
        {
            timeframe = std::to_string(nTickMinutes / 60) + "Hour";
        }
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

        if (timeframe.empty())
        {
            SPDLOG_ERROR("Invalid timeframe ({})", nTickMinutes);
            return Response<std::vector<Bar>>(1, "Invalid timeframe " + std::to_string(nTickMinutes));
        }
    }

    uint32_t i = 0;
    Response<std::vector<Bar>> response;
    auto& bars = response.content();
    bars.reserve(limit);
    Bar rtBar;
    __time32_t e = end;
    std::string page_token;
    bool bFirstBar = true;
    uint32_t bar_end_time;

    auto buildBar = [&limit, start, e, &rtBar, &bFirstBar, n, &i, nTickMinutes, &bars, &bar_end_time, granularity](std::vector<Bar>& dowloadedBars) -> bool {
        int residual = 0;
        for (auto ii = 0u; ii < dowloadedBars.size() && limit > 0; ++ii) {
            auto& bar = dowloadedBars[ii];
            if (bar.time < (uint32_t)start) {
                return false;
            }

            // change time to bar close time
            bar.time += nTickMinutes * 60;
            if (static_cast<__time32_t>(bar.time) > e) {
                // end time cannot exceeds tEnd
                SPDLOG_TRACE("Drop bar at {}", bar.time_string);
                continue;
            }

            if (n == 1) {
                bars.emplace_back(std::move(bar));
                --limit;
            }
            else {
                if (i == 0) {
                    if (bFirstBar) {
                        auto duration = nTickMinutes * 60;
                        if (((int)(e - bar.time) < duration) && ((e % duration) > (int)(bar.time % duration))) {
                            // drop incompleted bar data
                            SPDLOG_TRACE("Drop bar at {}", bar.time_string);
                            continue;
                        }
                        bFirstBar = false;
                    }
                    rtBar = bar;
                    bar_end_time = rtBar.time;
                    SPDLOG_TRACE("Bar end at {}", rtBar.time_string.c_str());
                }
                else {
                    if ((int)(bar_end_time - bar.time) > ((nTickMinutes - 1) * 60)) {
                        SPDLOG_WARN("Some data is missing, the data at {} belongs to different bar.", bar.time_string);
                        SPDLOG_TRACE("Bar end at {}", bar.time_string);
                        bars.emplace_back(std::move(rtBar));
                        rtBar = bar;
                        bar_end_time = bar.time;
                        i = 0;
                        --limit;
                    }
                    else {
                        SPDLOG_TRACE("Bar at {}", bar.time_string);
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
        }
        return true;
    };

    auto lmt = 10000 * granularity;
    std::string sStart = timeToString(start);
    do {
        if (e < start)
        {
            break;
        }
        std::string sEnd = timeToString(e);
        std::stringstream url;
        url << downloadBarBaseUri(symbol) << "start=" << sStart
            << "&end=" << sEnd << "&limit=10000&timeframe=" << timeframe
            << "&sort=desc";
        
        if (type() == AssetClass::US_EQUITY)
        {
            url << "&adjustment=" << to_string(Config::get().adjustment);
        }

        if (!page_token.empty()) {
            url << "&page_token=" << page_token;
        }

        auto retrieved = request<Bars>(url.str(), headers_.c_str());
        if (!retrieved) {
            if (retrieved.getCode() == 40010001 && retrieved.what() == "end is too late for subscription") {
                // Basic Plan has 15min delay
                SPDLOG_ERROR("{}({})", retrieved.what(), retrieved.getCode());
                response.onError(retrieved.what(), retrieved.getCode());
            }
            else {
                BrokerError(("Failed to get bars. " + retrieved.what()).c_str());
            }
            break;
        }

        if (!BrokerProgress(1)) {
            break;
        }

        auto& retrvievedBars = retrieved.content().bars;
        if (!retrvievedBars.empty()) {
            SPDLOG_TRACE("{} bars downloaded. {}-{}", retrvievedBars.size(), retrvievedBars.front().time_string, retrvievedBars.back().time_string);

            if (!retrieved.content().next_page_token.empty() && retrvievedBars.size() != 10000) {
                SPDLOG_TRACE("data pagenated");
                // need to get more data entil reach the end;
                page_token = retrieved.content().next_page_token;
            }
            else {
                page_token = "";
            }

            e = retrvievedBars.back().time - 30;
            if (!buildBar(retrvievedBars))
            {
                break;
            }
        }
        else {
            SPDLOG_TRACE("0 bars downloaded.");
            break;
        }
    } while (limit || !page_token.empty());
    SPDLOG_TRACE("limit: {}, bar size={}", limit, bars.size());
    return response;
}

void MarketData::getFromDownloadedBars(std::vector<Bar>& bars, __time32_t& end, uint32_t& limit) const {
    if (!downloaded_bars_.empty()) {
        // found in bars we already downloaded
        SPDLOG_TRACE("check dowloaded bars. returned_index_={}, bar={}", returned_index_, timeToString(downloaded_bars_[returned_index_].time));

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
        returned_index_ = std::max<uint32_t>(returned_index_ - limit + 1, 0);
        if (returned_index_ > 0 || ((upperbound - returned_index_ + 1) == limit)) {
            SPDLOG_TRACE("have enough data in downloaded. returned_index_={}, upperbound={}, first bar={}, last bar={}", returned_index_, upperbound, timeToString(downloaded_bars_[returned_index_].time), timeToString(downloaded_bars_[upperbound].time));
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
            SPDLOG_TRACE("{} bars already downloaded, first bar {}, last bar {}", leftover_bars_.size(), timeToString(leftover_bars_[0].time), timeToString(leftover_bars_.back().time));
            end = leftover_bars_[0].time;
        }
    }
}
