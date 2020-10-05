#include "stdafx.h"
#include "alpaca/clock.h"

#include "alpaca/json.h"
#include "date/date.h"
#include <sstream>

namespace alpaca {
    __time32_t parseTimeStamp(std::string&& timestamp) {
        using namespace date;
        std::istringstream infile{ std::move(timestamp) };
        sys_seconds tp;  // This is a system_clock time_point with seconds precision
        infile >> date::parse("%FT%T%Ez", tp);
        return tp.time_since_epoch().count();
    }

    void Clock::fromJSON(const rapidjson::Document& d) {
        is_open = parse<bool>(d, "is_open");
        next_close = parseTimeStamp(parse<std::string>(d, "next_close"));
        next_open = parseTimeStamp(parse<std::string>(d, "next_open"));
        timestamp = parseTimeStamp(parse<std::string>(d, "timestamp"));
    }
} // namespace alpaca
