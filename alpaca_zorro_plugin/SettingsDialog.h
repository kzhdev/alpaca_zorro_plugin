#pragma once

#include <windows.h>

constexpr UINT WM_LOG_LEVEL_CHANGE = WM_APP + 2;

extern DWORD g_mainThreadId;
extern HMODULE g_hModule;

INT_PTR CALLBACK SettingsDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
DWORD WINAPI SettingsThreadProc(LPVOID);
