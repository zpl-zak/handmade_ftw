// (c) 2016 ZaKlaus; All Rights Reserved

#define HANDMADE_SLOW

#include "hftw.h"

#include "formats/hformat_4ds.h"
#include "formats/hformat_klz.h"
#include "formats/hformat_bmp.h"

global_variable b32 Running = 1;
global_variable window_bitmap WindowBitmap;

internal void
RenderGradient(s32 XOffset, s32 YOffset)
{
    s32 Width = WindowBitmap.Width;
    s32 Height = WindowBitmap.Height;
    s32 BytesPerPixel = WindowBitmap.BPP;
    s32 Pitch = Width * BytesPerPixel;
    u8 *Row = (u8 *)WindowBitmap.BitmapMemory;
    for(s32 Y = 0;
        Y < Height;
        ++Y)
    {
        u32 *Pixel = (u32 *)Row;
        for(s32 X = 0;
            X < Width;
            ++X)
        {
            u8 Blue = (u8)(X + XOffset);
            u8 Green = (u8)(Y + YOffset);
            *Pixel++ = ((Green << 8) | Blue);
            }
        Row += Pitch;
    }
}

LRESULT CALLBACK
WndProc(HWND Window,
        UINT Message,
        WPARAM WParam,
        LPARAM LParam)
{
    switch(Message)
    {
        case WM_ACTIVATEAPP:
        case WM_SIZE:
        {
            RECT ClientRect;
            GetClientRect(Window, &ClientRect);
            s32 Width = ClientRect.right - ClientRect.left;
            s32 Height = ClientRect.bottom - ClientRect.top;
            
            window_resize_result ResizeResult = WindowResize(Width, Height, WindowBitmap);
            
            WindowBitmap = ResizeResult;
        }break;
        
        case WM_CLOSE:
        case WM_DESTROY:
        {
            Running = 0;
        }break;
        
        case WM_PAINT:
        {
            
        }break;
        
        default:
        {
            
        }break;
    }
    return(DefWindowProc(Window, Message, WParam, LParam));
}

int CALLBACK
WinMain(HINSTANCE Instance,
        HINSTANCE PrevInstance,
        LPSTR CmdLine,
        int CmdShow)
{
    WindowCreateClass(Instance, "Handmade FTW", &WndProc);
    
    window_dim PosDim = {0};
    window_dim ResDim = {0};
    ResDim.X = 800;
    ResDim.Y = 600;
    
    HWND Window;
    WindowCreateWindowed("Handmade FTW", "Win32 Test", Instance, 0, 0, ResDim, PosDim, CW_USEDEFAULT, &Window);
    
    WindowUpdate();
    TimeInit();
    
    r64 OldTime = TimeGet();
    
    while(Running)
    {
        r64 NewTime = TimeGet();
        {
            WindowUpdate();
            {
                local_persist s32 XOffset = 0;
                local_persist s32 YOffset = 0;
                RenderGradient(XOffset, YOffset);
                
                ++XOffset;
                ++YOffset;
                }
            WindowBlit(Window, &WindowBitmap);
            
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