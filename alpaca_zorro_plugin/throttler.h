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
            uint32_t count_ = 0;
        };

        std::atomic<throttler_info>* data_;

        HANDLE hMapFile_ = nullptr;
        LPVOID lpvMem_ = nullptr;

        static constexpr uint32_t BF_SZ = 64;

    public:
        Throttler(const std::string& account) {
            std::string shmName("AlpacaThrottler");
            shmName.append(account);
            hMapFile_ = CreateFileMapping(
                INVALID_HANDLE_VALUE,   // use paging file
                NULL,                   // default security
                PAGE_READWRITE,         // read/write access
                0,                      // maximum object size (high-order DWORD)
                BF_SZ,                  // maximum object size (low-order DWORD)
                (LPCWSTR)shmName.c_str()// name of mapping object
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
                SPDLOG_DEBUG(" {} create shm {}", _getpid(), shmName);
                data_ = new (lpvMem_) std::atomic<throttler_info>;
            }
            else {
                SPDLOG_DEBUG(" {} open shm {}", _getpid(), shmName);
                data_ = reinterpret_cast<std::atomic<throttler_info>*>(lpvMem_);
            }
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

        bool waitForSending() const noexcept {
            if (Config::get().alpacaPaidPlan) {
                return true;
            }

            auto timestamp = get_timestamp();
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

                while (data.count_ < 200) {
                    throttler_info new_data = data;
                    ++new_data.count_;
                    if (data_->compare_exchange_strong(data, new_data, std::memory_order_release, std::memory_order_relaxed)) {
                        return true;
                    }
                    SPDLOG_TRACE("compare_exchange_strong failed. {} -> {}", new_data.count_, data.count_);
                }

                SPDLOG_TRACE("waitForSending...");
                while (((timestamp = get_timestamp()) - data.start_timestamp_) < 60000 && data.count_ >= 200)
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

        uint64_t startTimestamp() const noexcept {
            return data_->load(std::memory_order_relaxed).start_timestamp_;
        }

    private:
        static uint64_t get_timestamp() {
            auto now = std::chrono::system_clock::now();
            return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
        }
    };
}