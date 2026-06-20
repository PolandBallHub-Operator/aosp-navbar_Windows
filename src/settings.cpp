#include "../include/settings.h"
#include "../include/common.h"
#include <commctrl.h>
#include <dwmapi.h>

#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif
#ifndef DWMWA_SYSTEMBACKDROP_TYPE
#define DWMWA_SYSTEMBACKDROP_TYPE 38
#endif

#pragma comment(lib, "dwmapi.lib")

namespace Settings {

HWND g_hEditNavHeight, g_hEditStatusHeight;
HWND g_hComboIconStyle, g_hComboBtnPos;
HFONT g_hModernFont = NULL;

void ApplyMica(HWND hwnd) {
    // Windows 11 Mica effect
    int value = 2; // DWMSBT_MAINWINDOW
    DwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE, &value, sizeof(value));
    
    // Dark mode for window frame
    BOOL dark = TRUE;
    DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &dark, sizeof(dark));
}

LRESULT CALLBACK SettingsWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            ApplyMica(hwnd);
            g_hModernFont = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI Variable Display");

            HWND hLabel1 = CreateWindow(L"STATIC", L"Navigation Bar Height", WS_CHILD | WS_VISIBLE, 20, 20, 200, 25, hwnd, NULL, NULL, NULL);
            g_hEditNavHeight = CreateWindow(L"EDIT", L"48", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, 230, 20, 60, 25, hwnd, NULL, NULL, NULL);
            
            HWND hLabel2 = CreateWindow(L"STATIC", L"Status Bar Height", WS_CHILD | WS_VISIBLE, 20, 60, 200, 25, hwnd, NULL, NULL, NULL);
            g_hEditStatusHeight = CreateWindow(L"EDIT", L"24", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, 230, 60, 60, 25, hwnd, NULL, NULL, NULL);
            
            HWND hLabel3 = CreateWindow(L"STATIC", L"Icon Style", WS_CHILD | WS_VISIBLE, 20, 100, 200, 25, hwnd, NULL, NULL, NULL);
            g_hComboIconStyle = CreateWindow(L"COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, 20, 130, 270, 150, hwnd, NULL, NULL, NULL);
            SendMessage(g_hComboIconStyle, CB_ADDSTRING, 0, (LPARAM)L"Fluent Fill");
            SendMessage(g_hComboIconStyle, CB_ADDSTRING, 0, (LPARAM)L"Fluent Outline");
            SendMessage(g_hComboIconStyle, CB_SETCURSEL, 0, 0);
            
            HWND hLabel4 = CreateWindow(L"STATIC", L"Button Alignment", WS_CHILD | WS_VISIBLE, 20, 170, 200, 25, hwnd, NULL, NULL, NULL);
            g_hComboBtnPos = CreateWindow(L"COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, 20, 200, 270, 150, hwnd, NULL, NULL, NULL);
            SendMessage(g_hComboBtnPos, CB_ADDSTRING, 0, (LPARAM)L"Center (Default)");
            SendMessage(g_hComboBtnPos, CB_ADDSTRING, 0, (LPARAM)L"Left Aligned");
            SendMessage(g_hComboBtnPos, CB_ADDSTRING, 0, (LPARAM)L"Right Aligned");
            SendMessage(g_hComboBtnPos, CB_SETCURSEL, 0, 0);
            
            HWND hBtn = CreateWindow(L"BUTTON", L"Save Changes", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 20, 260, 270, 40, hwnd, (HMENU)101, NULL, NULL);
            
            SendMessage(hLabel1, WM_SETFONT, (WPARAM)g_hModernFont, TRUE);
            SendMessage(hLabel2, WM_SETFONT, (WPARAM)g_hModernFont, TRUE);
            SendMessage(hLabel3, WM_SETFONT, (WPARAM)g_hModernFont, TRUE);
            SendMessage(hLabel4, WM_SETFONT, (WPARAM)g_hModernFont, TRUE);
            SendMessage(g_hEditNavHeight, WM_SETFONT, (WPARAM)g_hModernFont, TRUE);
            SendMessage(g_hEditStatusHeight, WM_SETFONT, (WPARAM)g_hModernFont, TRUE);
            SendMessage(g_hComboIconStyle, WM_SETFONT, (WPARAM)g_hModernFont, TRUE);
            SendMessage(g_hComboBtnPos, WM_SETFONT, (WPARAM)g_hModernFont, TRUE);
            SendMessage(hBtn, WM_SETFONT, (WPARAM)g_hModernFont, TRUE);
            
            return 0;
        }
        case WM_CTLCOLORSTATIC: {
            HDC hdc = (HDC)wParam;
            SetTextColor(hdc, RGB(255, 255, 255));
            SetBkMode(hdc, TRANSPARENT);
            return (LRESULT)GetStockObject(NULL_BRUSH);
        }
        case WM_COMMAND:
            if (LOWORD(wParam) == 101) {
                // 設定の読み取りと適用ロジック
                wchar_t buf[10];
                GetWindowText(g_hEditNavHeight, buf, 10);
                g_Settings.navBarHeight = _wtoi(buf);
                
                GetWindowText(g_hEditStatusHeight, buf, 10);
                g_Settings.statusBarHeight = _wtoi(buf);
                
                g_Settings.iconStyle = SendMessage(g_hComboIconStyle, CB_GETCURSEL, 0, 0);
                g_Settings.buttonPosition = SendMessage(g_hComboBtnPos, CB_GETCURSEL, 0, 0);
                
                MessageBox(hwnd, L"Settings Applied! (Requires restart for some changes)", L"Success", MB_OK);
                DestroyWindow(hwnd);
            }
            return 0;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void Show(HINSTANCE hInstance) {
    static bool registered = false;
    if (!registered) {
        WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, SettingsWndProc, 0, 0, hInstance, NULL, NULL, NULL, NULL, L"SettingsClass", NULL };
        RegisterClassEx(&wc);
        registered = true;
    }
    
    HWND hWndSettings = CreateWindowEx(0, L"SettingsClass", L"Android NavBar Settings", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 300, 250, NULL, NULL, hInstance, NULL);
}

} // namespace Settings
