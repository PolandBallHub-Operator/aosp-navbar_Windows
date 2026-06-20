#ifndef COMMON_H
#define COMMON_H

#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <commctrl.h>
#include <string>
#include <vector>

// 定数定義
#define ID_NAV_BACK   1001
#define ID_NAV_HOME   1002
#define ID_NAV_TASK   1003
#define ID_SETTINGS   1004

struct AppSettings {
    int navBarHeight = 48;
    int statusBarHeight = 24;
    bool leftHanded = false;
    int iconStyle = 0; // 0: Fill, 1: Outline, etc.
    int buttonPosition = 0; // 0: Center, 1: Left, 2: Right
};

extern AppSettings g_Settings;

#endif
