#include "../include/wm.h"
#include <vector>
#include <algorithm>

namespace WM {

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    if (IsWindowVisible(hwnd) && GetWindowTextLength(hwnd) > 0) {
        DWORD styles = GetWindowLong(hwnd, GWL_STYLE);
        if ((styles & WS_OVERLAPPEDWINDOW) == WS_OVERLAPPEDWINDOW) {
            std::vector<HWND>* windows = reinterpret_cast<std::vector<HWND>*>(lParam);
            windows->push_back(hwnd);
        }
    }
    return TRUE;
}

void CloseAllWindows() {
    std::vector<HWND> windows;
    EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&windows));
    for (HWND hwnd : windows) {
        // 最小化することでAndroidの「ホームに戻る」挙動を再現
        ShowWindow(hwnd, SW_MINIMIZE);
    }
}

void MaximizeActiveWindow() {
    HWND hwnd = GetForegroundWindow();
    if (hwnd) {
        char className[256];
        GetClassNameA(hwnd, className, sizeof(className));
        // シェルコンポーネントや自アプリのウィンドウは除外
        if (strcmp(className, "Progman") != 0 && 
            strcmp(className, "WorkerW") != 0 && 
            strcmp(className, "Shell_TrayWnd") != 0 &&
            strcmp(className, "NavBarClass") != 0 &&
            strcmp(className, "StatusBarClass") != 0) {
            
            WINDOWPLACEMENT wp;
            wp.length = sizeof(WINDOWPLACEMENT);
            GetWindowPlacement(hwnd, &wp);
            if (wp.showCmd != SW_MAXIMIZE) {
                ShowWindow(hwnd, SW_MAXIMIZE);
            }
        }
    }
}

void ShowTaskView() {
    // Windows 10/11 Task View (Win+Tab)
    // より確実に動作させるため、入力シミュレーションを整理
    INPUT inputs[4] = {};
    
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = VK_LWIN;
    
    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = VK_TAB;
    
    inputs[2].type = INPUT_KEYBOARD;
    inputs[2].ki.wVk = VK_TAB;
    inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;
    
    inputs[3].type = INPUT_KEYBOARD;
    inputs[3].ki.wVk = VK_LWIN;
    inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;
    
    SendInput(4, inputs, sizeof(INPUT));
}

void ShowQuickSettings() {
    // Windows 11 Quick Settings (Win+A)
    INPUT inputs[4] = {};
    
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = VK_LWIN;
    
    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = 'A';
    
    inputs[2].type = INPUT_KEYBOARD;
    inputs[2].ki.wVk = 'A';
    inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;
    
    inputs[3].type = INPUT_KEYBOARD;
    inputs[3].ki.wVk = VK_LWIN;
    inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;
    
    SendInput(4, inputs, sizeof(INPUT));
}

void GoBack() {
    // Androidの戻るボタンに近い挙動として、ブラウザの戻るやEscapeを送信
    // 多くのWindowsアプリではAlt+LeftまたはEscが戻るに対応
    keybd_event(VK_MENU, 0, 0, 0);
    keybd_event(VK_LEFT, 0, 0, 0);
    keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);
}

} // namespace WM
