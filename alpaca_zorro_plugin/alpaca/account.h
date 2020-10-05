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
        void fromJSON(const rapidjson::Document& d);
    };

    /**
     * @brief A type representing Alpaca account configuration.
     */
    struct AccountConfigurations {
        std::string dtbp_check;
        bool no_shorting;
        bool suspend_trade;
        std::string trade_confirm_email;

    private:
        template<typename> friend class Response;
        int fromJSON(const rapidjson::Document& d, std::string& error);
    };

    /**
     * @brief A type representing trade activity from an Alpaca account.
     */
    struct TradeActivity {
        std::string activity_type;
        std::string cum_qty;
        std::string id;
        std::string leaves_qty;
        std::string order_id;
        std::string price;
        std::string qty;
        std::string side;
        std::string symbol;
        std::string transaction_time;
        std::string type;

    private:
        template<typename> friend class Response;
        void fromJSON(const rapidjson::Document& d);
    };

    /**
     * @brief A type representing non-trade activity from an Alpaca account.
     */
    struct NonTradeActivity {
        std::string activity_type;
        std::string date;
        std::string id;
        std::string net_amount;
        std::string per_share_amount;
        std::string qty;
        std::string symbol;

    private:
        template<typename> friend class Response;
        void fromJSON(const rapidjson::Document& d);
    };



} // namespace alpaca