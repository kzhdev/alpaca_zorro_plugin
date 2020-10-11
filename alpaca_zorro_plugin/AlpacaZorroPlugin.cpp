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
#include "include/functions.h"

#define PLUGIN_VERSION	2

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
        // TODO
        return 0;
    }

    DLLFUNC_C double BrokerCommand(int Command, DWORD dwParameter)
    {
        static int SetMultiplier;
        static std::string SetSymbol;

        std::string Data, response;
        int i = 0;
        double price = 0., spread = 0.;

        switch (Command)
        {
        case GET_COMPLIANCE:
            return 15; // full NFA compliant

        case GET_BROKERZONE:
            return EST; // for now since Alpaca only support US

        case GET_MAXTICKS:
            return 1000;

        case GET_MAXREQUESTS:
            return 3;   // Alpaca rate limit is 200 requests per minutes

        case GET_POSITION: {
            break;
        }

        case SET_SYMBOL:
            SetSymbol = (char*)dwParameter;
            return 1;

        case SET_MULTIPLIER:
            SetMultiplier = (int)dwParameter;
            return 1;

        case SET_DIAGNOSTICS:
            if ((int)dwParameter == 1 || (int)dwParameter == 0) {
                client->set_diagnostic(dwParameter);
                return 1;
            }

        default:
            BrokerError((std::to_string(Command) + " " + std::to_string(dwParameter)).c_str());
            break;
        }
        return 0;
    }
}
