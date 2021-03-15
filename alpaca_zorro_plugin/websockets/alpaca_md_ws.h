#pragma once

#include <zorro_websocket_proxy_client.h>
#include <utils.h>
#include <unordered_set>
#include <sstream>
#include "logger.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <atomic>

namespace alpaca {

    class AlpacaMdWs : public zorro::websocket::ZorroWebsocketProxyClient, public zorro::websocket::WebsocketProxyCallback {
        enum Status : uint8_t {
            CONNECTING,
            DISCONNECTED,
            AUTHENTICATING,
            AUTHENTICATED,
            SUBSCRIBING,
            SUBSCRIBED,
            NOT_SUBSCRIBED,
            UNSUBSCRIBING,
            UNSUBSCRIBED,
            LOGOUT,
        };
        Logger* logger_ = nullptr;
        std::atomic_uint_fast32_t id_{ 0 };
        volatile Status status_{ Status::DISCONNECTED };
        std::stringstream ss_;
        std::string api_key_;
        std::string api_secret_;
        std::string url_;
        std::string pending_subscription_;
        uint32_t error_count_ = 0;

        struct Subscription {
            slick::SlickQueue<Trade> tradeQueue{ 8 };
            slick::SlickQueue<Quote> quoteQueue{ 8 };
            uint64_t tradeIndex{ 0 };
            uint64_t quoteIndex{ 0 };
        };

        std::shared_ptr<Subscription> subscribed_;
        std::unordered_map<std::string, std::shared_ptr<Subscription>> subscriptions_populator_;
        std::unordered_map<std::string, std::shared_ptr<Subscription>> subscriptions_reader_;

    public:
        AlpacaMdWs()
            : ZorroWebsocketProxyClient(this, "Alpaca", BrokerError, BrokerProgress)
        {}
        ~AlpacaMdWs() override = default;

        bool login(Logger* logger, std::string key, std::string secret, const std::string& url) noexcept {
            logger_ = logger;
            auto log_func = [this](zorro::websocket::LogLevel level, const std::string& msg) { 
                logger_->log(static_cast<alpaca::LogLevel>(level), "%s\n", msg.c_str()); 
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
            logger_->logInfo("Open Alpaca MD Websocket...\n");
            auto result = openWs(url_);
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

                BrokerError("Websocket proxy reopened.");
                for (auto& kvp : subscriptions_reader_) {
                    subscribe(kvp.first, true);
                }
            }
        }

        void logout() {
            logger_->logInfo("Logout\n");
            unsubscribe();
            closeWs(id_.load(std::memory_order_relaxed));
            status_ = Status::LOGOUT;
        }

        bool subscribe(const std::string& asset, bool force = false) {
            if (status_ < Status::AUTHENTICATED) {
                return false;
            }

            if (!force && subscriptions_reader_.find(asset) != subscriptions_reader_.end()) {
                return true;
            }
            
            rapidjson::StringBuffer s;
            rapidjson::Writer<rapidjson::StringBuffer> writer(s);
            writer.StartObject();
            writer.Key("action");
            writer.String("subscribe");
            writer.Key("trades");
            writer.StartArray();
            writer.String(asset.c_str());
            writer.EndArray();
            writer.Key("quotes");
            writer.StartArray();
            writer.String(asset.c_str());
            writer.EndArray();
            writer.EndObject();
            auto data = s.GetString();

            pending_subscription_ = asset;
            subscribed_.reset();
            status_ = Status::SUBSCRIBING;
            auto id = id_.load(std::memory_order_relaxed);
            logger_->logInfo("Subscribe %s, id=%d\n", asset.c_str(), id);
            send(id, data, s.GetSize());
            waitForWsReadyOrError(Status::SUBSCRIBED);
            auto b = status_ == Status::SUBSCRIBED;
            if (b) {
                subscriptions_reader_.emplace(asset, std::move(subscribed_));
                subscribed_.reset();
            }
            pending_subscription_ = "";
            return b;
        }

        Trade* getLastTrade(const std::string& asset) {
            if (status_ == Status::DISCONNECTED) {
                reopen();
            }

            auto it = subscriptions_reader_.find(asset);
            if (it == subscriptions_reader_.end()) {
                return nullptr;
            }

            auto& sub = it->second;
            auto result = sub->tradeQueue.read(sub->tradeIndex);
            if (result.first) {
                return result.first;
            }
            else if (sub->tradeIndex > 0) {
                return sub->tradeQueue[sub->tradeIndex - 1];
            }
            return nullptr;
        }

        Quote* getLastQuote(const std::string& asset) {
            if (status_ == Status::DISCONNECTED) {
                reopen();
            }

            auto it = subscriptions_reader_.find(asset);
            if (it == subscriptions_reader_.end()) {
                return nullptr;
            }

            auto& sub = it->second;
            auto result = sub->quoteQueue.read(sub->quoteIndex);
            if (result.first) {
                return result.first;
            }
            else if (sub->quoteIndex > 0) {
                return sub->quoteQueue[sub->quoteIndex - 1];
            }
            return nullptr;
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

        void unsubscribe() {
            // Other Zorro instances might still need the instrument
            // can't unsubscribe the instrument blindly
            if (status_ < Status::AUTHENTICATED) {
                return;
            }

            subscriptions_reader_.clear();
            return;

            //TODO: Need to unsubscribe properly

            //rapidjson::StringBuffer s;
            //rapidjson::Writer<rapidjson::StringBuffer> writer(s);
            //writer.StartObject();
            //writer.Key("action");
            //writer.String("unsubscribe");

            //writer.Key("trades");
            //writer.StartArray();
            //for (auto& kvp : subscriptions_reader_) {
            //    writer.String(kvp.first.c_str());
            //}
            //writer.EndArray();
            //writer.Key("quotes");
            //writer.StartArray();
            //for (auto& kvp : subscriptions_reader_) {
            //    writer.String(kvp.first.c_str());
            //}
            //writer.EndArray();
            //writer.EndObject();
            //auto data = s.GetString();

            //status_ = Status::UNSUBSCRIBING;
            //logger_->logInfo("Unsubscribe all Assets\n");
            //send(id_, data, s.GetSize());
            //waitForWsReadyOrError(Status::UNSUBSCRIBED);
            //subscriptions_reader_.clear();
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
            if (logger_) {
                logger_->logInfo("Websocket opened. id=%d\n", id);
            }
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
            if (err && len && logger_) {
                logger_->logError("WS error: %.*s\n", len, err);
            }
            else {
                logger_->logError("WS error\n");
            }
            ++error_count_;
            if (error_count_ > 10) {
                closeWs(id);
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
                                
                                if (logger_) {
                                    logger_->logError("On Ws error %s(%d).\n", msg.c_str(), code);
                                }
                                BrokerError((msg + " (" + std::to_string(code) + ")").c_str());

                                ++error_count_;
                                if (error_count_ > 10) {
                                    closeWs(id);
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
                                    if (logger_) {
                                        logger_->logInfo("Alpaca MarketData Websocket Opened.\n");
                                    }
                                    authenticate();
                                }
                                else if (msg == "authenticated") {
                                    if (logger_) {
                                        logger_->logInfo("Authenticated.\n");
                                    }
                                    status_ = Status::AUTHENTICATED;
                                }
                            }
                            else if (t == "subscription") {
                                if (status_ == Status::SUBSCRIBING) {
                                    auto it = ss_.str().find(pending_subscription_);
                                    if (it != std::string::npos) {
                                        subscribed_ = std::make_shared<Subscription>();
                                        subscriptions_populator_.insert(std::make_pair(pending_subscription_, subscribed_));
                                        MemoryBarrier();
                                        status_ = Status::SUBSCRIBED;
                                    }
                                    else {
                                        // should not come here
                                        status_ = Status::NOT_SUBSCRIBED;
                                    }
                                }
                                else if (status_ == Status::UNSUBSCRIBING &&
                                         ss_.str() == "[{\"T\":\"subscription\",\"trades\":[],\"quotes\":[],\"bars\":[]}]") {
                                    status_ = Status::UNSUBSCRIBED;
                                    if (logger_) {
                                        logger_->logInfo("Unsubscribed.\n");
                                    }
                                }
                            }
                            else if (t == "t") {
                                onTrade(objJson);
                            }
                            else if (t == "q") {
                                onQuote(objJson);
                            }
                            else if (logger_) {
                                logger_->logTrace("%s\n", ss_.str().c_str());
                                logger_->logWarning("Unhandled %s\n", t.GetString());
                            }
                        }
                    }
                }

                if (logger_) {
                    logger_->logTrace("%s\n", ss_.str().c_str());
                }

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
            if (logger_) {
                logger_->logInfo("Authenticating...\n");
            }
            send(id_.load(std::memory_order_relaxed), data, s.GetSize());
        }

        template<typename T>
        void onTrade(T& objJson) {
            std::string symbol;
            Parser<T> parser(objJson);
            parser.get<std::string>("S", symbol);
            auto it = subscriptions_populator_.find(symbol);
            if (it != subscriptions_populator_.end()) {
                auto& queue = it->second->tradeQueue;
                auto index = queue.reserve();
                auto trade = queue[index];
                parser.get<double>("p", trade->price);
                parser.get<int>("s", trade->size);
                queue.publish(index);
            }
        }

        template<typename T>
        void onQuote(T& objJson) {
            std::string symbol;
            Parser<T> parser(objJson);
            parser.get<std::string>("S", symbol);
            auto it = subscriptions_populator_.find(symbol);
            if (it != subscriptions_populator_.end()) {
                auto& queue = it->second->quoteQueue;
                auto index = queue.reserve();
                auto quote = queue[index];
                parser.get<double>("ap", quote->ask_price);
                parser.get<int>("as", quote->ask_size);
                parser.get<double>("bp", quote->bid_price);
                parser.get<int>("bs", quote->bid_size);
                queue.publish(index);
            }
        }
    };

}