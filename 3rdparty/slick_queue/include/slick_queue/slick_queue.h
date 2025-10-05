/********************************************************************************
 * Copyright (c) 2020-2025 SlickQuant
 * All rights reserved
 *
 * This file is part of the SlickQueue. Redistribution and use in source and
 * binary forms, with or without modification, are permitted exclusively under
 * the terms of the MIT license which is available at
 * https://github.com/SlickQuant/slick_queue/blob/main/LICENSE
 *
 ********************************************************************************/

#pragma once

#include <cstdint>
#include <atomic>
#include <stdexcept>
#include <string>
#include <cassert>

#if defined(_MSC_VER)
#ifndef NOMINMAX
#define NOMINMAX
#undef min  // to avoid conflicts with std::min
#undef max  // to avoid conflicts with std::max
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#else
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#endif

namespace slick {

/**
 * @brief A lock-free multi-producer multi-consumer queue with optional shared memory support.
 * 
 * This queue allows a multiple producer thread to write data and a multiple consumer thread to read data concurrently without locks.
 * It can optionally use shared memory for inter-process communication.
 * 
 * @tparam T The type of elements stored in the queue.
 */
template<typename T>
class SlickQueue {
    struct slot {
        std::atomic_uint_fast64_t data_index{ std::numeric_limits<uint64_t>::max() };
        uint32_t size = 1;
    };

    struct reserved_info {
        uint_fast64_t index_ = 0;
        uint_fast32_t size_ = 0;
    };

    uint32_t size_;
    uint32_t mask_;
    T* data_ = nullptr;
    slot* control_ = nullptr;
    std::atomic<reserved_info>* reserved_ = nullptr;
    std::atomic<reserved_info> reserved_local_;
    bool own_ = false;
    bool use_shm_ = false;
#if defined(_MSC_VER)
    HANDLE hMapFile_ = nullptr;
    LPVOID lpvMem_ = nullptr;
#else
    int shm_fd_ = -1;
    void* lpvMem_ = nullptr;
    std::string shm_name_;
#endif

public:
    /**
     * @brief Construct a new SlickQueue object
     * 
     * @param size The size of the queue, must be a power of 2.
     * @param shm_name The name of the shared memory segment. If nullptr, the queue will use local memory.
     * 
     * @throws std::runtime_error if shared memory allocation fails.
     * @throws std::invalid_argument if size is not a power of 2.
     */
    SlickQueue(uint32_t size, const char* const shm_name = nullptr)
        : size_(size)
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
    }

    /**
     * @brief Open an existing SlickQueue in shared memory
     * 
     * @param shm_name The name of the shared memory segment.
     * 
     * @throws std::runtime_error if shared memory allocation fails or the segment does not exist.
     */
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
#else
        if (lpvMem_) {
            auto BF_SZ = static_cast<size_t>(64 + sizeof(slot) * size_ + sizeof(T) * size_);
            munmap(lpvMem_, BF_SZ);
            lpvMem_ = nullptr;
        }
        if (shm_fd_ != -1) {
            close(shm_fd_);
            shm_fd_ = -1;
        }
        if (own_ && !shm_name_.empty()) {
	    shm_unlink(shm_name_.c_str());
	}
#endif

        if (!use_shm_) {
            delete[] data_;
            data_ = nullptr;

            delete[] control_;
            control_ = nullptr;
        }
    }

    /**
     * @brief Check if the queue owns the memory buffer
     * @return true if the queue owns the memory buffer, false otherwise
     */
    bool own_buffer() const noexcept { return own_; }

    /**
     * @brief Check if the queue uses shared memory
     * @return true if the queue uses shared memory, false otherwise
     */
    bool use_shm() const noexcept { return use_shm_; }

    /**
     * @brief Get the size of the queue
     * @return Size of the queue
     */
    constexpr uint32_t size() const noexcept { return size_; }

    /**
     * @brief Get the initial reading index, which is 0 if the queue is newly created or the current writing index if opened existing 
     * @return Initial reading index
     */
    uint64_t initial_reading_index() const noexcept {
        return reserved_->load(std::memory_order_relaxed).index_;
    }

    /**
     * @brief Reserve space in the queue for writing
     * @param n Number of slots to reserve, default is 1
     * @return The starting index of the reserved space
     */
    uint64_t reserve(uint32_t n = 1) {
        if (n > size_) [[unlikely]] {
            throw std::runtime_error(std::format("required size {} > queue size {}", n, size_));
        }
        auto reserved = reserved_->load(std::memory_order_relaxed);
        reserved_info next;
        uint64_t index;
        bool buffer_wrapped = false;
        do {
            buffer_wrapped = false;
            next = reserved;
            index = reserved.index_;
            auto idx = index & mask_;
            if ((idx + n) > size_) {
                // if there is no enough buffer left, start from the beginning
                index += size_ - idx;
                next.index_ = index + n;
                next.size_ = n;
                buffer_wrapped = true;
            }
            else {
                next.index_ += n;
                next.size_ = n;
            }
        } while(!reserved_->compare_exchange_weak(reserved, next, std::memory_order_release, std::memory_order_relaxed));
        if (buffer_wrapped) {
            // queue wrapped, set current slock.data_index to the reserved index to let the reader
            // know the next available data is in different slot.
            auto& slot = control_[reserved.index_ & mask_];
            slot.size = n;
            slot.data_index.store(index, std::memory_order_release);
        }
        return index;
    }

    /**
     * @brief Access the reserved space for writing
     * @param index The index returned by reserve()
     * @return Pointer to the reserved space
     */
    T* operator[] (uint64_t index) noexcept {
        return &data_[index & mask_];
    }

    /**
     * @brief Access the reserved space for writing (const version)
     * @param index The index returned by reserve()
     * @return Pointer to the reserved space
     */
    const T* operator[] (uint64_t index) const noexcept {
        return &data_[index & mask_];
    }

    /**
     * @brief Publish the data written in the reserved space
     * @param index The index returned by reserve()
     * @param n Number of slots to publish, default is 1
     */
    void publish(uint64_t index, uint32_t n = 1) noexcept {
        auto& slot = control_[index & mask_];
        slot.size = n;
        slot.data_index.store(index, std::memory_order_release);
    }

    /**
     * @brief Read data from the queue
     * @param read_index Reference to the reading index, will be updated to the next index after reading
     * @return Pair of pointer to the data and the size of the data, or nullptr and 0 if no data is available
     */
    std::pair<T*, uint32_t> read(uint64_t& read_index) noexcept {
        uint64_t index;
        slot* current_slot;
        while (true) {
            auto idx = read_index & mask_;
            current_slot = &control_[idx];
            index = current_slot->data_index.load(std::memory_order_acquire);
            if (index != std::numeric_limits<uint64_t>::max() && reserved_->load(std::memory_order_relaxed).index_ < index) [[unlikely]] {
                // queue has been reset
                read_index = 0;
            }
    
            if (index == std::numeric_limits<uint64_t>::max() || index < read_index) {
                // data not ready yet
                return std::make_pair(nullptr, 0);
            }
            else if (index > read_index && ((index & mask_) != idx)) {
                // queue wrapped, skip the unused slots
                read_index = index;
                continue;
            }
            break;
        }

        auto& data = data_[read_index & mask_];
        read_index = index + current_slot->size;
        return std::make_pair(&data, current_slot->size);
    }

    /**
     * @brief Read data from the queue using a shared atomic cursor
     * @param read_index Reference to the atomic reading index, will be atomically updated after reading
     * @return Pair of pointer to the data and the size of the data, or nullptr and 0 if no data is available
     *
     * This overload allows multiple consumers to share a single atomic cursor for load-balancing/work-stealing patterns.
     * Each consumer atomically claims the next item to process.
     */
    std::pair<T*, uint32_t> read(std::atomic<uint64_t>& read_index) noexcept {
        while (true) {
            uint64_t current_index = read_index.load(std::memory_order_acquire);
            auto idx = current_index & mask_;
            slot* current_slot = &control_[idx];
            uint64_t index = current_slot->data_index.load(std::memory_order_acquire);

            if (index != std::numeric_limits<uint64_t>::max() && reserved_->load(std::memory_order_relaxed).index_ < index) [[unlikely]] {
                // queue has been reset
                read_index.store(0, std::memory_order_release);
                continue;
            }

            if (index == std::numeric_limits<uint64_t>::max() || index < current_index) {
                // data not ready yet
                return std::make_pair(nullptr, 0);
            }
            else if (index > current_index && ((index & mask_) != idx)) {
                // queue wrapped, skip the unused slots
                read_index.compare_exchange_weak(current_index, index, std::memory_order_release, std::memory_order_relaxed);
                continue;
            }

            // Try to atomically claim this item
            uint64_t next_index = index + current_slot->size;
            if (read_index.compare_exchange_weak(current_index, next_index, std::memory_order_release, std::memory_order_relaxed)) {
                // Successfully claimed the item
                return std::make_pair(&data_[current_index & mask_], current_slot->size);
            }
            // CAS failed, another consumer claimed it, retry
        }
    }

    /**
    * @brief Read the last published data in the queue
    * @return Pointer to the last published data, or nullptr if no data is available
    */
    T* read_last() noexcept {
        auto reserved = reserved_->load(std::memory_order_relaxed);
        if (reserved.index_ == 0) {
            return nullptr;
        }
        auto index = reserved.index_ - reserved.size_;
        return &data_[index & mask_];
    }

    /**
     * @brief Reset the queue, invalidating all existing data
     * 
     * Note: This function is not thread-safe and should be called when no other threads are accessing the queue.
     */
    void reset() noexcept {
        if (use_shm_) {
            control_ = new ((uint8_t*)lpvMem_ + 64) slot[size_];
        } else {
            delete [] control_;
            control_ = new slot[size_];
        }
        reserved_->store(reserved_info(), std::memory_order_release);
    }

private:

#if defined(_MSC_VER)
    void allocate_shm_data(const char* const shm_name, bool open_only) {
        DWORD BF_SZ;
        hMapFile_ = NULL;

#ifndef UNICODE
        std::string shmName = shm_name;
#else
        int size_needed = MultiByteToWideChar(CP_UTF8, 0, shm_name, static_cast<int>(strlen(shm_name)), NULL, 0);
        std::wstring shmName(size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, shm_name, strlen(shm_name), &shmName[0], size_needed);
#endif

        if (open_only) {
#ifndef UNICODE
            hMapFile_ = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, (LPCSTR)shmName.c_str());
#else
            hMapFile_ = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, (LPCWSTR)shmName.c_str());
#endif
            own_ = false;
            auto err = GetLastError();
            if (hMapFile_ == NULL) {
                throw std::runtime_error("Failed to open shm. err=" + std::to_string(err));
            }

            auto lpvMem = MapViewOfFile(hMapFile_, FILE_MAP_ALL_ACCESS, 0, 0, 64);
            if (!lpvMem) {
                auto err = GetLastError();
                throw std::runtime_error("Failed to map shm. err=" + std::to_string(err));
            }
            size_ = *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(lpvMem) + sizeof(std::atomic<reserved_info>));
            mask_ = size_ - 1;
            BF_SZ = 64 + sizeof(slot) * size_ + sizeof(T) * size_;
            UnmapViewOfFile(lpvMem);
        }
        else {
            BF_SZ = 64 + sizeof(slot) * size_ + sizeof(T) * size_;

            SECURITY_ATTRIBUTES sa;
            sa.nLength = sizeof(SECURITY_ATTRIBUTES);
            sa.lpSecurityDescriptor = NULL;
            sa.bInheritHandle = FALSE;

            hMapFile_ = CreateFileMapping(
                INVALID_HANDLE_VALUE,               // use paging file
                &sa,                                // default security
                PAGE_READWRITE,                     // read/write access
                0,                                  // maximum object size (high-order DWORD)
                BF_SZ,                              // maximum object size (low-order DWORD)
#ifndef UNICODE
                (LPCSTR)shmName.c_str()             // name of mapping object
#else           
                (LPCWSTR)shmName.c_str()            // name of mapping object
#endif
            );

            own_ = false;
            auto err = GetLastError();
            if (hMapFile_ == NULL) {
		        throw std::runtime_error("Failed to create shm. err=" + std::to_string(err));
            }

            if (err != ERROR_ALREADY_EXISTS) {
                own_ = true;
            }
        }

        lpvMem_ = MapViewOfFile(hMapFile_, FILE_MAP_ALL_ACCESS, 0, 0, BF_SZ);
        if (!lpvMem_) {
            auto err = GetLastError();
            throw std::runtime_error("Failed to map shm. err=" + std::to_string(err));
        }

        if (own_) {
            reserved_ = new (lpvMem_) std::atomic<reserved_info>();
            *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(lpvMem_) + sizeof(std::atomic<reserved_info>)) = size_;
            control_ = new ((uint8_t*)lpvMem_ + 64) slot[size_];
            data_ = new ((uint8_t*)lpvMem_ + 64 + sizeof(slot) * size_) T[size_];
        }
        else {
            reserved_ = reinterpret_cast<std::atomic<reserved_info>*>(lpvMem_);
            control_ = reinterpret_cast<slot*>((uint8_t*)lpvMem_ + 64);
            data_ = reinterpret_cast<T*>((uint8_t*)lpvMem_ + 64 + sizeof(slot) * size_);
        }
    }
#else
    void allocate_shm_data(const char* const shm_name, bool open_only) {
        size_t BF_SZ = 64 + sizeof(slot) * size_ + sizeof(T) * size_;
        shm_name_ = shm_name;
        int flags = open_only ? O_RDWR : (O_RDWR | O_CREAT | O_EXCL);
        shm_fd_ = shm_open(shm_name, flags, 0666);
        if (shm_fd_ == -1) {
            if (!open_only && errno == EEXIST) {
                // Try opening existing
                shm_fd_ = shm_open(shm_name, O_RDWR, 0666);
                if (shm_fd_ == -1) {
                    throw std::runtime_error("Failed to open existing shm. err=" + std::to_string(errno));
                }
                own_ = false;
            } else {
                throw std::runtime_error("Failed to open/create shm. err=" + std::to_string(errno));
            }
        } else {
            own_ = !open_only;
        }

        if (own_) {
            if (ftruncate(shm_fd_, BF_SZ) == -1) {
                throw std::runtime_error("Failed to size shm. err=" + std::to_string(errno));
            }
        }

        lpvMem_ = mmap(nullptr, BF_SZ, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd_, 0);
        if (lpvMem_ == MAP_FAILED) {
            throw std::runtime_error("Failed to map shm. err=" + std::to_string(errno));
        }

        if (own_) {
            reserved_ = new (lpvMem_) std::atomic<reserved_info>();
            *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(lpvMem_) + sizeof(std::atomic<reserved_info>)) = mask_ + 1;
            control_ = new ((uint8_t*)lpvMem_ + 64) slot[size_];
            data_ = new ((uint8_t*)lpvMem_ + 64 + sizeof(slot) * size_) T[size_];
        } else {
            reserved_ = reinterpret_cast<std::atomic<reserved_info>*>(lpvMem_);
            control_ = reinterpret_cast<slot*>((uint8_t*)lpvMem_ + 64);
            data_ = reinterpret_cast<T*>((uint8_t*)lpvMem_ + 64 + sizeof(slot) * size_);
        }
    }
#endif

};

}
