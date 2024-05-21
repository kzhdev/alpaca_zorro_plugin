#pragma once

#include <cstdio>
#include <cstdint>
#include <ctime>
#include <string>

namespace alpaca {

    extern int(__cdecl* BrokerError)(const char* txt);

    enum LogLevel : uint8_t {
        L_OFF,
        L_ERROR,
        L_WARNING,
        L_INFO,
        L_DEBUG,
        L_TRACE,
    };

    enum LogType : uint8_t
    {
        LT_ACCOUNT = 1,
        LT_BALANCE = 1 << 1,
        LT_POSITION = 1 << 2,
        LT_ORDER = 1 << 3,
        LT_HISTORY = 1 << 4,
        LT_MD = 1 << 5,
        LT_WEB_SOCKET_DATA = 1 << 6,
        LT_MISC = 7,
        LT_ALL_WITHOUT_WEB_SOCKET = LT_MISC | LT_ACCOUNT | LT_BALANCE | LT_POSITION | LT_ORDER | LT_HISTORY | LT_MD,
        LT_ALL = LT_ALL_WITHOUT_WEB_SOCKET | LT_WEB_SOCKET_DATA,
    };

    static constexpr char* to_string(LogLevel level) {
        constexpr char* s_levels[] = {
            "OFF", "ERROR", "WARNING", "INFO", "DEBUG", "TRACE"
        };
        return s_levels[level];
    }

    class Logger {
    public:
        static Logger& instance() {
            static Logger inst;
            return inst;
        }

        void init(std::string&& name) {
            finit();
            name_ = std::move(name);
#ifdef _DEBUG
            setLevel(LogLevel::L_TRACE);
#endif
        }

        LogLevel getLevel() const noexcept { return level_; }

        void setLevel(LogLevel level) noexcept {
            level_ = level;
            unable_to_open_ = false;
        }

        uint8_t getLogType() const noexcept { return type_;  }
        void setLogType(uint8_t type) noexcept { type_ = type; }

        template<typename ... Args>
        inline void log(LogLevel level, LogType type, const char* format, Args... args) {
            if (level_ < level || !(type_ & type)) {
                return;
            }

            if (!log_ && !open_log()) {
                return;
            }
            std::time_t t = std::time(nullptr);
            struct tm _tm;
            localtime_s(&_tm, &t);
            //if (_tm.tm_isdst > 0)
            //{
            //    --_tm.tm_hour;
            //}
            char buf[25];
            std::strftime(buf, sizeof(buf), "%F %T", &_tm);

            fprintf(log_, "%s | %s | ", buf, to_string(level));
            fprintf(log_, format, std::forward<Args>(args)...);
            fflush(log_);
        }

    private:
        Logger() = default;

        ~Logger() {
            finit();
        }

        void finit() {
            if (log_) {
                fflush(log_);
                fclose(log_);
                log_ = nullptr;
            }
        }

        bool open_log() {
            if (unable_to_open_)
            {
                return false;
            }

            std::time_t t = std::time(nullptr);
            struct tm _tm;
            localtime_s(&_tm, &t);
            char buf[25];
            std::strftime(buf, sizeof(buf), "%F_%H%M%S", &_tm);
            std::string log_file = "./Log/" + name_ + "_" + std::string(buf) + ".log";
            log_ = fopen(log_file.c_str(), "w");

            if (!log_) {
                BrokerError(("Failed to open log " + log_file).c_str());
                unable_to_open_ = true;
                return false;
            }
            return true;
        }

    private:
        std::string name_;
        FILE* log_  = nullptr;
        LogLevel level_ = LogLevel::L_OFF;
        uint8_t type_ = LogType::LT_ALL_WITHOUT_WEB_SOCKET;
        bool unable_to_open_ = false;
    };


#ifndef _LOG
#define LOG_DEBUG(format, ...) Logger::instance().log(L_DEBUG, LT_ALL, format, __VA_ARGS__);
#define LOG_DEBUG_EXT(type, format, ...) Logger::instance().log(L_DEBUG, type, format, __VA_ARGS__);
#define LOG_INFO(format, ...) Logger::instance().log(L_INFO, LT_ALL, format, __VA_ARGS__);
#define LOG_INFO_EXT(type, format, ...) Logger::instance().log(L_INFO, type, format, __VA_ARGS__);
#define LOG_WARNING(format, ...) Logger::instance().log(L_WARNING, LT_ALL, format, __VA_ARGS__);
#define LOG_WARNING_EXT(type, format, ...) Logger::instance().log(L_WARNING, type, format, __VA_ARGS__);
#define LOG_ERROR(format, ...) Logger::instance().log(L_ERROR, LT_ALL, format, __VA_ARGS__);
#define LOG_ERROR_EXT(type, format, ...) Logger::instance().log(L_ERROR, type, format, __VA_ARGS__);
#define LOG_TRACE(format, ...) Logger::instance().log(L_TRACE, LT_ALL, format, __VA_ARGS__);
#define LOG_TRACE_EXT(type, format, ...) Logger::instance().log(L_TRACE, type, format, __VA_ARGS__);
#ifdef _DEBUG
#define LOG_DIAG(format, ...) Logger::instance().log(L_DEBUG, LT_ALL, format, __VA_ARGS__);
#define LOG_DIAG_EXT(type, format, ...) Logger::instance().log(L_DEBUG, type, format, __VA_ARGS__);
#else
#define LOG_DIAG(format, ...)
#define LOG_DIAG_EXT(type, format, ...)
#endif
#endif
}