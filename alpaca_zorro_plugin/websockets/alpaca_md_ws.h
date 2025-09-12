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
        
        std::string message_buffer_;
        std::string api_key_;
        std::string api_secret_;
        uint32_t error_count_ = 0;
        const std::unordered_map<std::string, AssetBase*>* assets_cache_ = nullptr;

        struct Subscription {
            slick::SlickQueue<Trade> tradeQueue{256};
            slick::SlickQueue<Quote> quoteQueue{256};
            uint8_t subscriptionType = websocket_proxy::SubscriptionType::None;

            ~Subscription() {
                LOG_INFO("**** Subscription destroyed {:p}", static_cast<void*>(this));
            }
        };

        std::vector<std::shared_ptr<Subscription>> subscriptions_;
        std::vector<const AssetBase*> subscribed_assets_;
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
            // Defer initialization until client is ready
        }
        ~AlpacaMdWs() override = default;

        void logError(std::function<std::string()> &&msg) override { LOG_ERROR(msg()); }
        void logWarning(std::function<std::string()> &&msg) override { LOG_WARN(msg()); }
        void logInfo(std::function<std::string()> &&msg) override { LOG_INFO(msg()); }
        void logDebug(std::function<std::string()> &&msg) override { LOG_DEBUG(msg()); }

        void init(std::string key, std::string secret) {
            api_key_ = std::move(key);
            api_secret_ = std::move(secret);
            
            // Initialize subscription containers now that client is ready
            if (client && subscriptions_.empty()) {
                subscriptions_.resize(client->allAssets().size(), nullptr);
                subscribed_assets_.reserve(client->allAssets().size());
                assets_cache_ = &client->allAssets();
                message_buffer_.reserve(8192);
            }
        }

        bool open(AssetClass asset_class) {
            if (!Config::get().useWebsocket) {
                return false;
            }
            auto& connection = connections_[asset_class];
            connection.status_ = Status::CONNECTING;
            LOG_INFO("Open Alpaca MD Websocket {}...", connection.url_);
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
                for (const auto *asset : subscribed_assets_) {
                    subscribeAsset(asset, true);
                }
            }
        }

        void logout(ConnectionInfo &connection) {
            LOG_INFO("Logout {}", connection.url_);
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

        bool hasSubscription(const AssetBase* asset) const noexcept {
            return subscriptions_[asset->index] != nullptr;
        }

        bool subscribeAsset(const AssetBase* asset, bool force = false) {
            auto &global = zorro::Global::get();
            if (!authenticated(asset->asset_class)) {
                return false;
            }

            if (!hasSubscription(asset))
            {
                subscriptions_[asset->index] = std::make_shared<Subscription>();
                assert(subscriptions_[asset->index]->quoteQueue.use_shm() == false);
                assert(subscriptions_[asset->index]->quoteQueue.size() == 256);
                assert(subscriptions_[asset->index]->tradeQueue.use_shm() == false);
                assert(subscriptions_[asset->index]->tradeQueue.size() == 256);
                subscribed_assets_.push_back(asset);

                LOG_INFO("**** Subscription created {:p}", static_cast<void*>(subscriptions_[asset->index].get()));
            }
            else {
                auto sub_type = subscriptions_[asset->index]->subscriptionType;
                if (!force && ((global.price_type_ == 2 && (sub_type & SubscriptionType::Trades)) || (global.price_type_ != 2 && (sub_type & SubscriptionType::Quotes)))) {
                    // already subscribed
                    return true;
                }
            }

            auto &subscription = subscriptions_[asset->index];

            rapidjson::StringBuffer s;
            rapidjson::Writer<rapidjson::StringBuffer> writer(s);
            writer.StartObject();
            writer.Key("action");
            writer.String("subscribe");
            if (global.price_type_ == 2) {
                if (!Config::get().alpacaPaidPlan && (subscription->subscriptionType & SubscriptionType::Quotes))
                {
                    unsubscribeAsset(asset, true);
                }
                writer.Key("trades");
                writer.StartArray();
                writer.String(asset->symbol.c_str());
                writer.EndArray();
                subscription->subscriptionType |= SubscriptionType::Trades;
            }
            if (Config::get().alpacaPaidPlan || global.price_type_ != 2)
            {
                writer.Key("quotes");
                writer.StartArray();
                writer.String(asset->symbol.c_str());
                writer.EndArray();
                subscription->subscriptionType |= SubscriptionType::Quotes;
            }
            writer.EndObject();

            auto data = s.GetString();

            auto& connection = connections_[asset->asset_class];
            connection.pending_subscription_ = asset->symbol;
            connection.status_ = Status::SUBSCRIBING;
            auto id = connection.id_.load(std::memory_order_relaxed);
            LOG_INFO("Subscribe {}, id={}, {}", asset->symbol, id, data);

            bool existing = false;
            if (!subscribe(id, asset->symbol, data, (uint32_t)s.GetSize(), (SubscriptionType)subscription->subscriptionType, existing)) {
                subscription.reset();
                return false;
            }
            if (existing) {
                connection.status_ = Status::SUBSCRIBED;
                connection.pending_subscription_ = "";
                return true;
            }
            waitForWsReadyOrError(connection, Status::SUBSCRIBED);
            connection.pending_subscription_ = "";
            LOG_DEBUG("**** Subscribe status: {}", (int)connection.status_);
            return connection.status_ == Status::SUBSCRIBED;
        }

        Trade* getLastTrade(const AssetBase* asset) {
            auto& connection = connections_[asset->asset_class];
            if (connection.status_ == Status::DISCONNECTED) {
                reopen(asset->asset_class);
                return nullptr;
            }

            auto &subscription = subscriptions_[asset->index];
            if (!subscription || !(subscription->subscriptionType & SubscriptionType::Trades)) {
                return nullptr;
            }

            return subscription->tradeQueue.read_last();
        }

        Quote* getLastQuote(const AssetBase* asset) {
            auto& connection = connections_[asset->asset_class];
            if (connection.status_ == Status::DISCONNECTED) {
                reopen(asset->asset_class);
                return nullptr;
            }

            auto &subscription = subscriptions_[asset->index];
            if (!subscription || !(subscription->subscriptionType & SubscriptionType::Quotes)) {
                return nullptr;
            }

            return subscription->quoteQueue.read_last();
        }

        void setLastTrade(const AssetBase* asset, const LastTrade& last_trade) {
            auto &subscription = subscriptions_[asset->index];
            if (subscription) {
                auto& queue = subscription->tradeQueue;
                auto index = queue.reserve();
                auto *t = queue[index];
                (*t) = last_trade.trade;
                queue.publish(index);
            }
        }

        void setLastQuote(const AssetBase* asset, const LastQuote& last_quote) {
            auto &subscription = subscriptions_[asset->index];
            if (subscription) {
                auto& queue = subscription->quoteQueue;
                auto index = queue.reserve();
                auto* q = queue[index];
                (*q) = last_quote.quote;
                queue.publish(index);
            }
        }

    private:

        void waitForWsReadyOrError(ConnectionInfo &connection, Status expected_status, uint32_t timeout = 10000) {
            auto start = websocket_proxy::get_timestamp();
            uint64_t now;
            while (((now = websocket_proxy::get_timestamp()) - start) < timeout) {
                if (connection.status_ == expected_status || connection.status_ == Status::DISCONNECTED) {
                    return;
                }
                BrokerProgress(1);
                std::this_thread::yield();
            }
            LOG_DEBUG("**** Wait for ws ready timeout. {} started at {}", now, start);
        }

        void unsubscribeAsset(const AssetBase* asset, bool keep_subscription = false)
        {
            auto& connection = connections_[asset->asset_class];
            if (connection.status_ < Status::AUTHENTICATED)
            {
                return;
            }
            LOG_INFO("Unsubscribe {}", asset->symbol);
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
            auto &subscription = subscriptions_[asset->index];
            if (subscription) {
                subscription->subscriptionType = SubscriptionType::None;
            }
            connection.status_ = Status::UNSUBSCRIBED;
            if (!keep_subscription && subscription) {
                subscription.reset();
            }
        }

        void unsubscribeAll(bool keep_subscriptions = false) {
            for (const auto *asset : subscribed_assets_) {
                unsubscribeAsset(asset, keep_subscriptions);
            }
            if (!keep_subscriptions)
            {
                subscribed_assets_.clear();
            }
        }

        // Following functions are called from websocket proxy client thread

        void onWebsocketProxyServerDisconnected() override {
            for (auto& connection : connections_)
            {
                if (connection.status_ != Status::LOGOUT && connection.status_ != Status::CONNECTING) {
                    connection.id_.store(0, std::memory_order_release);
                    message_buffer_.clear();
                    connection.status_ = Status::DISCONNECTED;
                }
            }
        }

        void onWebsocketOpened(uint64_t id) override {
            LOG_INFO("Websocket opened. id={}", id);
            for (auto& connection : connections_)
            {
                if (connection.status_ == Status::CONNECTING)
                {
                    connection.id_.store(id, std::memory_order_release);
                    message_buffer_.clear();
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
                    message_buffer_.clear();
                    connection.status_ = Status::DISCONNECTED;
                    BrokerError("Websocket disconnected");
                    break;
                }
            }
        }

        void onWebsocketError(uint64_t id, const char* err, uint32_t len) override {
            if (err && len) {
                LOG_ERROR("WS error: {}", std::string(err, len));
            }
            else {
                LOG_ERROR("WS error");
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
            ConnectionInfo* connection = nullptr;

            // Fast connection lookup using cached map
            auto conn_it = connection_by_id_.find(id);
            if (conn_it != connection_by_id_.end()) {
                connection = conn_it->second;
            }
            else {
                for (auto& conn : connections_) {
                    if (conn.id_.load(std::memory_order_relaxed) == id) {
                        connection = &conn;
                    }
                }

                if (!connection) {
                    return;
                }
            }

            if (data && len) {
                message_buffer_.append(data, len);
            }

            if (remaining == 0) { // message complete
                rapidjson::Document d;
                if (d.Parse(message_buffer_.c_str()).HasParseError()) {
                    // When mutile instances connected to the proxy, a new instance 
                    // might reading a partial message from the queue. Drop the invalid message
                    message_buffer_.clear();
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
                                
                                LOG_ERROR("On Ws error {}({}).", msg, code);
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
                                    LOG_INFO("Alpaca MarketData Websocket Opened.");
                                    BrokerError("Alpaca MarketData Websocket Opened");
                                    authenticate(connection);
                                }
                                else if (msg == "authenticated") {
                                    LOG_INFO("Authenticated.");
                                    BrokerError("Websocket Authenticated");
                                    connection->status_ = Status::AUTHENTICATED;
                                }
                            }
                            else if (t == "subscription") {
                                if (connection->status_ == Status::SUBSCRIBING) {
                                    auto it = message_buffer_.find(connection->pending_subscription_);
                                    if (it != std::string::npos) {
                                        connection->status_ = Status::SUBSCRIBED;
                                    }
                                    else {
                                        // should not come here
                                        connection->status_ = Status::NOT_SUBSCRIBED;
                                    }
                                }
                                else /*if (status_ == Status::UNSUBSCRIBING &&
                                         message_buffer_ == "[{\"T\":\"subscription\",\"trades\":[],\"quotes\":[],\"bars\":[]}]")*/ {
                                    //status_ = Status::UNSUBSCRIBED;
                                        //LOG_INFO("Unsubscribed.\n");
                                    LOG_TRACE(message_buffer_);
                                }
                            }
                            else if (t == "t") {
                                onTrade(objJson);
                            }
                            else if (t == "q") {
                                onQuote(objJson);
                            }
                            else {
                                LOG_WARN("Unhandled {}. {}", t.GetString(), message_buffer_);
                            }
                        }
                    }
                }

                LOG_TRACE(message_buffer_);

                // reset message
                message_buffer_.clear();
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
            LOG_DEBUG("Authenticating...");
            send(connection->id_.load(std::memory_order_relaxed), data, (uint32_t)s.GetSize());
        }

        template<typename T>
        void onTrade(T& objJson) {
            std::string symbol;
            Parser<T> parser(objJson);
            parser.get<std::string>("S", symbol);
            auto it = assets_cache_->find(symbol);
            if (it == assets_cache_->end()) return;
            
            auto* asset = it->second;
            auto subscription = subscriptions_[asset->index];  // Copy shared_ptr for safety, it could reset in unsubscribeAsset
            if (subscription) {
                auto& queue = subscription->tradeQueue;
                auto index = queue.reserve();
                auto *trade = queue[index];
                parser.get<double>("p", trade->price);
                parser.get<int>("s", trade->size);
                queue.publish(index);
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
            auto it = assets_cache_->find(symbol);
            if (it == assets_cache_->end()) return;
            
            auto* asset = it->second;
            auto subscription = subscriptions_[asset->index];   // Copy shared_ptr for safety, it could reset in unsubscribeAsset
            if (subscription) {
                auto& queue = subscription->quoteQueue;
                auto index = queue.reserve();
                auto quote = queue[index];
                parser.get<double>("ap", quote->ask_price);
                parser.get<int>("as", quote->ask_size);
                parser.get<double>("bp", quote->bid_price);
                parser.get<int>("bs", quote->bid_size);
                queue.publish(index);
            }

            auto &global = zorro::Global::get();
            if (global.handle_ && global.price_type_ != 2)
            {
                PostMessage(global.handle_, WM_APP + 1, 0, 0);
            }
        }
    };

}
