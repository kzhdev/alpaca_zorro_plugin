// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "SettingsDialog.h"

static HANDLE g_settingsThread = nullptr;
static DWORD g_settingsThreadId = 0;

BOOL APIENTRY DllMain(HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        g_hModule = hModule;
        g_settingsThread = CreateThread(nullptr, 0, SettingsThreadProc, nullptr, 0, &g_settingsThreadId);
        break;
    case DLL_PROCESS_DETACH:
        if (g_settingsThread)
        {
            PostThreadMessage(g_settingsThreadId, WM_QUIT, 0, 0);
            WaitForSingleObject(g_settingsThread, INFINITE);
            CloseHandle(g_settingsThread);
            g_settingsThread = nullptr;
        }
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    }
    return TRUE;
}

