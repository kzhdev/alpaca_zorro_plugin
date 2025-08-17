#include "stdafx.h"
#include "SettingsDialog.h"
#include "resource.h"
#include "config.h"
#include "global.h"

namespace {
    Config &s_config = Config::get();
}

DWORD WINAPI SettingsThreadProc(LPVOID)
{
    RegisterHotKey(nullptr, 1, 0, VK_F2);
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (msg.message == WM_HOTKEY && msg.wParam == 1)
        {
            DialogBox(g_hModule, MAKEINTRESOURCE(IDD_SETTINGS_DIALOG), nullptr, SettingsDlgProc);
        }
    }
    UnregisterHotKey(nullptr, 1);
    return 0;
}

INT_PTR CALLBACK SettingsDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
    {
        HWND hCombo = GetDlgItem(hDlg, IDC_LOG_LEVEL);
        const char* levels[] = { "Off", "Error", "Warn", "Info", "Debug", "Trace" };
        for (int i = 0; i < 6; ++i)
        {
            SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)levels[i]);
        }
        SendMessage(hCombo, CB_SETCURSEL, s_config.logLevel, 0);
        return (INT_PTR)TRUE;
    }
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
        {
            int sel = (int)SendMessage(GetDlgItem(hDlg, IDC_LOG_LEVEL), CB_GETCURSEL, 0, 0);
            PostThreadMessage(g_mainThreadId, WM_LOG_LEVEL_CHANGE, sel, 0);
            auto &global = zorro::Global::get();
            if (global.handle_)
            {
                PostMessage(global.handle_, WM_APP + 3, 0, 0);
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
