// (c) 2016 ZaKlaus; All Rights Reserved

#if !defined(WIN_WINDOW_H)

typedef struct
{
    u32 X,Y;
} window_dim, window_pos, window_size;

typedef struct
{
    BITMAPINFO BitmapInfo;
    s32 Width;
    s32 Height;
    s32 BPP;
    void *Memory;
} window_resize_result, window_bitmap, window_backbuffer, window;

internal window_dim
WindowGetClientRect(HWND Window)
{
    RECT ClientRect;
    GetClientRect(Window, &ClientRect);
    u32 Width = ClientRect.right - ClientRect.left;
    u32 Height = ClientRect.bottom - ClientRect.top;
    
    window_dim Result = {0};
    Result.X = Width;
    Result.Y = Height;
    
    return(Result);
}

internal b32
WindowCreateClass(HINSTANCE Instance, LPCSTR ClassName, LRESULT (*WndProc)(HWND,UINT,WPARAM,LPARAM))
{
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = (WNDPROC)WndProc;
    wc.hInstance = Instance;
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.lpszClassName = ClassName;
    
    if(!RegisterClassEx(&wc))
    {
        return(0);
    }
    
    return(1);
}

internal b32
WindowCreateWindowed(LPCSTR ClassName, LPCSTR Title, HINSTANCE Instance, s32 Style, s32 ExStyle, window_dim Resolution, window_dim Position, int PositionSpecial, HWND *Window)
{
    DWORD WindowExStyle = ExStyle;
    DWORD WindowStyle = 
        CS_HREDRAW
        |CS_VREDRAW
        |CS_OWNDC
        |WS_OVERLAPPEDWINDOW
        |WS_VISIBLE
        |Style;
    
    if(PositionSpecial)
    {
        Position.X = PositionSpecial;
        Position.Y = PositionSpecial;
    }
    
    RECT r = {0};
    r.right = Resolution.X;
    r.bottom = Resolution.Y;
    AdjustWindowRectEx(&r, WindowStyle, 0, WindowExStyle);
    
    *Window = CreateWindowEx(WindowExStyle, ClassName, Title, WindowStyle,
                                  Position.X, Position.Y,
                                  r.right - r.left, r.bottom - r.top,
                                  0, 0,
                                  Instance, 0);
                                  
    
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

internal window_resize_result
WindowResize(s32 Width, s32 Height, window_bitmap WindowBitmap, b32 ResizeOnly)
{
    window_resize_result Result = WindowBitmap;
    
    if(ResizeOnly)
    {
        Result.Width = Width;
        Result.Height = Height;
        Result.BPP = 4;
        return(Result);
    }
    
    if(Result.Memory)
    {
        PlatformMemFree(Result.Memory);
    }
    
    
    BITMAPINFO BitmapInfo = {0};
    BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
    BitmapInfo.bmiHeader.biWidth = Width;
    BitmapInfo.bmiHeader.biHeight = Height;
    BitmapInfo.bmiHeader.biPlanes = 1;
    BitmapInfo.bmiHeader.biBitCount = 32;
    BitmapInfo.bmiHeader.biCompression = BI_RGB;
    
    s32 BytesPerPixel = 4;
    void *Memory = PlatformMemAlloc((Width * Height) * BytesPerPixel);
    
    Result.Memory = Memory;
    Result.BitmapInfo = BitmapInfo;
    
    return(Result);
}

internal void
WindowPaint(HDC DeviceContext, RECT *ClientRect, window_bitmap *WindowBitmap)
{
    BITMAPINFO *BitmapInfo = &WindowBitmap->BitmapInfo;
     void *Memory = WindowBitmap->Memory;
    s32 WindowWidth = ClientRect->right - ClientRect->left;
    s32 WindowHeight = ClientRect->bottom - ClientRect->top;
    StretchDIBits(DeviceContext,  
                  0, 0, WindowBitmap->Width, WindowBitmap->Height,
                  0, 0, WindowWidth, WindowHeight,
                  Memory,
                  BitmapInfo,
                  DIB_RGB_COLORS, SRCCOPY);
}

internal void
WindowBlit(HWND Window, window_bitmap *WindowBitmap)
{
    HDC DeviceContext = GetDC(Window);
    RECT ClientRect;
    GetClientRect(Window, &ClientRect);
    
    WindowPaint(DeviceContext, &ClientRect, WindowBitmap);
    ReleaseDC(Window, DeviceContext);
}

#define WIN_WINDOW_H
#endif