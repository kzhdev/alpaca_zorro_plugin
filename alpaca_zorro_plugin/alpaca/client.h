#pragma once

#include <string>
#include <vector>

#include "request.h"
#include "logger.h"
#include "alpaca/account.h"
#include "alpaca/asset.h"
#include "market_data/bars.h"
#include "market_data/quote.h"
#include "alpaca/clock.h"
#include "alpaca/order.h"
#include "alpaca/position.h"

namespace alpaca {

    class MarketData;

    class Client final {
    public:
        explicit Client() = delete;
        explicit Client(std::string key, std::string secret, bool isPaperTrading);
        ~Client() = default;

        Logger& logger() noexcept { return logger_;  }

        bool isLiveMode() const noexcept { return isLiveMode_;  }

        const std::string& headers() const noexcept { return headers_;  }

        Response<Account> getAccount() const;

        Response<Clock> getClock() const;

        Response<Asset> getAsset(const std::string& symbol) const;

        Response<std::vector<Order>> getOrders(
            const ActionStatus status = ActionStatus::Open,
            const int limit = 50,
            const std::string& after = "",
            const std::string& until = "",
            const OrderDirection = OrderDirection::Descending,
            const bool nested = false) const;

        Response<Order> getOrder(const std::string& id, const bool nested = false, const bool logResponse = false) const;
        Response<Order> getOrderByClientOrderId(const std::string& clientOrderId) const;

        Response<Order> submitOrder(
            const std::string& symbol,
            const int quantity,
            const OrderSide side,
            const OrderType type,
            const TimeInForce tif,
            const std::string& limit_price = "",
            const std::string& stop_price = "",
            bool extended_hours = false,
            const std::string& client_order_id = "",
            const OrderClass order_class = OrderClass::Simple,
            TakeProfitParams* take_profit_params = nullptr,
            StopLossParams* stop_loss_params = nullptr) const;

        Response<Order> replaceOrder(
            const std::string& id,
            const int quantity,
            const TimeInForce tif,
            const std::string& limit_price = "",
            const std::string& stop_price = "",
            const std::string& client_order_id = "") const;

        Response<Order> cancelOrder(const std::string& id) const;

        Response<Position> getPosition(const std::string& symbol) const;

    private:
        const std::string baseUrl_;
        const std::string apiKey_;
        const std::string headers_;
        std::unordered_map<std::string, Asset> subscribed_assets_;
        mutable bool is_open_ = false;
        const bool isLiveMode_;
        mutable Logger logger_;
    };

} // namespace alpaca