// The MIT License (MIT)
// Copyright (c) 2025 Kun Zhao
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <string>
#include <unordered_set>
#include <atomic>
#include <windows.h>

namespace zorro {

struct SettingUpdate
{
    uint8_t log_level_ = 0;
    bool pass_log_level_to_websocket_proxy_ = false;
};

struct Global
{
    static Global& get()
    {
        static Global global;
        return global;
    }

    Global(const Global&) = delete;
    Global& operator=(const Global&) = delete;
    Global(Global&&) = delete;
    Global& operator=(Global&&) = delete;

    std::string symbol_;
    std::string order_text_;
    std::string last_asset_;

    double amount_ = 1.;
    double leverage_ = 1.;
    double limit_price_ = NAN;

    uint64_t wait_time_ = 60000000000;
    double multiplier_ = 1.0;
    int32_t price_type_ = 0;
    int32_t vol_type_ = 0;

    std::unordered_set<std::string> asset_no_data_;

    HWND handle_ = nullptr;

    std::atomic_bool logged_in_ = false;
    std::atomic<std::shared_ptr<SettingUpdate>> setting_update_ = nullptr;

    void reset()
    {
        symbol_ = "";
        order_text_ = "";
        amount_ = 1.;
        leverage_ = 1.;
        limit_price_ = NAN;
        wait_time_ = 60000000000;
        multiplier_ = 1.0;
        asset_no_data_.clear();
        price_type_ = 0;
        logged_in_.store(false, std::memory_order_release);
        vol_type_ = 0;
        setting_update_.store(nullptr, std::memory_order_release);
    }

private:
    Global() = default;
    ~Global() = default;
};
    
} // namespace zorro
