#pragma once

#include <string>
#include <vector>

#include "request.h"
#include "logger.h"
#include "alpaca/account.h"
#include "alpaca/asset.h"
#include "alpaca/option_contract.h"
#include "market_data/bars.h"
#include "market_data/quote.h"
#include "alpaca/clock.h"
#include "alpaca/order.h"
#include "alpaca/position.h"
#include "alpaca/Balance.h"

namespace alpaca {

    extern double fix_floating_error(double value, int32_t norm_factor = 1e9);

    class Client final {
    public:
        explicit Client() = delete;
        explicit Client(std::string key, std::string secret, bool isPaperTrading);
        ~Client() = default;

        bool isLiveMode() const noexcept { return isLiveMode_;  }
        bool isOpen() const noexcept { return is_open_; }

        const std::string& headers() const noexcept { return headers_;  }

        const std::string& getApiKey() const noexcept {
            return apiKey_;
        }

        const std::unordered_map<std::string, AssetBase*>& allAssets() const noexcept
        {
            return all_assets_;
        }

        Response<Account> getAccount() const;
        Response<Balance> getBalance() const;
        void getBalance(Response<Balance>& rsp, uint64_t timestamp) const;

        Response<Clock> getClock() const;
        void getClock(Response<Clock> &rsp, uint64_t timestamp) const;

        Response<std::vector<Asset>> getAssets(bool active_only = true) const;
        Response<Asset> getAsset(const std::string& symbol) const;

        Response<OptionContracts> getOptionContracts(const std::string &symbols = "", bool active_only = true) const;
        Response<OptionContract> getOptionContract(const std::string& symbol) const;

        Response<std::vector<Order>> getOrders(
            const ActionStatus status = ActionStatus::Open,
            const int limit = 50,
            const std::string& after = "",
            const std::string& until = "",
            const OrderDirection = OrderDirection::Descending,
            const bool nested = false) const;

        Response<Order> getOrder(const std::string& id, const bool nested = false) const;
        Response<Order> getOrderByClientOrderId(const std::string& clientOrderId) const;

        Response<Order> submitOrder(
            const AssetBase* asset,
            const double quantity,
            const OrderSide side,
            const OrderType type,
            const TimeInForce tif,
            const double limit_price = NAN,
            const double stop_price = NAN,
            const std::string& client_order_id = "",
            double minFractionalQty = 1,
            const OrderClass order_class = OrderClass::Simple,
            TakeProfitParams* take_profit_params = nullptr,
            StopLossParams* stop_loss_params = nullptr) const;

        Response<Order> replaceOrder(
            const std::string& id,
            const double quantity,
            const TimeInForce tif,
            const std::string& limit_price = "",
            const std::string& stop_price = "",
            const std::string& client_order_id = "") const;

        Response<Order> cancelOrder(const std::string& id) const;

        Response<Position> getPosition(const std::string& symbol) const;
        Response<std::vector<Position>> getPositions() const;

    private:
        void getAllAssets();

    private:
        const std::string baseUrl_;
        const std::string apiKey_;
        const std::string headers_;
        mutable bool is_open_ = true;
        const bool isLiveMode_;
        std::vector<Asset> assets_;
        std::vector<OptionContract> option_contracts_;
        std::unordered_map<std::string, AssetBase*> all_assets_;
    };

} // namespace alpaca