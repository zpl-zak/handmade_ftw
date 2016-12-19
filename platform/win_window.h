// (c) 2016 ZaKlaus; All Rights Reserved

#if !defined(WIN_WINDOW_H)

typedef struct
{
    u32 X,Y;
} window_dim;

global_variable HINSTANCE GlobalInstance;
global_variable HWND GlobalWindow;

internal b32
WindowCreateClass(HINSTANCE Instance, LRESULT (*WndProc)(HWND,UINT,WPARAM,LPARAM))
{
    GlobalInstance = Instance;
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = Instance;
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.lpszClassName = "Handmade FTW";
    
    if(!RegisterClassEx(&wc))
    {
        return(0);
    }
    
    return(1);
}

internal b32
WindowCreateWindowed(LPCSTR Title, s32 Style, s32 ExStyle, window_dim Resolution, window_dim Position, int PositionSpecial)
{
    DWORD WindowExStyle = ExStyle;
    DWORD WindowStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE | Style;
    
    if(PositionSpecial)
    {
        Position.X = PositionSpecial;
        Position.Y = PositionSpecial;
    }
    
    RECT r = {0};
    r.right = Resolution.X;
    r.bottom = Resolution.Y;
    AdjustWindowRectEx(&r, WindowStyle, 0, WindowExStyle);
    
    GlobalWindow = CreateWindowEx(WindowExStyle, "Handmade FTW", Title, WindowStyle,
                                  Position.X, Position.Y,
                                  r.right - r.left, r.bottom - r.top,
                                  0, 0,
                                  GlobalInstance, 0);
                                  
    
    return(1);
}

internal void
WindowUpdate(void)
{
    MSG msg;
    
    while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

#define WIN_WINDOW_H
#endif