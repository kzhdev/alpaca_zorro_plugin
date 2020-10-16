#include "stdafx.h"
#include "alpaca/client.h"

#include <sstream>
#include <memory>

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "date/date.h"
#include "alpaca/client_order_id_generator.h"

namespace {
    /// The base URL for API calls to the live trading API
    constexpr const char* kAPIBaseURLLive = "https://api.alpaca.markets";
    /// The base URL for API calls to the paper trading API
    constexpr const char* kAPIBaseURLPaper = "https://paper-api.alpaca.markets";
    /// The base URL for API calls to the data API
    constexpr const char* kAPIDataURL = "https://data.alpaca.markets";
    constexpr const char* kPolygonDataURL = "https://api.polygon.io";
    std::unique_ptr<alpaca::ClientOrderIdGenerator> s_orderIdGen;
}

namespace alpaca {

    Client::Client(std::string key, std::string secret, bool isPaperTrading)
        : baseUrl_(isPaperTrading ? kAPIBaseURLPaper : kAPIBaseURLLive)
        , dataUrl_(isPaperTrading ? kAPIDataURL : kPolygonDataURL)
        , headers_("Content-Type:application/json\nAPCA-API-KEY-ID:" + std::move(key) + "\n" + "APCA-API-SECRET-KEY:" + std::move(secret))
    {
        s_orderIdGen = std::make_unique<ClientOrderIdGenerator>(*this);
    }

    Response<Account> Client::getAccount() const {
        return request<Account>(baseUrl_ + "/v2/account");
    }

    Response<Clock> Client::getClock() const {
        auto rt = request<Clock>(baseUrl_ + "/v2/clock");
        is_open_ = rt.content().is_open;
        return rt;
    }

    Response<Asset> Client::getAsset(const std::string& symbol) const {
        return request<Asset>(baseUrl_ + "/v2/assets/" + symbol);
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

        return request<std::vector<Order>>(url.str());
    }

    Response<Order> Client::getOrder(const std::string& id, const bool nested) const {
        auto url = baseUrl_ + "/v2/orders/" + id;
        if (nested) {
            url += "?nested=true";
        }

        return request<Order>(url);
    }

    Response<Order> Client::getOrderByClientOrderId(const std::string& clientOrderId) const {
        return request<Order>(baseUrl_ + "/v2/orders:by_client_order_id?client_order_id=" + clientOrderId);
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
        StopLossParams* stop_loss_params) {

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

            response = request<Order>(baseUrl_ + "/v2/orders", data);
            if (!response && response.what() == "client_order_id must be unique") {
                // clinet order id has been used.
                // increment conflict count and try again.
                s_orderIdGen->onIdConflict();
            }   
        } while (!response && retry--);

        response.content().internal_id = internalOrderId;
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
        writer.Int(quantity);

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

        if (client_order_id != "") {
            writer.Key("client_order_id");
            writer.String(client_order_id.c_str());
        }

        writer.EndObject();
        std::string body("#PATCH");
        body.append(s.GetString());

        auto url = "/v2/orders/" + id;
        return request<Order>(baseUrl_ + "/v2/orders/" + id, body.c_str());
    }

    Response<Order> Client::cancelOrder(const std::string& id) const {
        return request<Order>(baseUrl_ + "/v2/orders/" + id, "#DELETE");
    }

    Response<Bars> Client::getBars(
        const std::vector<std::string>& symbols,
        const __time32_t start,
        const __time32_t end,
        int nTickMinutes,
        const const uint32_t limit) const {
        std::string symbols_string = "";
        for (size_t i = 0; i < symbols.size(); ++i) {
            symbols_string += symbols[i];
            symbols_string += ",";
        }
        // remove trailing ','
        symbols_string.pop_back();

        std::string timeframe = "1Min";
        if (nTickMinutes >= 5 && nTickMinutes < 15) {
            timeframe = "5Min";
        }
        else if (nTickMinutes >= 15 && nTickMinutes < 1440) {
            timeframe = "15Min";
        } 
        else if (nTickMinutes >= 1440) {
            timeframe = "1D";
        }

        std::string sStart;
        std::string sEnd;
        {
            using namespace date;
            try {
                if (start) {
                    sStart = format("%FT%T", date::sys_seconds{ std::chrono::seconds{ start } });
                    sStart.append("-00:00");    // add UTC timezone offset
                }
                
                if (end) {
                    sEnd = format("%FT%T", date::sys_seconds{ std::chrono::seconds{ end } });
                    sEnd.append("-00:00");  // add UTC timezone offset
                }
            }
            catch (const std::exception& e) {
                assert(false);
            }
        }

        std::stringstream url;
        url << dataUrl_ << "/v1/bars/" << timeframe << "?symbols=" << symbols_string << "&limit=" << limit
            << (sStart.empty() ? "" : "&start=" + sStart) << (sEnd.empty() ? "" : "&end=" + sEnd);

        return request<Bars>(url.str());
    }

    Response<LastQuote> Client::getLastQuote(const std::string& symbol) const {
        return request<LastQuote>(dataUrl_ + "/v1/last_quote/stocks/" + symbol);
    }

    Response<Position> Client::getPosition(const std::string& symbol) const {
        return request<Position>(baseUrl_ + "/v2/positions/" + symbol);
    }
} // namespace alpaca