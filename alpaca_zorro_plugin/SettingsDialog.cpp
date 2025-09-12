#include "stdafx.h"
#include "SettingsDialog.h"
#include <resource.h>
#include "config.h"
#include "global.h"

namespace {
   alpaca::Config  &s_config = alpaca::Config::get();
   auto &global = zorro::Global::get();
}

void SettingsThreadProc()
{
    RegisterHotKey(nullptr, 1, 0, VK_F2);
    MSG msg;
    
    while(global.logged_in_.load(std::memory_order_relaxed))
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_HOTKEY && msg.wParam == 1 )
            {
                DialogBoxW(g_hModule, MAKEINTRESOURCE(IDD_SETTINGS_DIALOG), nullptr, SettingsDlgProc);
            }
        }
        else
        {
            std::this_thread::yield();
        }
    }
    UnregisterHotKey(nullptr, 1);
}

INT_PTR CALLBACK SettingsDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
    {
        // Get the dialog window's dimensions, move dialg to center screen
        RECT rcDlg;
        GetWindowRect(hDlg, &rcDlg);
        int dlgWidth = rcDlg.right - rcDlg.left;
        int dlgHeight = rcDlg.bottom - rcDlg.top;

        MONITORINFO mi = { sizeof(mi) };
        HMONITOR hMonitor = MonitorFromWindow(hDlg, MONITOR_DEFAULTTONEAREST);
        if (GetMonitorInfo(hMonitor, &mi))
        {
            int centerX = (mi.rcWork.left + mi.rcWork.right - dlgWidth) / 2;
            int centerY = (mi.rcWork.top + mi.rcWork.bottom - dlgHeight) / 2;
            SetWindowPos(hDlg, nullptr, centerX, centerY, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
            SetForegroundWindow(hDlg);
        }

        HWND hCombo = GetDlgItem(hDlg, IDC_LOG_LEVEL);
        std::wstring levels[] = { L"Off", L"Error", L"Warn", L"Info", L"Debug", L"Trace" };
        for (int i = 0; i < 6; ++i)
        {
            SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)levels[i].c_str());
        }
        SendMessageW(hCombo, CB_SETCURSEL, s_config.logLevel, 0);

        CheckDlgButton(hDlg, IDC_PASS_LOG_LEVEL_TO_WEBSOCKET_PROXY, s_config.passLogLevelToWebsocketProxy ? BST_CHECKED : BST_UNCHECKED);
        return (INT_PTR)TRUE;
    }
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
        {
            int sel = (int)SendMessageW(GetDlgItem(hDlg, IDC_LOG_LEVEL), CB_GETCURSEL, 0, 0);
            bool pass_log_level_to_websocket_proxy = (IsDlgButtonChecked(hDlg, IDC_PASS_LOG_LEVEL_TO_WEBSOCKET_PROXY) == BST_CHECKED);
            global.setting_update_.store(std::make_shared<zorro::SettingUpdate>(zorro::SettingUpdate{ (uint8_t)sel, pass_log_level_to_websocket_proxy }), std::memory_order_release);
            if (global.handle_)
            {
                PostMessage(global.handle_, WM_APP + 3, 0, (LPARAM)sel);
            }
            EndDialog(hDlg, IDOK);
            return (INT_PTR)TRUE;
        }
        case IDCANCEL:
            EndDialog(hDlg, IDCANCEL);
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
