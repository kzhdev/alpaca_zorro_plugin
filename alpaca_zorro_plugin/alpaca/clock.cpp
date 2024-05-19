#include "stdafx.h"

#include "date/date.h"
#include <sstream>

namespace alpaca{
    __time32_t parseTimeStamp(std::string&& timestamp) {
        using namespace date;

        if (timestamp.size() > 25) {
            // remove digis after seconds
            timestamp = timestamp.substr(0, 19) + timestamp.substr(29);
        }
        std::istringstream infile{ std::move(timestamp) };
        sys_seconds tp;  // This is a system_clock time_point with seconds precision
        infile >> date::parse("%FT%T%Ez", tp);
        return static_cast<__time32_t>(tp.time_since_epoch().count());
    }

    __time32_t parseTimeStamp2(const std::string& timestamp) {
        std::istringstream iss{ timestamp };
        date::sys_seconds tp;
        iss >> date::parse("%FT%T", tp);
        return static_cast<__time32_t>(tp.time_since_epoch().count());
    }

    int32_t getTimeZoneOffset(const std::string& timestamp) {
        using namespace date;
        if (timestamp.size() > 19) {
            // remove digis after seconds
            auto pos = timestamp.rfind('+');
            if (pos != std::string::npos) {
                return atoi(timestamp.substr(pos + 1, 2).c_str());
            }
            else {
                pos = timestamp.rfind('-');
                if (pos >= 19) {
                    return -1 * atoi(timestamp.substr(pos + 1, 2).c_str());
                }
            }
        }
        return 0;
    }

    std::string timeToString(__time32_t time) {
        using namespace date;
        return format("%FT%TZ", date::sys_seconds{ std::chrono::seconds{ time } });
    }

}