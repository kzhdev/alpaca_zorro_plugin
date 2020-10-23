#pragma once

#include <string>
#include "quote.h"
#include "bars.h"

namespace alpaca {

    class MarketData {
    public:
        virtual ~MarketData() = default;

        virtual Response<LastQuote> getLastQuote(const std::string& symbol) const = 0;
        virtual Response<std::vector<Bar>> getBars(
            const std::string& symbol,
            const __time32_t start,
            const __time32_t end,
            const int nTickMinutes = 1,
            const uint32_t limit = 100) const = 0;
    };
}