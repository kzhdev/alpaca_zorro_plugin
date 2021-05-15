#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace alpaca {

    extern __time32_t parseTimeStamp2(const std::string& timestamp);

    struct Trade {
        double price;
        int size;
        int trade_id;
        uint64_t timestamp;
        std::string exchange;
        std::string z;
        std::vector<std::string> conditions;
        std::string sTime;


    private:
        template <typename> friend class Response;
        friend struct Snapshot;
        friend struct LastTrade;
        friend struct Trades;

        template<typename CallerT, typename T>
        std::pair<int, std::string> fromJSON(const T& parser, typename std::enable_if<std::is_same<CallerT, class AlpacaMarketData>::value>::type* = 0) {
            parser.get<double>("p", price);
            parser.get<int>("s", size);
            parser.get<int>("t", trade_id);
            parser.get<std::string>("x", exchange);
            parser.get<std::string>("t", sTime);
            timestamp = parseTimeStamp2(sTime);
            return std::make_pair(0, "OK");
        }

        template<typename CallerT, typename T>
        std::pair<int, std::string> fromJSON(const T& parser, typename std::enable_if<std::is_same<CallerT, class Polygon>::value>::type* = 0) {
            parser.get<double>("price", price);
            parser.get<int>("size", size);
            parser.get<uint64_t>("timestamp", timestamp);
            return std::make_pair(0, "OK");
        }
    };

    struct LastTrade {
        std::string status;
        std::string symbol;
        Trade trade;

    private:
        template <typename> friend class Response;

        template <typename CallerT, typename T>
        std::pair<int, std::string> fromJSON(const T& parser) {
            parser.get<std::string>("status", status);
            parser.get<std::string>("symbol", symbol);

            if (status != "success") {
                return std::make_pair(1, symbol + " " + status);
            }

            if (parser.json.HasMember("last") && parser.json["last"].IsObject()) {
                auto obj = parser.json["last"].GetObject();
                Parser<decltype(parser.json["last"].GetObject())> p(obj);
                trade.fromJSON<CallerT>(p);
            }

            return std::make_pair(0, "OK");
        }
    };

    struct Trades {
        std::string symbol;
        std::string next_page_token;
        std::vector<Trade> trades;

    private:
        template <typename> friend class Response;

        template <typename CallerT, typename T>
        std::pair<int, std::string> fromJSON(const T& parser) {
            parser.get<std::string>("symbol", symbol);

            parser.get<std::string>("next_page_token", next_page_token);
            if (parser.json.HasMember("trades")) {
                for (auto& trade : parser.json["trades"].GetArray()) {
                    auto tradeJson = trade.GetObject();
                    Parser<decltype(tradeJson)> p(tradeJson);
                    Trade t;
                    t.fromJSON<CallerT>(p);
                    trades.emplace_back(std::move(t));
                }
            }
            return std::make_pair(0, "OK");
        }
    };
}