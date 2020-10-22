#include "stdafx.h"
#include "market_data/polygon.h"
#include "date/date.h"

using namespace alpaca;

Response<std::vector<Bar>> Polygon::getBars(
    const std::string& symbol,
    const __time32_t start,
    const __time32_t end,
    const int nTickMinutes,
    const uint32_t limit) const {
    std::stringstream url;
    if (!nTickMinutes) {
        // tick data
        url << baseUrl_ << "/v2/ticks/stocks/nbbo/" << symbol;
        logger_.logDebug("get tick data not implemented");
        return Response<std::vector<Bar>>();
    }
    else {
        url << baseUrl_ << "/v2/aggs/ticker/" << symbol << "/range/" << nTickMinutes << "/minute";
        {
            using namespace date;
            try {
                url << "/" << format("%F", date::sys_seconds{ std::chrono::seconds{ start } });

                url << "/" << format("%F", date::sys_seconds{ std::chrono::seconds{ end } });
            }
            catch (const std::exception& e) {
                assert(false);
                return Response<std::vector<Bar>>(1, "invalid time");
            }
        }
        url << "?sort=asc";
        logger_.logDebug("--> %s\n", url.str().c_str());
        url << "&" << apiKey_;
    }

    auto response = request<std::vector<Bar>, Polygon>(url.str());

    auto& bars = response.content();
    size_t nExclude = 0;
    logger_.logDebug("%d bars downloaded\n", bars.size());
    for (auto it = bars.rbegin(); it != bars.rend(); ++it) {
        if ((*it).time > end) {
            ++nExclude;
        }

        if ((*it).time < start) {
            break;
        }
    }

    if (nExclude) {
        // remove record exceeds the end time
        logger_.logDebug("Remove %d records at the end\n", nExclude);
        bars.resize(bars.size() - nExclude);
        assert(bars.back().time <= end);
    }

    if (bars.size() > limit) {
        // make sure only required number bars are given, otherwise zorro will crash
        auto itEnd = bars.cbegin() + (bars.size() - limit);
        auto it = bars.cbegin();
        bars.erase(it, itEnd);
    }
    assert(bars.size() <= limit);
    return response;
}