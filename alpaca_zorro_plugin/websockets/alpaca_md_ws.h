#pragma once

#include <zorro_websocket_proxy_client.h>
#include <utils.h>
#include <unordered_set>
#include <sstream>
#include "logger.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <atomic>

#ifdef _WIN32
// Remove GetObject definition from windows.h, which prevents calls to
// RapidJSON's GetObject.
// https://github.com/Tencent/rapidjson/issues/1448
#undef GetObject
#endif  // _WIN32

namespace alpaca {

    class AlpacaMdWs : public zorro::websocket::ZorroWebsocketProxyClient, public zorro::websocket::WebsocketProxyCallback {
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
        std::atomic_uint_fast32_t id_{ 0 };
        volatile Status status_{ Status::DISCONNECTED };
        std::stringstream ss_;
        std::string api_key_;
        std::string api_secret_;
        std::string url_;
        std::string pending_subscription_;
        uint32_t error_count_ = 0;

        struct Subscription {
            std::unique_ptr<slick::SlickQueue<Trade>> tradeQueue = std::make_unique<slick::SlickQueue<Trade>>(256);
            std::unique_ptr<slick::SlickQueue<Quote>> quoteQueue = std::make_unique<slick::SlickQueue<Quote>>(256);
            bool quoteSubscribed{ false };
            bool tradeSubscribed{ false };
        };

        std::unordered_map<std::string, Subscription> subscriptions_;

    public:
        AlpacaMdWs()
            : ZorroWebsocketProxyClient(this, "Alpaca", BrokerError, BrokerProgress)
        {}
        ~AlpacaMdWs() override = default;

        bool login(std::string key, std::string secret, const std::string& url) noexcept {
            auto log_func = [this](zorro::websocket::LogLevel level, const std::string& msg) { 
                Logger::instance().log(static_cast<alpaca::LogLevel>(level), LogType::LT_WEB_SOCKET_DATA, "%s\n", msg.c_str());
            };
            // pass logger to WebsocketProxyClient
            setLogger(log_func);
            api_key_ = std::move(key);
            api_secret_ = std::move(secret);
            url_ = url;
            return open();
        }

        bool open() {
            status_ = Status::CONNECTING;
            LOG_INFO("Open Alpaca MD Websocket...\n");
            auto result = openWebSocket(url_);
            if (result.first) {
                if (!result.second) {
                    // WS already openeda
                    id_.store(result.first, std::memory_order_release);
                    status_ = Status::AUTHENTICATED;
                    return true;
                }
                // New connection opened, need to authenticate
                waitForWsReadyOrError(Status::AUTHENTICATED, 30000);
                return status_ == Status::AUTHENTICATED;
            }
            return false;
        }

        void reopen() {
            if (status_ != Status::LOGOUT) {
                if (!open()) {
                    return;
                }

                BrokerError("Websocket reopened.");
                for (auto& kvp : subscriptions_) {
                    subscribeAsset(kvp.first, true);
                }
            }
        }

        void logout() {
            LOG_INFO("Logout\n");
            unsubscribeAll();
            closeWebSocket(id_.load(std::memory_order_relaxed));
            status_ = Status::LOGOUT;
        }

        bool authenticated() const noexcept {
            return status_ >= Status::AUTHENTICATED;
        }

        bool isSubscribed(const std::string& asset) const noexcept {
            return subscriptions_.find(asset) != subscriptions_.end();
        }

        bool subscribeAsset(const std::string& asset, bool force = false) {
            if (!authenticated()) {
                return false;
            }

            auto iter = subscriptions_.find(asset);
            if (iter == subscriptions_.end()) {
                iter = subscriptions_.emplace(asset, Subscription()).first;
            }
            else if (!force && ((Config::get().priceType == 2 && iter->second.tradeSubscribed) || iter->second.quoteSubscribed)) {
                return true;
            }

            auto& subscription = iter->second;

            rapidjson::StringBuffer s;
            rapidjson::Writer<rapidjson::StringBuffer> writer(s);
            writer.StartObject();
            writer.Key("action");
            writer.String("subscribe");
            if (Config::get().priceType == 2)
            {
                writer.Key("trades");
                writer.StartArray();
                writer.String(asset.c_str());
                writer.EndArray();
                subscription.tradeSubscribed = true;
            }
            writer.Key("quotes");
            writer.StartArray();
            writer.String(asset.c_str());
            writer.EndArray();
            writer.EndObject();
            subscription.quoteSubscribed = true;

            auto data = s.GetString();

            pending_subscription_ = asset;
            status_ = Status::SUBSCRIBING;
            auto id = id_.load(std::memory_order_relaxed);
            LOG_INFO("Subscribe %s, id=%d\n", asset.c_str(), id);

            bool existing = false;
            if (!subscribe(id, asset, data, s.GetSize(), existing)) {
                subscriptions_.erase(iter);
                return false;
            }
            if (existing) {
                status_ = Status::SUBSCRIBED;
                pending_subscription_ = "";
                return true;
            }
            waitForWsReadyOrError(Status::SUBSCRIBED);
            pending_subscription_ = "";
            return status_ == Status::SUBSCRIBED;
        }

        Trade* getLastTrade(const std::string& asset) {
            if (status_ == Status::DISCONNECTED) {
                reopen();
                return nullptr;
            }

            auto it = subscriptions_.find(asset);
            if (it == subscriptions_.end() || !it->second.tradeSubscribed) {
                return nullptr;
            }

            return it->second.tradeQueue->read_last();
        }

        Quote* getLastQuote(const std::string& asset) {
            if (status_ == Status::DISCONNECTED) {
                reopen();
                return nullptr;
            }

            auto it = subscriptions_.find(asset);
            if (it == subscriptions_.end() || !it->second.quoteSubscribed) {
                return nullptr;
            }

            return it->second.quoteQueue->read_last();
        }

    private:

        void waitForWsReadyOrError(Status expected_status, uint32_t timeout = 10000) {
            auto start = zorro::websocket::get_timestamp();
            while ((zorro::websocket::get_timestamp() - start) < timeout) {
                if (status_ == expected_status || status_ == Status::DISCONNECTED) {
                    break;
                }
                BrokerProgress(1);
                std::this_thread::yield();
            }
        }

        void unsubscribeAll() {
            if (status_ < Status::AUTHENTICATED) {
                return;
            }

            LOG_INFO("Unsubscribe all Assets\n");
            status_ = Status::UNSUBSCRIBING;
            for (auto& kvp : subscriptions_) {
                rapidjson::StringBuffer s;
                rapidjson::Writer<rapidjson::StringBuffer> writer(s);
                writer.StartObject();
                writer.Key("action");
                writer.String("unsubscribe");

                writer.Key("trades");
                writer.StartArray();
                writer.String(kvp.first.c_str());
                writer.EndArray();
                writer.Key("quotes");
                writer.StartArray();
                writer.String(kvp.first.c_str());
                writer.EndArray();
                writer.EndObject();
                auto data = s.GetString();

                unsubscribe(id_.load(std::memory_order_relaxed), kvp.first, data, s.GetSize());
            }
            status_ = Status::UNSUBSCRIBED;
            subscriptions_.clear();
        }


        // Following functions are called from websocket proxy client thread

        void onWebsocketProxyServerDisconnected() override{
            if (status_ != Status::LOGOUT && status_ != Status::CONNECTING) {
                id_.store(0, std::memory_order_release);
                ss_.str() = "";
                status_ = Status::DISCONNECTED;
            }
        }

        void onWebsocketOpened(uint32_t id) override {
            LOG_INFO("Websocket opened. id=%d\n", id);
            id_.store(id, std::memory_order_release);
            ss_.str("");
            BrokerProgress(1);
        }

        void onWebsocketClosed(uint32_t id) override {
            if (status_ != Status::LOGOUT) {
                id_.store(0, std::memory_order_release);
                ss_.str() = "";
                status_ = Status::DISCONNECTED;
                BrokerError("Websocket disconnected");
            }
        }

        void onWebsocketError(uint32_t id, const char* err, size_t len) override {
            if (err && len) {
                LOG_ERROR("WS error: %.*s\n", len, err);
            }
            else {
                LOG_ERROR("WS error\n");
            }
            ++error_count_;
            if (error_count_ > 10) {
                closeWebSocket(id);
                status_ = Status::DISCONNECTED;
            }
        }

        void onWebsocketData(uint32_t id, const char* data, size_t len, size_t remaining) override {
            error_count_ = 0;

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
                                Parser<decltype(item.GetObject())> parser(objJson);
                                std::string msg;
                                parser.get<std::string>("msg", msg);
                                int32_t code;
                                parser.get<int32_t>("code", code);
                                
                                LOG_ERROR("On Ws error %s(%d).\n", msg.c_str(), code);
                                BrokerError((msg + " (" + std::to_string(code) + ")").c_str());

                                ++error_count_;
                                if (error_count_ > 10) {
                                    closeWebSocket(id);
                                    MemoryBarrier();
                                    status_ = Status::DISCONNECTED;
                                }
                                break;
                            }

                            if (t == "success") {
                                Parser<decltype(item.GetObject())> parser(objJson);
                                std::string msg;
                                parser.get<std::string>("msg", msg);
                                if (msg == "connected") {
                                    LOG_INFO("Alpaca MarketData Websocket Opened.\n");
                                    authenticate();
                                }
                                else if (msg == "authenticated") {
                                    LOG_INFO("Authenticated.\n");
                                    status_ = Status::AUTHENTICATED;
                                }
                            }
                            else if (t == "subscription") {
                                if (status_ == Status::SUBSCRIBING) {
                                    auto it = ss_.str().find(pending_subscription_);
                                    if (it != std::string::npos) {
                                        status_ = Status::SUBSCRIBED;
                                    }
                                    else {
                                        // should not come here
                                        status_ = Status::NOT_SUBSCRIBED;
                                    }
                                }
                                else /*if (status_ == Status::UNSUBSCRIBING &&
                                         ss_.str() == "[{\"T\":\"subscription\",\"trades\":[],\"quotes\":[],\"bars\":[]}]")*/ {
                                    //status_ = Status::UNSUBSCRIBED;
                                        //LOG_INFO("Unsubscribed.\n");
                                    LOG_TRACE_EXT(LogType::LT_WEB_SOCKET_DATA, "%s\n", ss_.str().c_str());
                                }
                            }
                            else if (t == "t") {
                                onTrade(objJson);
                            }
                            else if (t == "q") {
                                onQuote(objJson);
                            }
                            else {
                                LOG_DEBUG_EXT(LogType::LT_WEB_SOCKET_DATA, "%s\n", ss_.str().c_str());
                                LOG_WARNING_EXT(LogType::LT_WEB_SOCKET_DATA, "Unhandled %s\n", t.GetString());
                            }
                        }
                    }
                }

                LOG_TRACE_EXT(LogType::LT_WEB_SOCKET_DATA, "%s\n", ss_.str().c_str());

                // reset message
                ss_.str("");
            }
        }

        void authenticate() {
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
            LOG_INFO_EXT(LogType::LT_WEB_SOCKET_DATA, "Authenticating...\n");
            send(id_.load(std::memory_order_relaxed), data, s.GetSize());
        }

        template<typename T>
        void onTrade(T& objJson) {
            std::string symbol;
            Parser<T> parser(objJson);
            parser.get<std::string>("S", symbol);
            auto it = subscriptions_.find(symbol);
            if (it != subscriptions_.end()) {
                auto& queue = it->second.tradeQueue;
                auto index = queue->reserve();
                auto trade = (*queue)[index];
                parser.get<double>("p", trade->price);
                parser.get<int>("s", trade->size);
                queue->publish(index);
            }
        }

        template<typename T>
        void onQuote(T& objJson) {
            std::string symbol;
            Parser<T> parser(objJson);
            parser.get<std::string>("S", symbol);
            auto it = subscriptions_.find(symbol);
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
        }
    };

}
