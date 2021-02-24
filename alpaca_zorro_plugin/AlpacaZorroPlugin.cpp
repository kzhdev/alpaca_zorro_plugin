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

#include "alpaca/client.h"
#include "logger.h"
#include "include/functions.h"
#include "market_data/alpaca_market_data.h"

#define PLUGIN_VERSION	2

using namespace alpaca;

namespace {
    TimeInForce s_tif = TimeInForce::FOK;
    std::string s_asset;
    int s_multiplier = 1;
    Logger* s_logger = nullptr;
    std::string s_nextOrderText;
    int s_priceType = 0;
    std::unordered_map<uint32_t, Order> s_mapOrderByClientOrderId;
}

namespace alpaca
{
    std::unique_ptr<Client> client = nullptr;
    std::unique_ptr<AlpacaMarketData> alpacaMD = nullptr;
    MarketData* pMarketData = nullptr;

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
        return;
    }

    DLLFUNC_C int BrokerLogin(char* User, char* Pwd, char* Type, char* Account)
    {
        if (!User) // log out
        {
            return 0;
        }

        bool isPaperTrading = strcmp(Type, "Demo") == 0;

        std::string apiKey(User);
        //auto pos = apiKey.find('_');
        //if (pos != std::string::npos) {
        //    polygonApiKey = apiKey.substr(pos + 1);
        //    apiKey = apiKey.substr(0, pos);
        //}

        client = std::make_unique<Client>(apiKey, Pwd, isPaperTrading);
        s_logger = &client->logger();

        //if (!polygonApiKey.empty()) {
        //    polygon = std::move(std::make_unique<Polygon>(polygonApiKey, client->logger()));
        //    pMarketData = polygon.get();
        //    BrokerError("Use Polygon market data");
        //    s_logger->logInfo("Use Polygon market data\n");
        //} 
        //else {
            alpacaMD = std::move(std::make_unique<AlpacaMarketData>(client->headers(), client->logger()));
            pMarketData = alpacaMD.get();
            BrokerError("Use Alpaca market data");
            s_logger->logInfo("Use Alpaca market data\n");
        //}

        //attempt login
        auto response = client->getAccount();
        if (!response) {
            BrokerError("Login failed.");
            BrokerError(response.what().c_str());
            return 0;
        }

        auto& account = response.content().account_number;
        BrokerError(("Account " + account).c_str());
        sprintf_s(Account, 1024, account.c_str());
        return 1;
    }

    DATE convertTime(__time32_t t32)
    {
        return (DATE)t32 / (24. * 60. * 60.) + 25569.; // 25569. = DATE(1.1.1970 00:00)
    }

    __time32_t convertTime(DATE date)
    {
        return (__time32_t)((date - 25569.) * 24. * 60. * 60.);
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
        if (!pPrice) {
            // this is subscribe
            return 1;
        }

        if (s_priceType == 2) {
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

            if (pSpread) {
                *pSpread = 0;
            }
        }
        else {
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

        if (pLotAmount) {
            *pLotAmount = 1;
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

        s_logger->logDebug("BorkerHisotry %s start: %d end: %d nTickMinutes: %d nTicks: %d\n", Asset, start, end, nTickMinutes, nTicks);

        int barsDownloaded = 0;

        auto response = pMarketData->getBars({ Asset }, start, end, nTickMinutes, nTicks);
        if (!response) {
            BrokerError(response.what().c_str());
            return barsDownloaded;
        }

        auto& bars = response.content();
        if (bars.empty()) {
            return barsDownloaded;
        }

        s_logger->logDebug("%d bar downloaded\n", bars.size());

        for (int i = bars.size() - 1; i >= 0; --i) {
            auto& bar = bars[i];
            // change time to bar close time
            __time32_t barCloseTime = bar.time + nTickMinutes * 60;
            if (barCloseTime > end) {
                // end time cannot exceeds tEnd
                continue;
            }
            auto& tick = ticks[barsDownloaded++];
            tick.time = convertTime(barCloseTime);
            tick.fOpen = bar.open_price;
            tick.fHigh = bar.high_price;
            tick.fLow = bar.low_price;
            tick.fClose = bar.close_price;
            tick.fVol = bar.volume;
        }
        s_logger->logDebug("%d bar returned\n", bars.size());
        return barsDownloaded;
    }

    DLLFUNC_C int BrokerAccount(char* Account, double* pdBalance, double* pdTradeVal, double* pdMarginVal)
    {
        auto response = client->getAccount();
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
        if (dLimit) {
            type = OrderType::Limit;
        }
        std::string limit;
        if (dLimit) {
            limit = std::to_string(dLimit);
        }
        std::string stop;
        if (dStopDist) {

        }

        s_logger->logDebug("BrokerBuy2 %s orderText=%s nAmount=%d dStopDist=%f limit=%f\n", Asset, s_nextOrderText.c_str(), nAmount, dStopDist, dLimit);

        auto response = client->submitOrder(Asset, std::abs(nAmount), side, type, s_tif, limit, stop, false, s_nextOrderText);
        if (!response) {
            BrokerError(response.what().c_str());
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
                *pFill = response.content().filled_qty;
            }
            //return -1;
            return internalOrdId;
        }

        if (s_tif == TimeInForce::IOC || s_tif == TimeInForce::FOK) {
            // order not filled in the submitOrder response
            // query order status to get fill status
            do {
                auto response2 = client->getOrder(exchOrdId, false, true);
                if (!response2) {
                    break;
                }
                order = &response2.content();
                s_mapOrderByClientOrderId[internalOrdId] = *order;
                if (pPrice) {
                    *pPrice = order->filled_avg_price;
                }
                if (pFill) {
                    *pFill = order->filled_qty;
                }

                if (order->status == "canceled" ||
                    order->status == "filled" ||
                    order->status == "expired") {
                    break;
                }

                auto timePast = std::difftime(std::time(nullptr), start);
                if (timePast >= 30) {
                    auto response3 = client->cancelOrder(exchOrdId);
                    if (!response3) {
                        BrokerError(("Failed to cancel unfilled FOK/IOC order " + exchOrdId + " " + response3.what()).c_str());
                    }
                    return 0;
                }
            } while (!order->filled_qty);
        }
        //return -1;
        return internalOrdId;
    }

    DLLFUNC_C int BrokerTrade(int nTradeID, double* pOpen, double* pClose, double* pCost, double *pProfit) {
        s_logger->logInfo("BrokerTrade: %d\n", nTradeID);
       /* if (nTradeID != -1) {
            BrokerError(("nTradeID " + std::to_string(nTradeID) + " not valid. Need to be an UUID").c_str());
            return NAY;
        }*/
        
        Response<Order> response;
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
            s_mapOrderByClientOrderId.insert(std::make_pair(nTradeID, response.content()));
        }
        else {
            response = client->getOrder(iter->second.id);
            if (!response) {
                BrokerError(response.what().c_str());
                return NAY;
            }
        }

        auto& order = response.content();
        s_mapOrderByClientOrderId[nTradeID] = order;

        if (pOpen) {
            *pOpen = order.filled_avg_price;
        }

        if (pProfit && order.filled_qty) {
            auto resp = pMarketData->getLastQuote(order.symbol);
            if (resp) {
                auto& quote = resp.content().quote;
                *pProfit = order.side == OrderSide::Buy ? ((quote.ask_price - order.filled_avg_price) * order.filled_qty) : (order.filled_avg_price - quote.bid_price) * order.filled_qty;
            }
        }
        return order.filled_qty;
    }

    DLLFUNC_C int BrokerSell2(int nTradeID, int nAmount, double Limit, double* pClose, double* pCost, double* pProfit, int* pFill) {
        s_logger->logDebug("BrokerSell2 nTradeID=%d nAmount=%d limit=%f\n", nTradeID,nAmount, Limit);

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
                        *pFill = closeTrade.filled_qty;
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
            BrokerError(("Close working order " + std::to_string(nTradeID)).c_str());
            if (std::abs(nAmount) == order.qty) {
                auto response = client->cancelOrder(iter->second.id);
                if (response) {
                    return nTradeID;
                }
                BrokerError(("Failed to close trade " + std::to_string(nTradeID) + " " + response.what()).c_str());
                return 0;
            }
            else {
                auto response = client->replaceOrder(order.id, iter->second.qty - nAmount, order.tif, (Limit ? std::to_string(Limit) : ""), "", iter->second.client_order_id);
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
        if (fopen_s(&f, "./Log/AssetsAlpaca.csv", "w+")) {
            s_logger->logError("Failed to open ./Log/AssetsAlpaca file\n");
            return;
        }

        BrokerError("Generating Asset List...");
        fprintf(f, "Name,Price,Spread,RollLong,RollShort,PIP,PIPCost,MarginCost,Leverage,LotAmount,Commission\n");

        auto getAsset = [f](const std::string& asset) {
            BrokerError(("Asset " + asset).c_str());
            BrokerProgress(1);
            auto quote = pMarketData->getLastQuote(asset);
            if (quote) {
                auto& q = quote.content().quote;
                fprintf(f, "%s,%f,%f,0.0,0.0,0.01,0.01,0.0,1,1,0.000,%s\n", asset.c_str(), q.ask_price, (q.ask_price - q.bid_price), asset.c_str());
            }
            else {
                BrokerError(quote.what().c_str());
            }
        };

        if (!symbols) {
            auto assets = client->getAssets();
            for (auto& asset : assets.content()) {
                if (!asset.tradable) {
                    continue;
                }
                getAsset(asset.symbol);
            }
        }
        else {
            const char* delim = ",";
            char* next_token;
            char* token = strtok_s(symbols, delim, &next_token);
            while (token != nullptr) {
                getAsset(token);
                token = strtok_s(nullptr, delim, &next_token);
            }
        }
        
        fflush(f);
        fclose(f);
        s_logger->logDebug("close file\n");
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

        //case GET_BROKERZONE:
            //return ET; // for now since Alpaca only support US

        case GET_MAXTICKS:
            return 1000;

        case GET_MAXREQUESTS:
            return 3;   // Alpaca rate limit is 200 requests per minutes

        case GET_LOCK:
            return -1;

        case GET_POSITION:
            return getPosition((char*)dwParameter);

        case SET_ORDERTEXT:
            s_nextOrderText = (char*)dwParameter;
            client->logger().logDebug("SET_ORDERTEXT: %s\n", s_nextOrderText.c_str());
            return dwParameter;

        case SET_SYMBOL:
            s_asset = (char*)dwParameter;
            return 1;

        case SET_MULTIPLIER:
            s_multiplier = (int)dwParameter;
            return 1;

        case SET_ORDERTYPE: {
           
            switch ((int)dwParameter) {
            case 0:
                return 0;
            case 1:
                s_tif = TimeInForce::IOC;
                break;
            case 2:
                s_tif = TimeInForce::GTC;
                break;
            case 3:
                s_tif = TimeInForce::FOK;
                break;
            case 4:
                s_tif = TimeInForce::Day;
                break;
            case 5:
                s_tif = TimeInForce::OPG;
                break;
            case 6:
                s_tif = TimeInForce::CLS;
                break;
            }

            if ((int)dwParameter >= 8) {
                return (int)dwParameter;
            }

            s_logger->logDebug("SET_ORDERTYPE: %d s_tif=%s\n", (int)dwParameter, to_string(s_tif));
            return tifToZorroOrderType(s_tif);
        }

        case GET_PRICETYPE:
            return s_priceType;

        case SET_PRICETYPE:
            s_priceType = (int)dwParameter;
            s_logger->logDebug("SET_PRICETYPE: %d\n", s_priceType);
            return dwParameter;

        case GET_VOLTYPE:
          return 0;

        case SET_DIAGNOSTICS:
            if ((int)dwParameter == 1 || (int)dwParameter == 0) {
                client->logger().setLevel((int)dwParameter ? LogLevel::L_DEBUG : LogLevel::L_OFF);
                return dwParameter;
            }
            break;

        case GET_BROKERZONE:
        case SET_HWND:
        case GET_CALLBACK:
            break;

        case 2001: {
            downloadAssets((char*)dwParameter);
            break;
        }
            

        default:
            s_logger->logDebug("Unhandled command: %d %lu\n", Command, dwParameter);
            break;
        }
        return 0;
    }
}
