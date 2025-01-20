#pragma once

#include <market_data/market_data.h>
#include <regex>

namespace alpaca {

    class StockMarketData : public MarketData {

    public:
        StockMarketData(std::string headers)
            : MarketData("https://data.alpaca.markets/v2/stocks/", std::move(headers))
        {}

        ~StockMarketData() override = default;

        AssetClass type() const noexcept override { return AssetClass::US_EQUITY; }
    };

}