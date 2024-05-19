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

#include "alpaca/client.h"
#include "logger.h"
#include "include/functions.h"
#include "market_data/alpaca_market_data.h"
#include "config.h"
#include "websockets/alpaca_md_ws.h"
#include "AlpacaBrokerCommands.h"

#define PLUGIN_VERSION	2

using namespace alpaca;
using namespace zorro::websocket;

#define ALPACA_PAPER_BASIC_DATA_WS_URL "wss://stream.data.sandbox.alpaca.markets/v2/iex"
#define ALPACA_PAPER_PRO_DATA_WS_URL "wss://stream.data.sandbox.alpaca.markets/v2/sip"
#define ALPACA_BASIC_DATA_WS_URL "wss://stream.data.alpaca.markets/v2/iex"
#define ALPACA_PRO_DATA_WS_URL "wss://stream.data.alpaca.markets/v2/sip"
#define ALPACA_STREAM_URL "wss://api.alpaca.markets/stream"

namespace {
    TimeInForce s_tif = TimeInForce::FOK;
    std::string s_asset;
    int s_multiplier = 1;
    std::string s_nextOrderText;
    int s_priceType = 0;
    std::unordered_map<uint32_t, Order> s_mapOrderByClientOrderId;
    Config &s_config = Config::get();
    uint32_t alpaca_md_ws_id = 0;
    double s_amount = 1;
    std::unordered_map<std::string, Asset> s_mapAssets;
    std::string s_subscribedAssets;
    uint64_t s_lastQuotesRequestTime = 0;
    uint64_t s_lastTradesRequestTime = 0;
    Response<LastQuotes> s_lastQuotes{ 1, "No Data"};
    Response<LastTrades> s_lastTrades{ 1, "No Data"};
} 

namespace alpaca
{
    std::unique_ptr<Client> client = nullptr;
    std::unique_ptr<AlpacaMarketData> pMarketData = nullptr;
    std::unique_ptr<AlpacaMdWs> wsClient = nullptr;
    std::unique_ptr<AlpacaMdWs> wsCryptoClient = nullptr;
    
    ////////////////////////////////////////////////////////////////
    DLLFUNC_C int BrokerOpen(char* Name, FARPROC fpError, FARPROC fpProgress)
    {
        strcpy_s(Name, 32, "Alpaca");
        (FARPROC&)BrokerError = fpError;
        (FARPROC&)BrokerProgress = fpProgress;
        Logger::instance().init("Alpaca");
        return PLUGIN_VERSION;
    }

    DLLFUNC_C void BrokerHTTP(FARPROC fpSend, FARPROC fpStatus, FARPROC fpResult, FARPROC fpFree)
    {
        (FARPROC&)http_send = fpSend;
        (FARPROC&)http_status = fpStatus;
        (FARPROC&)http_result = fpResult;
        (FARPROC&)http_free = fpFree;

        if (s_config.useWebsocket) {
            wsClient = std::make_unique<AlpacaMdWs>();
        }
    }

    DLLFUNC_C int BrokerLogin(char* User, char* Pwd, char* Type, char* Account)
    {
        if (!User) // log out
        {
            if (wsClient) {
                wsClient->logout();
            }
            alpaca_md_ws_id = 0;
            return 0;
        }

        s_tif = TimeInForce::FOK;
        s_nextOrderText = "";
        s_priceType = 0;
        s_amount = 1.;
        s_subscribedAssets = "";
        s_config.adjustment = Adjustment::all;
       

        bool isPaperTrading = strcmp(Type, "Demo") == 0;
        Logger::instance().setLevel(static_cast<LogLevel>(s_config.logLevel));
        Logger::instance().setLogType(s_config.logType);

        if (!client || client->getApiKey() != User) {
            client = std::make_unique<Client>(User, Pwd, isPaperTrading);

            pMarketData = std::make_unique<AlpacaMarketData>(client->headers(), s_config.alpacaPaidPlan);

            if (s_config.alpacaPaidPlan) {
                BrokerError("Use Alpaca Pro Market Data");
                LOG_INFO("Use Alpaca Pro Market Data\n");
            }
            else {
                BrokerError("Use Alpaca Basic Market Data");
                LOG_INFO("Use Alpaca Basic Market Data\n");
            }
        }

        //attempt login
        if (wsClient)
        {
            if (isPaperTrading)
            {
                if (!wsClient->login(User, Pwd, s_config.alpacaPaidPlan ? ALPACA_PAPER_PRO_DATA_WS_URL : ALPACA_PAPER_BASIC_DATA_WS_URL)) {
                    BrokerError("Unable to open Alpaca websocket. Prices will be pulled from REST API.");
                }
            }
            else
            {
                if (!wsClient->login(User, Pwd, s_config.alpacaPaidPlan ? ALPACA_PRO_DATA_WS_URL : ALPACA_BASIC_DATA_WS_URL)) {
                    BrokerError("Unable to open Alpaca websocket. Prices will be pulled from REST API.");
                }
            }
        }

        auto response = client->getAccount();
        if (!response) {
            BrokerError("Login failed.");
            BrokerError(response.what().c_str());
            return 0;
        }

        if (s_config.fractionalLotAmount > 1) {
            s_config.fractionalLotAmount = 1;
            BrokerError(("Fractional qty disabled. Invalid config: AlpacaFranctionalLotAmount must be less than 1. AlpacaFranctionalLotAmount=" + std::to_string(s_config.fractionalLotAmount)).c_str());
        }

        const auto& account = response.content().account_number;
        BrokerError(("Account " + account).c_str());
        memcpy(Account, account.c_str(), 1024);
        sprintf_s(Account, 1024, account.c_str());
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
        auto response = client->getClock();
        if (!response) {
#ifdef _DEBUG
            BrokerError(response.what().c_str());
#endif
            return 0;
        }

        auto& clock = response.content();

        *pTimeGMT = convertTime(clock.timestamp);
        return clock.is_open ? 2 : 1;
    }

    DLLFUNC_C int BrokerAsset(char* Asset, double* pPrice, double* pSpread, double* pVolume, double* pPip, double* pPipCost, double* pLotAmount, double* pMarginCost, double* pRollLong, double* pRollShort)
    {
        if (!pPrice) { // this is subscribe
            BrokerError(("Subscribe " + std::string(Asset)).c_str());
            if (s_subscribedAssets.empty()) {
                s_subscribedAssets.append(Asset);
            }
            else {
                s_subscribedAssets.append(",").append(Asset);
            }

            if (!wsClient || !wsClient->authenticated()) {
                // not using websocket
                return 1;
            }

            if (wsClient->subscribeAsset(Asset)) {
                LOG_DEBUG("%s subscribed\n", Asset);
            }
            else {
                LOG_DEBUG("Failed to subscribed %s\n", Asset);
                BrokerError(("Failed to subscribe " + std::string(Asset) + " from Websocket. Price will be polled from REST API.").c_str());
            }
            return 1;
        }

        if (wsClient && wsClient->authenticated() && !wsClient->isSubscribed(Asset)) {
            // When strategy logged out and re-logged in after market close re-open, Zorro does not 
            // pass the NULL pPrice to subscribe.
            //
            // Need to resubscribe Websocket price update
            if (wsClient->subscribeAsset(Asset)) {
                LOG_DEBUG("%s subscribed\n", Asset);
            }
            else {
                LOG_DEBUG("Failed to subscribed %s\n", Asset);
                BrokerError(("Failed to subscribe " + std::string(Asset) + " from Websocket. Price will be polled from REST API.").c_str());
            }
        }

        if (s_priceType == 2) {
            Trade* trade = nullptr;
            if (wsClient && wsClient->authenticated()) {
                trade = wsClient->getLastTrade(Asset);
            }
            if (trade) {
                if (pPrice) {
                    *pPrice = trade->price;
                }
            }
            else {
                auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                if (!s_lastTrades || (now - s_lastTradesRequestTime) > 333) {
                    s_lastTrades = pMarketData->getLastTrades(s_subscribedAssets);
                    if (!s_lastTrades) {
                        BrokerError(("Failed to get lastTrades for " + std::string(s_subscribedAssets) +
                            " error: " + s_lastTrades.what()).c_str());
                        return 0;
                    }
                    s_lastTradesRequestTime = now;
                }
                
                if (s_lastTrades) {
                    auto& lastTrades = s_lastTrades.content().trades;
                    auto it = lastTrades.find(Asset);
                    if (it != lastTrades.end()) {
                        if (pPrice) {
                            *pPrice = it->second.price;
                        }
                    }
                    else {
                        assert(false);
                        auto response = pMarketData->getLastTrade(Asset);
                        if (!response) {
                            BrokerError(("Failed to get lastTrade " + std::string(Asset) +
                                " error: " + response.what()).c_str());
                            return 0;
                        }

                        auto& lastTrade = response.content();

                        if (pPrice) {
                            *pPrice = lastTrade.trade.price;
                        }
                    }
                }
            }

            if (pSpread) {
                Quote* quote = nullptr;
                if (wsClient && wsClient->authenticated()) {
                    quote = wsClient->getLastQuote(Asset);
                }
                if (quote) {
                    *pSpread = quote->ask_price - quote->bid_price;
                }
                else {
                    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                    if (!s_lastQuotes || (now - s_lastQuotesRequestTime) > 1) {
                        s_lastQuotes = pMarketData->getLastQuotes(s_subscribedAssets);
                        if (s_lastQuotes) {
                            s_lastQuotesRequestTime = now;
                        }
                        else {
                            BrokerError(("Failed to get lastQuotes for " + std::string(s_subscribedAssets) +
                                " error: " + s_lastQuotes.what()).c_str());
                        }
                    }

                    if (s_lastQuotes) {
                        auto& lastQuotes = s_lastQuotes.content().quotes;
                        auto it = lastQuotes.find(Asset);
                        if (it != lastQuotes.end()) {
                            *pSpread = it->second.ask_price - it->second.bid_price;
                        }
                        else {
                            assert(false);
                            auto response = pMarketData->getLastQuote(Asset);
                            if (response) {
                                auto& lastQuote = response.content();
                                *pSpread = lastQuote.quote.ask_price - lastQuote.quote.bid_price;
                            }
                            else
                            {
                                BrokerError(("Failed to get lastQuote " + std::string(Asset) +
                                    " error: " + response.what()).c_str());
                            }
                        }
                    }
                }
            }
        }
        else {
            Quote* quote = nullptr;
            if (wsClient && wsClient->authenticated()) {
                quote = wsClient->getLastQuote(Asset);
            }
            if (quote) {
                if (pPrice) {
                    *pPrice = quote->ask_price;
                }

                if (pSpread) {
                    *pSpread = quote->ask_price - quote->bid_price;
                }
            }
            else {
                auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                if (!s_lastQuotes || (now - s_lastQuotesRequestTime) > 1) {
                    s_lastQuotes = pMarketData->getLastQuotes(s_subscribedAssets);
                    if (!s_lastQuotes) {
                        BrokerError(("Failed to get lastQuotes for " + std::string(s_subscribedAssets) +
                            " error: " + s_lastQuotes.what()).c_str());
                        return 0;
                    }
                    s_lastQuotesRequestTime = now;
                }

                if (s_lastQuotes) {
                    auto& lastQuotes = s_lastQuotes.content().quotes;
                    auto it = lastQuotes.find(Asset);
                    if (it != lastQuotes.end()) {
                        if (pPrice) {
                            *pPrice = it->second.ask_price;
                        }

                        if (pSpread) {
                            *pSpread = it->second.ask_price - it->second.bid_price;
                        }
                    }
                    else {
                        assert(false);
                        auto response = pMarketData->getLastQuote(Asset);
                        if (!response) {
                            BrokerError(("Failed to get lastQuote " + std::string(Asset) +
                                " error: " + response.what()).c_str());
                            return 0;
                        }

                        auto& lastQuote = response.content();

                        if (pPrice) {
                            *pPrice = lastQuote.quote.ask_price;
                        }

                        if (pSpread) {
                            *pSpread = lastQuote.quote.ask_price - lastQuote.quote.bid_price;
                        }
                    }
                }
            }
        }

        if (pLotAmount) {
            auto iter = s_mapAssets.find(Asset);
            if (iter == s_mapAssets.end()) {
                auto response = client->getAsset(Asset);
                if (response) {
                    auto& asset = response.content();
                    s_mapAssets.emplace(Asset, asset);
                    if (asset.fractionable) {
                        *pLotAmount = s_config.fractionalLotAmount;
                    }
                    else {
                        *pLotAmount = 1;
                    }
                }
                else {
                    *pLotAmount = 1;
                }
            }
            else {
                auto& asset = iter->second;
                if (asset.fractionable) {
                    *pLotAmount = s_config.fractionalLotAmount;
                }
                else {
                    *pLotAmount = 1;
                }
            }
        }

        if (pRollLong) {
            *pRollLong = 0.;
        }

        if (pRollShort) {
            *pRollShort = 0.;
        }

        return 1;
    }

    DLLFUNC_C int BrokerHistory2(char* Asset, DATE tStart, DATE tEnd, int nTickMinutes, int nTicks, T6* ticks)
    {
        if (!client || !Asset || !ticks || !nTicks) return 0;

        if (!nTickMinutes) {
            BrokerError("Tick data download is not supported by Alpaca.");
            return 0;
        }

        auto start = convertTime(tStart);
        auto end = convertTime(tEnd);

        int barsDownloaded = 0;

        auto populateTicks = [&barsDownloaded, nTickMinutes, &end, &ticks, nTicks](std::vector<Bar>& bars) {
            for (int i = bars.size() - 1; i >= 0 && barsDownloaded < nTicks; --i) {
                auto& bar = bars[i];
                // change time to bar close time
                __time32_t barCloseTime = bar.time + nTickMinutes * 60;
                if (barCloseTime > end) {
                    // end time cannot exceeds tEnd
                    continue;
                }
                auto& tick = ticks[barsDownloaded++];
                tick.time = convertTime(barCloseTime);
                tick.fOpen = static_cast<float>(bar.open_price);
                tick.fHigh = static_cast<float>(bar.high_price);
                tick.fLow = static_cast<float>(bar.low_price);
                tick.fClose = static_cast<float>(bar.close_price);
                tick.fVol = static_cast<float>(bar.volume);
            }
        };

        if (!s_config.alpacaPaidPlan) {
            LOG_DEBUG("BrokerHistory %s start: %d end: %d nTickMinutes: %d nTicks: %d\n", Asset, start, end, nTickMinutes, nTicks);
            auto now = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            if ((now - end) <= 900) {
                //if (s_config.fillDelayedDataWithPolygon && polygon) {
                //    // Fill missed delay data from Polygon
                //    s_logger->logInfo("Fill 15 minutes delay data with polygon\n", Asset, start, end, nTickMinutes, std::max<int>((30 / nTickMinutes), 1));
                //    auto response = polygon->getBars(Asset, start, end, nTickMinutes, nTicks);
                //    if (response) {
                //        populateTicks(response.content());
                //        if (!response.content().empty()) {
                //            end = response.content()[0].time - 30;
                //        }
                //        else {
                //            end = static_cast<__time32_t>(now) - 900;
                //        }
                //    }
                //}
                //else {
                    end = static_cast<__time32_t>(now) - 900;
                    LOG_INFO("Alpaca Basic Plan. Adjust end to %d\n", end);
                //}
            }
        }

        while(true) {
            LOG_DEBUG("BrokerHistory %s start: %s(%d) end: %s(%d) nTickMinutes: %d nTicks: %d\n", Asset, timeToString(start).c_str(), start, timeToString(end).c_str(), end, nTickMinutes, nTicks);

            auto response = pMarketData->getBars(Asset, start, end, nTickMinutes, nTicks, s_priceType);
            if (!response) {
                if (response.getCode() == 40010001 && response.what() == "end is too late for subscription" &&
                    pMarketData->name() == "Alpaca") {
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

            LOG_DIAG("%d bar downloaded. %s-%s\n", bars.size(), (bars.size() > 0 ? bars[0].time_string.c_str() : ""), (bars.size() > 0 ? (*bars.rbegin()).time_string.c_str() : ""));

            populateTicks(bars);
            LOG_DIAG("%d bar returned\n", barsDownloaded);
            break;
        }
        return barsDownloaded;
    }

    DLLFUNC_C int BrokerAccount(char* Account, double* pdBalance, double* pdTradeVal, double* pdMarginVal)
    {
        auto response = client->getBalance();
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
        auto start = std::time(nullptr);

        OrderSide side = nAmount > 0 ? OrderSide::Buy : OrderSide::Sell;
        OrderType type = OrderType::Market;
        double qty = std::abs(nAmount);
        if (dLimit) {
            type = OrderType::Limit;
        }
        else {
            dLimit = NAN;
            qty *= s_amount;
        }

        if (dStopDist) {

        }


        LOG_DEBUG("BrokerBuy2 %s orderText=%s nAmount=%d qty=%f dStopDist=%f limit=%f\n", Asset, s_nextOrderText.c_str(), nAmount, qty, dStopDist, dLimit);

        auto response = client->submitOrder(Asset, qty, side, type, s_tif, dLimit, NAN, false, s_nextOrderText, s_config.fractionalLotAmount);
        if (!response) {
            BrokerError(response.what().c_str());
            // reset s_amount, next asset might have lotAmount = 1, in that case SET_AMOUNT will not be called in advance
            s_amount = 1.;
            return 0;
        }

        auto* order = &response.content();
        auto exchOrdId = order->id;
        auto internalOrdId = order->internal_id;
        s_mapOrderByClientOrderId.emplace(internalOrdId, *order);

        if (order->filled_qty) {
            if (pPrice) {
                *pPrice = response.content().filled_avg_price;
            }
            if (pFill) {
                *pFill = static_cast<int>(alpaca::fix_floating_error(response.content().filled_qty / s_amount));
            }
            // reset s_amount, next asset might have lotAmount = 1, in that case SET_AMOUNT will not be called in advance
            s_amount = 1.;
            return internalOrdId;
        }

        if ((type == OrderType::Limit || s_tif == TimeInForce::CLS || s_tif == TimeInForce::OPG) && order->status == "new") {
            return internalOrdId;
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
            s_mapOrderByClientOrderId[internalOrdId] = *order;

            LOG_DEBUG_EXT(LT_ORDER, "Order status: %s\n", order->status.c_str());

            if ((type == OrderType::Limit || s_tif == TimeInForce::CLS || s_tif == TimeInForce::OPG) && order->status == "new") {
                break;
            }

            if (order->filled_qty) {
                if (pPrice) {
                    *pPrice = order->filled_avg_price;
                }
                if (pFill) {
                    *pFill = (int)alpaca::fix_floating_error(order->filled_qty / s_amount);
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
                LOG_INFO("Order submit timed out. Canceling order %s\n", exchOrdId.c_str());
                auto response3 = client->cancelOrder(exchOrdId);
                if (!response3) {
                    BrokerError(("Failed to cancel order " + exchOrdId + " " + response3.what()).c_str());
                }
                return (s_tif == TimeInForce::IOC || s_tif == TimeInForce::FOK) ? 0 : -2;
            }
        } while (!order->filled_qty);
        // reset s_amount, next asset might have lotAmount = 1, in that case SET_AMOUNT will not be called in advance
        s_amount = 1.;
        return internalOrdId;
    }

    DLLFUNC_C int BrokerTrade(int nTradeID, double* pOpen, double* pClose, double* pCost, double *pProfit) {
        LOG_INFO("BrokerTrade: %d\n", nTradeID);
       /* if (nTradeID != -1) {
            BrokerError(("nTradeID " + std::to_string(nTradeID) + " not valid. Need to be an UUID").c_str());
            return NAY;
        }*/
        
        Response<Order> response;
        Order* order = nullptr;
        auto iter = s_mapOrderByClientOrderId.find(nTradeID);
        if (iter == s_mapOrderByClientOrderId.end()) {
            // unknown order?
            std::stringstream clientOrderId;
            clientOrderId << "ZORRO_";
            if (!s_nextOrderText.empty()) {
                clientOrderId << s_nextOrderText << "_";
            }
            clientOrderId << nTradeID;
            response = client->getOrderByClientOrderId(clientOrderId.str());
            if (!response) {
                BrokerError(response.what().c_str());
                return NAY;
            }
            order = &response.content();
            s_mapOrderByClientOrderId.insert(std::make_pair(nTradeID, response.content()));
        }
        else {
            order = &iter->second;
            if (order->status != "filled" && order->status != "canceled" && order->status != "expired") {
                response = client->getOrder(iter->second.id);
                if (!response) {
                    BrokerError(response.what().c_str());
                    return NAY;
                }
                order = &response.content();
                s_mapOrderByClientOrderId[nTradeID] = *order;
            }
        }

        if (pOpen) {
            *pOpen = order->filled_avg_price;
        }

        if (pProfit && order->filled_qty) {
            Quote* quote = nullptr;
            if (wsClient) {
                quote = wsClient->getLastQuote(order->symbol);
            }
            if (quote) {
                *pProfit = order->side == OrderSide::Buy ? ((quote->ask_price - order->filled_avg_price) * order->filled_qty) : (order->filled_avg_price - quote->bid_price) * order->filled_qty;
            }
            else {
                auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                if (!s_lastQuotes || (now - s_lastQuotesRequestTime) > 250) {
                    s_lastQuotes = pMarketData->getLastQuotes(s_subscribedAssets);
                    if (s_lastQuotes) {
                        s_lastQuotesRequestTime = now;
                    }
                    else {
                        BrokerError(("Failed to get lastQuotes for " + std::string(s_subscribedAssets) +
                            " error: " + s_lastQuotes.what()).c_str());
                    }
                }

                if (s_lastQuotes) {
                    auto& lastQuotes = s_lastQuotes.content().quotes;
                    auto it = lastQuotes.find(order->symbol);
                    if (it != lastQuotes.end()) {
                        *pProfit = order->side == OrderSide::Buy ? ((it->second.ask_price - order->filled_avg_price) * order->filled_qty) : (order->filled_avg_price - it->second.bid_price) * order->filled_qty;
                    }
                    else {
                        assert(false);
                        auto response = pMarketData->getLastQuote(order->symbol);
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
        return static_cast<int>(order->filled_qty / s_config.fractionalLotAmount);
    }

    DLLFUNC_C int BrokerSell2(int nTradeID, int nAmount, double Limit, double* pClose, double* pCost, double* pProfit, int* pFill) {
        LOG_DEBUG("BrokerSell2 nTradeID=%d nAmount=%d limit=%f\n", nTradeID,nAmount, Limit);

        auto iter = s_mapOrderByClientOrderId.find(nTradeID);
        if (iter == s_mapOrderByClientOrderId.end()) {
            BrokerError(("Order " + std::to_string(nTradeID) + " not found.").c_str());
            return 0;
        }

        auto& order = iter->second;
        if (order.status == "filled") {
            // order has been filled
            auto closeTradeId = BrokerBuy2((char*)order.symbol.c_str(), -nAmount, 0, Limit, pProfit, pFill);
            if (closeTradeId) {
                auto iter2 = s_mapOrderByClientOrderId.find(closeTradeId);
                if (iter2 != s_mapOrderByClientOrderId.end()) {
                    auto& closeTrade = iter2->second;
                    if (pClose) {
                        *pClose = closeTrade.filled_avg_price;
                    }
                    if (pFill) {
                        *pFill = static_cast<int>(closeTrade.filled_qty);
                    }
                    if (pProfit) {
                        *pProfit = (closeTrade.filled_avg_price - order.filled_avg_price) * closeTrade.filled_qty;
                    }
                }
                return nTradeID;
            }
            return 0;
        }
        else {
            // close working order?
            auto qty = nAmount * s_amount;
            // reset s_amount, next asset might have lotAmount = 1, in that case SET_AMOUNT will not be called in advance
            s_amount = 1.;
            BrokerError(("Close working order " + std::to_string(nTradeID)).c_str());
            if (std::abs(qty) == order.qty) {
                auto response = client->cancelOrder(iter->second.id);
                if (response) {
                    return nTradeID;
                }
                BrokerError(("Failed to close trade " + std::to_string(nTradeID) + " " + response.what()).c_str());
                return 0;
            }
            else {
                auto response = client->replaceOrder(order.id, iter->second.qty - qty, order.tif, (Limit ? std::to_string(Limit) : ""), "", iter->second.client_order_id);
                if (response) {
                    auto& replacedOrder = response.content();
                    uint32_t orderId = replacedOrder.internal_id;
                    s_mapOrderByClientOrderId.emplace(orderId, std::move(replacedOrder));
                    return orderId;
                }
                BrokerError(("Failed to modify trade " + std::to_string(nTradeID) + " " + response.what()).c_str());
                return 0;
            }
        }
    }

    int32_t getPosition(const std::string& asset) {
        auto response = client->getPosition(asset);
        if (!response) {
            if (response.getCode() == 40410000) {
                // no open position
                return 0;
            }

            BrokerError(("Get position failed. " + response.what()).c_str());
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
            LOG_ERROR("Failed to open %s file\n", filename.c_str());
            return;
        }

        BrokerError("Generating Asset List...");
        fprintf(f, "Name,Price,Spread,RollLong,RollShort,PIP,PIPCost,MarginCost,Leverage,LotAmount,Commission,Symbol,Type,Description\n");

        auto getAsset = [f](const alpaca::Asset& asset, LastQuotes lastQuotes) -> bool {
            BrokerError(("Asset " + asset.symbol).c_str());
            auto rt = BrokerProgress(0);
            if (!rt) {
                return false;
            }

            auto iter = lastQuotes.quotes.find(asset.symbol);
            if (iter == lastQuotes.quotes.end()) {
                BrokerError(("no quote found for " + asset.symbol).c_str());
                if (!asset.fractionable) {
                    fprintf(f, "%s,%f,%f,0.0,0.0,0.01,0.01,-100,1,1,0.000,%s,0,\"%s\"\n", asset.symbol.c_str(), 0., 0., asset.symbol.c_str(), asset.name.c_str());
                }
                else {
                    fprintf(f, "%s,%f,%f,0.0,0.0,0.01,0.01,-100,1,%f,0.000,%s,0,\"%s\"\n", asset.symbol.c_str(), 0., 0., s_config.fractionalLotAmount, asset.symbol.c_str(), asset.name.c_str());
                }
            }
            else {
                auto& q = iter->second;
                if (!asset.fractionable) {
                    fprintf(f, "%s,%f,%f,0.0,0.0,0.01,0.01,-100,1,1,0.000,%s,0,\"%s\"\n", asset.symbol.c_str(), q.ask_price, (q.ask_price - q.bid_price), asset.symbol.c_str(), asset.name.c_str());
                }
                else {
                    fprintf(f, "%s,%f,%f,0.0,0.0,0.01,0.01,-100,1,%f,0.000,%s,0,\"%s\"\n", asset.symbol.c_str(), q.ask_price, (q.ask_price - q.bid_price), s_config.fractionalLotAmount, asset.symbol.c_str(), asset.name.c_str());
                }
            }
            return true;
        };

        static LastQuotes empty_quotes;
        if (!symbols) {
            auto assets = client->getAssets();
            std::ostringstream oss;
            for (auto& asset : assets.content()) {
                if (!asset.tradable) {
                    continue;
                }
                oss << asset.symbol << ',';
            }

            auto last_quotes = pMarketData->getLastQuotes(oss.str());
            for (auto& asset : assets.content()) {
                if (!asset.tradable) {
                    continue;
                }
                try {
                    if (!getAsset(asset, last_quotes ? last_quotes.content() : empty_quotes)) {
                        break;
                    }
                }
                catch (...) {}
            }
        }
        else {
            auto last_quotes = pMarketData->getLastQuotes(symbols);
            const char* delim = ",";
            char* next_token;
            char* token = strtok_s(symbols, delim, &next_token);
            while (token != nullptr) {
                try {
                    std::string symbol = token;
                    trim(symbol);
                    auto response = client->getAsset(symbol);
                    if (response) {
                        if (!getAsset(response.content(), last_quotes ? last_quotes.content() : empty_quotes)) {
                            break;
                        }
                    }
                    else {
                        BrokerError(response.what().c_str());
                    }
                    token = strtok_s(nullptr, delim, &next_token);
                }
                catch (...) {}
            }
        }
        
        fflush(f);
        fclose(f);
        LOG_DEBUG("close file\n");
    }
    
    DLLFUNC_C double BrokerCommand(int Command, DWORD dwParameter)
    {
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
            return 10000;

        case GET_MAXREQUESTS:
            // Alpaca rate limit is 200 requests per minutes.
            // The rate is throttled by throtter
            return 0;

        case GET_LOCK:
            return 1;

        case GET_POSITION:
            return getPosition((char*)dwParameter);

        case SET_ORDERTEXT:
            s_nextOrderText = (char*)dwParameter;
            LOG_DEBUG("SET_ORDERTEXT: %s\n", s_nextOrderText.c_str());
            return dwParameter;

        case SET_SYMBOL:
            s_asset = (char*)dwParameter;
            LOG_DEBUG("SET_SYMBOL: %s\n", s_asset.c_str());
            return 1;

        case SET_MULTIPLIER:
            s_multiplier = (int)dwParameter;
            return 1;

        case SET_ORDERTYPE: {
           
            switch ((int)dwParameter) {
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

            if ((int)dwParameter >= 8) {
                return (int)dwParameter;
            }

            LOG_DEBUG("SET_ORDERTYPE: %d s_tif=%s\n", (int)dwParameter, to_string(s_tif));
            return tifToZorroOrderType(s_tif);
        }

        case GET_PRICETYPE:
            return s_priceType;

        case SET_PRICETYPE:
            s_priceType = (int)dwParameter;
            LOG_DEBUG("SET_PRICETYPE: %d\n", s_priceType);
            return dwParameter;

        case GET_VOLTYPE:
          return 0;

        case SET_AMOUNT:
            s_amount = *(double*)dwParameter;
            LOG_DIAG("SET_AMOUNT: %.8f\n", s_amount);
            break;

        case SET_DIAGNOSTICS:
            if ((int)dwParameter == 1 || (int)dwParameter == 0) {
                Logger::instance().setLevel((int)dwParameter ? LogLevel::L_TRACE : LogLevel::L_OFF);
                return dwParameter;
            }
            break;

        case SET_HWND:
        case GET_CALLBACK:
        case SET_CCY:
            break;

        case CREATE_ASSETLIST: {
            downloadAssets((char*)dwParameter);
            break;
        }

        case IS_ASSET_FRACTIONABLE: {
            auto asset_str = (char*)dwParameter;
            auto iter = s_mapAssets.find(asset_str);
            if (iter != s_mapAssets.end()) {
                return (int)iter->second.fractionable;
            }
            else {
                auto response = client->getAsset(asset_str);
                if (response) {
                    auto& asset = response.content();
                    s_mapAssets.emplace(asset_str, asset);
                    return (int)asset.fractionable;
                }
                BrokerError(response.what().c_str());
                return 0;
            }
            break;
        }

        case IS_ASSET_SHORTABLE: {
            auto asset_str = (char*)dwParameter;
            auto iter = s_mapAssets.find(asset_str);
            if (iter != s_mapAssets.end()) {
                return (int)iter->second.shortable;
            }
            else {
                auto response = client->getAsset(asset_str);
                if (response) {
                    auto& asset = response.content();
                    s_mapAssets.emplace(asset_str, asset);
                    return (int)asset.shortable;
                }
                BrokerError(response.what().c_str());
                return 0;
            }
            break;
        }

        case IS_ASSET_EASY_TO_BORROW: {
            auto asset_str = (char*)dwParameter;
            auto iter = s_mapAssets.find(asset_str);
            if (iter != s_mapAssets.end()) {
                return (int)iter->second.easy_to_borrow;
            }
            else {
                auto response = client->getAsset(asset_str);
                if (response) {
                    auto& asset = response.content();
                    s_mapAssets.emplace(asset_str, asset);
                    return (int)asset.easy_to_borrow;
                }
                BrokerError(response.what().c_str());
                return 0;
            }
            break;
        }

        case IS_ASSET_MARGINABLE: {
            auto asset_str = (char*)dwParameter;
            auto iter = s_mapAssets.find(asset_str);
            if (iter != s_mapAssets.end()) {
                return (int)iter->second.marginable;
            }
            else {
                auto response = client->getAsset(asset_str);
                if (response) {
                    auto& asset = response.content();
                    s_mapAssets.emplace(asset_str, asset);
                    return (int)asset.marginable;
                }
                BrokerError(response.what().c_str());
                return 0;
            }
            break;
        }

        case SET_LOG_TYPE: {
            BrokerError(("Set LogType to " + std::to_string((int)dwParameter)).c_str());
            Logger::instance().setLogType((LogType)dwParameter);
            break;
        }

        case SET_LOG_LEVEL: {
            BrokerError(("Set LogLevel to " + std::to_string((int)dwParameter)).c_str());
            Logger::instance().setLevel((LogLevel)dwParameter);
            break;
        }

        case SET_ADJUSTMENT: {
            s_config.adjustment = static_cast<Adjustment>((int)dwParameter);
            BrokerError(("Set Adjustment to " + std::string(to_string(s_config.adjustment))).c_str());
            break;
        }


        default:
            LOG_DEBUG("Unhandled command: %d %lu\n", Command, dwParameter);
            break;
        }
        return 0;
    }
}
