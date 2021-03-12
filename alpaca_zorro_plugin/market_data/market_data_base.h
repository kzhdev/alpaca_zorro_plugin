#pragma once

#include <string>
#include "quote.h"
#include "bars.h"
#include "trade.h"

namespace alpaca {

    class MarketData {
    public:
        MarketData(bool paidPlan = false) : paidPlan_(paidPlan) {}
        virtual ~MarketData() = default;

        bool isPaidPlan() const noexcept {
            return paidPlan_;
        }

        virtual const char* name() const noexcept = 0;
        virtual Response<LastQuote> getLastQuote(const std::string& symbol) const = 0;
        virtual Response<LastTrade> getLastTrade(const std::string& symbol) const = 0;
        virtual Response<std::vector<Bar>> getBars(
            const std::string& symbol,
            const __time32_t start,
            const __time32_t end,
            const int nTickMinutes = 1,
            const uint32_t limit = 100) const = 0;
    protected:
        bool paidPlan_ = false;;
    };
}