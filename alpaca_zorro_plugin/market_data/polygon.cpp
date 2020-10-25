#include "stdafx.h"
#include "market_data/polygon.h"
#include "date/date.h"

using namespace alpaca;

#define TWO_DAYS_IN_SEC 172800  // Assume 8 hour an day
#define DAY_IN_SEC 86400

Response<std::vector<Bar>> Polygon::getBars(
    const std::string& symbol,
    __time32_t start,
    __time32_t end,
    const int nTickMinutes,
    const uint32_t limit) const {

    if (end == 0) {
        end = std::time(nullptr);
    }

    __time32_t t_start;
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
            t_start = t_end - TWO_DAYS_IN_SEC;
            url << "/" << format("%F", date::sys_seconds{ std::chrono::seconds{ t_start } });
            if (t_start < start) {
                t_start = start;
            }

            url << "/" << format("%F", date::sys_seconds{ std::chrono::seconds{ t_end } });
        }
        catch (const std::exception& e) {
            assert(false);
            return Response<std::vector<Bar>>(1, "invalid time");
        }
        url << "?sort=desc";    // in desending order
        logger_.logDebug("--> %s\n", url.str().c_str());
        url << "&" << apiKey_;

        auto response = request<std::vector<Bar>, Polygon>(url.str());

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

        // remove record passed end time
        auto it = bars.begin();
        for (; it != bars.cend();) {
            if ((__time32_t)(*it).time <= upperBound) {
                break;
            }
            ++it;
        }

        if (it != bars.cbegin()) {
            bars.erase(bars.cbegin(), it);
        }

        rtBars.insert(rtBars.end(), bars.begin(), bars.end());
        if (!rtBars.empty()) {
            upperBound = rtBars.back().time - 1;
        }
        else {
            upperBound = t_start;
        }
        t_end = t_start - DAY_IN_SEC;
    } while (rtBars.size() < limit && t_start > start);

    // remove order older than start
    auto it = rtBars.end();
    while (it != rtBars.begin()) {
        --it;
        if ((*it).time >= start) {
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