#pragma once

#include <windows.h>
#include <atomic>

// extern DWORD g_mainThreadId;
extern HMODULE g_hModule;

INT_PTR CALLBACK SettingsDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void SettingsThreadProc();
