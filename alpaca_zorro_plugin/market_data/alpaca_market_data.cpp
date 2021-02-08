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
        sStart = format("%FT%T", date::sys_seconds{std::chrono::seconds{start}});
        sStart.append("-00:00");  // add UTC timezone offset
      }

      if (end) {
        sEnd = format("%FT%T", date::sys_seconds{std::chrono::seconds{end}});
        sEnd.append("-00:00");  // add UTC timezone offset
      }
    } catch (const std::exception& e) {
      assert(false);
    }
  }

  if (standard) {

    std::stringstream url;
    url << baseUrl_ << "/v1/bars/" << timeframe << "?symbols=" << symbol
        << "&limit=" << limit << (sStart.empty() ? "" : "&start=" + sStart)
        << (sEnd.empty() ? "" : "&end=" + sEnd);

    logger_.logDebug("--> %s\n", url.str().c_str());
    return request<std::vector<Bar>, AlpacaMarketData>(url.str(), headers_, nullptr);
  }


  uint32_t n = 0;
  uint32_t span = 1;
  if ((nTickMinutes % 1440) == 0) {
    n = nTickMinutes / 1440;
    timeframe = "1D";
    span = 1440;
  } else if ((nTickMinutes % 15) == 0) {
    n = nTickMinutes / 15;
    timeframe = "15Min";
    span = 15;
  } else if ((nTickMinutes % 5) == 0) {
    n = nTickMinutes / 5;
    timeframe = "5Min";
    span = 5;
  } else {
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
      retrieved = request<std::vector<Bar>, AlpacaMarketData>(url.str(), headers_, nullptr);
      logger_.logDebug("%d bars downloaded\n", retrieved.content().size());
      if (!retrieved) {
        retrieved.content().swap(response.content());
        return retrieved;
      }
      nLimit = 0;
    } else {
      // limit exceeds 1000, need to download multiple times

      std::stringstream url;
      url << baseUrl_ << "/v1/bars/" << timeframe << "?symbols=" << symbol
          << "&limit=" << 1000 << (sStart.empty() ? "" : "&start=" + sStart)
          << (sEnd.empty() ? "" : "&end=" + sEnd);

      logger_.logDebug("--> %s\n", url.str().c_str());
      retrieved = request<std::vector<Bar>, AlpacaMarketData>(url.str(), headers_, nullptr);
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

      sStart = date::format("%FT%T", date::sys_seconds{std::chrono::seconds{retrieved.content().back().time + 60 * span}});
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
      } else {
        rtBar.high_price = std::max<double>(rtBar.high_price, bar.high_price);
        rtBar.low_price = std::min<double>(rtBar.low_price, bar.low_price);
        rtBar.close_price = bar.close_price;
        rtBar.volume += bar.volume;
      }
      ++i;
    }

    if (!exceeded_end && (i % n) == 0) {
      response.content().push_back(rtBar);
    } else if (exceeded_end) {
      break;
    }
  }
  return response;
}