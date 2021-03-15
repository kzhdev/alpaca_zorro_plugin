#pragma once

#include <windows.h>
#include <tchar.h>
#include <cstdint>
#include <atomic>
#include "logger.h"
#include "alpaca/client.h"

namespace alpaca {

    /**
    * Zorro currently does not persist order's UUID, after restarting it will use an interger which Zorro maintained internally to
    * query trades. The plugin won't be able to relate Zorro's tradeId to any orders. Fortunately Alpaca support ClientOrderId
    * and able to query orders by ClientOrderId.
    * 
    * Alpaca ClientOrderId must be unique. A ClientOrderId is composed from multiple parts as following:
    * 
    *     ZORRO_TTTTT_YYDDDCNNNN
    *     |     |     | |  | |________ A incremental order counter [0 - 8191]
    *     |     |     | |  |__________ A conflict count from [0 - 9]. See onIdConflit function for detailed information          
    *     |     |     | |_____________ Today's days of year (days since Jan 1 [0 - 365])
    *     |     |     |_______________ Years Since 2020
    *     |     |_____________________ A user specified order text, up to 31 charactors
    *     |___________________________ An identifier to distiguash clientOrderId from other application
    * 
    * 
    * When Zorro starts up, the first instance will request orders from Alpaca and retrieve the last ClientOrderId used.
    * Following order request will increment the NNNN part accordingly.
    * 
    * This is a helper class to generate a unique YYDDDCNNNN part of the ClientOrderId. The id is stored in a shared memory
    * as an atomic object. Therefore multiple Zorro-S instances can send Alpaca order reqeust concurrently
    * 
    * The YYDDDCNNNN is the TradeId turned to Zorro.
    * 
    */
    class ClientOrderIdGenerator {
        static constexpr TCHAR szName[] = TEXT("AlpacaOrderCounter");
        static constexpr uint32_t BF_SZ = 64;
    public:
        ClientOrderIdGenerator(Client& client) {
            HANDLE hMapFile = CreateFileMapping(
                INVALID_HANDLE_VALUE,   // use paging file
                NULL,                   // default security
                PAGE_READWRITE,         // read/write access
                0,                      // maximum object size (high-order DWORD)
                BF_SZ,                  // maximum object size (low-order DWORD)
                szName                  // name of mapping object
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
                // Retrive the last client order id from Alpaca
                int retry = 10;
                int32_t last_order_id = 0;
                int32_t conflict_count = 0;
                auto newBase = getBase() / 100000;
                std::string until;
                while (!last_order_id && retry--) {
                    auto response = client.getOrders(ActionStatus::All, 50, "", until);
                    if (response) {
                        if (!response.content().empty()) {
                            // Returned orders are in desending order
                            for (const auto& order : response.content()) {
                                if (order.client_order_id.substr(0, 6) != "ZORRO_") {
                                    // order placed by other application, try next order
                                    continue;
                                }

                                client.logger().logDebug("cleintOrderId=%s\n", order.client_order_id.c_str());

                                if (order.internal_id) {
                                    int32_t base = order.internal_id / 100000;
                                    if (base != newBase) {
                                        // New day reset counter
                                        last_order_id = 1;
                                        conflict_count = 0;
                                    }
                                    else {
                                        conflict_count = (order.internal_id % 100000) / 10000;
                                        last_order_id = order.internal_id % 10000;
                                    }
                                    break;
                                }
                            }
                            until = response.content().back().canceled_at;
                        }
                    }
                }
                
                next_order_id_ = new (lpvMem_) std::atomic<NextId>{NextId(last_order_id, conflict_count)};
            }
            else {
                next_order_id_ = reinterpret_cast<std::atomic<NextId>*>(lpvMem_);
            }
            auto next = next_order_id_->load(std::memory_order_relaxed);
            client.logger().logInfo("last_order_id_: conflit_cout=%d, id=%d\n", next.conflict_count, next.next_id);
        }

        ~ClientOrderIdGenerator() {
            if (lpvMem_) {
                UnmapViewOfFile(lpvMem_);
                lpvMem_ = nullptr;
            }

            if (hMapFile_) {
                CloseHandle(hMapFile_);
                hMapFile_ = nullptr;
            }
        }

        int32_t nextOrderId() noexcept {
            constexpr const uint32_t mask = 8191;   // Max order count allowed in single day is 8192.
            auto current = next_order_id_->load(std::memory_order_relaxed);
            NextId next = current;
            do {
                next.next_id = (next.next_id + 1) & mask;
            } while (!next_order_id_->compare_exchange_weak(current, next, std::memory_order_release));
            return getBase() + next.conflict_count + next.next_id;
        }
            
        /**
        * Alpaca ClientOrderId must be unique, 
        */
        void onIdConflict() noexcept {
            auto current = next_order_id_->load(std::memory_order_relaxed);
            NextId next = current;
            do {
                next.conflict_count += 10000;
                if (next.conflict_count > 90000) {
                    // can't have 9 conflict in a day
                    break;
                }
            } while (!next_order_id_->compare_exchange_weak(current, next, std::memory_order_release));
        }

    private:
        int32_t getBase() const noexcept {
            constexpr const uint32_t baseYear = 120;
            auto t = std::time(nullptr);
            struct tm now;
            localtime_s(&now, &t);
            return (now.tm_year - baseYear) * 100000000 + now.tm_yday * 100000;
        }

    private:
        HANDLE hMapFile_ = nullptr;
        LPVOID lpvMem_ = nullptr;
        struct NextId {
            int32_t next_id = 0;
            uint32_t conflict_count = 0;

            NextId(int32_t id, uint32_t conflictCount) : next_id(id), conflict_count(conflictCount) {}
        };
        std::atomic<NextId>* next_order_id_ = nullptr;
    };
}