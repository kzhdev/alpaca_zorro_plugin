#pragma once

#include <string>
#include <vector>

#include "alpaca/response.h"
#include "alpaca/account.h"
#include "alpaca/asset.h"
#include "alpaca/bars.h"
#include "alpaca/quote.h"
#include "alpaca/clock.h"
#include "alpaca/order.h"
#include "alpaca/logger.h"
#include "alpaca/position.h"

namespace alpaca {

    extern int(__cdecl* BrokerError)(const char* txt);
    extern int(__cdecl* BrokerProgress)(const int percent);
    extern int(__cdecl* http_send)(char* url, char* data, char* header);
    extern long(__cdecl* http_status)(int id);
    extern long(__cdecl* http_result)(int id, char* content, long size);
    extern void(__cdecl* http_free)(int id);

    class Client final {
    public:
        explicit Client() = delete;
        explicit Client(std::string key, std::string secret, bool isPaperTrading);
        ~Client() = default;

        Logger& logger() noexcept { return logger_;  }

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

        Response<Order> getOrder(const std::string& id, const bool nested = false) const;
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
            StopLossParams* stop_loss_params = nullptr);

        Response<Order> replaceOrder(
            const std::string& id,
            const int quantity,
            const TimeInForce tif,
            const std::string& limit_price = "",
            const std::string& stop_price = "",
            const std::string& client_order_id = "") const;

        Response<std::vector<Order>> cancelOrders() const;

        Response<Order> cancelOrder(const std::string& id) const;

        Response<Bars> getBars(
            const std::vector<std::string>& symbols,
            const __time32_t start,
            const __time32_t end,
            const int nTickMinutes = 1,
            const uint32_t limit = 100) const;

        Response<LastQuote> getLastQuote(const std::string& symbol) const;

        Response<Position> getPosition(const std::string& symbol) const;

    private:
        template<typename T>
        inline Response<T> request(const std::string& path, const char* data = nullptr) const;

    private:
        const std::string baseUrl_;
        const std::string dataUrl_;
        const std::string headers_;
        std::unordered_map<std::string, Asset> subscribed_assets_;
        mutable bool is_open_ = false;
        mutable Logger logger_;
    };


    template<typename T>
    inline Response<T> Client::request(const std::string& url, const char* data) const {
        // unfortunately need to make a copy of headers for every request. Otherwise only the first request has headers.
        auto headers = headers_;
        int id = http_send((char*)url.c_str(), (char*)data, (char*)headers.c_str());

        logger_.logTrace("--> %d %s\n", id, url.c_str());
        if (data) {
            logger_.logTrace("Data:\n%s\n", data);
        }

        if (!id) {
            return Response<T>(1, "Cannot connect to server");
        }

        long n = 0;
        std::stringstream ss;
        while (!(n = http_status(id))) {
            Sleep(100); // wait for the server to reply
            if (!BrokerProgress(1)) {
                http_free(id);
                return Response<T>(1, "Brokerprogress returned zero. Aborting...");
            }
            // print dots, abort if returns zero.
        }

        if (n > 0) {
            char* buffer = (char*)malloc(n + 1);
            auto received = http_result(id, buffer, n);
            ss << buffer;
            free(buffer); //free up memory allocation
        }
        http_free(id); //always clean up the id!

        logger_.logTrace("<-- %d %s\n", id, ss.str().c_str());

        Response<T> response;
        response.parseContent(ss.str());
        return response;
    }

} // namespace alpaca