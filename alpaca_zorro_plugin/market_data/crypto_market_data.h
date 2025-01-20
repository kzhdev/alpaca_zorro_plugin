#pragma once

#include <market_data/market_data.h>
#include <regex>

namespace alpaca {

    class CryptoMarketData : public MarketData {

    public:
        CryptoMarketData(std::string headers)
            : MarketData("https://data.alpaca.markets/v1beta3/crypto/us/", std::move(headers))
        {}

        ~CryptoMarketData() override = default;

        AssetClass type() const noexcept override { return AssetClass::CRYPTO; }

        Response<LastQuotes> getLastQuotes(const std::string& symbols) const override {
            return request<LastQuotes>(base_url_ + "latest/quotes?symbols=" + symbols, headers_.c_str(), nullptr, LogLevel::L_DEBUG, LogType::LT_MD);
        }

        Response<LastTrades> getLastTrades(const std::string& symbols) const override {
            return request<LastTrades>(base_url_ + "latest/trades?symbols=" + symbols, headers_.c_str(), nullptr, LogLevel::L_DEBUG, LogType::LT_MD);
        }

        Response<LastQuote> getLastQuote(const std::string& symbol) const override {
            auto rsp = getLastQuotes(symbol);
            if (!rsp)
            {
                return Response<LastQuote>(rsp.getCode(), rsp.what());
            }
            if (rsp.content().quotes.empty())
            {
                return Response<LastQuote>(1, "Quote not found");
            }
            Response<LastQuote> rt;
            auto& last_quote = rt.content();
            last_quote.symbol = symbol;
            last_quote.quote = rsp.content().quotes.at(symbol);
            return rt;
        }

        Response<LastTrade> getLastTrade(const std::string& symbol) const override {
            auto rsp = getLastTrades(symbol);
            if (!rsp)
            {
                return Response<LastTrade>(rsp.getCode(), rsp.what());
            }
            if (rsp.content().trades.empty())
            {
                return Response<LastTrade>(1, "Quote not found");
            }
            Response<LastTrade> rt;
            auto& last_quote = rt.content();
            last_quote.symbol = symbol;
            last_quote.trade = rsp.content().trades.at(symbol);
            return rt;
        }

    protected:

        std::string downloadBarBaseUri(const std::string &symbol) const noexcept override {
            return base_url_ + "bars?symbols=" + symbol + "&";
        }

        Response<std::vector<Bar>> downloadBars(
            const std::string& symbol,
            __time32_t start,
            __time32_t end,
            int nTickMinutes = 1,
            uint32_t limit = 100) const override {
            auto sym = std::regex_replace(symbol, std::regex("/"), "%2F");
            return MarketData::downloadBars(sym, start, end, nTickMinutes, limit);
        }
    };

}