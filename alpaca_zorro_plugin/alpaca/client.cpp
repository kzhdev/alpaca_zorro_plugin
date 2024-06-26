#include "stdafx.h"
#include "alpaca/client.h"

#include <sstream>
#include <memory>
#include <optional>

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "date/date.h"
#include "alpaca/client_order_id_generator.h"
#include "market_data/alpaca_market_data.h"
#include "zorro_websocket_proxy_client.h"
#include "account.h"

namespace {
    /// The base URL for API calls to the live trading API
    constexpr const char* s_APIBaseURLLive = "https://api.alpaca.markets";
    /// The base URL for API calls to the paper trading API
    constexpr const char* s_APIBaseURLPaper = "https://paper-api.alpaca.markets";
    std::unique_ptr<alpaca::ClientOrderIdGenerator> s_orderIdGen;

    std::unique_ptr<alpaca::AlpacaMarketData> alpacaMarketData;

    constexpr double epsilon = 1e-9;
    constexpr uint64_t default_norm_factor = 100000000llu;

    inline uint32_t compute_number_decimals(double value) {
        value = std::abs(alpaca::fix_floating_error(value));
        uint32_t count = 0;
        while (value - std::floor(value) > epsilon) {
            ++count;
            value *= 10;
        }
        return count;
    }
}

namespace alpaca {
    inline uint64_t get_timestamp() {
        auto now = std::chrono::system_clock::now();
        return std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
    }

    inline double fix_floating_error(double value, int32_t norm_factor) {
        auto v = value + epsilon;
        return ((double)((int64_t)(v * norm_factor)) / norm_factor);
    }

    Client::Client(std::string key, std::string secret, bool isPaperTrading)
        : baseUrl_(isPaperTrading ? s_APIBaseURLPaper : s_APIBaseURLLive)
        , apiKey_(std::move(key))
        , headers_("Content-Type:application/json\nAPCA-API-KEY-ID:" + apiKey_ + "\n" + "APCA-API-SECRET-KEY:" + std::move(secret))
        , isLiveMode_(!isPaperTrading)
    {
        s_orderIdGen = std::make_unique<ClientOrderIdGenerator>(*this);
    }

    Response<Account> Client::getAccount() const {
        return request<Account, Client>(baseUrl_ + "/v2/account", headers_.c_str(), nullptr, LogLevel::L_DEBUG, LogType::LT_ACCOUNT);
    }

    Response<Balance> Client::getBalance() const {
        return request<Balance, Client>(baseUrl_ + "/v2/account", headers_.c_str(), nullptr, LogLevel::L_DEBUG, LogType::LT_BALANCE);
    }

    Response<Clock> Client::getClock() const {
        auto rsp = request<Clock, Client>(baseUrl_ + "/v2/clock", headers_.c_str(), nullptr, LogLevel::L_TRACE, LogType::LT_MISC);
        if (rsp)
        {
            is_open_ = rsp.content().is_open;
        }
        return rsp;
    }

    Response<std::vector<Asset>> Client::getAssets(bool active_only) const {
        return active_only 
            ? request<std::vector<Asset>, Client>(baseUrl_ + "/v2/assets?asset_class=us_equity&status=active", headers_.c_str()) 
            : request<std::vector<Asset>, Client>(baseUrl_ + "/v2/assets?asset_class=us_equity", headers_.c_str());
    }

    Response<Asset> Client::getAsset(const std::string& symbol) const {
        return request<Asset, Client>(baseUrl_ + "/v2/assets/" + symbol, headers_.c_str());
    }

    Response<std::vector<Order>> Client::getOrders(
        ActionStatus status,
        int limit,
        const std::string& after,
        const std::string& until,
        OrderDirection direction,
        bool nested) const {
        std::vector<std::string> queries;
        if (status != ActionStatus::Open) {
            queries.emplace_back("status=" + std::string(to_string(status)));
        }
        if (limit != 50) {
            queries.emplace_back("limit=" + std::to_string(limit));
        }
        if (!after.empty()) {
            queries.emplace_back("after=" + after);
        }
        if (!until.empty()) {
            queries.emplace_back("until=" + until);
        }
        if (direction != OrderDirection::Descending) {
            queries.emplace_back("direction=" + std::string(to_string(direction)));
        }
        if (nested) {
            queries.emplace_back("nested=1");
        }

        std::stringstream url;
        url << baseUrl_ << "/v2/orders";
        if (!queries.empty()) {
            url << "?";
            int32_t i = 0;
            for (; i < (int32_t)queries.size() - 1; ++i) {
                url << queries[i] << '&';
            }
            url << queries[i];
        }
        return request<std::vector<Order>, Client>(url.str(), headers_.c_str(), nullptr, LogLevel::L_TRACE, LogType::LT_ORDER);
    }

    Response<Order> Client::getOrder(const std::string& id, const bool nested) const {
        auto url = baseUrl_ + "/v2/orders/" + id;
        if (nested) {
            url += "?nested=true";
        }

        Response<Order> response;
        return request<Order, Client>(url, headers_.c_str(), nullptr, LogLevel::L_DEBUG, LogType::LT_ORDER);
    }

    Response<Order> Client::getOrderByClientOrderId(const std::string& clientOrderId) const {
        return request<Order, Client>(baseUrl_ + "/v2/orders:by_client_order_id?client_order_id=" + clientOrderId, headers_.c_str(), nullptr, LogLevel::L_DEBUG, LogType::LT_ORDER);
    }

    Response<Order> Client::submitOrder(
        const std::string& symbol,
        const double quantity,
        const OrderSide side,
        const OrderType type,
        const TimeInForce tif,
        const double limit_price,
        const double stop_price,
        bool extended_hours,
        const std::string& client_order_id,
        double minFractionalQty,
        const OrderClass order_class,
        TakeProfitParams* take_profit_params,
        StopLossParams* stop_loss_params) const {

        if (!is_open_ && !extended_hours) {
            return Response<Order>(1, "Market Close.");
        }

        if (std::floor(quantity) != quantity) {
            if (type != OrderType::Market || (tif != TimeInForce::Day)) {
                return Response<Order>(1, "Fractional qty only for Market and Day order type");
            }
        }

        Response<Order> response;
        int32_t internalOrderId;
        int retry = 1;
        do {
            rapidjson::StringBuffer s;
            s.Clear();
            rapidjson::Writer<rapidjson::StringBuffer> writer(s);
            writer.StartObject();

            writer.Key("symbol");
            writer.String(symbol.c_str());

            writer.Key("qty");
            if (std::floor(quantity) == quantity) {
                writer.Int(static_cast<int>(quantity));
            }
            else {
                assert(minFractionalQty < 1);
                std::ostringstream q;
                q.precision(compute_number_decimals(minFractionalQty));
                q << std::fixed << fix_floating_error(quantity);
                writer.String(q.str().c_str());
                //writer.Double(fix_floating_error(quantity));
            }

            writer.Key("side");
            writer.String(to_string(side));

            writer.Key("type");
            writer.String(to_string(type));

            writer.Key("time_in_force");
            writer.String(to_string(tif));

            if (!std::isnan(limit_price)) {
                writer.Key("limit_price");
                writer.String(std::to_string(limit_price).c_str());
            }

            if (!std::isnan(stop_price)) {
                writer.Key("stop_price");
                writer.String(std::to_string(stop_price).c_str());
            }

            if (extended_hours) {
                writer.Key("extended_hours");
                writer.Bool(extended_hours);
            }

            internalOrderId = s_orderIdGen->nextOrderId();
            std::stringstream clientOrderId;
            clientOrderId << "ZORRO_";
            if (!client_order_id.empty()) {
                if (client_order_id.size() > 32) {  // Alpaca client order id max length is 48
                    clientOrderId << client_order_id.substr(0, 32);
                }
                else {
                    clientOrderId << client_order_id;
                }
                clientOrderId << "_";
            }

            clientOrderId << internalOrderId;
            writer.Key("client_order_id");
            writer.String(clientOrderId.str().c_str());

            if (order_class != OrderClass::Simple) {
                writer.Key("order_class");
                writer.String(to_string(order_class));
            }

            if (take_profit_params != nullptr) {
                writer.Key("take_profit");
                writer.StartObject();
                if (take_profit_params->limitPrice != "") {
                    writer.Key("limit_price");
                    writer.String(take_profit_params->limitPrice.c_str());
                }
                writer.EndObject();
            }

            if (stop_loss_params != nullptr) {
                writer.Key("stop_loss");
                writer.StartObject();
                if (!stop_loss_params->limitPrice.empty()) {
                    writer.Key("limit_price");
                    writer.String(stop_loss_params->limitPrice.c_str());
                }
                if (!stop_loss_params->stopPrice.empty()) {
                    writer.Key("stop_price");
                    writer.String(stop_loss_params->stopPrice.c_str());
                }
                writer.EndObject();
            }

            writer.EndObject();
            auto data = s.GetString();

            LOG_DEBUG_EXT(LogType::LT_ORDER, "--> POST %s/v2/orders\n", baseUrl_.c_str());
            if (data) {
                LOG_TRACE_EXT(LogType::LT_ORDER, "Data:\n%s\n", data);
            }
            response = request<Order, Client>(baseUrl_ + "/v2/orders", headers_.c_str(), data, LogLevel::L_DEBUG, LT_ORDER);
            if (!response && response.what() == "client_order_id must be unique") {
                // clinet order id has been used.
                // increment conflict count and try again.
                s_orderIdGen->onIdConflict();
            }   
        } while (!response && retry--);

        assert(!response || response.content().internal_id == internalOrderId);
        return response;
    }

    Response<Order> Client::replaceOrder(
        const std::string& id,
        const double quantity,
        const TimeInForce tif,
        const std::string& limit_price,
        const std::string& stop_price,
        const std::string& client_order_id) const {

        rapidjson::StringBuffer s;
        s.Clear();
        rapidjson::Writer<rapidjson::StringBuffer> writer(s);
        writer.StartObject();

        writer.Key("qty");
        writer.String(std::to_string(quantity).c_str());

        writer.Key("time_in_force");
        writer.String(to_string(tif));

        if (limit_price != "") {
            writer.Key("limit_price");
            writer.String(limit_price.c_str());
        }

        if (stop_price != "") {
            writer.Key("stop_price");
            writer.String(stop_price.c_str());
        }

        auto internalOrderId = s_orderIdGen->nextOrderId();
        std::stringstream clientOrderId;
        clientOrderId << "ZORRO_";
        if (!client_order_id.empty()) {
            if (client_order_id.size() > 32) {  // Alpaca client order id max length is 48
                clientOrderId << client_order_id.substr(0, 32);
            }
            else {
                clientOrderId << client_order_id;
            }
            clientOrderId << "_";
        }

        clientOrderId << internalOrderId;
        writer.Key("client_order_id");
        writer.String(clientOrderId.str().c_str());

        writer.EndObject();
        std::string body("#PATCH ");
        body.append(s.GetString());

        return request<Order, Client>(baseUrl_ + "/v2/orders/" + id, headers_.c_str(), body.c_str(), LogLevel::L_DEBUG, LogType::LT_ORDER);
    }

    Response<Order> Client::cancelOrder(const std::string& id) const {
        LOG_DEBUG("--> DELETE %s/v2/orders/%s\n", baseUrl_.c_str(), id.c_str());
        auto response = request<Order, Client>(baseUrl_ + "/v2/orders/" + id, headers_.c_str(), "#DELETE", LogLevel::L_DEBUG, LogType::LT_ORDER);
        if (!response) {
            // Alpaca cancelOrder not return a object
            Order* order;
            do {
                auto resp = getOrder(id, false);
                if (resp) {
                    order = &resp.content();
                    if (order->status == "canceled" || order->status == "filled") {
                        return resp;
                    }
                }
            } while (order->status == "pending_cancel");
            LOG_WARNING("failed to cancel order %s. order status=%s", id.c_str(), order->status.c_str());
            return Response<Order>(1, "Failed to cancel order");
        }
        return response;
    }

    Response<Position> Client::getPosition(const std::string& symbol) const {
        return request<Position, Client>(baseUrl_ + "/v2/positions/" + symbol, headers_.c_str(), nullptr, LogLevel::L_DEBUG, LogType::LT_POSITION);
    }

    Response<std::vector<Position>> Client::getPositions() const {
        return request<std::vector<Position>, Client>(baseUrl_ + "/v2/positions", headers_.c_str(), nullptr, LogLevel::L_DEBUG, LogType::LT_POSITION);
    }
} // namespace alpaca