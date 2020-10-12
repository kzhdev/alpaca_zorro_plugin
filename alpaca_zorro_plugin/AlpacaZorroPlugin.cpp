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
#include <iomanip> // setprecision
#include <chrono>
#include <thread>
#include <algorithm> // transform

#include "alpaca/client.h"
#include "alpaca/logger.h"
#include "include/functions.h"

#define PLUGIN_VERSION	2

using namespace alpaca;

namespace {
    TimeInForce s_tif = TimeInForce::Day;
    std::string s_asset;
    int s_multiplier = 1;
    std::string s_lastUUID;
    Logger* s_logger = nullptr;
    std::string s_nextOrderText;
    int s_priceType = 0;
}

namespace alpaca
{
    std::unique_ptr<Client> client = nullptr;

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
        client = std::make_unique<Client>(User, Pwd, isPaperTrading);
        s_logger = &client->logger();

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
        auto response = client->getLastQuote(Asset);
        if (!response) {
            return 0;
        }

        if (!pPrice) {
            // this is subscribe
            return 1;
        }

        auto& lastQuote = response.content();

        if (pPrice) {
            *pPrice = lastQuote.quote.ask_price;
        }

        if (pSpread) {
            *pSpread = lastQuote.quote.ask_price - lastQuote.quote.bid_price;
        }

        if (pVolume) {
            auto barResponse = client->getBars({ Asset }, 0, 0, 1, 1);
            auto& bars = barResponse.content().bars;
            if (barResponse && !bars.empty() && !bars[Asset].empty()) {
                *pVolume = bars[Asset][0].volume;
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

        auto start = convertTime(tStart);
        auto end = convertTime(tEnd);

        s_logger->logInfo("BorkerHisotry %s start: %d end: %d nTickMinutes: %d nTicks: %d\n", Asset, start, end, nTickMinutes, nTicks);

        auto response = client->getBars({ Asset }, start, end, nTickMinutes, nTicks);
        if (!response) {
            BrokerError(response.what().c_str());
            return 0;
        }

        auto& bars = response.content().bars;
        auto iter = bars.find(Asset);
        if (iter == bars.end()) {
            return 0;
        }

        if (iter->second.empty()) {
            return 0;
        }
        
        int n = 0;
        for (int i = iter->second.size() - 1; i >= 0; --i) {
            auto& tick = ticks[n++];
            auto& bar = iter->second[i];
            tick.time = convertTime((__time32_t)bar.time);
            tick.fOpen = bar.open_price;
            tick.fHigh = bar.high_price;
            tick.fLow = bar.low_price;
            tick.fClose = bar.close_price;
            tick.fVol = bar.volume;
        }
        return n;
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
        auto response = client->submitOrder(Asset, std::abs(nAmount), side, type, s_tif, limit, stop, false, s_nextOrderText);
        if (!response) {
            BrokerError(response.what().c_str());
            return 0;
        }
        s_lastUUID = response.content().id;
        if (pPrice) {
            *pPrice = response.content().filled_avg_price;
        }
        if (pFill) {
            *pFill = response.content().filled_qty;
        }
        return -1;
    }

    DLLFUNC_C int BrokerTrade(int nTradeID, double* pOpen, double* pClose, double* pCost, double *pProfit) {
        assert(nTradeID == -1);
        auto response = client->getOrder(s_lastUUID);
        if (!response) {
            BrokerError(response.what().c_str());
            return NAY;
        }

        auto& order = response.content();

        if (pOpen) {
            *pOpen = order.filled_avg_price;
        }

        if (pProfit && order.filled_qty) {
            auto resp = client->getLastQuote(order.symbol);
            if (resp) {
                auto& quote = resp.content().quote;
                *pProfit = order.side == OrderSide::Buy ? ((quote.ask_price - order.filled_avg_price) * order.filled_qty) : (order.filled_avg_price - quote.bid_price) * order.filled_qty;
            }
        }
        return order.filled_qty;
    }

    double getPosition(const std::string& asset) {
        auto response = client->getPosition(asset);
        if (!response) {
            BrokerError(("Get position failed. " + response.what()).c_str());
            return 0;
        }

        return response.content().qty * (response.content().side == PositionSide::Long ? 1 : -1);
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
            return ET; // for now since Alpaca only support US

        case GET_MAXTICKS:
            return 1000;

        case GET_MAXREQUESTS:
            return 3;   // Alpaca rate limit is 200 requests per minutes

        case GET_LOCK:
            return 0;

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
                s_tif = TimeInForce::IOC;
                return 0;
            case 1:
                s_tif = TimeInForce::FOK;
                return 1;
            case 2:
                s_tif = TimeInForce::GTC;
                return 2;
            case 3:
                s_tif = TimeInForce::FOK;
                return 0;
            }

            if ((int)dwParameter >= 8) {
                return (int)dwParameter;
            }
            return 0;
        }
        case SET_PRICETYPE:
            return dwParameter;

        case GET_UUID:
            strcpy((char*)dwParameter, s_lastUUID.c_str());
            s_logger->logDebug("GET_UUID %s\n", (char*)(dwParameter));
            return dwParameter;

        case SET_UUID:
            s_logger->logDebug("SET_UUID %s\n", (char*)(dwParameter));
            return dwParameter;

        case SET_DIAGNOSTICS:
            if ((int)dwParameter == 1 || (int)dwParameter == 0) {
                client->logger().level = (int)dwParameter ? LogLevel::L_DEBUG : LogLevel::L_INFO;
                return 1;
            }
            break;

        case SET_HWND:
        case GET_CALLBACK:
            break;

        default:
            s_logger->logDebug("Unhandled command: %d %lu\n", Command, dwParameter);
            break;
        }
        return 0;
    }
}
