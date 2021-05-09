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
#include "market_data/polygon.h"

namespace {
    /// The base URL for API calls to the live trading API
    constexpr const char* s_APIBaseURLLive = "https://api.alpaca.markets";
    /// The base URL for API calls to the paper trading API
    constexpr const char* s_APIBaseURLPaper = "https://paper-api.alpaca.markets";
    std::unique_ptr<alpaca::ClientOrderIdGenerator> s_orderIdGen;

    std::unique_ptr<alpaca::AlpacaMarketData> alpacaMarketData;
    std::unique_ptr<alpaca::Polygon> polygon;
}

namespace alpaca {

    Client::Client(std::string key, std::string secret, bool isPaperTrading)
        : baseUrl_(isPaperTrading ? s_APIBaseURLPaper : s_APIBaseURLLive)
        , apiKey_(std::move(key))
        , headers_("Content-Type:application/json\nAPCA-API-KEY-ID:" + apiKey_ + "\n" + "APCA-API-SECRET-KEY:" + std::move(secret))
        , isLiveMode_(!isPaperTrading)
    {
        s_orderIdGen = std::make_unique<ClientOrderIdGenerator>(*this);
    }

    Response<Account> Client::getAccount() const {
        return request<Account, Client>(baseUrl_ + "/v2/account", headers_.c_str());
    }

    Response<Clock> Client::getClock() const {
        auto rt = request<Clock, Client>(baseUrl_ + "/v2/clock", headers_.c_str());
        is_open_ = rt.content().is_open;
        return rt;
    }

    Response<std::vector<Asset>> Client::getAssets() const {
        LOG_DEBUG("%s/v2/assets\n", baseUrl_.c_str());
        return request<std::vector<Asset>, Client>(baseUrl_ + "/v2/assets", headers_.c_str());
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
        return request<std::vector<Order>, Client>(url.str(), headers_.c_str());
    }

    Response<Order> Client::getOrder(const std::string& id, const bool nested, const bool logResponse) const {
        auto url = baseUrl_ + "/v2/orders/" + id;
        if (nested) {
            url += "?nested=true";
        }

        Response<Order> response;
        if (logResponse) {
            return request<Order, Client>(url, headers_.c_str());
        }
        return request<Order, Client>(url, headers_.c_str(), nullptr, LogLevel::L_TRACE);
    }

    Response<Order> Client::getOrderByClientOrderId(const std::string& clientOrderId) const {
        return request<Order, Client>(baseUrl_ + "/v2/orders:by_client_order_id?client_order_id=" + clientOrderId, headers_.c_str());
    }

    Response<Order> Client::submitOrder(
        const std::string& symbol,
        const int quantity,
        const OrderSide side,
        const OrderType type,
        const TimeInForce tif,
        const std::string& limit_price,
        const std::string& stop_price,
        bool extended_hours,
        const std::string& client_order_id,
        const OrderClass order_class,
        TakeProfitParams* take_profit_params,
        StopLossParams* stop_loss_params) const {

        if (!is_open_ && !extended_hours) {
            return Response<Order>(1, "Market Close.");
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
            writer.Int(quantity);

            writer.Key("side");
            writer.String(to_string(side));

            writer.Key("type");
            writer.String(to_string(type));

            writer.Key("time_in_force");
            writer.String(to_string(tif));

            if (!limit_price.empty()) {
                writer.Key("limit_price");
                writer.String(limit_price.c_str());
            }

            if (!stop_price.empty()) {
                writer.Key("stop_price");
                writer.String(stop_price.c_str());
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

            LOG_DEBUG("--> POST %s/v2/orders\n", baseUrl_.c_str());
            if (data) {
                LOG_TRACE("Data:\n%s\n", data);
            }
            response = request<Order, Client>(baseUrl_ + "/v2/orders", headers_.c_str(), nullptr, LogLevel::L_TRACE);
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
        const int quantity,
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

        return request<Order, Client>(baseUrl_ + "/v2/orders/" + id, headers_.c_str(), body.c_str(), LogLevel::L_TRACE);
    }

    Response<Order> Client::cancelOrder(const std::string& id) const {
        LOG_DEBUG("--> DELETE %s/v2/orders/%s\n", baseUrl_.c_str(), id.c_str());
        auto response = request<Order, Client>(baseUrl_ + "/v2/orders/" + id, headers_.c_str(), "#DELETE", LogLevel::L_TRACE);
        if (!response) {
            // Alpaca cancelOrder not return a object
            Order* order;
            do {
                auto resp = getOrder(id, false, true);
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
        return request<Position, Client>(baseUrl_ + "/v2/positions/" + symbol, headers_.c_str());
    }
} // namespace alpaca