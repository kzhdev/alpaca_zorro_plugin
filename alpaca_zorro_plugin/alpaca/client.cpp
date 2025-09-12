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
#include "market_data/market_data.h"
#include "websocket_proxy/websocket_proxy_client.h"
#include "account.h"

namespace {
    /// The base URL for API calls to the live trading API
    constexpr const char* s_APIBaseURLLive = "https://api.alpaca.markets/v2/";
    /// The base URL for API calls to the paper trading API
    constexpr const char* s_APIBaseURLPaper = "https://paper-api.alpaca.markets/v2/";
    std::unique_ptr<alpaca::ClientOrderIdGenerator> s_orderIdGen;

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
        getAllAssets();
    }

    void Client::getAllAssets()
    {
        auto asset_rsp = getAssets();
        if (asset_rsp)
        {
            assets_ = std::move(asset_rsp.content());
        }
        else
        {
            LOG_ERROR("Failed to get assets {}", asset_rsp.what());
        }
        //auto option_contracts_rsp = getOptionContracts();
        //if (option_contracts_rsp)
        //{
        //    option_contracts_ = std::move(option_contracts_rsp.content().contracts);
        //}
        //else
        //{
        //    LOG_ERROR("Failed to get option contracts %s\n", option_contracts_rsp.what());
        //}

        all_assets_.reserve(assets_.size() + option_contracts_.size());
        for (auto& asset : assets_)
        {
            asset.index = static_cast<uint32_t>(all_assets_.size());
            all_assets_.emplace(asset.symbol, &asset);
        }
        for (auto& option : option_contracts_)
        {
            option.index = static_cast<uint32_t>(all_assets_.size());
            all_assets_.emplace(option.symbol, &option);
        }
    }

    Response<Account> Client::getAccount() const {
        return request<Account>(baseUrl_ + "account", headers_.c_str(), nullptr, slick_logger::LogLevel::L_DEBUG);
    }

    Response<Balance> Client::getBalance() const {
        return request<Balance>(baseUrl_ + "account", headers_.c_str());
    }

    void Client::getBalance(Response<Balance> &rsp, uint64_t timestamp) const {
        request<Balance>(rsp, baseUrl_ + "account", headers_.c_str(), nullptr, slick_logger::LogLevel::L_TRACE, timestamp);
    }

    Response<Clock> Client::getClock() const {
        auto rsp = request<Clock>(baseUrl_ + "clock", headers_.c_str());
        if (rsp)
        {
            is_open_ = rsp.content().is_open;
        }
        return rsp;
    }

    void Client::getClock(Response<Clock> &rsp, uint64_t timestamp) const {
        request<Clock>(rsp, baseUrl_ + "clock", headers_.c_str(), nullptr, slick_logger::LogLevel::L_TRACE, timestamp);
        if (rsp)
        {
            is_open_ = rsp.content().is_open;
        }
    }

    Response<std::vector<Asset>> Client::getAssets(bool active_only) const {
        return active_only 
            ? request<std::vector<Asset>>(baseUrl_ + "assets?status=active", headers_.c_str())
            : request<std::vector<Asset>>(baseUrl_ + "assets", headers_.c_str());
    }

    Response<Asset> Client::getAsset(const std::string& symbol) const {
        return request<Asset>(baseUrl_ + "assets/" + symbol, headers_.c_str(), nullptr, slick_logger::LogLevel::L_DEBUG);
    }

    Response<OptionContracts> Client::getOptionContracts(const std::string &symbols, bool active_only) const {
        if (symbols.empty()) {
            return { 1, "No underlying symbols specified." };
        }
        Response<OptionContracts> rsp;
        do {
            BrokerProgress(0);
            if (active_only) {
                request<OptionContracts>(rsp, baseUrl_ + "options/contracts?status=active&limit=10000&underlying_symbols=" + symbols
                    + (rsp.content().next_page_token.empty() ? "" : "&page_token=" + rsp.content().next_page_token), headers_.c_str());
            }
            else
            {
                request<OptionContracts>(rsp, baseUrl_ + "options/contracts?limit=10000&underlying_symbols=" + symbols
                    + (rsp.content().next_page_token.empty() ? "" : "&page_token=" + rsp.content().next_page_token), headers_.c_str());
            }
        } while (!rsp.content().next_page_token.empty());
        return rsp;
    }

    Response<OptionContract> Client::getOptionContract(const std::string& symbol) const {
        return request<OptionContract>(baseUrl_ + "options/contracts/" + symbol, headers_.c_str(), nullptr, slick_logger::LogLevel::L_DEBUG);
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
        url << baseUrl_ << "orders";
        if (!queries.empty()) {
            url << "?";
            int32_t i = 0;
            for (; i < (int32_t)queries.size() - 1; ++i) {
                url << queries[i] << '&';
            }
            url << queries[i];
        }
        return request<std::vector<Order>>(url.str(), headers_.c_str());
    }

    Response<Order> Client::getOrder(const std::string& id, const bool nested) const {
        auto url = baseUrl_ + "orders/" + id;
        if (nested) {
            url += "?nested=true";
        }

        Response<Order> response;
        return request<Order>(url, headers_.c_str(), nullptr, slick_logger::LogLevel::L_DEBUG);
    }

    Response<Order> Client::getOrderByClientOrderId(const std::string& clientOrderId) const {
        return request<Order>(baseUrl_ + "orders:by_client_order_id?client_order_id=" + clientOrderId, headers_.c_str(), nullptr, slick_logger::LogLevel::L_DEBUG);
    }

    Response<Order> Client::submitOrder(
        const AssetBase* asset,
        const double quantity,
        const OrderSide side,
        const OrderType type,
        TimeInForce tif,
        const double limit_price,
        const double stop_price,
        const std::string& client_order_id,
        double minFractionalQty,
        const OrderClass order_class,
        TakeProfitParams* take_profit_params,
        StopLossParams* stop_loss_params) const {

        bool extended_hours = false;
        if (!is_open_ && asset->asset_class == AssetClass::CRYPTO)
        {
            return Response<Order>(1, "Market Close.");
        }

        if (asset->asset_class != AssetClass::CRYPTO) {
            if (std::floor(quantity) != quantity && (type != OrderType::Market || tif != TimeInForce::Day)) {
                return Response<Order>(1, "Fractional qty only for Market and Day order type");
            }
        }
        else if (tif != TimeInForce::GTC) {
            tif = TimeInForce::IOC;
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
            writer.String(asset->symbol.c_str());

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

            if (!is_open_) {
                writer.Key("extended_hours");
                writer.Bool(true);
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

            LOG_DEBUG("--> POST {}orders", baseUrl_.c_str());
            if (data) {
                LOG_TRACE("Data:\n{}", data);
            }
            response = request<Order>(baseUrl_ + "orders", headers_.c_str(), data, slick_logger::LogLevel::L_DEBUG);
            if (!response && response.what() == "client_order_id must be unique") {
                // client order id has been used.
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

        return request<Order>(baseUrl_ + "orders/" + id, headers_.c_str(), body.c_str(), slick_logger::LogLevel::L_DEBUG);
    }

    Response<Order> Client::cancelOrder(const std::string& id) const {
        LOG_DEBUG("--> DELETE {}orders/{}", baseUrl_, id);
        auto response = request<Order>(baseUrl_ + "orders/" + id, headers_.c_str(), "#DELETE", slick_logger::LogLevel::L_DEBUG);
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
            LOG_WARN("failed to cancel order {}. order status={}", id, order->status);
            return Response<Order>(1, "Failed to cancel order");
        }
        return response;
    }

    Response<Position> Client::getPosition(const std::string& symbol) const {
        return request<Position>(baseUrl_ + "positions/" + symbol, headers_.c_str(), nullptr, slick_logger::LogLevel::L_DEBUG);
    }

    Response<std::vector<Position>> Client::getPositions() const {
        return request<std::vector<Position>>(baseUrl_ + "positions", headers_.c_str(), nullptr, slick_logger::LogLevel::L_DEBUG);
    }
} // namespace alpaca