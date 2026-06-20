#include "../include/common.h"
#include "../include/navbar.h"
#include "../include/statusbar.h"
#include "../include/wm.h"

AppSettings g_Settings;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    StatusBar::Create(hInstance);
    NavBar::Create(hInstance);
    
    SetTimer(NULL, 1, 500, [](HWND, UINT, UINT_PTR, DWORD) {
        WM::MaximizeActiveWindow();
    });
    
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return (int)msg.wParam;
}
