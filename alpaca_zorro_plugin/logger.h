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
        Logger(std::string&& name) : name_(std::move(name)) {
#ifdef _DEBUG
            setLevel(LogLevel::L_TRACE);
#endif
            if (level_ != LogLevel::L_OFF) {
                open_log();
            }
        }

        ~Logger() {
            if (log_) {
                fflush(log_);
                fclose(log_);
                log_ = nullptr;
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
        void logInfo(const char* format, Args... args) {
            log(LogLevel::L_INFO, format, std::forward<Args>(args)...);
        }

        template<typename ... Args>
        void logDebug(const char* format, Args... args) {
            log(LogLevel::L_DEBUG, format, std::forward<Args>(args)...);
        }

        template<typename ... Args>
        void logWarning(const char* format, Args... args) {
            log(LogLevel::L_DEBUG, format, std::forward<Args>(args)...);
        }

        template<typename ... Args>
        void logError(const char* format, Args... args) {
            log(LogLevel::L_ERROR, format, std::forward<Args>(args)...);
        }

        template<typename ... Args>
        void logTrace(const char* format, Args... args) {
            log(LogLevel::L_TRACE, format, std::forward<Args>(args)...);
        }

        template<typename ... Args>
        void logTrace2(const char* format, Args... args) {
            log(LogLevel::L_TRACE2, format, std::forward<Args>(args)...);
        }

        template<typename ... Args>
        void logDiag(const char* format, Args... args) {
#ifdef  _DEBUG
            log(LogLevel::L_DEBUG, format, std::forward<Args>(args)...);
#endif //  _DEBUG
        }


        template<typename ... Args>
        inline void log(LogLevel level, const char* format, Args... args) {
            if (level_ >= level) {
                std::time_t t = std::time(nullptr);
                struct tm _tm;
                localtime_s(&_tm, &t);
                char buf[25];
                std::strftime(buf, sizeof(buf), "%F %T", &_tm);

                fprintf(log_, "%s | %s | ", buf, to_string(level));
                fprintf(log_, format, std::forward<Args>(args)...);
                fflush(log_);
            }
        }

    private:
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
#ifdef _DEBUG
        LogLevel level_ = LogLevel::L_DEBUG;
#else
        LogLevel level_ = LogLevel::L_OFF;
#endif
    };
}