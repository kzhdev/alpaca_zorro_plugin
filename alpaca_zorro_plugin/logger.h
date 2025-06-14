#pragma once

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>

namespace alpaca {

    inline spdlog::level::level_enum toLogLevel(uint8_t level) {
        switch (level) {
            case 0: return spdlog::level::off;
            case 1: return spdlog::level::err;
            case 2: return spdlog::level::warn;
            case 3: return spdlog::level::info;
            case 4: return spdlog::level::debug;
            case 5: return spdlog::level::trace;
            default: return spdlog::level::off;
        }
    }
}