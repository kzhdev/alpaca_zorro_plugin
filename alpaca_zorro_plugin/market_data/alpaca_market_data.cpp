#include "stdafx.h"
#include "market_data/alpaca_market_data.h"
#include "date/date.h"

using namespace alpaca;

Response<std::vector<Bar>> AlpacaMarketData::getBars(
    const std::string& symbol,
    const __time32_t start,
    const __time32_t end,
    const int nTickMinutes,
    const uint32_t limit) const {

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
    bars.reserve(nLimit);
    Bar rtBar;
    __time32_t e = end;

    std::string sStart;
    {
        using namespace date;
        try {
            if (start) {
                sStart = format("%FT%TZ", date::sys_seconds{ std::chrono::seconds{start} });
            }
        }
        catch (const std::exception&) {
            assert(false);
        }
    }

    auto downloadBars = [this, &i, &response, &bars, &rtBar, &e, &nLimit, &timeframe, start, sStart, symbol, n](uint32_t nbars) {
        std::string page_token;
        do {
           
            std::string sEnd;
            {
                using namespace date;
                try {

                    sEnd = format("%FT%TZ", date::sys_seconds{ std::chrono::seconds{e} });
                }
                catch (const std::exception&) {
                    assert(false);
                }
            }

            std::stringstream url;
            url << baseUrl_ << "/v2/stocks/" << symbol << "/bars?start=" << sStart
                << "&end=" << sEnd << "&limit=" << nbars << "&timeframe=" << timeframe;

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
            while (!retrvievedBars.empty()) {
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

    // adjust start to 
    if (nLimit <= 10000) {
        downloadBars(nLimit);
    }
    else {
        // limit exceeds 10000, need to download multiple times
        while (nLimit) {
            downloadBars(10000);
        }
    }
            
    return response;
}