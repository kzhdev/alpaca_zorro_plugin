#pragma once

#include <market_data/market_data.h>

namespace alpaca {

    class OptionMarketData : public MarketData {

    public:
        OptionMarketData(std::string headers)
            : MarketData("https://data.alpaca.markets/v1beta1/options/", std::move(headers))
        {}

        ~OptionMarketData() override = default;

        AssetClass type() const noexcept override { return AssetClass::OPTIONS; }
    };

}