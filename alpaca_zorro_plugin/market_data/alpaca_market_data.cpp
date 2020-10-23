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

    std::string timeframe = "1Min";
    if (nTickMinutes >= 5 && nTickMinutes < 15) {
        timeframe = "5Min";
    }
    else if (nTickMinutes >= 15 && nTickMinutes < 1440) {
        timeframe = "15Min";
    }
    else if (nTickMinutes >= 1440) {
        timeframe = "1D";
    }

    std::string sStart;
    std::string sEnd;
    {
        using namespace date;
        try {
            if (start) {
                sStart = format("%FT%T", date::sys_seconds{ std::chrono::seconds{ start } });
                sStart.append("-00:00");    // add UTC timezone offset
            }

            if (end) {
                sEnd = format("%FT%T", date::sys_seconds{ std::chrono::seconds{ end } });
                sEnd.append("-00:00");  // add UTC timezone offset
            }
        }
        catch (const std::exception& e) {
            assert(false);
        }
    }

    std::stringstream url;
    url << baseUrl_ << "/v1/bars/" << timeframe << "?symbols=" << symbol << "&limit=" << limit
        << (sStart.empty() ? "" : "&start=" + sStart) << (sEnd.empty() ? "" : "&end=" + sEnd);

    logger_.logDebug("--> %s\n", url.str().c_str());
    return request<std::vector<Bar>, AlpacaMarketData>(url.str(), headers_, nullptr);
}