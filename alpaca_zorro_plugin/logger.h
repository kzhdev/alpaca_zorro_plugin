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
        L_TRACE2,
    };

    static constexpr char* to_string(LogLevel level) {
        constexpr char* s_levels[] = {
            "OFF", "ERROR", "WARNING", "INFO", "DEBUG", "TRACE", "TRACE2"
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
            if (level_ != LogLevel::L_OFF) {
                open_log();
            }
        }

        LogLevel getLevel() const noexcept { return level_; }

        void setLevel(LogLevel level) noexcept {
            level_ = level;
            if (level != LogLevel::L_OFF && !log_) {
                open_log();
            }
        }

        template<typename ... Args>
        inline void log(LogLevel level, const char* format, Args... args) {
            std::time_t t = std::time(nullptr);
            struct tm _tm;
            localtime_s(&_tm, &t);
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

        void open_log() {
            std::time_t t = std::time(nullptr);
            struct tm _tm;
            localtime_s(&_tm, &t);
            char buf[25];
            std::strftime(buf, sizeof(buf), "%F_%H%M%S", &_tm);
            std::string log_file = "./Log/" + name_ + "_" + std::string(buf) + ".log";
            log_ = fopen(log_file.c_str(), "w");

            if (!log_) {
                BrokerError(("Failed to open log " + log_file).c_str());
                level_ = LogLevel::L_OFF;
            }
        }

    private:
        std::string name_;
        FILE* log_  = nullptr;
        LogLevel level_ = LogLevel::L_OFF;
    };


#ifndef _LOG
#define _LOG(level, format, ...)           \
{\
    auto& logger = Logger::instance();              \
    auto lvl = logger.getLevel();                 \
    if (lvl >= level) {   \
        logger.log(level, format, __VA_ARGS__);      \
    }\
}

#define LOG_DEBUG(format, ...) _LOG(L_DEBUG, format, __VA_ARGS__);
#define LOG_INFO(format, ...) _LOG(L_INFO, format, __VA_ARGS__);
#define LOG_WARNING(format, ...) _LOG(L_WARNING, format, __VA_ARGS__);
#define LOG_ERROR(format, ...) _LOG(L_ERROR, format, __VA_ARGS__);
#define LOG_TRACE(format, ...) _LOG(L_TRACE, format, __VA_ARGS__);
#define LOG_TRACE2(format, ...) _LOG(L_TRACE, format, __VA_ARGS__);
#ifdef _DEBUG
#define LOG_DIAG(format, ...) _LOG(L_DEBUG, format, __VA_ARGS__);
#else
#define LOG_DIAG(format, ...)
#endif
#endif
}