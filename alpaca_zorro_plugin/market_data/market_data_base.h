#pragma once

#include <string>
#include "quote.h"
#include "market_data/bars.h"
#include "market_data/trade.h"

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
            __time32_t start,
            __time32_t end,
            int nTickMinutes = 1,
            uint32_t limit = 100,
            int32_t priceType = 0) const = 0;  // 0 = bar, 1 = quote, 2 = trade
    protected:
        bool paidPlan_ = false;;
    };
}