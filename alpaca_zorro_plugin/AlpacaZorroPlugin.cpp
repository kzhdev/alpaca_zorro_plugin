// AlpacaZorroPlugin.cpp : Defines the exported functions for the DLL application.
//
// Alpaca plugin for Zorro Automated Trading System
// Written by Kun Zhao
// 
// Alpaca - a modern platform for algorithmic trading. (https://alpaca.markets/)
//

#include "stdafx.h"

#include "AlpacaZorroPlugin.h"

// standard library
#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include <fstream>
#include <type_traits>
#include <unordered_set>
#include <array>

#include "alpaca/client.h"
#include "alpaca/asset.h"
#include "logger.h"
#include "market_data/stock_market_data.h"
#include "market_data/option_market_data.h"
#include "market_data/crypto_market_data.h"
#include "config.h"
#include "websockets/alpaca_md_ws.h"
#include "AlpacaBrokerCommands.h"
#include "global.h"
#include <resource.h>
#include "SettingsDialog.h"
#include <zorro/include/trading.h>

#define PLUGIN_VERSION	2

using namespace alpaca;
using namespace websocket_proxy;

#define ALPACA_STREAM_URL "wss://api.alpaca.markets/stream"

HMODULE g_hModule = nullptr;

namespace {
    TimeInForce s_tif = TimeInForce::FOK;
    // std::unordered_map<uint32_t, Order> s_mapOrderByClientOrderId;
    std::unordered_map<std::string, Order> s_mapOrderByUUID;
    Config &s_config = Config::get();
    uint32_t alpaca_md_ws_id = 0;
    std::unordered_set<AssetBase*> s_activeAssets;
    std::array<std::string, AssetClass::__count__> s_subscribedAssets;
    uint64_t s_lastRequestTime = 0;
    std::array<Response<LastQuotes>, AssetClass::__count__> s_lastQuotes{ Response<LastQuotes>{1, "No Data"}, Response<LastQuotes>{1, "No Data"}, Response<LastQuotes>{1, "No Data"} };
    std::array<Response<LastTrades>, AssetClass::__count__> s_lastTrades{ Response<LastTrades>{1, "No Data"}, Response<LastTrades>{1, "No Data"}, Response<LastTrades>{1, "No Data"} };
    constexpr uint64_t REQUEST_BANNING_TIME = 333;  // 333 ms;
    constexpr double EPSILON = 0.000000001;
    bool isZero(double val) noexcept {
        return val > 0 ? val < EPSILON : val > -EPSILON;
    }
    std::string s_lastOrderUUID;
    std::string s_nextOrderUUID;

    auto &global = zorro::Global::get();

    void handleSettingUpdate()
    {
        if (global.logged_in_.load(std::memory_order_relaxed))
        {
            auto setting_update = global.setting_update_.exchange(nullptr, std::memory_order_acq_rel);
            if (setting_update)
            {
                auto log_level = toLogLevel(setting_update->log_level_);
                auto current_level = spdlog::get_level();
                if (log_level != current_level)
                {
                    s_config.logLevel = setting_update->log_level_;
                    SPDLOG_INFO("Log level changed to {}", to_string(s_config.logLevel));
                    spdlog::set_level(log_level);
                    if (log_level > SPDLOG_LEVEL_INFO)
                    {
                        spdlog::flush_on(log_level);
                    }
                }
            }
        }
    }

    DLLFUNC_C void pluginCallback(void*)
    {
        handleSettingUpdate();
    }
}

namespace alpaca
{
    std::unique_ptr<Client> client = nullptr;
    std::array<std::unique_ptr<MarketData>, AssetClass::__count__> pMarketData;
    std::unique_ptr<OptionMarketData> pOptionMarketData = nullptr;
    std::unique_ptr<AlpacaMdWs> wsClient;
    std::unique_ptr<Throttler> s_throttler;
    std::string account_number;

    void shutdown()
    {
        if (wsClient) {
            wsClient->logoutAll();
            wsClient.reset();
        }
        alpaca_md_ws_id = 0;
        client.reset();
        spdlog::shutdown();
    }

    inline AssetBase* getAsset(const std::string &symbol)
    {
        auto& assets = client->allAssets();
        auto iter = assets.find(symbol);
        if (iter != assets.end())
        {
            return iter->second;
        }
        return nullptr;
    }
    
    ////////////////////////////////////////////////////////////////
    DLLFUNC_C int BrokerOpen(char* Name, FARPROC fpError, FARPROC fpProgress)
    {
        strcpy_s(Name, 32, "Alpaca");
        (FARPROC&)BrokerError = fpError;
        (FARPROC&)BrokerProgress = fpProgress;
        return PLUGIN_VERSION;
    }

    DLLFUNC_C void BrokerHTTP(FARPROC fpSend, FARPROC fpStatus, FARPROC fpResult, FARPROC fpFree)
    {
        (FARPROC&)http_send = fpSend;
        (FARPROC&)http_status = fpStatus;
        (FARPROC&)http_result = fpResult;
        (FARPROC&)http_free = fpFree;
    }

    DLLFUNC_C int BrokerLogin(char* User, char* Pwd, char* Type, char* Account)
    {
        if (!User) // log out
        {
            global.logged_in_.store(false, std::memory_order_release);
            shutdown();
            return 0;
        }

        try
        {
            spdlog::init_thread_pool(8192, 1);
            auto async_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(std::format("./Log/Alpaca_{}_{}.log", report(25), Type), 524288000, 5);
            auto async_logger = std::make_shared<spdlog::async_logger>("async_logger", async_sink, spdlog::thread_pool(), spdlog::async_overflow_policy::overrun_oldest);
            spdlog::set_default_logger(async_logger);
// #ifdef DEBUG
//             spdlog::set_level(spdlog::level::trace);
//             spdlog::flush_on(spdlog::level::trace);
// #else
            auto log_level = toLogLevel(s_config.logLevel);
            spdlog::set_level(log_level);
            if (log_level > SPDLOG_LEVEL_INFO)
            {
                spdlog::flush_on(log_level);
            }
// #endif
            spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%F][%t][%l] %v");
            spdlog::flush_every(std::chrono::seconds(3));
        }
        catch(const std::exception& e)
        {
            BrokerError(e.what());
            shutdown();
            return 0;
        }

        global.reset();
        s_tif = TimeInForce::FOK;
        s_subscribedAssets = { "", "", "" };
        s_config.adjustment = Adjustment::all;
        s_activeAssets.clear();        

        bool isPaperTrading = strcmp(Type, "Demo") == 0;

        if (!client || client->getApiKey() != User) {
            s_throttler = std::make_unique<Throttler>(User, !isPaperTrading);
            client = std::make_unique<Client>(User, Pwd, isPaperTrading);

            pMarketData[AssetClass::US_EQUITY] = std::make_unique<StockMarketData>(client->headers());
            pMarketData[AssetClass::CRYPTO] = std::make_unique<CryptoMarketData>(client->headers());
            pMarketData[AssetClass::OPTIONS] = std::make_unique<OptionMarketData>(client->headers());

            if (s_config.alpacaPaidPlan) {
                BrokerError("Use Alpaca Pro Market Data");
                SPDLOG_INFO("Use Alpaca Pro Market Data");
            }
            else {
                BrokerError("Use Alpaca Basic Market Data");
                SPDLOG_INFO("Use Alpaca Basic Market Data");
            }
        }

        if (isPaperTrading)
        {
            BrokerError("Paper mode");
            SPDLOG_INFO("Paper mode");
        }
        else
        {
            BrokerError("Live mode");
            SPDLOG_INFO("Live mode");
        }

        if (s_config.useWebsocket) {
            try
            {
                wsClient = std::make_unique<AlpacaMdWs>();
                wsClient->init(User, Pwd);
            }
            catch (std::runtime_error& err)
            {
                BrokerError(err.what());
                SPDLOG_ERROR(err.what());
            }
            catch (...)
            {
            }
        }

        auto response = client->getAccount();
        if (!response) {
            BrokerError("Login failed.");
            BrokerError(response.what().c_str());
            return 0;
        }

        global.logged_in_.store(true, std::memory_order_release);
        std::thread([]() {
            SettingsThreadProc();
        }).detach();

        if (s_config.fractionalLotAmount > 1) {
            s_config.fractionalLotAmount = 1;
            BrokerError(("Fractional qty disabled. Invalid config: AlpacaFranctionalLotAmount must be less than 1. AlpacaFranctionalLotAmount=" + std::to_string(s_config.fractionalLotAmount)).c_str());
        }

        account_number = response.content().account_number;
        BrokerError(("Account " + account_number).c_str());
        sprintf_s(Account, 1024, account_number.c_str());
        return 1;
    }

    DATE convertTime(__time32_t t32)
    {
        return (DATE)t32 / 86400. + 25569.; // 25569. = DATE(1.1.1970 00:00)
    }

    __time32_t convertTime(DATE date)
    {
        return (__time32_t)((date - 25569.) * 86400.);
    }

    DLLFUNC_C int BrokerTime(DATE* pTimeGMT)
    {
        handleSettingUpdate();
        __time32_t now = get_timestamp();
        auto next_clock_request_time = s_throttler->nextClockRequestTime();
        if (next_clock_request_time && now < next_clock_request_time)
        {
            *pTimeGMT = convertTime(now);
            if (!s_subscribedAssets[AssetClass::CRYPTO].empty())
            {
                // crypto trading open 24 hours
                return 2;
            }
            return client->isOpen() ? 2 : 1;
        }

        auto response = client->getClock();
        s_throttler->setNextClockRequestTime(now + 1000); // set next clock request time to 1 second later

        if (!response) {
#ifdef _DEBUG
            BrokerError(response.what().c_str());
#endif
            return 0;
        }

        auto& clock = response.content();
        *pTimeGMT = convertTime(clock.timestamp);
        if (!s_subscribedAssets[AssetClass::CRYPTO].empty())
        {
            // crypto trading open 24 hours
            return 2;
        }
        return clock.is_open ? 2 : 1;
    }

    DLLFUNC_C int BrokerAsset(char* Asset, double* pPrice, double* pSpread, double* pVolume, double* pPip, double* pPipCost, double* pLotAmount, double* pMarginCost, double* pRollLong, double* pRollShort)
    {
        handleSettingUpdate();
        AssetBase* asset = nullptr;

        {
            auto &assets = client->allAssets();
            auto iter = assets.find(Asset);
            if (iter == assets.end())
            {
                return 0;
            }
            asset = iter->second;
        }
        auto asset_class = asset->asset_class;

        auto iter = s_activeAssets.find(asset);
        if (iter == s_activeAssets.end())
        {
            BrokerError(("Subscribe " + std::string(Asset)).c_str());
            s_activeAssets.emplace(asset);
            if (s_subscribedAssets[asset_class].empty())
            {
                s_subscribedAssets[asset_class].append(Asset);
            }
            else
            {
                s_subscribedAssets[asset_class].append(",").append(Asset);
            }

            if (wsClient && !wsClient->hasSubscription(asset))
            {
                if (!wsClient->authenticated(asset_class))
                {
                    if (!wsClient->open(asset_class)) {
                        BrokerError(("Unable to open Alpaca websocket " + wsClient->url(asset_class) + ". Prices will be pulled from REST API.").c_str());
                    }
                }

                if (wsClient->authenticated(asset_class))
                {
                    if (wsClient->subscribeAsset(asset))
                    {
                        SPDLOG_DEBUG("{} subscribed", Asset);

                        // Query Last Quote/Trade once, in case the symbol is iliquid we don't get any update from WebSocket
                        if (global.price_type_ == 2)
                        {
                            auto last_trade = pMarketData[asset_class]->getLastTrade(Asset);
                            if (last_trade)
                            {
                                wsClient->setLastTrade(asset, last_trade.content());
                            }
                        }
                        else
                        {
                            auto last_quote = pMarketData[asset_class]->getLastQuote(Asset);
                            if (last_quote)
                            {
                                wsClient->setLastQuote(asset, last_quote.content());
                            }
                        }
                    }
                    else 
                    {
                        SPDLOG_DEBUG("Failed to subscribed {}", Asset);
                        BrokerError(("Failed to subscribe " + std::string(Asset) + " from Websocket. Price will be polled from REST API.").c_str());
                        wsClient.reset();
                    }
                }
            }
        }

        if (!pPrice)
        {
            return 1;
        }

        if (global.price_type_ == 2)
        {
            Trade* trade = nullptr;
            if (wsClient && wsClient->authenticated(asset_class))
            {
                trade = wsClient->getLastTrade(asset);
            }
            if (trade)
            {
                *pPrice = trade->price;

                auto quote = wsClient->getLastQuote(asset);
                if (quote && !isZero(quote->ask_price) && pSpread)
                {
                    *pSpread = quote->ask_price - quote->bid_price;
                }
            }
            else 
            {
                auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                if (!s_lastTrades[asset_class] || s_config.alpacaPaidPlan || (now - s_lastRequestTime) > REQUEST_BANNING_TIME)
                {
                    s_lastTrades[asset_class] = pMarketData[asset->asset_class]->getLastTrades(s_subscribedAssets[asset_class]);
                    if (!s_lastTrades[asset_class]) {
                        BrokerError(("Failed to get lastTrades for " + std::string(s_subscribedAssets[asset_class]) +
                            " error: " + s_lastTrades[asset_class].what()).c_str());
                        return 0;
                    }
                    s_lastRequestTime = now;
                }
                
                if (s_lastTrades[asset_class])
                {
                    auto& lastTrades = s_lastTrades[asset_class].content().trades;
                    auto it = lastTrades.find(Asset);
                    if (it != lastTrades.end())
                    {
                        *pPrice = it->second.price;
                    }
                    else
                    {
                        assert(false);
                        auto response = pMarketData[asset->asset_class]->getLastTrade(Asset);
                        if (!response)
                        {
                            BrokerError(("Failed to get lastTrade " + std::string(Asset) +
                                " error: " + response.what()).c_str());
                            return 0;
                        }

                        auto& lastTrade = response.content();
                        *pPrice = lastTrade.trade.price;
                    }
                }
            }
        }
        else
        {
            Quote* quote = nullptr;
            if (wsClient && wsClient->authenticated(asset_class))
            {
                quote = wsClient->getLastQuote(asset);
            }
            if (quote)
            {
                if (!isZero(quote->ask_price))
                {
                    *pPrice = quote->ask_price;

                    if (pSpread)
                    {
                        *pSpread = quote->ask_price - quote->bid_price;
                    }
                }
            }
            else {
                auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                if (!s_lastQuotes[asset_class] || s_config.alpacaPaidPlan || (now - s_lastRequestTime) > REQUEST_BANNING_TIME)
                {
                    s_lastQuotes[asset_class] = pMarketData[asset_class]->getLastQuotes(s_subscribedAssets[asset_class]);
                    if (!s_lastQuotes[asset_class])
                    {
                        BrokerError(("Failed to get lastQuotes for " + std::string(s_subscribedAssets[asset_class]) +
                            " error: " + s_lastQuotes[asset_class].what()).c_str());
                        return 0;
                    }
                    s_lastRequestTime = now;
                }

                if (s_lastQuotes[asset_class])
                {
                    auto& lastQuotes = s_lastQuotes[asset_class].content().quotes;
                    auto it = lastQuotes.find(Asset);
                    if (it != lastQuotes.end())
                    {
                        if (!isZero(it->second.ask_price))
                        {
                            *pPrice = it->second.ask_price;

                            if (pSpread) 
                            {
                                *pSpread = it->second.ask_price - it->second.bid_price;
                            }
                        }
                    }
                    else 
                    {
                        auto response = pMarketData[asset_class]->getLastQuote(Asset);
                        if (!response)
                        {
                            BrokerError(("Failed to get lastQuote " + std::string(Asset) +
                                " error: " + response.what()).c_str());
                            return 0;
                        }

                        auto& lastQuote = response.content();
                        if (!isZero(lastQuote.quote.ask_price))
                        {
                            *pPrice = lastQuote.quote.ask_price;

                            if (pSpread) {
                                *pSpread = lastQuote.quote.ask_price - lastQuote.quote.bid_price;
                            }
                        }
                    }
                }
            }
        }

        if (pLotAmount)
        {
            if (asset_class == AssetClass::US_EQUITY)
            {
                auto* equity = reinterpret_cast<alpaca::Asset*>(asset);
                if (equity->fractionable)
                {
                    *pLotAmount = s_config.fractionalLotAmount;
                }
                else
                {
                    *pLotAmount = 1;
                }
            }
            else if (asset_class == AssetClass::CRYPTO)
            {
                auto* crypto = reinterpret_cast<alpaca::Asset*>(asset);
                *pLotAmount = crypto->min_order_size;
            }
        }

        if (pRollLong)
        {
            *pRollLong = 0.;
        }

        if (pRollShort)
        {
            *pRollShort = 0.;
        }

        return 1;
    }

    DLLFUNC_C int BrokerHistory2(char* Asset, DATE tStart, DATE tEnd, int nTickMinutes, int nTicks, T6* ticks)
    {
        handleSettingUpdate();
        if (!client || !Asset || !ticks || !nTicks) return 0;

        auto& assets = client->allAssets();
        auto iter = assets.find(Asset);
        if (iter == assets.end())
        {
            return 0;
        }

        auto asset_class = iter->second->asset_class;

        if (!nTickMinutes) {
            BrokerError("Tick data download is not supported by Alpaca.");
            return 0;
        }

        auto start = convertTime(tStart);
        auto end = convertTime(tEnd);

        int barsDownloaded = 0;

        auto populateTicks = [&barsDownloaded, nTickMinutes, &end, &ticks, nTicks](std::vector<Bar>& bars) {
            for (auto i = 0u; i < bars.size() && barsDownloaded < nTicks; ++i) {
                auto& bar = bars[i];
                auto& tick = ticks[barsDownloaded++];
                tick.time = convertTime(static_cast<__time32_t>(bar.time));
                tick.fOpen = static_cast<float>(bar.open_price);
                tick.fHigh = static_cast<float>(bar.high_price);
                tick.fLow = static_cast<float>(bar.low_price);
                tick.fClose = static_cast<float>(bar.close_price);
                tick.fVol = static_cast<float>(bar.volume);
            }
        };

        if (!s_config.alpacaPaidPlan) {
            SPDLOG_DEBUG("BrokerHistory {} start: {} end: {} nTickMinutes: {} nTicks: {}", Asset, start, end, nTickMinutes, nTicks);
            auto now = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            if ((now - end) <= 900) {
                end = static_cast<__time32_t>(now) - 930;
                SPDLOG_INFO("Alpaca Basic Plan. Adjust end to {}", end);
            }
        }

        while(true) {
            SPDLOG_DEBUG("BrokerHistory {} start: {}({}) end: {}({}) nTickMinutes: {} nTicks: {}", Asset, timeToString(start).c_str(), start, timeToString(end).c_str(), end, nTickMinutes, nTicks);

            auto response = pMarketData[asset_class]->getBars(Asset, start, end, nTickMinutes, nTicks, global.price_type_);
            if (!response) {
                if (response.getCode() == 40010001 && response.what() == "end is too late for subscription") {
                    // Alpaca V2 Basic Plan has 15 min delay. Retry to find the last data allowed.
                    end = (end - end % 60) - 60;
                    continue;
                }
                else {
                    BrokerError(response.what().c_str());
                    return barsDownloaded;
                }
            }

            auto& bars = response.content();
            if (bars.empty()) {
                return barsDownloaded;
            }

            SPDLOG_TRACE("{} bar downloaded. {}-{}", bars.size(), (bars.size() > 0 ? bars[0].time_string.c_str() : ""), (bars.size() > 0 ? (*bars.rbegin()).time_string.c_str() : ""));

            populateTicks(bars);
            SPDLOG_TRACE("{} bar returned", barsDownloaded);
            break;
        }
        return barsDownloaded;
    }

    DLLFUNC_C int BrokerAccount(char* Account, double* pdBalance, double* pdTradeVal, double* pdMarginVal)
    {
        handleSettingUpdate();
        static thread_local Response<Balance> response;

        __time32_t now = get_timestamp();
        auto next_balance_request_time = s_throttler->nextBalanceRequestTime();
        if (!next_balance_request_time || now >= next_balance_request_time)
        {
            client->getBalance(response, now);
            s_throttler->setNextBalanceRequestTime(now + 1000); // set next balance request time to 1 second later
        }

        if (!response) {
            return 0;
        }

        if (pdBalance) {
            *pdBalance = response.content().cash;
        }

        if (pdTradeVal) {
            *pdTradeVal = response.content().equity - response.content().cash;
        }
        return 1;
    }

    DLLFUNC_C int BrokerBuy2(char* Asset, int nAmount, double dStopDist, double dLimit, double* pPrice, int* pFill)
    {
        handleSettingUpdate();
        auto start = std::time(nullptr);

        OrderSide side = nAmount > 0 ? OrderSide::Buy : OrderSide::Sell;
        OrderType type = OrderType::Market;
        double qty = std::abs(nAmount);
        if (dLimit) {
            type = OrderType::Limit;
        }
        else {
            dLimit = NAN;
        }
        qty *= global.amount_;

        if (dStopDist) {

        }

        SPDLOG_DEBUG("BrokerBuy2 {} orderText={} nAmount={} qty={} dStopDist={} limit={}", Asset, global.order_text_, nAmount, qty, dStopDist, dLimit);

        auto asset = client->allAssets().at(Asset);
        auto response = client->submitOrder(asset, qty, side, type, s_tif, dLimit, NAN, global.order_text_, asset->asset_class == AssetClass::US_EQUITY ? s_config.fractionalLotAmount : global.amount_);
        if (!response) {
            BrokerError(response.what().c_str());
            // reset amount, next asset might have lotAmount = 1, in that case SET_AMOUNT will not be called in advance
            global.amount_ = 1.;
            return 0;
        }

        auto* order = &response.content();
        auto exchOrdId = order->id;
        // auto internalOrdId = order->internal_id;
        // s_mapOrderByClientOrderId.emplace(internalOrdId, *order);
        s_mapOrderByUUID.emplace(exchOrdId, *order);
        s_lastOrderUUID = exchOrdId;

        if (order->filled_qty) {
            if (pPrice) {
                *pPrice = response.content().filled_avg_price;
            }
            if (pFill) {
                *pFill = static_cast<int>(alpaca::fix_floating_error(response.content().filled_qty / global.amount_));
            }
            // reset amount, next asset might have lotAmount = 1, in that case SET_AMOUNT will not be called in advance
            global.amount_ = 1.;
            // return internalOrdId;
            return -1;
        }
        
        if (pFill)
        {
            *pFill = 0;
        }

        if ((type == OrderType::Limit || s_tif == TimeInForce::CLS || s_tif == TimeInForce::OPG) && order->status == "new") {
            // return internalOrdId;
            return -1;
        }

        // Limit order has not accepted by Exchange yet
        // or market order hasn't been filled yet
        // wait until the order is in proper state
        do {
            auto response2 = client->getOrder(exchOrdId, false);
            if (!response2) {
                break;
            }
            order = &response2.content();
            // s_mapOrderByClientOrderId[internalOrdId] = *order;
            s_mapOrderByUUID[exchOrdId] = *order;

            SPDLOG_DEBUG("Order status: {}", order->status.c_str());

            if ((type == OrderType::Limit || s_tif == TimeInForce::CLS || s_tif == TimeInForce::OPG) && order->status == "new") {
                break;
            }

            if (order->filled_qty) {
                if (pPrice) {
                    *pPrice = order->filled_avg_price;
                }
                if (pFill) {
                    *pFill = (int)alpaca::fix_floating_error(order->filled_qty / global.amount_);
                }
            }

            if (order->status == "canceled" ||
                order->status == "filled" ||
                order->status == "expired")
            {
                break;
            }

            auto timePast = std::difftime(std::time(nullptr), start);
            if (timePast >= 30) {
                SPDLOG_INFO("Order submit timed out. Canceling order {}", exchOrdId.c_str());
                auto response3 = client->cancelOrder(exchOrdId);
                if (!response3) {
                    BrokerError(("Failed to cancel order " + exchOrdId + " " + response3.what()).c_str());
                }
                return (s_tif == TimeInForce::IOC || s_tif == TimeInForce::FOK) ? 0 : -2;
            }
        } while (!order->filled_qty);
        // reset amount, next asset might have lotAmount = 1, in that case SET_AMOUNT will not be called in advance
        global.amount_ = 1.;
        // return internalOrdId;
        return -1;
    }

    DLLFUNC_C int BrokerTrade(int nTradeID, double* pOpen, double* pClose, double* pCost, double *pProfit) {
        handleSettingUpdate();
        if (nTradeID != -1) {
            BrokerError(("nTradeID " + std::to_string(nTradeID) + " not valid. Need to be an UUID").c_str());
            return NAY;
        }
        
        Response<Order> response;
        Order* order = nullptr;
        // auto iter = s_mapOrderByClientOrderId.find(nTradeID);
        // if (iter == s_mapOrderByClientOrderId.end()) {
        //    // unknown order?
        //    std::stringstream clientOrderId;
        //    clientOrderId << "ZORRO_";
        //    if (!global.order_text_.empty()) {
        //        clientOrderId << global.order_text_ << "_";
        //    }
        //    clientOrderId << nTradeID;
        //    response = client->getOrderByClientOrderId(clientOrderId.str());
        //    if (!response) {
        //        BrokerError(response.what().c_str());
        //        return NAY;
        //    }
        //    order = &response.content();
        //    s_mapOrderByClientOrderId.insert(std::make_pair(nTradeID, response.content()));
        // }
        // else {
        //    order = &iter->second;
        //    if (order->status != "filled" && order->status != "canceled" && order->status != "expired") {
        //        response = client->getOrder(iter->second.id);
        //        if (!response) {
        //            BrokerError(response.what().c_str());
        //            return NAY;
        //        }
        //        order = &response.content();
        //        s_mapOrderByClientOrderId[nTradeID] = *order;
        //    }
        // }
        
        if (s_nextOrderUUID.empty())
        {
            BrokerError("BrokerTrade: Order UUID not specifid");
            return NAY;
        }
        auto iter = s_mapOrderByUUID.find(s_nextOrderUUID);
        if (iter == s_mapOrderByUUID.end()) {
            // unknown order?
            response = client->getOrder(s_nextOrderUUID, false);
            if (!response) {
                BrokerError(response.what().c_str());
                return NAY;
            }
            order = &response.content();
            iter = s_mapOrderByUUID.emplace(s_nextOrderUUID, response.content()).first;
        }

        order = &iter->second;
        if (order->status == "canceled" || order->status == "expired")
        {
            return NAY - 1;
        }

        if (order->status != "filled" || order->filled_qty != order->qty) {
            response = client->getOrder(iter->second.id);
            if (!response) {
                BrokerError(response.what().c_str());
                return NAY;
            }
            order = &response.content();
            s_mapOrderByUUID[s_nextOrderUUID] = *order;
        }

        if (pOpen) {
            *pOpen = order->filled_avg_price;
        }

        if (pProfit && order->filled_qty) {
            Quote* quote = nullptr;
            if (wsClient) {
                auto* asset = getAsset(order->symbol);
                if (asset)
                {
                    quote = wsClient->getLastQuote(asset);
                }
            }
            if (quote) {
                *pProfit = order->side == OrderSide::Buy ? ((quote->ask_price - order->filled_avg_price) * order->filled_qty) : (order->filled_avg_price - quote->bid_price) * order->filled_qty;
            }
            else {
                auto& assets = client->allAssets();
                auto iter = assets.find(order->symbol);
                if (iter != assets.end())
                {
                    auto asset_class = iter->second->asset_class;
                    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                    if (!s_lastQuotes[asset_class] || s_config.alpacaPaidPlan || (now - s_lastRequestTime) > REQUEST_BANNING_TIME) {
                        s_lastQuotes[asset_class] = pMarketData[asset_class]->getLastQuotes(s_subscribedAssets[asset_class]);
                        if (s_lastQuotes[asset_class]) {
                            s_lastRequestTime = now;
                        }
                        else {
                            BrokerError(("Failed to get lastQuotes for " + std::string(s_subscribedAssets[asset_class]) +
                                " error: " + s_lastQuotes[asset_class].what()).c_str());
                        }
                    }

                    if (s_lastQuotes[asset_class]) {
                        auto& lastQuotes = s_lastQuotes[asset_class].content().quotes;
                        auto it = lastQuotes.find(order->symbol);
                        if (it != lastQuotes.end()) {
                            *pProfit = order->side == OrderSide::Buy ? ((it->second.ask_price - order->filled_avg_price) * order->filled_qty) : (order->filled_avg_price - it->second.bid_price) * order->filled_qty;
                        }
                        else {
                            assert(false);
                            auto response = pMarketData[asset_class]->getLastQuote(order->symbol);
                            if (response) {
                                auto& quote = response.content().quote;
                                *pProfit = order->side == OrderSide::Buy ? ((quote.ask_price - order->filled_avg_price) * order->filled_qty) : (order->filled_avg_price - quote.bid_price) * order->filled_qty;
                            }
                            else
                            {
                                BrokerError(("Failed to get lastQuote " + std::string(order->symbol) +
                                    " error: " + response.what()).c_str());
                            }
                        }
                    }
                }
            }
        }
        return static_cast<int>(order->filled_qty / s_config.fractionalLotAmount);
    }

    // DLLFUNC_C int BrokerSell2(int nTradeID, int nAmount, double Limit, double* pClose, double* pCost, double* pProfit, int* pFill) {
    //     SPDLOG_DEBUG("BrokerSell2 nTradeID={} nAmount={} limit={}", nTradeID, nAmount, Limit);

    //    auto iter = s_mapOrderByClientOrderId.find(nTradeID);
    //    if (iter == s_mapOrderByClientOrderId.end()) {
    //        BrokerError(("Order " + std::to_string(nTradeID) + " not found.").c_str());
    //        return 0;
    //    }

    //    auto& order = iter->second;
    //    if (order.status == "filled") {
    //        // order has been filled
    //        auto closeTradeId = BrokerBuy2((char*)order.symbol.c_str(), -nAmount, 0, Limit, pProfit, pFill);
    //        if (closeTradeId) {
    //            auto iter2 = s_mapOrderByClientOrderId.find(closeTradeId);
    //            if (iter2 != s_mapOrderByClientOrderId.end()) {
    //                auto& closeTrade = iter2->second;
    //                if (pClose) {
    //                    *pClose = closeTrade.filled_avg_price;
    //                }
    //                if (pFill) {
    //                    *pFill = static_cast<int>(closeTrade.filled_qty);
    //                }
    //                if (pProfit) {
    //                    *pProfit = (closeTrade.filled_avg_price - order.filled_avg_price) * closeTrade.filled_qty;
    //                }
    //            }
    //            return nTradeID;
    //        }
    //        return 0;
    //    }
    //    else {
    //        // close working order?
    //        auto qty = nAmount * global.amount_;
    //        // reset amount, next asset might have lotAmount = 1, in that case SET_AMOUNT will not be called in advance
    //        global.amount_ = 1.;
    //        BrokerError(("Close working order " + std::to_string(nTradeID)).c_str());
    //        if (!qty) {
    //            auto response = client->cancelOrder(iter->second.id);
    //            if (response) {
    //                return nTradeID;
    //            }
    //            BrokerError(("Failed to close trade " + std::to_string(nTradeID) + " " + response.what()).c_str());
    //            return 0;
    //        }
    //        else {
    //            auto response = client->replaceOrder(order.id, iter->second.qty - qty, order.tif, (Limit ? std::to_string(Limit) : ""), "", iter->second.client_order_id);
    //            if (response) {
    //                auto& replacedOrder = response.content();
    //                uint32_t orderId = replacedOrder.internal_id;
    //                s_mapOrderByClientOrderId.emplace(orderId, std::move(replacedOrder));
    //                return orderId;
    //            }
    //            BrokerError(("Failed to modify trade " + std::to_string(nTradeID) + " " + response.what()).c_str());
    //            return 0;
    //        }
    //    }
    //}

    int32_t getPosition(const std::string& asset) {
        auto response = client->getPosition(asset);
        if (!response) {
            if (response.getCode() == 40410000) {
                // no open position
                return 0;
            }

            BrokerError(std::format("Get {} position failed. {}", asset, response.what()).c_str());
            return 0;
        }

        return response.content().qty;
    }

    constexpr int tifToZorroOrderType(TimeInForce tif) noexcept {
        constexpr const int converter[] = {0, 2, 0, 0, 1, 0};
        assert(tif >= 0 && tif < sizeof(converter) / sizeof(int));
        return converter[tif];
    }

    void downloadAssets(char* symbols) {
        FILE* f;
        std::string filename = s_config.alpacaPaidPlan ? "./Log/AssetsAlpaca.csv" : "./Log/AssetsAlpacaPaper.csv";
        if (fopen_s(&f, filename.c_str(), "w+")) {
            SPDLOG_ERROR("Failed to open {} file", filename.c_str());
            return;
        }

        BrokerError("Generating Asset List...");
        fprintf(f, "Name,Price,Spread,RollLong,RollShort,PIP,PIPCost,MarginCost,Leverage,LotAmount,Commission,Symbol,Type,Description\n");

        auto getAsset = [f](const alpaca::AssetBase *asset, LastQuotes lastQuotes) -> bool {
            BrokerError(("Asset " + asset->symbol).c_str());
            auto rt = BrokerProgress(0);
            if (!rt) {
                return false;
            }

            auto iter = lastQuotes.quotes.find(asset->symbol);
            if (iter == lastQuotes.quotes.end()) {
                BrokerError(("no quote found for " + asset->symbol).c_str());
                if (asset->asset_class == AssetClass::OPTIONS || !reinterpret_cast<const Asset*>(asset)->fractionable) {
                    fprintf(f, "%s,%f,%f,0.0,0.0,0.01,0.01,-100,1,1,0.000,%s,0,\"%s\"\n", asset->symbol.c_str(), 0., 0., asset->symbol.c_str(), asset->name.c_str());
                }
                else if (asset->asset_class == AssetClass::US_EQUITY) {
                    fprintf(f, "%s,%f,%f,0.0,0.0,0.01,0.01,-100,1,%f,0.000,%s,0,\"%s\"\n", asset->symbol.c_str(), 0., 0., s_config.fractionalLotAmount, asset->symbol.c_str(), asset->name.c_str());
                }
                else {
                    fprintf(f, "%s,%f,%f,0.0,0.0,0.01,0.01,-100,1,%f,0.000,%s,0,\"%s\"\n", asset->symbol.c_str(), 0., 0., reinterpret_cast<const Asset*>(asset)->min_order_size, asset->symbol.c_str(), asset->name.c_str());
                }
            }
            else {
                auto& q = iter->second;
                if (asset->asset_class == AssetClass::OPTIONS || !reinterpret_cast<const Asset*>(asset)->fractionable) {
                    fprintf(f, "%s,%f,%f,0.0,0.0,0.01,0.01,-100,1,1,0.000,%s,0,\"%s\"\n", asset->symbol.c_str(), q.ask_price, (q.ask_price - q.bid_price), asset->symbol.c_str(), asset->name.c_str());
                }
                else if (asset->asset_class == AssetClass::US_EQUITY) {
                    fprintf(f, "%s,%f,%f,0.0,0.0,0.01,0.01,-100,1,%f,0.000,%s,0,\"%s\"\n", asset->symbol.c_str(), q.ask_price, (q.ask_price - q.bid_price), s_config.fractionalLotAmount, asset->symbol.c_str(), asset->name.c_str());
                }
                else {
                    fprintf(f, "%s,%f,%f,0.0,0.0,0.01,0.01,-100,1,%f,0.000,%s,0,\"%s\"\n", asset->symbol.c_str(), 0., 0., reinterpret_cast<const Asset*>(asset)->min_order_size, asset->symbol.c_str(), asset->name.c_str());
                }
            }
            return true;
        };

        std::ostringstream equity_assets;
        std::ostringstream crypto_assets;
        std::ostringstream option_assets;
        LastQuotes last_quotes[AssetClass::__count__];
        if (!symbols) {
            auto &assets = client->allAssets();
            for (auto &kvp : assets) {
                auto *asset = kvp.second;
                if (!asset->tradable) {
                    continue;
                }

                switch (asset->asset_class)
                {
                case AssetClass::US_EQUITY:
                    equity_assets << asset->symbol << ',';
                    break;
                case AssetClass::CRYPTO:
                    crypto_assets << asset->symbol << ',';
                    break;
                case AssetClass::OPTIONS:
                    option_assets << asset->symbol << ',';
                    break;
                case AssetClass::__count__:
                    break;
                }
            }

            if (equity_assets.tellp())
            {
                auto last_quotes_rsp = pMarketData[AssetClass::US_EQUITY]->getLastQuotes(equity_assets.str());
                if (last_quotes_rsp)
                {
                    last_quotes[AssetClass::US_EQUITY] = std::move(last_quotes_rsp.content());
                }
            }
            if (crypto_assets.tellp())
            {
                auto last_quotes_rsp = pMarketData[AssetClass::CRYPTO]->getLastQuotes(equity_assets.str());
                if (last_quotes_rsp)
                {
                    last_quotes[AssetClass::CRYPTO] = std::move(last_quotes_rsp.content());
                }
            }
            if (option_assets.tellp())
            {
                auto last_quotes_rsp = pMarketData[AssetClass::OPTIONS]->getLastQuotes(equity_assets.str());
                if (last_quotes_rsp)
                {
                    last_quotes[AssetClass::OPTIONS] = std::move(last_quotes_rsp.content());
                }
            }
            for (auto& kvp : assets) {
                auto* asset = kvp.second;
                if (!asset->tradable) {
                    continue;
                }
                try {
                    if (!getAsset(asset, last_quotes[asset->asset_class])) {
                        break;
                    }
                }
                catch (...) {}
            }
            
        }
        else {
            std::vector<AssetBase*> assets;
            const char* delim = ",";
            char* next_token;
            char* token = strtok_s(symbols, delim, &next_token);
            auto& all_assets = client->allAssets();
            while (token != nullptr) {
                try {
                    std::string symbol = token;
                    trim(symbol);

                    auto iter = all_assets.find(symbol);
                    if (iter == all_assets.end())
                    {
                        SPDLOG_ERROR("Asset {} invalid", symbol);
                        BrokerError(("Asset " + symbol + " invalid").c_str());
                    }
                    assets.emplace_back(iter->second);
                    token = strtok_s(nullptr, delim, &next_token);
                }
                catch (...) {}
            }

            for (auto* asset : assets)
            {
                if (!asset->tradable) {
                    continue;
                }

                switch (asset->asset_class)
                {
                case AssetClass::US_EQUITY:
                    equity_assets << asset->symbol << ',';
                    break;
                case AssetClass::CRYPTO:
                    crypto_assets << asset->symbol << ',';
                    break;
                case AssetClass::OPTIONS:
                    option_assets << asset->symbol << ',';
                    break;
                case AssetClass::__count__:
                    break;
                }
            }
            if (equity_assets.tellp())
            {
                auto last_quotes_rsp = pMarketData[AssetClass::US_EQUITY]->getLastQuotes(equity_assets.str());
                if (last_quotes_rsp)
                {
                    last_quotes[AssetClass::US_EQUITY] = std::move(last_quotes_rsp.content());
                }
            }
            if (crypto_assets.tellp())
            {
                auto last_quotes_rsp = pMarketData[AssetClass::CRYPTO]->getLastQuotes(equity_assets.str());
                if (last_quotes_rsp)
                {
                    last_quotes[AssetClass::CRYPTO] = std::move(last_quotes_rsp.content());
                }
            }
            if (option_assets.tellp())
            {
                auto last_quotes_rsp = pMarketData[AssetClass::OPTIONS]->getLastQuotes(equity_assets.str());
                if (last_quotes_rsp)
                {
                    last_quotes[AssetClass::OPTIONS] = std::move(last_quotes_rsp.content());
                }
            }
            for (auto* asset : assets) {
                if (!asset->tradable) {
                    continue;
                }
                try {
                    if (!getAsset(asset, last_quotes[asset->asset_class])) {
                        break;
                    }
                }
                catch (...) {}
            }
        }
        
        fflush(f);
        fclose(f);
    }
    
    DLLFUNC_C double BrokerCommand(int Command, intptr_t parameter)
    {
        handleSettingUpdate();
        static int SetMultiplier;
        std::string Data, response;
        int i = 0;
        double price = 0., spread = 0.;

        switch (Command)
        {
        case GET_COMPLIANCE:
            return 15; // full NFA compliant

        case GET_BROKERZONE:
            return 0;   // historical data in UTC time

        case GET_MAXTICKS:
            return 9999;

        case GET_MAXREQUESTS:
        {
            // Alpaca rate limit is 200 requests per minutes for Free plan.
            // GET_MAXREQUESTS: maximum number of requests per second allowed by the broker API
            auto rt = s_config.alpacaPaidPlan ? 0 : 3;
            SPDLOG_DEBUG("GET_MAXREQUESTS {}", rt);
            return rt;
        }

        case GET_LOCK:
            return 1;

        case GET_POSITION:
            return getPosition((char*)(parameter));

        case SET_ORDERTEXT:
            global.order_text_ = (char*)parameter;
            SPDLOG_DEBUG("SET_ORDERTEXT: {}", global.order_text_.c_str());
            return (double)parameter;

        case SET_SYMBOL:
            global.symbol_ = (char*)parameter;
            SPDLOG_TRACE("SET_SYMBOL: {}", global.symbol_.c_str());
            return 1;

        case SET_MULTIPLIER:
            global.multiplier_ = (double)parameter;
            return 1;

        case SET_ORDERTYPE: {
           
            switch ((int)parameter) {
            case 0:
                return 0;
            case ORDERTYPE_IOC:
                s_tif = TimeInForce::IOC;
                break;
            case ORDERTYPE_GTC:
                s_tif = TimeInForce::GTC;
                break;
            case ORDERTYPE_FOK:
                s_tif = TimeInForce::FOK;
                break;
            case ORDERTYPE_DAY:
                s_tif = TimeInForce::Day;
                break;
            case ORDERTYPE_OPG:
                s_tif = TimeInForce::OPG;
                break;
            case ORDERTYPE_CLS:
                s_tif = TimeInForce::CLS;
                break;
            }

            if ((int)parameter >= 8) {
                return (int)parameter;
            }

            SPDLOG_DEBUG("SET_ORDERTYPE: {} s_tif={}", (int)parameter, to_string(s_tif));
            return tifToZorroOrderType(s_tif);
        }

        case GET_PRICETYPE:
            return global.price_type_;

        case SET_PRICETYPE:
        {
            auto price_type = (int)parameter;
            bool resubscribe = global.price_type_ != price_type;
            global.price_type_ = (int)parameter;
            SPDLOG_DEBUG("SET_PRICETYPE: {} resubscribe: {}", global.price_type_, resubscribe);
            if (resubscribe && wsClient) {
                // price type changed. 
                // change websocket subscription
                for (auto* asset : s_activeAssets) {
                    if (wsClient->subscribeAsset(asset))
                    {
                        SPDLOG_DEBUG("{} subscribed", asset->name);

                        // Query Last Quote/Trade once, in case the symbol is iliquid we don't get any update from WebSocket
                        if (global.price_type_ == 2)
                        {
                            auto last_trade = pMarketData[asset->asset_class]->getLastTrade(asset->symbol);
                            if (last_trade)
                            {
                                wsClient->setLastTrade(asset, last_trade.content());
                            }
                        }
                        else
                        {
                            auto last_quote = pMarketData[asset->asset_class]->getLastQuote(asset->symbol);
                            if (last_quote)
                            {
                                wsClient->setLastQuote(asset, last_quote.content());
                            }
                        }
                    }
                }
            }
            return (double)parameter;
        }

        case GET_UUID:
            snprintf((char*)parameter, s_lastOrderUUID.size() + 1, s_lastOrderUUID.c_str());
            SPDLOG_TRACE("GET_UUID: {}", (char*)parameter);
            return (double)parameter;

        case SET_UUID:
            s_nextOrderUUID = (char*)parameter;
            SPDLOG_TRACE("SET_UUID: {}", s_nextOrderUUID);
            return (double)parameter;

        case SET_AMOUNT:
            global.amount_ = *(double*)parameter;
            SPDLOG_TRACE("SET_AMOUNT: {:.8f}", global.amount_);
            return (double)parameter;

        case SET_DIAGNOSTICS:
            if ((int)parameter == 1)
            {
                spdlog::set_level(spdlog::level::trace);
            }
            else if ((int)parameter == 0)
            {
                spdlog::set_level(toLogLevel(s_config.logLevel));
            }
            return (double)parameter;

        case GET_VOLTYPE:
        {
            auto rt = global.price_type_ == 2 ? 4 : 3;
            SPDLOG_TRACE("GET_VOLTYPE: {}", rt);
            return rt;
        }

        case SET_VOLTYPE:
            SPDLOG_TRACE("SET_VOLTYPE: {}", (int)parameter);
            global.vol_type_ = (int)parameter;
            return (double)parameter;

        case SET_HWND:
        {
            global.handle_ = (HWND)parameter;
            return (double)parameter;
        }

        case SET_FUNCTIONS:
        {
            FARPROC* Functions = (FARPROC*)parameter;
            (FARPROC &)report = Functions[565];
            break;
        }

        case GET_CALLBACK:
            return (double)(intptr_t)pluginCallback;

        case SET_CCY:
            break;

        case GET_VOLUME:
            switch ((int)parameter)
            {
            case 5:
                break;
            case 6:
                break;
            }
            break;

        case GET_OPTIONS: {
            auto rsp = client->getOptionContracts(global.symbol_);
            if (!rsp) {
                BrokerError(rsp.what().c_str());
                return 0;
            }
            int nOptions = rsp.content().contracts.size();
            CONTRACT *contract = (CONTRACT*)parameter;
            auto& options = rsp.content();
            for (auto & option : options.contracts) {
                if (option.symbol.empty()) {
                    --nOptions;
                    continue;
                }
                auto expiry = yyyymmdd_from_string(option.str_expration_date);
                if (expiry == -1) {
                    BrokerError(("Invalid option expiration date: " + option.str_expration_date).c_str());
                    --nOptions;
                    continue; // skip expired options
                }
                contract->fStrike = option.strike_price;
                contract->fVal = option.multiplier;
                contract->Expiry = expiry;
                contract->Type = option.type == OptionContract::Type::OPT_CALL ? CALL : PUT;
                if (option.style == OptionContract::Style::OPT_EUROPEAN) {
                    contract->Type |= EUROPEAN;
                }
                contract++;
            }
            return nOptions;
        }

        case CREATE_ASSETLIST: {
            downloadAssets((char*)parameter);
            break;
        }

        case IS_ASSET_FRACTIONABLE: {
            auto asset_str = (char*)parameter;
            auto& assets = client->allAssets();
            auto iter = assets.find(asset_str);
            if (iter != assets.end()) {
                auto* asset = iter->second;
                if (asset->asset_class != AssetClass::OPTIONS)
                {
                    return reinterpret_cast<Asset*>(iter->second)->fractionable;
                }
            }
            return false;
        }

        case IS_ASSET_SHORTABLE: {
            auto asset_str = (char*)parameter;
            auto& assets = client->allAssets();
            auto iter = assets.find(asset_str);
            if (iter != assets.end()) {
                auto* asset = iter->second;
                if (asset->asset_class != AssetClass::OPTIONS)
                {
                    return reinterpret_cast<Asset*>(iter->second)->shortable;
                }
            }
            return false;
        }

        case IS_ASSET_EASY_TO_BORROW: {
            auto asset_str = (char*)parameter;
            auto& assets = client->allAssets();
            auto iter = assets.find(asset_str);
            if (iter != assets.end()) {
                auto* asset = iter->second;
                if (asset->asset_class != AssetClass::OPTIONS)
                {
                    return reinterpret_cast<Asset*>(iter->second)->easy_to_borrow;
                }
            }
            return false;
        }

        case IS_ASSET_MARGINABLE: {
            auto asset_str = (char*)parameter;
            auto& assets = client->allAssets();
            auto iter = assets.find(asset_str);
            if (iter != assets.end()) {
                auto* asset = iter->second;
                if (asset->asset_class != AssetClass::OPTIONS)
                {
                    return reinterpret_cast<Asset*>(iter->second)->marginable;
                }
            }
            return false;
        }

        case SET_LOG_LEVEL: {
            auto level = toLogLevel((int)parameter);
            BrokerError(("Set LogLevel to " + std::to_string((int)parameter)).c_str());
            spdlog::set_level(level);
            if (level > SPDLOG_LEVEL_INFO)
            {
                spdlog::flush_on(level);
            }
            break;
        }

        case SET_ADJUSTMENT: {
            s_config.adjustment = static_cast<Adjustment>((int)parameter);
            BrokerError(("Set Adjustment to " + std::string(to_string(s_config.adjustment))).c_str());
            break;
        }

        default:
            SPDLOG_DEBUG("Unhandled command: {} {}", Command, parameter);
            break;
        }
        return 0;
    }
}
