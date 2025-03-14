/********************************************************************************
 * Copyright (c) 2020-0201, SlickTech
 * All rights reserved
 *
 * This file is part of the SlickQueue. Redistribution and use in source and
 * binary forms, with or without modification, are permitted exclusively under
 * the terms of the MIT license which is available at
 * https://github.com/SlickTech/slick_queue/blob/main/LICENSE
 *
 ********************************************************************************/

#pragma once

#include <cstdint>
#include <atomic>
#include <stdexcept>
#include <string>
#include <cassert>

#if defined(_MSC_VER)
#include <windows.h>
#include <tchar.h>
#endif

namespace slick {

template<typename T>
class SlickQueue {
    struct slot {
        std::atomic_uint_fast64_t data_index{ 0 };
        uint32_t size = 1;
    };

    uint32_t size_;
    uint32_t mask_;
    T* data_ = nullptr;
    slot* control_ = nullptr;
    std::atomic_uint_fast64_t* reserved_ = nullptr;
    std::atomic_uint_fast64_t reserved_local_{ 0 };
    bool own_ = false;
    bool use_shm_ = false;
#if defined(_MSC_VER)
    HANDLE hMapFile_ = nullptr;
    LPVOID lpvMem_ = nullptr;
#endif

public:
    SlickQueue(uint32_t size, const char* const shm_name = nullptr)
        : size_(size + 1024)    // add some buffer at the end
        , mask_(size - 1)
        , data_(shm_name ? nullptr : new T[size_])   
        , control_(shm_name ? nullptr : new slot[size_])
        , reserved_(shm_name ? nullptr : &reserved_local_)
        , own_(shm_name == nullptr)
        , use_shm_(shm_name != nullptr)
    {
        assert((size && !(size & (size - 1))) && "size must power of 2");
        if (shm_name) {
            allocate_shm_data(shm_name, false);
        }

        if (own_) {
            // invalidate first slot
            control_[0].data_index.store(-1, std::memory_order_relaxed);
        }
    }

    SlickQueue(const char* const shm_name)
        : own_(false)
        , use_shm_(true)
    {
        allocate_shm_data(shm_name, true);
    }

    virtual ~SlickQueue() noexcept {
#if defined(_MSC_VER)
        if (lpvMem_) {
            UnmapViewOfFile(lpvMem_);
            lpvMem_ = nullptr;
        }

        if (hMapFile_) {
            CloseHandle(hMapFile_);
            hMapFile_ = nullptr;
        }

        if (!use_shm_) {
            delete[] data_;
            data_ = nullptr;

            delete[] control_;
            control_ = nullptr;
        }
#endif
    }

    bool own_buffer() const noexcept { return own_; }
    bool use_shm() const noexcept { return use_shm_; }

    uint64_t initial_reading_index() const noexcept {
        return reserved_->load(std::memory_order_relaxed);
    }

    uint64_t reserve(uint32_t n = 1) noexcept {
        return reserved_->fetch_add(n, std::memory_order_acq_rel);
    }

    T* operator[] (uint64_t index) noexcept {
        return &data_[index & mask_];
    }

    const T* operator[] (uint64_t index) const noexcept {
        return &data_[index & mask_];
    }

    void publish(uint64_t index, uint32_t n = 1) noexcept {
        auto& slot = control_[index & mask_];
        slot.size = n;
        slot.data_index.store(index, std::memory_order_release);
    }

    std::pair<T*, uint32_t> read(uint64_t& read_index) noexcept {
        auto& slot = control_[read_index & mask_];
        auto index = slot.data_index.load(std::memory_order_relaxed);
        if (reserved_->load(std::memory_order_relaxed) < index) {
            // queue has been reset
            read_index = 0;
        }

        if (index == -1 || index < read_index) {
            // data not ready yet
            return std::make_pair(nullptr, 0);
        }

        auto& data = data_[read_index & mask_];
        read_index = slot.data_index + slot.size;
        return std::make_pair(&data, slot.size);
    }

    /**
    * Read the last published data in the queue
    */
    T* read_last() noexcept {
        auto reserved = reserved_->load(std::memory_order_relaxed);
        if (reserved == 0) {
            return nullptr;
        }
        auto index = reserved - 1;

        // find last published data
        auto begin = index & mask_;
        while (control_[index & mask_].data_index.load(std::memory_order_relaxed) != index)
        {
            --index;
            if ((index & mask_) == begin    // looped entire queue 
                || index >= reserved)       // passed 0
            {
                return nullptr;
            }
        }
        return &data_[index & mask_];
    }

    void reset() noexcept {
        // invalidate first slot
        control_[0].data_index.store(-1, std::memory_order_release);
        reserved_->store(0, std::memory_order_release);
    }

private:

#if defined(_MSC_VER)
    void allocate_shm_data(const char* const shm_name, bool open_only) {
        SIZE_T BF_SZ;
        HANDLE hMapFile = NULL;
        if (open_only) {
            hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, (LPCWSTR)shm_name);
            own_ = false;
            auto err = GetLastError();
            if (hMapFile == NULL) {
                throw std::runtime_error("Failed to open shm. err=" + std::to_string(err));
            }

            void* lpvMem = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 64);
            if (!lpvMem) {
                auto err = GetLastError();
                throw std::runtime_error("Failed to map shm. err=" + std::to_string(err));
            }
            mask_ = *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(lpvMem) + sizeof(std::atomic_uint_fast64_t)) - 1;
            size_ = mask_ + 1025;
            BF_SZ = 64 + sizeof(slot) * size_ + sizeof(T) * size_;
            UnmapViewOfFile(lpvMem_);
            lpvMem_ = nullptr;
        }
        else {
            BF_SZ = 64 + sizeof(slot) * size_ + sizeof(T) * size_;

            hMapFile = CreateFileMapping(
                INVALID_HANDLE_VALUE,               // use paging file
                NULL,                               // default security
                PAGE_READWRITE,                     // read/write access
                0,                                  // maximum object size (high-order DWORD)
                BF_SZ,                              // maximum object size (low-order DWORD)
                (LPCWSTR)shm_name                   // name of mapping object
            );

            own_ = false;
            auto err = GetLastError();
            if (hMapFile == NULL) {
                throw std::runtime_error("Failed to create shm. err=" + std::to_string(err));
            }

            if (err != ERROR_ALREADY_EXISTS) {
                own_ = true;
            }
        }

        void* lpvMem = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, BF_SZ);
        if (!lpvMem) {
            auto err = GetLastError();
            throw std::runtime_error("Failed to map shm. err=" + std::to_string(err));
        }

        if (own_) {
            reserved_ = new (lpvMem) std::atomic_uint_fast64_t{ 0 };
            *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(lpvMem) + sizeof(std::atomic_uint_fast64_t)) = mask_ + 1;
            control_ = new ((uint8_t*)lpvMem + 64) slot[size_];
            data_ = new ((uint8_t*)lpvMem + 64 + sizeof(slot) * size_) T[size_];
        }
        else {
            reserved_ = reinterpret_cast<std::atomic_uint_fast64_t*>(lpvMem);
            control_ = reinterpret_cast<slot*>((uint8_t*)lpvMem + 64);
            data_ = reinterpret_cast<T*>((uint8_t*)lpvMem + 64 + sizeof(slot) * size_);
        }
    }
#else
    void allocateShmData(const char* const shm_name) noexcept {
    }
#endif

};

}
