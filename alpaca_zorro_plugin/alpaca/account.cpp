#include "stdafx.h"
#include "alpaca/account.h"

#include "alpaca/json.h"

namespace alpaca {
    void Account::fromJSON(const rapidjson::Document& d) {
        account_blocked = parse<bool>(d, "account_blocked");
        account_number = parse<std::string>(d, "account_number");
        buying_power = parse<double>(d, "buying_power");
        cash = parse<double>(d, "cash");
        created_at = parse<std::string>(d, "created_at");
        currency = parse<std::string>(d, "currency");
        daytrade_count = parse<int32_t>(d, "daytrade_count");
        daytrading_buying_power = parse<double>(d, "daytrading_buying_power");
        equity = parse<double>(d, "equity");
        id = parse<std::string>(d, "id");
        initial_margin = parse<double>(d, "initial_margin");
        last_equity = parse<double>(d, "last_equity");
        last_maintenance_margin = parse<double>(d, "last_maintenance_margin");
        long_market_value = parse<double>(d, "long_market_value");
        maintenance_margin = parse<double>(d, "maintenance_margin");
        multiplier = parse<int32_t>(d, "multiplier");
        pattern_day_trader = parse<bool>(d, "pattern_day_trader");
        portfolio_value = parse<double>(d, "portfolio_value");
        regt_buying_power = parse<double>(d, "regt_buying_power");
        short_market_value = parse<double>(d, "short_market_value");
        shorting_enabled = parse<bool>(d, "shorting_enabled");
        sma = parse<std::string>(d, "sma");
        status = parse<std::string>(d, "status");
        trade_suspended_by_user = parse<bool>(d, "trade_suspended_by_user");
        trading_blocked = parse<bool>(d, "trading_blocked");
        transfers_blocked = parse<bool>(d, "transfers_blocked");
    }
} // namespace alpaca