// (c) 2016 ZaKlaus; All Rights Reserved

#define HANDMADE_SLOW

#include "hftw.h"

#include "formats/hformat_4ds.h"
#include "formats/hformat_klz.h"
#include "formats/hformat_bmp.h"

#define COLOR_PURPLE 0x00ff00ff

global_variable b32 Running = 1;
global_variable window WindowArea;

internal void
RenderGradient(s32 XOffset, s32 YOffset)
{
    s32 Width = WindowArea.Width;
    s32 Height = WindowArea.Height;
    s32 BytesPerPixel = WindowArea.BPP;
    s32 Pitch = Width * BytesPerPixel;
    u8 *Row = (u8 *)WindowArea.Memory;
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

internal void
RenderPixel(v3 Color, v2 Position, s32 Margin)
{
    s32 Width = WindowArea.Width;
    s32 Height = WindowArea.Height;
    s32 BytesPerPixel = WindowArea.BPP;
    s32 Pitch = Width * BytesPerPixel;
    u32 *Memory = (u32 *)WindowArea.Memory;
    
    if(!Margin)
    {
        s32 X = (s32)Position.X;
        s32 Y = (s32)Position.Y;
        u8 Red = (u8)Color.R;
        u8 Green = (u8)Color.G;
        u8 Blue = (u8)Color.B;
        Memory[Y*Width+X] = ((Red << 16) | (Green << 8) | Blue);
        return;
    }
    
    for(s32 Y = 0;
        Y < Height;
        ++Y)
    {
        for(s32 X = 0;
            X < Width;
            ++X)
        {
            if((Position.X >= X - Margin && Position.X <= X + Margin) &&
               (Position.Y >= Y - Margin && Position.Y <= Y + Margin))
            {
                u8 Red = (u8)Color.R;
                u8 Green = (u8)Color.G;
                u8 Blue = (u8)Color.B;
                Memory[Y*Width+X] = ((Red << 16) | (Green << 8) | Blue);
            }
        }
    }
}

internal void
RenderBlank(u8 Red, u8 Green, u8 Blue)
{
    s32 Width = WindowArea.Width;
    s32 Height = WindowArea.Height;
    s32 BytesPerPixel = WindowArea.BPP;
    s32 Pitch = Width * BytesPerPixel;
    u8 *Row = (u8 *)WindowArea.Memory;
    for(s32 Y = 0;
        Y < Height;
        ++Y)
    {
        u32 *Pixel = (u32 *)Row;
        for(s32 X = 0;
            X < Width;
            ++X)
        {
            *Pixel++ = ((Red << 16) | (Green << 8) | Blue);
        }
        Row += Pitch;
    }
}

internal void
RenderBMPImage(s32 sX, s32 sY, hformat_bmp *Image, u32 TransparencyKey)
{
    s32 Width = Image->Header.Width;
    s32 Height = Image->Header.Height;
    
    u8 *ImageMemory = Image->Data;
    s32 BytesPerPixel = WindowArea.BPP;
    s32 Pitch = WindowArea.Width * BytesPerPixel;
    u8 *Memory = (u8 *)WindowArea.Memory;
    u8 *MemoryEnd = Memory + Pitch*WindowArea.Height + WindowArea.Width*BytesPerPixel;
    u8 *Row = (u8 *)WindowArea.Memory + sY*Pitch + sX*BytesPerPixel;
    
    for(s32 Y = 0;
        Y < Height;
        ++Y)
    {
        u32 *Pixel = (u32 *)Row;
        for(s32 X = 0;
            X < Width;
            ++X)
        {
            u8 Blue = *ImageMemory++;
            u8 Green = *ImageMemory++;
            u8 Red = *ImageMemory++;
            u32 Data = ((Red << 16) | (Green << 8) | Blue);
            
            if(Pixel > (u32 *)Memory && Pixel < (u32 *)MemoryEnd)
            {
            if(Data != TransparencyKey 
               ||TransparencyKey == 0)
            {
            *Pixel = Data;
            }
        }
            ++Pixel;
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
            window_dim Dim = WindowGetClientRect(Window);
            
            window_resize_result ResizeResult = WindowResize(Dim.X, Dim.Y, WindowArea, 1);
            
            glViewport(0, 0, WindowArea.Width, WindowArea.Height);
            
            WindowArea = ResizeResult;
            
            WindowBlit(Window, &WindowArea);
        }break;
        
        case WM_CLOSE:
        case WM_DESTROY:
        {
            Running = 0;
        }break;
        
        case WM_KEYDOWN:
        {
            if(WParam == VK_ESCAPE)
            {
                Running = 0;
            }
        }break;
    }
    return(DefWindowProc(Window, Message, WParam, LParam));
}

int CALLBACK
_WinMain(HINSTANCE Instance,
        HINSTANCE PrevInstance,
        LPSTR CmdLine,
        int CmdShow)
{
    WindowCreateClass(Instance, "Handmade FTW", &WndProc);
    
    window_dim PosDim = {0};
    window_dim ResDim = {800, 600};
    
    HWND Window;
    WindowCreateWindowed("Handmade FTW", "Win32 Test", Instance, 0, 0, ResDim, PosDim, CW_USEDEFAULT, &Window);
    
    window_dim Dim = WindowGetClientRect(Window);
    window_resize_result ResizeResult = WindowResize(Dim.X, Dim.Y, WindowArea, 1);
    
    WindowArea = ResizeResult;
    
    WindowUpdate();
    TimeInit();
    
    r64 OldTime = TimeGet();
    
    s32 FileIndex = IOFileOpenRead("data/test.bmp", 0);
    hformat_bmp *Image = HFormatLoadBMPImage(FileIndex, 0);
    IOFileClose(FileIndex);
    
    HDC DeviceContext = GetDC(Window);
    
    b32 ModernContext = 1;
    
    Win32InitOpenGL(DeviceContext, &ModernContext);
    
    glEnable(GL_TEXTURE_2D);
    
    u32 VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f,  1.0f, 0.0f,
    };
    
    GLuint vertexbuffer;
    
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    
    while(Running)
    {
        r64 NewTime = TimeGet();
        r64 DeltaTime = NewTime - OldTime;
        {
            WindowUpdate();
            {
                s32 WindowWidth = WindowArea.Width;
                s32 WindowHeight = WindowArea.Height;
                glClearColor(1.f, 1.f, 1.f, 0.f);
                glClear(GL_COLOR_BUFFER_BIT);
                
                glEnableVertexAttribArray(0);
                glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
                glVertexAttribPointer(
                    0,
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stride
                    (void*)0            // array buffer offset
                    );
                // Draw the triangle !
                glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
                glDisableVertexAttribArray(0);
                }
                SwapBuffers(DeviceContext);
                
            Sleep(10);
        }
        OldTime = NewTime;
    }
    
    ReleaseDC(Window, DeviceContext);
    
    PlatformMemFree(Image->Data);
    
    return(0);
}

int
main(void)
{
     LRESULT Result = _WinMain(GetModuleHandle(0), 0, GetCommandLine(), SW_SHOW);
    
    return(Result);
}