#pragma once

#include <slick_logger/logger.hpp>

namespace alpaca {

    inline slick_logger::LogLevel toLogLevel(uint8_t level) {
        switch (level) {
            case 0: return slick_logger::LogLevel::L_OFF;
            case 1: return slick_logger::LogLevel::L_ERROR;
            case 2: return slick_logger::LogLevel::L_WARN;
            case 3: return slick_logger::LogLevel::L_INFO;
            case 4: return slick_logger::LogLevel::L_DEBUG;
            case 5: return slick_logger::LogLevel::L_TRACE;
            default: return slick_logger::LogLevel::L_OFF;
        }
    }

    inline std::string to_string(uint8_t level) {
        switch (level) {
            case 0: return "OFF";
            case 1: return "ERROR";
            case 2: return "WARN";
            case 3: return "INFO";
            case 4: return "DEBUG";
            case 5: return "TRACE";
            default: return "UNKNOWN";
        }
    }
}