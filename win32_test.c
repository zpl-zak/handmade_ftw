// (c) 2016 ZaKlaus; All Rights Reserved

#define HANDMADE_SLOW

#include "hftw.h"

#include "formats/hformat_4ds.h"
#include "formats/hformat_klz.h"

global_variable b32 IsRunning;

LRESULT CALLBACK
WndProc(HWND hwnd,
        UINT uMsg,
        WPARAM wParam,
        LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_CLOSE:
        {
            IsRunning = 0;
        }break;
        
        default:
        {
            
        }break;
    }
    return(DefWindowProc(hwnd, uMsg, wParam, lParam));
}


int CALLBACK
WinMain(HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR lpCmdLine,
        int nCmdShow)
{
    WindowCreateClass(hInstance, &WndProc);
    
    window_dim PosDim = {0};
    window_dim ResDim = {0};
    ResDim.X = 800;
    ResDim.Y = 600;
    
    WindowCreateWindowed("Win32 Test", 0, 0, ResDim, PosDim, CW_USEDEFAULT);
    
    WindowUpdate();
    TimeInit();
    
    IsRunning = 1;
    r64 OldTime = TimeGet();
    
    while(IsRunning)
    {
        r64 NewTime = TimeGet();
        {
            WindowUpdate();
            Sleep(10);
        }
        OldTime = NewTime;
    }
    
    return(0);
}

void WinMainCRTStartup(void)
{
    int Result = WinMain(GetModuleHandle(0), 0, 0, 0);
    ExitProcess(Result);
}