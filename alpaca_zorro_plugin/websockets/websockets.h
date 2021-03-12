#pragma once

#include <Windows.h>

#ifdef ALPACA_EXPORTS 
#ifndef DELLFUNC
#define DLLFUNC extern __declspec(dllexport)
#define DLLFUNC_C extern "C" __declspec(dllexport)
#else  
#define DLLFUNC extern __declspec(dllimport)
#define DLLFUNC_C extern "C" __declspec(dllimport)
#endif // !DELLFUNC
#endif  

namespace alpaca {
    DLLFUNC_C void onWebsocketConnected();
    DLLFUNC_C void onWebsocketDisconnected();
    DLLFUNC_C void onWebsocketError(const char* msg, int len);
    DLLFUNC_C void onWebsocketData(const char* msg, int len, int remainging);
}
