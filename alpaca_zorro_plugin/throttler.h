#pragma once

#include <cstdint>
#include <chrono>

namespace alpaca {

    class Throttler {
        uint64_t start_timestamp_ = 0;
        uint64_t timestamp_ = 0;
        uint32_t count_ = 0;
        uint32_t limit_ = 0;
    public:
        Throttler(uint32_t limit) : limit_(limit) {}
        ~Throttler() = default;

        bool canSent() noexcept {
            timestamp_ = get_timestamp();
            if ((timestamp_ - start_timestamp_) > 1000) {
                count_ = 1;
                start_timestamp_ = timestamp_;
                return true;
            }
            else if (count_ < limit_) {
                ++count_;
                return true;
            }
            return false;
        }

        uint32_t getCount() const noexcept {
            return count_;
        }

        uint64_t startTimestamp() const noexcept {
            return start_timestamp_;
        }

        uint64_t lastTimestamp() const noexcept {
            return timestamp_;
        }

    private:
        static uint64_t get_timestamp() {
            auto now = std::chrono::system_clock::now();
            return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
        }
    };
}