#pragma once

#include <string>
#include <vector>

#include "alpaca/account.h"
#include "alpaca/asset.h"
#include "alpaca/bars.h"
#include "alpaca/clock.h"
#include "alpaca/order.h"
#include "alpaca/response.h"

//extern int print(int to, char* format, ...);

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

        Response<Account> getAccount() const;

        Response<Clock> getClock() const;

        Response<Asset> getAsset(const std::string& symbol) const;

        Response<std::vector<Order>> getOrders(const ActionStatus status = ActionStatus::Open,
            const int limit = 50,
            const std::string& after = "",
            const std::string& until = "",
            const OrderDirection = OrderDirection::Descending,
            const bool nested = false) const;

        Response<Order> getOrder(const std::string& id, const bool nested = false) const;

        Response<Order> getOrderByClientOrderID(const std::string& client_order_id) const;

        Response<Order> submitOrder(
            const std::string& symbol,
            const int quantity,
            const OrderSide side,
            const OrderType type,
            const OrderTimeInForce tif,
            const std::string& limit_price = "",
            const std::string& stop_price = "",
            const bool extended_hours = false,
            const std::string& client_order_id = "",
            const OrderClass order_class = OrderClass::Simple,
            TakeProfitParams* take_profit_params = nullptr,
            StopLossParams* stop_loss_params = nullptr) const;

        Response<Order> replaceOrder(const std::string& id,
            const int quantity,
            const OrderTimeInForce tif,
            const std::string& limit_price = "",
            const std::string& stop_price = "",
            const std::string& client_order_id = "") const;

        Response<std::vector<Order>> cancelOrders() const;

        Response<Order> cancelOrder(const std::string& id) const;

        Response<Bars> getBars(
            const std::vector<std::string>& symbols,
            const std::string& start,
            const std::string& end,
            const std::string& after = "",
            const std::string& until = "",
            const std::string& timeframe = "1D",
            const uint32_t limit = 100) const;

    private:
        template<typename T>
        inline Response<T> request(const std::string& path, const char* data = nullptr) const;

    private:
        const std::string baseUrl_;
        const std::string headers_;
#ifdef _DEBUG
        FILE* log_;
#endif;
    };


    template<typename T>
    inline Response<T> Client::request(const std::string& path, const char* data) const {
        int id = http_send((char*)(baseUrl_ + path).c_str(), (char*)data, (char*)headers_.c_str());

#ifdef _DEBUG
        fprintf(log_, "--> %d %s%s\n", id, baseUrl_.c_str(), path.c_str());
        if (data) {
            fprintf(log_, "Data:\n%s\n", data);
        }
#endif

        if (!id) {
#ifdef _DEBUG
            fprintf(log_, "Error: Cannot connect to server.\n");
#endif
            return Response<T>(1, "Cannot connect to server");
        }

        long n = 0;
        std::stringstream ss;
        while (!(n = http_status(id))) {
            Sleep(100); // wait for the server to reply
            if (!BrokerProgress(1)) {
#ifdef _DEBUG
                fprintf(log_, "Brokerprogress returned zero. Aborting...\n");
#endif
                http_free(id);
                return Response<T>(1, "Brokerprogress returned zero. Aborting...");
            }
            // print dots, abort if returns zero.
        }

        if (n > 0) {
            char* buffer = (char*)malloc(n + 1);
            auto received = http_result(id, buffer, n);
            //fprintf(log_, "\n%i, received=%i:\n %s\n", n, received, buffer);
            ss << buffer;
            //fflush(log_);
            free(buffer); //free up memory allocation
        }
        http_free(id); //always clean up the id!

#ifdef _DEBUG
        fprintf(log_, "<-- %d %s\n", id, ss.str().c_str());
        fflush(log_);
#endif
        Response<T> response;
        response.parseContent(ss.str());
        return response;
    }

} // namespace alpaca