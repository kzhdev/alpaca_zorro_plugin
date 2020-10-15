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

    class Logger {
    public:
        Logger() {
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
            if (level_ >= LogLevel::L_INFO) {
                log("INFO", format, std::forward<Args>(args)...);
            }
        }

        template<typename ... Args>
        void logDebug(const char* format, Args... args) {
            if (level_ >= LogLevel::L_DEBUG) {
                log("DEBUG", format, std::forward<Args>(args)...);
            }
        }

        template<typename ... Args>
        void logWarning(const char* format, Args... args) {
            if (level_ >= LogLevel::L_WARNING) {
                log("WARN", format, std::forward<Args>(args)...);
            }
        }

        template<typename ... Args>
        void logError(const char* format, Args... args) {
            if (level_ >= LogLevel::L_ERROR) {
                log("ERROR", format, std::forward<Args>(args)...);
            }
        }

        template<typename ... Args>
        void logTrace(const char* format, Args... args) {
            if (level_ >= LogLevel::L_TRACE) {
                log("TRACE", format, std::forward<Args>(args)...);
            }
        }

        template<typename ... Args>
        void logTrace2(const char* format, Args... args) {
            if (level_ >= LogLevel::L_TRACE2) {
                log("TRACE2", format, std::forward<Args>(args)...);
            }
        }

    private:
        void open_log() {
            std::time_t t = std::time(nullptr);
            char buf[25];
            std::strftime(buf, sizeof(buf), "%F_%H%M%S", std::localtime(&t));
            std::string log_file = "./Log/alpaca_" + std::string(buf) + ".log";
            log_ = fopen(log_file.c_str(), "w");

            if (!log_) {
                BrokerError(("Failed to open log " + log_file).c_str());
                level_ = LogLevel::L_OFF;
            }
        }

        template<typename ... Args>
        void log(const char* level, const char* format, Args... args) {
            std::time_t t = std::time(nullptr);
            char buf[25];
            std::strftime(buf, sizeof(buf), "%F %T", std::localtime(&t));

            fprintf(log_, "%s | %s | ", buf, level);
            fprintf(log_, format, std::forward<Args>(args)...);
            fflush(log_);
        }

    private:
        FILE* log_  = nullptr;
#ifdef _DEBUG
        LogLevel level_ = LogLevel::L_DEBUG;
#else
        LogLevel level_ = LogLevel::L_OFF;
#endif
    };
}