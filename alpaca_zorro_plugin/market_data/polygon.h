#pragma once

#include <string>
#include "request.h"
#include "market_data/market_data_base.h"

namespace alpaca {

    class Polygon : public MarketData {
        static constexpr const char* baseUrl_ = "https://api.polygon.io";

    public:
        Polygon(const std::string& apiKey, bool paidPlan) : MarketData(paidPlan), apiKey_("apiKey=" + apiKey) {}

        const char* name() const noexcept { return "Polygon"; }

        Response<LastQuote> getLastQuote(const std::string& symbol) const override {
            std::stringstream url;
            url << baseUrl_ << "/v1/last_quote/stocks/" << symbol;
            LOG_DEBUG("--> %s\n", url.str().c_str());
            url << "?" << apiKey_;
            return request<LastQuote, Polygon>(url.str(), "");
        }

        Response<LastTrade> getLastTrade(const std::string& symbol) const override {
            std::stringstream url;
            url << baseUrl_ << "/v1/last/stocks/" << symbol;
            LOG_DEBUG("--> %s\n", url.str().c_str());
            url << "?" << apiKey_;
            return request<LastTrade, Polygon>(url.str(), "");
        }

        Response<std::vector<Bar>> getBars(
            const std::string& symbol,
            __time32_t start,
            __time32_t end,
            const int nTickMinutes = 1,
            const uint32_t limit = 100,
            int32_t price_type = 0) const override;

    private:
        std::string apiKey_;
    };
}
