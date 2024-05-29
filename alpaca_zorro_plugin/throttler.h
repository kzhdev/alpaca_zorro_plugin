#pragma once

#include <windows.h>
#include <tchar.h>
#include <atomic>
#include <cstdint>
#include <chrono>
#include "logger.h"

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
            HANDLE hMapFile = CreateFileMapping(
                INVALID_HANDLE_VALUE,   // use paging file
                NULL,                   // default security
                PAGE_READWRITE,         // read/write access
                0,                      // maximum object size (high-order DWORD)
                BF_SZ,                  // maximum object size (low-order DWORD)
                (LPCWSTR)shmName.c_str()// name of mapping object
            );

            bool own = false;
            if (hMapFile == NULL) {
                throw std::runtime_error("Failed to create shm. err=" + std::to_string(GetLastError()));
            }

            if (GetLastError() != ERROR_ALREADY_EXISTS) {
                own = true;
            }

            lpvMem_ = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, BF_SZ);
            if (!lpvMem_) {
                throw std::runtime_error("Failed to create shm. err=" + std::to_string(GetLastError()));
            }

            if (own) {
                LOG_DEBUG(" %d create shm %s\n", _getpid(), shmName.c_str());
                data_ = new (lpvMem_) std::atomic<throttler_info>;
            }
            else {
                LOG_DEBUG(" %d open shm %s\n", _getpid(), shmName.c_str());
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

        bool waitForSending(LogLevel logLevel = LogLevel::L_TRACE) const noexcept {
            while (true) {
                auto timestamp = get_timestamp();
                auto data = data_->load(std::memory_order_relaxed);
                if ((timestamp - data.start_timestamp_) >= 1000) {
                    throttler_info new_data;
                    new_data.start_timestamp_ = timestamp;
                    new_data.count_ = 1;
                    if (data_->compare_exchange_strong(data, new_data, std::memory_order_release, std::memory_order_relaxed)) {
                        return true;
                    }
                }

                while (data.count_ < 3) {
                    throttler_info new_data = data;
                    ++new_data.count_;
                    if (data_->compare_exchange_strong(data, new_data, std::memory_order_release, std::memory_order_relaxed)) {
                        return true;
                    }
                    LOG_TRACE("compare_exchange_strong failed. %d -> %d\n", new_data.count_, data.count_);
                }

                if (!BrokerProgress(1)) {
                    return false;
                }
                LOG_TRACE("waitForSending...\n");
                while ((get_timestamp() - data.start_timestamp_) < 1000)
                {
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