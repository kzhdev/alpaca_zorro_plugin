#pragma once

#include <string>
#include "rapidjson/document.h"

namespace alpaca {

    /**
     * @brief A type representing an Alpaca account.
     */
    struct Account {
        double buying_power;
        double cash;
        double daytrading_buying_power;
        double equity;
        double initial_margin;
        double last_equity;
        double last_maintenance_margin;
        double long_market_value;
        double maintenance_margin;
        double portfolio_value;
        double regt_buying_power;
        double short_market_value;
        int32_t daytrade_count;
        int32_t multiplier;
        bool account_blocked;
        bool pattern_day_trader;
        bool shorting_enabled;
        bool trade_suspended_by_user;
        bool trading_blocked;
        bool transfers_blocked;
        std::string account_number;
        std::string created_at;
        std::string currency;
        std::string id;
        std::string sma;
        std::string status;

    private:
        template<typename> friend class Response;

        template<typename CallerT, typename parserT>
        std::pair<int, std::string> fromJSON(const parserT& parser) {
            parser.get<bool>("account_blocked", account_blocked);
            parser.get<std::string>("account_number", account_number);
            parser.get<double>("buying_power", buying_power);
            parser.get<double>("cash", cash);
            parser.get<std::string>("created_at", created_at);
            parser.get<std::string>("currency", currency);
            parser.get<int32_t>("daytrade_count", daytrade_count);
            parser.get<double>("daytrading_buying_power", daytrading_buying_power);
            parser.get<double>("equity", equity);
            parser.get<std::string>("id", id);
            parser.get<double>("initial_margin", initial_margin);
            parser.get<double>("last_equity", last_equity);
            parser.get<double>("last_maintenance_margin", last_maintenance_margin);
            parser.get<double>("long_market_value", long_market_value);
            parser.get<double>("maintenance_margin", maintenance_margin);
            parser.get<int32_t>("multiplier", multiplier);
            parser.get<bool>("pattern_day_trader", pattern_day_trader);
            parser.get<double>("portfolio_value", portfolio_value);
            parser.get<double>("regt_buying_power", regt_buying_power);
            parser.get<double>("short_market_value", short_market_value);
            parser.get<bool>("shorting_enabled", shorting_enabled);
            parser.get<std::string>("sma", sma);
            parser.get<std::string>("status", status);
            parser.get<bool>("trade_suspended_by_user", trade_suspended_by_user);
            parser.get<bool>("trading_blocked", trading_blocked);
            parser.get<bool>("transfers_blocked", transfers_blocked);
            return std::make_pair(0, "OK");
        }
    };

} // namespace alpaca