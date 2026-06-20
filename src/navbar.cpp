#include "../include/navbar.h"
#include "../include/wm.h"
#include "../include/settings.h"

namespace NavBar {

HWND g_hWndNav = NULL;
WNDPROC g_OldBtnProc = NULL;

LRESULT CALLBACK BtnSubclassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static DWORD dwClickTime = 0;
    switch (msg) {
        case WM_LBUTTONDOWN:
            dwClickTime = GetTickCount();
            SetTimer(hwnd, 2, 500, NULL);
            break;
        case WM_LBUTTONUP:
            KillTimer(hwnd, 2);
            if (GetTickCount() - dwClickTime < 500) {
                SendMessage(GetParent(hwnd), WM_COMMAND, GetWindowLong(hwnd, GWL_ID), (LPARAM)hwnd);
            }
            break;
        case WM_TIMER:
            if (wParam == 2) {
                KillTimer(hwnd, 2);
                Settings::Show((HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE));
            }
            break;
    }
    return CallWindowProc(g_OldBtnProc, hwnd, msg, wParam, lParam);
}

void DrawAndroidButton(LPDRAWITEMSTRUCT lpDrawItem) {
    HDC hdc = lpDrawItem->hDC;
    RECT rect = lpDrawItem->rcItem;
    
    // 背景（黒）
    HBRUSH hBg = CreateSolidBrush(RGB(0, 0, 0));
    FillRect(hdc, &rect, hBg);
    DeleteObject(hBg);
    
    // アイコン色（白）
    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
    SelectObject(hdc, hPen);
    SelectObject(hdc, hBrush);
    
    int cx = (rect.left + rect.right) / 2;
    int cy = (rect.top + rect.bottom) / 2;
    
    switch (lpDrawItem->CtlID) {
        case ID_NAV_BACK: {
            POINT pts[3] = { {cx+8, cy-10}, {cx-10, cy}, {cx+8, cy+10} };
            Polygon(hdc, pts, 3);
            break;
        }
        case ID_NAV_HOME: {
            Ellipse(hdc, cx-10, cy-10, cx+10, cy+10);
            if (g_Settings.iconStyle == 0) { // Fill
                // そのまま
            } else {
                // Outlineの場合は中を抜く
                HBRUSH hBlack = CreateSolidBrush(RGB(0, 0, 0));
                SelectObject(hdc, hBlack);
                Ellipse(hdc, cx-7, cy-7, cx+7, cy+7);
                DeleteObject(hBlack);
            }
            break;
        }
        case ID_NAV_TASK: {
            Rectangle(hdc, cx-9, cy-9, cx+9, cy+9);
            break;
        }
    }
    
    DeleteObject(hPen);
    DeleteObject(hBrush);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT rect;
            GetClientRect(hwnd, &rect);
            HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
            FillRect(hdc, &rect, hBrush);
            DeleteObject(hBrush);
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_DRAWITEM:
            DrawAndroidButton((LPDRAWITEMSTRUCT)lParam);
            return TRUE;
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case ID_NAV_BACK: WM::GoBack(); break;
                case ID_NAV_HOME: WM::CloseAllWindows(); break;
                case ID_NAV_TASK: WM::ShowTaskView(); break;
            }
            return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void Create(HINSTANCE hInstance) {
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, WndProc, 0, 0, hInstance, NULL, NULL, NULL, NULL, L"NavBarClass", NULL };
    RegisterClassEx(&wc);
    
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int height = g_Settings.navBarHeight;
    
    g_hWndNav = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE, L"NavBarClass", NULL, WS_POPUP | WS_VISIBLE, 0, screenHeight - height, screenWidth, height, NULL, NULL, hInstance, NULL);
    
    int btnWidth = 80;
    int spacing;
    int startX;
    
    if (g_Settings.buttonPosition == 1) { // Left
        startX = 20;
        spacing = 10;
    } else if (g_Settings.buttonPosition == 2) { // Right
        startX = screenWidth - (btnWidth * 3) - 40;
        spacing = 10;
    } else { // Center
        spacing = (screenWidth - (btnWidth * 3)) / 4;
        startX = spacing;
    }
    
    HWND hBack = CreateWindow(L"BUTTON", NULL, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, startX, 4, btnWidth, height-8, g_hWndNav, (HMENU)ID_NAV_BACK, hInstance, NULL);
    HWND hHome = CreateWindow(L"BUTTON", NULL, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, startX + btnWidth + spacing, 4, btnWidth, height-8, g_hWndNav, (HMENU)ID_NAV_HOME, hInstance, NULL);
    HWND hTask = CreateWindow(L"BUTTON", NULL, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, startX + (btnWidth + spacing) * 2, 4, btnWidth, height-8, g_hWndNav, (HMENU)ID_NAV_TASK, hInstance, NULL);
    
    g_OldBtnProc = (WNDPROC)SetWindowLongPtr(hHome, GWLP_WNDPROC, (LONG_PTR)BtnSubclassProc);
    SetWindowLongPtr(hBack, GWLP_WNDPROC, (LONG_PTR)BtnSubclassProc);
    SetWindowLongPtr(hTask, GWLP_WNDPROC, (LONG_PTR)BtnSubclassProc);
}

} // namespace NavBar
