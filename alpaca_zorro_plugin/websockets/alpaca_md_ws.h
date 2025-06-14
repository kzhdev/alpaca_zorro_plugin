#pragma once

#include <websocket_proxy/websocket_proxy_client.h>
//#include <utils.h>
#include <unordered_set>
#include <sstream>
#include "logger.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <atomic>
#include "config.h"
#include "global.h"

#ifdef _WIN32
// Remove GetObject definition from windows.h, which prevents calls to
// RapidJSON's GetObject.
// https://github.com/Tencent/rapidjson/issues/1448
#undef GetObject
#endif  // _WIN32

namespace alpaca {

#define ALPACA_BASIC_DATA_WS_URL "wss://stream.data.alpaca.markets/v2/iex"
#define ALPACA_PRO_DATA_WS_URL "wss://stream.data.alpaca.markets/v2/sip"
#define ALPACA_CRYPTO_WS_URL "wss://stream.data.alpaca.markets/v1beta3/crypto/us"
#define ALPACA_BASIC_OPTION_WS_URL "wss://stream.data.alpaca.markets/v1beta1/indicative"
#define ALPACA_PRO_OPTION_WS_URL "wss://stream.data.alpaca.markets/v1beta1/opra"

    using SubscriptionType = websocket_proxy::SubscriptionType;
    extern std::unique_ptr<Client> client;

    class AlpacaMdWs : public websocket_proxy::WebsocketProxyClient, public websocket_proxy::WebsocketProxyCallback {
        enum Status : uint8_t {
            DISCONNECTED,
            LOGOUT,
            CONNECTING,
            AUTHENTICATING,
            AUTHENTICATED,
            SUBSCRIBING,
            SUBSCRIBED,
            NOT_SUBSCRIBED,
            UNSUBSCRIBING,
            UNSUBSCRIBED,
        };
        
        std::stringstream ss_;
        std::string api_key_;
        std::string api_secret_;
        uint32_t error_count_ = 0;

        struct Subscription {
            std::unique_ptr<slick::SlickQueue<Trade>> tradeQueue = std::make_unique<slick::SlickQueue<Trade>>(256);
            std::unique_ptr<slick::SlickQueue<Quote>> quoteQueue = std::make_unique<slick::SlickQueue<Quote>>(256);
            uint8_t subscriptionType = websocket_proxy::SubscriptionType::None;
        };

        std::unordered_map<const AssetBase*, Subscription> subscriptions_;
        struct ConnectionInfo
        {
            std::atomic_uint_fast64_t id_{ 0 };
            volatile Status status_{ Status::DISCONNECTED };
            std::string url_;
            std::string pending_subscription_;

            bool authenticated() const noexcept {
                return status_ >= Status::AUTHENTICATED;
            }
        };
        std::array<ConnectionInfo, AssetClass::__count__> connections_;
        std::unordered_map<uint64_t, ConnectionInfo*> connection_by_id_;

    public:
        AlpacaMdWs()
            : WebsocketProxyClient(this, "Alpaca", std::filesystem::current_path().append("Plugin\\websocket_proxy\\websocket_proxy.exe").string())
        {
            connections_[AssetClass::US_EQUITY].url_ = Config::get().alpacaPaidPlan ? ALPACA_PRO_DATA_WS_URL : ALPACA_BASIC_DATA_WS_URL;
            connections_[AssetClass::CRYPTO].url_ = ALPACA_CRYPTO_WS_URL;
            connections_[AssetClass::OPTIONS].url_ = Config::get().alpacaPaidPlan ? ALPACA_BASIC_OPTION_WS_URL : ALPACA_PRO_OPTION_WS_URL;
        }
        ~AlpacaMdWs() override = default;

        void init(std::string key, std::string secret) {
            api_key_ = std::move(key);
            api_secret_ = std::move(secret);

            //auto log_func = [this](websocket_proxy::LogLevel level, const std::string& msg) {
            //    Logger::instance().log(static_cast<alpaca::LogLevel>(level), LogType::LT_WEB_SOCKET_DATA, "%s\n", msg.c_str());
            //};
            //// pass logger to WebsocketProxyClient
            //setLogger(log_func);
        }

        bool open(AssetClass asset_class) {
            if (!Config::get().useWebsocket) {
                return false;
            }
            auto& connection = connections_[asset_class];
            connection.status_ = Status::CONNECTING;
            SPDLOG_INFO("Open Alpaca MD Websocket {}...", connection.url_);
            auto result = openWebSocket(connection.url_, api_key_);
            if (result.first) {
                if (!result.second) {
                    // WS already opened
                    connection.id_.store(result.first, std::memory_order_release);
                    connection.status_ = Status::AUTHENTICATED;
                    return true;
                }
                // New connection opened, need to authenticate
                waitForWsReadyOrError(connection, Status::AUTHENTICATED, 30000);
                if (connection.status_ == Status::AUTHENTICATED)
                {
                    connection_by_id_.emplace(connection.id_.load(std::memory_order_relaxed), &connection);
                    return true;
                }
                else
                {
                    connection.status_ = Status::DISCONNECTED;
                }
            }
            return false;
        }

        void reopen(AssetClass asset_class) {
            auto& connection = connections_[asset_class];
            if (connection.status_ != Status::LOGOUT) {
                if (!open(asset_class)) {
                    return;
                }

                BrokerError("Websocket reopened.");
                for (auto& kvp : subscriptions_) {
                    subscribeAsset(kvp.first, true);
                }
            }
        }

        void logout(ConnectionInfo &connection) {
            SPDLOG_INFO("Logout {}", connection.url_);
            unsubscribeAll();
            closeWebSocket(connection.id_.load(std::memory_order_relaxed));
            connection.status_ = Status::LOGOUT;
        }

        void logoutAll() {
            for (auto &connection : connections_)
            {
                if (connection.authenticated())
                {
                    logout(connection);
                }
            }
        }

        const std::string& url(AssetClass asset_class) const noexcept {
            return connections_[asset_class].url_;
        }

        bool authenticated(AssetClass asset_class) const noexcept {
            return connections_[asset_class].authenticated();
        }

        bool isSubscribed(const AssetBase* asset) const noexcept {
            return subscriptions_.find(asset) != subscriptions_.end();
        }

        bool subscribeAsset(const AssetBase* asset, bool force = false) {
            auto &global = zorro::Global::get();
            if (!authenticated(asset->asset_class)) {
                return false;
            }

            auto iter = subscriptions_.find(asset);
            if (iter == subscriptions_.end()) {
                iter = subscriptions_.emplace(asset, Subscription()).first;
            }
            else if (!force && ((global.price_type_ == 2 && (iter->second.subscriptionType & SubscriptionType::Trades)) || (global.price_type_ != 2 && (iter->second.subscriptionType & SubscriptionType::Quotes)))) {
                return true;
            }

            auto& subscription = iter->second;

            rapidjson::StringBuffer s;
            rapidjson::Writer<rapidjson::StringBuffer> writer(s);
            writer.StartObject();
            writer.Key("action");
            writer.String("subscribe");
            if (global.price_type_ == 2) {
                if (!Config::get().alpacaPaidPlan && (subscription.subscriptionType & SubscriptionType::Quotes))
                {
                    unsubscribeAsset(asset);
                }
                writer.Key("trades");
                writer.StartArray();
                writer.String(asset->symbol.c_str());
                writer.EndArray();
                subscription.subscriptionType |= SubscriptionType::Trades;
            }
            if (Config::get().alpacaPaidPlan || global.price_type_ != 2)
            {
                writer.Key("quotes");
                writer.StartArray();
                writer.String(asset->symbol.c_str());
                writer.EndArray();
                subscription.subscriptionType |= SubscriptionType::Quotes;
            }
            writer.EndObject();

            auto data = s.GetString();

            auto& connection = connections_[asset->asset_class];
            connection.pending_subscription_ = asset->symbol;
            connection.status_ = Status::SUBSCRIBING;
            auto id = connection.id_.load(std::memory_order_relaxed);
            SPDLOG_INFO("Subscribe {}, id={}, {}", asset->symbol, id, data);

            bool existing = false;
            if (!subscribe(id, asset->symbol, data, (uint32_t)s.GetSize(), (SubscriptionType)subscription.subscriptionType, existing)) {
                subscriptions_.erase(iter);
                return false;
            }
            if (existing) {
                connection.status_ = Status::SUBSCRIBED;
                connection.pending_subscription_ = "";
                return true;
            }
            waitForWsReadyOrError(connection, Status::SUBSCRIBED);
            connection.pending_subscription_ = "";
            return connection.status_ == Status::SUBSCRIBED;
        }

        Trade* getLastTrade(const AssetBase* asset) {
            auto& connection = connections_[asset->asset_class];
            if (connection.status_ == Status::DISCONNECTED) {
                reopen(asset->asset_class);
                return nullptr;
            }

            auto it = subscriptions_.find(asset);
            if (it == subscriptions_.end() || !(it->second.subscriptionType & SubscriptionType::Trades)) {
                return nullptr;
            }

            return it->second.tradeQueue->read_last();
        }

        Quote* getLastQuote(const AssetBase* asset) {
            auto& connection = connections_[asset->asset_class];
            if (connection.status_ == Status::DISCONNECTED) {
                reopen(asset->asset_class);
                return nullptr;
            }

            auto it = subscriptions_.find(asset);
            if (it == subscriptions_.end()) {
                return nullptr;
            }

            return it->second.quoteQueue->read_last();
        }

        void setLastTrade(const AssetBase* asset, const LastTrade& last_trade) {
            auto it = subscriptions_.find(asset);
            if (it != subscriptions_.end()) {
                auto& queue = it->second.tradeQueue;
                auto index = queue->reserve();
                auto *t = (*queue)[index];
                (*t) = last_trade.trade;
                queue->publish(index);
            }
        }

        void setLastQuote(const AssetBase* asset, const LastQuote& last_quote) {
            auto it = subscriptions_.find(asset);
            if (it != subscriptions_.end()) {
                auto& queue = it->second.quoteQueue;
                auto index = queue->reserve();
                auto* q = (*queue)[index];
                (*q) = last_quote.quote;
                queue->publish(index);
            }
        }

    private:

        void waitForWsReadyOrError(ConnectionInfo &connection, Status expected_status, uint32_t timeout = 10000) {
            auto start = websocket_proxy::get_timestamp();
            while ((websocket_proxy::get_timestamp() - start) < timeout) {
                if (connection.status_ == expected_status || connection.status_ == Status::DISCONNECTED) {
                    break;
                }
                BrokerProgress(1);
                std::this_thread::yield();
            }
        }

        void unsubscribeAsset(const AssetBase* asset)
        {
            auto& connection = connections_[asset->asset_class];
            if (connection.status_ < Status::AUTHENTICATED)
            {
                return;
            }
            SPDLOG_INFO("Unsubscribe {}", asset->symbol);
            connection.status_ = Status::UNSUBSCRIBING;

            rapidjson::StringBuffer s;
            rapidjson::Writer<rapidjson::StringBuffer> writer(s);
            writer.StartObject();
            writer.Key("action");
            writer.String("unsubscribe");

            writer.Key("trades");
            writer.StartArray();
            writer.String(asset->symbol.c_str());
            writer.EndArray();
            writer.Key("quotes");
            writer.StartArray();
            writer.String(asset->symbol.c_str());
            writer.EndArray();
            writer.EndObject();
            auto data = s.GetString();

            unsubscribe(connection.id_.load(std::memory_order_relaxed), asset->symbol, data, (uint32_t)s.GetSize());
            auto iter = subscriptions_.find(asset);
            if (iter != subscriptions_.end()) {
                iter->second.subscriptionType = SubscriptionType::None;
            }
            connection.status_ = Status::UNSUBSCRIBED;
        }

        void unsubscribeAll(bool keep_subscriptions = false) {
            for (auto& kvp : subscriptions_) {
                unsubscribeAsset(kvp.first);
            }
            if (!keep_subscriptions)
            {
                subscriptions_.clear();
            }
        }

        // Following functions are called from websocket proxy client thread

        void onWebsocketProxyServerDisconnected() override{
            for (auto& connection : connections_)
            {
                if (connection.status_ != Status::LOGOUT && connection.status_ != Status::CONNECTING) {
                    connection.id_.store(0, std::memory_order_release);
                    ss_.str() = "";
                    connection.status_ = Status::DISCONNECTED;
                }
            }
        }

        void onWebsocketOpened(uint64_t id) override {
            SPDLOG_INFO("Websocket opened. id={}", id);
            for (auto& connection : connections_)
            {
                if (connection.status_ == Status::CONNECTING)
                {
                    connection.id_.store(id, std::memory_order_release);
                    ss_.str("");
                    BrokerProgress(1);
                    break;
                }
            }
        }

        void onWebsocketClosed(uint64_t id) override {
            for (auto& connection : connections_)
            {
                if (connection.id_.load(std::memory_order_relaxed) == id && connection.status_ != Status::LOGOUT)
                {
                    connection.id_.store(0, std::memory_order_release);
                    ss_.str() = "";
                    connection.status_ = Status::DISCONNECTED;
                    BrokerError("Websocket disconnected");
                    break;
                }
            }
        }

        void onWebsocketError(uint64_t id, const char* err, uint32_t len) override {
            if (err && len) {
                SPDLOG_ERROR("WS error: {}", std::string(err, len));
            }
            else {
                SPDLOG_ERROR("WS error");
            }
            ++error_count_;
            if (error_count_ > 10) {
                for (auto& connection : connections_)
                {
                    if (connection.id_.load(std::memory_order_relaxed) == id)
                    {
                        closeWebSocket(id);
                        connection.status_ = Status::DISCONNECTED;
                        break;
                    }
                }
            }
        }

        void onWebsocketData(uint64_t id, const char* data, uint32_t len, uint32_t remaining) override {
            error_count_ = 0;

            ConnectionInfo *connection = nullptr;
            for (auto& conn : connections_)
            {
                if (conn.id_.load(std::memory_order_relaxed) == id) {
                    connection = &conn;
                }
            }
            if (!connection)
            {
                return;
            }

            if (data && len) {
                ss_ << std::string(data, len);
            }

            if (remaining == 0) { // message complete
                rapidjson::Document d;
                if (d.Parse(ss_.str().c_str()).HasParseError()) {
                    // When mutile instances connected to the proxy, a new instance 
                    // might reading a partial message from the queue. Drop the invalid message
                    ss_.str("");
                    return;
                }

                if (d.IsArray()) {
                    for (auto& item : d.GetArray()) {
                        if (!item.IsObject()) {
                            assert(false);
                            continue;
                        }
                        auto objJson = item.GetObject();
                        if (objJson.HasMember("T")) {
                            const auto& t = objJson["T"];
                            if (t == "error") {
                                Parser parser(objJson);
                                std::string msg;
                                parser.get<std::string>("msg", msg);
                                int32_t code;
                                parser.get<int32_t>("code", code);
                                
                                SPDLOG_ERROR("On Ws error {}({}).", msg, code);
                                BrokerError((msg + " (" + std::to_string(code) + ")").c_str());

                                ++error_count_;
                                if (error_count_ > 10) {
                                    closeWebSocket(id);
                                    MemoryBarrier();
                                    connection->status_ = Status::DISCONNECTED;
                                }
                                break;
                            }

                            if (t == "success") {
                                Parser<decltype(item.GetObject())> parser(objJson);
                                std::string msg;
                                parser.get<std::string>("msg", msg);
                                if (msg == "connected") {
                                    SPDLOG_INFO("Alpaca MarketData Websocket Opened.");
                                    BrokerError("Alpaca MarketData Websocket Opened");
                                    authenticate(connection);
                                }
                                else if (msg == "authenticated") {
                                    SPDLOG_INFO("Authenticated.");
                                    BrokerError("Websocket Authenticated");
                                    connection->status_ = Status::AUTHENTICATED;
                                }
                            }
                            else if (t == "subscription") {
                                if (connection->status_ == Status::SUBSCRIBING) {
                                    auto it = ss_.str().find(connection->pending_subscription_);
                                    if (it != std::string::npos) {
                                        connection->status_ = Status::SUBSCRIBED;
                                    }
                                    else {
                                        // should not come here
                                        connection->status_ = Status::NOT_SUBSCRIBED;
                                    }
                                }
                                else /*if (status_ == Status::UNSUBSCRIBING &&
                                         ss_.str() == "[{\"T\":\"subscription\",\"trades\":[],\"quotes\":[],\"bars\":[]}]")*/ {
                                    //status_ = Status::UNSUBSCRIBED;
                                        //LOG_INFO("Unsubscribed.\n");
                                    SPDLOG_TRACE(ss_.str());
                                }
                            }
                            else if (t == "t") {
                                onTrade(objJson);
                            }
                            else if (t == "q") {
                                onQuote(objJson);
                            }
                            else {
                                SPDLOG_WARN("Unhandled {}. {}", t.GetString(), ss_.str());
                            }
                        }
                    }
                }

                SPDLOG_TRACE(ss_.str());

                // reset message
                ss_.str("");
            }
        }

        void authenticate(ConnectionInfo *connection) {
            rapidjson::StringBuffer s;
            rapidjson::Writer<rapidjson::StringBuffer> writer(s);
            writer.StartObject();

            writer.Key("action");
            writer.String("auth");

            writer.Key("key");
            writer.String(api_key_.c_str());

            writer.Key("secret");
            writer.String(api_secret_.c_str());

            writer.EndObject();
            auto data = s.GetString();
            SPDLOG_DEBUG("Authenticating...");
            send(connection->id_.load(std::memory_order_relaxed), data, (uint32_t)s.GetSize());
        }

        template<typename T>
        void onTrade(T& objJson) {
            std::string symbol;
            Parser<T> parser(objJson);
            parser.get<std::string>("S", symbol);
            auto &assets = client->allAssets();
            auto* asset = assets.at(symbol);
            auto it = subscriptions_.find(asset);
            if (it != subscriptions_.end()) {
                auto& queue = it->second.tradeQueue;
                auto index = queue->reserve();
                auto *trade = (*queue)[index];
                parser.get<double>("p", trade->price);
                parser.get<int>("s", trade->size);
                queue->publish(index);
            }

            auto &global = zorro::Global::get();
            if (global.handle_ && global.price_type_ == 2)
            {
                PostMessage(global.handle_, WM_APP + 1, 0, 0);
            }
        }

        template<typename T>
        void onQuote(T& objJson) {
            std::string symbol;
            Parser<T> parser(objJson);
            parser.get<std::string>("S", symbol);
            auto& assets = client->allAssets();
            auto* asset = assets.at(symbol);
            auto it = subscriptions_.find(asset);
            if (it != subscriptions_.end()) {
                auto& queue = it->second.quoteQueue;
                auto index = queue->reserve();
                auto quote = (*queue)[index];
                parser.get<double>("ap", quote->ask_price);
                parser.get<int>("as", quote->ask_size);
                parser.get<double>("bp", quote->bid_price);
                parser.get<int>("bs", quote->bid_size);
                queue->publish(index);
            }

            auto &global = zorro::Global::get();
            if (global.handle_ && global.price_type_ != 2)
            {
                PostMessage(global.handle_, WM_APP + 1, 0, 0);
            }
        }
    };

}
