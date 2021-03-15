#include "stdafx.h"
#include "market_data/polygon.h"
#include "date/date.h"

using namespace alpaca;

Response<std::vector<Bar>> Polygon::getBars(
    const std::string& symbol,
    __time32_t start,
    __time32_t end,
    int nTickMinutes,
    uint32_t limit,
    int32_t price_type) const {

    if (end == 0) {
        end = static_cast<__time32_t>(std::time(nullptr));
    }

    auto t_end = end;
    Response<std::vector<Bar>> result;
    auto& rtBars = result.content();
    rtBars.reserve(limit);
    __time32_t upperBound = t_end;

    do {
        std::stringstream url;

        url << baseUrl_ << "/v2/aggs/ticker/" << symbol << "/range/" << nTickMinutes << "/minute";
        try {
            using namespace date;
            url << "/" << format("%F", date::sys_seconds{ std::chrono::seconds{ start } });
            url << "/" << format("%F", date::sys_seconds{ std::chrono::seconds{ t_end } });
        }
        catch (const std::exception& e) {
            assert(false);
            return Response<std::vector<Bar>>(1, "invalid time. " + std::string(e.what()));
        }
        url << "?sort=desc&limit="<<limit;    // in desending order
        logger_.logDebug("--> %s\n", url.str().c_str());
        url << "&" << apiKey_;

        auto response = request<std::vector<Bar>, Polygon>(url.str(), nullptr, nullptr);

        if (!response) {
            BrokerError(response.what().c_str());
            break;
        }

        auto& bars = response.content();
        size_t nExclude = 0;
        logger_.logDebug("%d bars downloaded.\n", bars.size());
        if (!bars.empty()) {
            using namespace date;
            auto from = bars.front().time;
            auto to = bars.back().time;
            logger_.logDebug("%s(%d) - %s(%d)\n", format("%F %T", date::sys_seconds{ std::chrono::seconds{ from } }).c_str(), from,
                format("%F %T", date::sys_seconds{ std::chrono::seconds{ to } }).c_str(), to);
        }
        else {
            // no more data
            break;
        }

        // remove record passed end time
        auto it = bars.cbegin();
        for (; it != bars.cend();) {
            if ((__time32_t)(*it).time <= upperBound) {
                break;
            }
            ++it;
        }

        if (it != bars.cbegin()) {
            bars.erase(bars.cbegin(), it);
        }

        if (bars.empty()) {
            break;
        }

        rtBars.insert(rtBars.end(), bars.begin(), bars.end());
        if (!rtBars.empty()) {
            upperBound = rtBars.back().time - 1;
        }
    } while (rtBars.size() < limit);

    // remove order older than start
    auto it = rtBars.end();
    while (it != rtBars.begin()) {
        --it;
        if ((*it).time >= (uint32_t)start) {
            break;
        }

        it = rtBars.erase(it);
    }

    // keep number of records up to limit
    if (rtBars.size() > limit) {
        rtBars.resize(limit);
    }
    logger_.logDebug("return %d bars.\n", rtBars.size());
    if (!rtBars.empty()) {
        using namespace date;
        logger_.logDebug("%s(%d) - %s(%d)\n", format("%F %T", date::sys_seconds{ std::chrono::seconds{ rtBars.front().time } }).c_str(), rtBars.front().time,
            format("%F %T", date::sys_seconds{ std::chrono::seconds{ rtBars.back().time } }).c_str(), rtBars.back().time);
    }
    // change to asending order, BrokerHistory2 handles asending order
    std::reverse(rtBars.begin(), rtBars.end());
    return result;
}