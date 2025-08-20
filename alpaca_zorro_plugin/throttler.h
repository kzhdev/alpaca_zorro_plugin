#pragma once

#include <tchar.h>
#include <atomic>
#include <cstdint>
#include <chrono>
#include "logger.h"
#include "config.h"

namespace alpaca {
    extern int(__cdecl* BrokerError)(const char* txt);
    extern int(__cdecl* BrokerProgress)(const int percent);

    class Throttler {
        struct throttler_info {
            uint64_t start_timestamp_ = 0;
            __time32_t next_balance_request_time_ = 0;
            __time32_t next_clock_request_time_ = 0;
            uint32_t count_ = 0;
        };

        std::atomic<throttler_info>* data_;

        HANDLE hMapFile_ = nullptr;
        LPVOID lpvMem_ = nullptr;

        static constexpr uint32_t BF_SZ = 64;

        uint32_t threshold_ = 200;
        uint32_t throttle_breach_count_ = 0;
        bool enable_throttle_ = false;
        bool is_live_mode_ = false;

    public:
        Throttler(const std::string& account, bool is_live_mode) {
            std::string shmName("AlpacaThrottler");
            shmName.append(account);
            
            // Convert to wide string properly
            int wideSize = MultiByteToWideChar(CP_UTF8, 0, shmName.c_str(), -1, NULL, 0);
            std::wstring wideShmName(wideSize, 0);
            MultiByteToWideChar(CP_UTF8, 0, shmName.c_str(), -1, &wideShmName[0], wideSize);
            
            hMapFile_ = CreateFileMapping(
                INVALID_HANDLE_VALUE,   // use paging file
                NULL,                   // default security
                PAGE_READWRITE,         // read/write access
                0,                      // maximum object size (high-order DWORD)
                BF_SZ,                  // maximum object size (low-order DWORD)
                wideShmName.c_str()     // name of mapping object
            );

            bool own = false;
            if (hMapFile_ == NULL) {
                throw std::runtime_error("Failed to create shm. err=" + std::to_string(GetLastError()));
            }

            if (GetLastError() != ERROR_ALREADY_EXISTS) {
                own = true;
            }

            lpvMem_ = MapViewOfFile(hMapFile_, FILE_MAP_ALL_ACCESS, 0, 0, BF_SZ);
            if (!lpvMem_) {
                throw std::runtime_error("Failed to create shm. err=" + std::to_string(GetLastError()));
            }

            if (own) {
                SPDLOG_DEBUG(" {} create shm {} sz: {} {}", _getpid(), shmName, sizeof(std::atomic<throttler_info>), sizeof(DWORD));
                data_ = new (lpvMem_) std::atomic<throttler_info>;
            }
            else {
                SPDLOG_DEBUG(" {} open shm sz: {} {}", _getpid(), shmName, sizeof(std::atomic<throttler_info>));
                data_ = reinterpret_cast<std::atomic<throttler_info>*>(lpvMem_);
            }

            enable_throttle_ = !Config::get().alpacaPaidPlan;
            threshold_ = !Config::get().alpacaPaidPlan ? 200 : 10000;
            is_live_mode_ = is_live_mode;
        }

        ~Throttler() {
            if (lpvMem_) {
                UnmapViewOfFile(lpvMem_);
                lpvMem_ = nullptr;
            }

            if (hMapFile_) {
                CloseHandle(hMapFile_);
                hMapFile_ = nullptr;
            }
        }

        void enableThrottle(bool enable) noexcept {
            if (enable ^ enable_throttle_) {
                enable_throttle_ = enable;
                if (enable) {
                    SPDLOG_INFO("Throttler enabled");
                }
                else {
                    SPDLOG_INFO("Throttler disabled");
                }
            }
            else if (enable) {
                if ((threshold_ > 100) && (++throttle_breach_count_ > 10)) {
                    if (Config::get().alpacaPaidPlan) {
                        threshold_ -= 100;
                    } else {
                        threshold_ -= 10;
                    }
                    throttle_breach_count_ = 0;
                    SPDLOG_WARN("Throttler threshold reduced to {} due to repeated throttle breaches", threshold_);
                }
            }
        }

        bool waitForSending(uint64_t timestamp = get_timestamp()) const noexcept {
            if (!enable_throttle_) {
                return true;
            }

            while (true) {
                auto data = data_->load(std::memory_order_relaxed);
                if ((timestamp - data.start_timestamp_) >= 60000) {
                    throttler_info new_data;
                    new_data.start_timestamp_ = timestamp;
                    new_data.count_ = 1;
                    if (data_->compare_exchange_strong(data, new_data, std::memory_order_release, std::memory_order_relaxed)) {
                        return true;
                    }
                }

                while (data.count_ < threshold_) {
                    throttler_info new_data = data;
                    ++new_data.count_;
                    if (data_->compare_exchange_strong(data, new_data, std::memory_order_release, std::memory_order_relaxed)) {
                        return true;
                    }
                    SPDLOG_TRACE("compare_exchange_strong failed. {} -> {}", new_data.count_, data.count_);
                }

                SPDLOG_TRACE("waitForSending...");
                while (((timestamp = get_timestamp()) - data.start_timestamp_) < 60000 && data.count_ >= threshold_)
                {
                    if (!BrokerProgress(1)) {
                        return false;
                    }
                    std::this_thread::yield();
                }
            }
        }

        uint32_t getCount() const noexcept {
            return data_->load(std::memory_order_relaxed).count_;
        }

        __time32_t startTimestamp() const noexcept {
            return data_->load(std::memory_order_relaxed).start_timestamp_;
        }

        __time32_t nextBalanceRequestTime() const noexcept {
            return data_->load(std::memory_order_relaxed).next_balance_request_time_;
        }

        void setNextBalanceRequestTime(__time32_t time) noexcept {
            auto info = data_->load(std::memory_order_relaxed);
            throttler_info new_info;
            do {
                if (info.next_balance_request_time_ >= time) {
                    return;
                }
                new_info = info;
                new_info.next_balance_request_time_ = time;
            } while (!data_->compare_exchange_weak(info, new_info, std::memory_order_release, std::memory_order_relaxed));
        }

        __time32_t nextClockRequestTime() const noexcept {
            return data_->load(std::memory_order_relaxed).next_clock_request_time_;
        }

        void setNextClockRequestTime(__time32_t time) noexcept {
            auto info = data_->load(std::memory_order_relaxed);
            throttler_info new_info;
            do {
                if (info.next_clock_request_time_ >= time) {
                    return;
                }
                new_info = info;
                new_info.next_clock_request_time_ = time;
            } while (!data_->compare_exchange_weak(info, new_info, std::memory_order_release, std::memory_order_relaxed));
        }

    private:
        static uint64_t get_timestamp() {
            auto now = std::chrono::system_clock::now();
            return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
        }
    };
}