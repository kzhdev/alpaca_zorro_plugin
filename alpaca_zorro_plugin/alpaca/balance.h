#pragma once

#include <string>
#include "rapidjson/document.h"

namespace alpaca {

    /**
     * @brief A type representing an Alpaca account.
     */
    struct Balance {
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

    private:
        template<typename> friend class Response;

        template<typename CallerT, typename parserT>
        std::pair<int, std::string> fromJSON(const parserT& parser) {
            parser.get<double>("buying_power", buying_power);
            parser.get<double>("cash", cash);
            parser.get<int32_t>("daytrade_count", daytrade_count);
            parser.get<double>("daytrading_buying_power", daytrading_buying_power);
            parser.get<double>("equity", equity);
            parser.get<double>("initial_margin", initial_margin);
            parser.get<double>("last_equity", last_equity);
            parser.get<double>("last_maintenance_margin", last_maintenance_margin);
            parser.get<double>("long_market_value", long_market_value);
            parser.get<double>("maintenance_margin", maintenance_margin);
            parser.get<double>("portfolio_value", portfolio_value);
            parser.get<double>("regt_buying_power", regt_buying_power);
            parser.get<double>("short_market_value", short_market_value);
            return std::make_pair(0, "OK");
        }
    };

} // namespace alpaca