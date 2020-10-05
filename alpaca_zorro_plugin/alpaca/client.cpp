#include "stdafx.h"
#include "alpaca/client.h"

#include <utility>

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

namespace {
    /// The base URL for API calls to the live trading API
    constexpr const char* kAPIBaseURLLive = "https://api.alpaca.markets";
    /// The base URL for API calls to the paper trading API
    constexpr const char* kAPIBaseURLPaper = "https://paper-api.alpaca.markets";
    /// The base URL for API calls to the data API
    constexpr const char* kAPIDataURL = "https://data.alpaca.markets";
    constexpr const char* kPolygonDataURL = "https://api.polygon.io";
}

namespace alpaca {

    Client::Client(std::string key, std::string secret, bool isPaperTrading)
        : baseUrl_(isPaperTrading ? kAPIBaseURLPaper : kAPIBaseURLLive)
        , headers_("APCA-API-KEY-ID: " + std::move(key) + "\n" + "APCA-API-SECRET-KEY: " + std::move(secret))
#ifdef _DEBUG
        , log_(fopen("./Log/alpaca.log", "w"))
#endif
    {
#ifdef _DEBUG
        if (!log_) {
            throw std::runtime_error("Failed to open log. ./Log/alpaca.log");
        }
#endif
    }

    Response<Account> Client::getAccount() const {
        return request<Account>("/v2/account");
    }

    Response<Clock> Client::getClock() const {
        return request<Clock>("/v2/clock");
    }

    Response<Asset> Client::getAsset(const std::string& symbol) const {
        return request<Asset>("/v2/assets/" + symbol);
    }

    Response<Order> Client::getOrder(const std::string& id, const bool nested) const {
        auto url = "/v2/orders/" + id;
        if (nested) {
            url += "?nested=true";
        }

        return request<Order>(url);
    }

    //std::pair<Status, Order> Client::getOrderByClientOrderID(const std::string& client_order_id) const {

    //}
    //
    //std::pair<Status, std::vector<Order>> Client::getOrders(const ActionStatus status,
    //                                                        const int limit,
    //                                                        const std::string& after,
    //                                                        const std::string& until,
    //                                                        const OrderDirection direction,
    //                                                        const bool nested) const {
    //}

    Response<Order> Client::submitOrder(
        const std::string& symbol,
        const int quantity,
        const OrderSide side,
        const OrderType type,
        const OrderTimeInForce tif,
        const std::string& limit_price,
        const std::string& stop_price,
        const bool extended_hours,
        const std::string& client_order_id,
        const OrderClass order_class,
        TakeProfitParams* take_profit_params,
        StopLossParams* stop_loss_params) const {

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

        if (limit_price != "") {
            writer.Key("limit_price");
            writer.String(limit_price.c_str());
        }

        if (stop_price != "") {
            writer.Key("stop_price");
            writer.String(stop_price.c_str());
        }

        if (extended_hours) {
            writer.Key("extended_hours");
            writer.Bool(extended_hours);
        }

        if (client_order_id != "") {
            writer.Key("client_order_id");
            writer.String(client_order_id.c_str());
        }

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
            if (stop_loss_params->limitPrice != "") {
                writer.Key("limit_price");
                writer.String(stop_loss_params->limitPrice.c_str());
            }
            if (stop_loss_params->stopPrice != "") {
                writer.Key("stop_price");
                writer.String(stop_loss_params->stopPrice.c_str());
            }
            writer.EndObject();
        }

        writer.EndObject();
        auto data = s.GetString();

        return request<Order>("/v2/orders", data);
    }

    //Response<Order> Client::replaceOrder(const std::string& id,
    //                                              const int quantity,
    //                                              const OrderTimeInForce tif,
    //                                              const std::string& limit_price,
    //                                              const std::string& stop_price,
    //                                              const std::string& client_order_id) const {
    //  Order order;
    //
    //  rapidjson::StringBuffer s;
    //  s.Clear();
    //  rapidjson::Writer<rapidjson::StringBuffer> writer(s);
    //  writer.StartObject();
    //
    //  writer.Key("qty");
    //  writer.Int(quantity);
    //
    //  writer.Key("time_in_force");
    //  writer.String(orderTimeInForceToString(tif).c_str());
    //
    //  if (limit_price != "") {
    //    writer.Key("limit_price");
    //    writer.String(limit_price.c_str());
    //  }
    //
    //  if (stop_price != "") {
    //    writer.Key("stop_price");
    //    writer.String(stop_price.c_str());
    //  }
    //
    //  if (client_order_id != "") {
    //    writer.Key("client_order_id");
    //    writer.String(client_order_id.c_str());
    //  }
    //
    //  writer.EndObject();
    //  auto body = s.GetString();
    //
    //  auto url = "/v2/orders/" + id;
    //}

    //Response<std::vector<Order>> Client::cancelOrders() const {
    //}
    //

    Response<Bars> Client::getBars(
        const std::vector<std::string>& symbols,
        const std::string& start,
        const std::string& end,
        const std::string& after,
        const std::string& until,
        const std::string& timeframe,
        const uint32_t limit) const {
        Bars bars;

        std::string symbols_string = "";
        for (size_t i = 0; i < symbols.size(); ++i) {
            symbols_string += symbols[i];
            symbols_string += ",";
        }
        // remove trailing ','
        symbols_string.pop_back();

        auto url = "/v1/bars/" + timeframe;

        return request<Bars>(url);
    }

} // namespace alpaca