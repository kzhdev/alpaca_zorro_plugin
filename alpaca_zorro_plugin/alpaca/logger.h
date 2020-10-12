#pragma once

#include <cstdio>
#include <cstdint>
#include <ctime>
#include <string>

namespace alpaca {

    enum LogLevel : uint8_t {
        L_ERROR,
        L_WARNING,
        L_INFO,
        L_DEBUG,
        L_TRACE,
    };

    class Logger {
    public:
        Logger() {
            std::time_t t = std::time(nullptr);
            char buf[25];
            std::strftime(buf, sizeof(buf), "%F_%H%M%S", std::localtime(&t));
            log_ = fopen(("./Log/alpaca_" + std::string(buf) + ".log").c_str(), "w");
        }

        ~Logger() {
            if (log_) {
                fflush(log_);
                fclose(log_);
                log_ = nullptr;
            }
        }

#ifdef _DEBUG
        LogLevel level = LogLevel::L_TRACE;
#else
        LogLevel level = LogLevel::L_INFO;
#endif

        template<typename ... Args>
        void logInfo(const char* format, Args... args) {
            if (level >= LogLevel::L_INFO) {
                log("INFO", format, std::forward<Args>(args)...);
            }
        }

        template<typename ... Args>
        void logDebug(const char* format, Args... args) {
            if (level >= LogLevel::L_DEBUG) {
                log("DEBUG", format, std::forward<Args>(args)...);
            }
        }

        template<typename ... Args>
        void logWarning(const char* format, Args... args) {
            if (level >= LogLevel::L_WARNING) {
                log("WARN", format, std::forward<Args>(args)...);
            }
        }

        template<typename ... Args>
        void logError(const char* format, Args... args) {
            if (level >= LogLevel::L_ERROR) {
                log("ERROR", format, std::forward<Args>(args)...);
            }
        }

        template<typename ... Args>
        void logTrace(const char* format, Args... args) {
            if (level >= LogLevel::L_TRACE) {
                log("TRACE", format, std::forward<Args>(args)...);
            }
        }

    private:
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
    };
}