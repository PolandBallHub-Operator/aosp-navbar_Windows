#include "../include/statusbar.h"
#include "../include/wm.h"
#include <time.h>

namespace StatusBar {

HWND g_hWndStatus = NULL;
HFONT g_hFont = NULL;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE:
            g_hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
            return 0;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT rect;
            GetClientRect(hwnd, &rect);
            
            // 背景を黒に
            HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
            FillRect(hdc, &rect, hBrush);
            DeleteObject(hBrush);
            
            SelectObject(hdc, g_hFont);
            SetTextColor(hdc, RGB(255, 255, 255));
            SetBkMode(hdc, TRANSPARENT);
            
            // 左揃えで時計とWiFiを表示
            time_t now = time(0);
            struct tm tstruct = *localtime(&now);
            char timeBuf[10];
            strftime(timeBuf, sizeof(timeBuf), "%H:%M", &tstruct);
            
            // Android風に左側に配置
            TextOutA(hdc, 15, 4, timeBuf, strlen(timeBuf));
            
            // WiFiアイコンのシミュレーション（Material Symbolsが利用可能な場合はそれを使用）
            // ここでは簡易的にテキストで表示
            TextOutA(hdc, 60, 4, "WiFi", 4);
            
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_LBUTTONDOWN:
            WM::ShowQuickSettings();
            return 0;
        case WM_TIMER:
            InvalidateRect(hwnd, NULL, FALSE);
            return 0;
        case WM_DESTROY:
            if (g_hFont) DeleteObject(g_hFont);
            return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void Create(HINSTANCE hInstance) {
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, WndProc, 0, 0, hInstance, NULL, NULL, NULL, NULL, L"StatusBarClass", NULL };
    RegisterClassEx(&wc);
    
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    // 上部に配置、最前面
    g_hWndStatus = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE, L"StatusBarClass", NULL, WS_POPUP | WS_VISIBLE, 0, 0, screenWidth, 24, NULL, NULL, hInstance, NULL);
    
    SetTimer(g_hWndStatus, 1, 1000, NULL);
}

} // namespace StatusBar
