#pragma once

#include <string>
#include "request.h"
#include "market_data/market_data_base.h"

namespace alpaca {

    class AlpacaMarketData : public MarketData {
        static constexpr const char* baseUrl_ = "https://data.alpaca.markets";

    public:
        AlpacaMarketData(std::string headers, Logger& logger) : headers_(std::move(headers)), logger_(logger) {}
        ~AlpacaMarketData() override = default;

        Response<LastQuote> getLastQuote(const std::string& symbol) const override {
            return request<LastQuote, AlpacaMarketData>(std::string(baseUrl_) + "/v1/last_quote/stocks/" + symbol, headers_);
        }

        Response<LastTrade> getLastTrade(const std::string& symbol) const override {
            return request<LastTrade, AlpacaMarketData>(std::string(baseUrl_) + "/v1/last/stocks/" + symbol, headers_);
        }

        Response<std::vector<Bar>> getBars(
            const std::string& symbol,
            const __time32_t start,
            const __time32_t end,
            const int nTickMinutes = 1,
            const uint32_t limit = 100) const override;

    private:
        std::string headers_;
        Logger& logger_;
    };
}