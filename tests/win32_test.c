// (c) 2016 ZaKlaus; All Rights Reserved

#define HANDMADE_SLOW

#include "hftw.h"

#include "formats/hformat_4ds.h"
#include "formats/hformat_klz.h"
#include "formats/hformat_bmp.h"

#define COLOR_PURPLE 0x00ff00ff

global_variable b32 Running = 1;
global_variable window WindowArea;
global_variable b32 ShowTestWindow = 1;

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
    GUIProcessFrame(Window, Message, WParam, LParam);
    
    switch(Message)
    {
        case WM_ACTIVATEAPP:
        case WM_SIZE:
        {
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
            
            if(WParam == VK_RETURN)
            {
                ShowTestWindow = !ShowTestWindow;
            }
        }break;
    }
    return(DefWindowProc(Window, Message, WParam, LParam));
}

// TODO(zaklaus): Temporary code.
internal GLuint
TESTLoadShader(char *ShaderOnePath, char *ShaderTwoPath)
{
    ms ShaderOneFileSize;
    s32 ShaderOneFile = IOFileOpenRead(ShaderOnePath, &ShaderOneFileSize);
    char *ShaderOne = PlatformMemAlloc(ShaderOneFileSize + 1);
    IOFileRead(ShaderOneFile, ShaderOne, ShaderOneFileSize);
    ShaderOne[ShaderOneFileSize] = 0;
    IOFileClose(ShaderOneFile);
    
    ms ShaderTwoFileSize;
    s32 ShaderTwoFile = IOFileOpenRead(ShaderTwoPath, &ShaderTwoFileSize);
    char *ShaderTwo = PlatformMemAlloc(ShaderTwoFileSize + 1);
    IOFileRead(ShaderTwoFile, ShaderTwo, ShaderTwoFileSize);
    ShaderTwo[ShaderTwoFileSize] = 0;
    IOFileClose(ShaderTwoFile);
    
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    
    GLint ErrResult = GL_FALSE;
    s32 InfoLogLength;
    
    glShaderSource(VertexShaderID, 1, &ShaderOne, 0);
    glCompileShader(VertexShaderID);
    
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &ErrResult);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if(InfoLogLength > 0)
    {
        char *InfoLog = PlatformMemAlloc(InfoLogLength+1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, 0, InfoLog);
        printf("Vertex: %.*s", InfoLogLength, InfoLog);
        PlatformMemFree(InfoLog);
    }
    
    glShaderSource(FragmentShaderID, 1, &ShaderTwo, 0);
    glCompileShader(FragmentShaderID);
    
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &ErrResult);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if(InfoLogLength > 0)
    {
        char *InfoLog = PlatformMemAlloc(InfoLogLength+1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, 0, InfoLog);
        printf("Fragment: %.*s", InfoLogLength, InfoLog);
        PlatformMemFree(InfoLog);
    }
    
    PlatformMemFree(ShaderOne);
PlatformMemFree(ShaderTwo);
    
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);
    
    
    glGetShaderiv(ProgramID, GL_COMPILE_STATUS, &ErrResult);
    glGetShaderiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if(InfoLogLength > 0)
    {
        char *InfoLog = PlatformMemAlloc(InfoLogLength+1);
        glGetShaderInfoLog(ProgramID, InfoLogLength, 0, InfoLog);
        printf("Program: %.*s", InfoLogLength, InfoLog);
        PlatformMemFree(InfoLog);
    }
    
    glDetachShader(ProgramID, VertexShaderID);
    glDetachShader(ProgramID, FragmentShaderID);
    
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
    
    return(ProgramID);
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
    glEnable(GL_DEPTH_TEST);
    GLuint ProgramID = TESTLoadShader("simple.vert", "simple.frag");
                  
    u32 VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
                  
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f,-1.0f,-1.0f, // triangle 1 : begin
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, // triangle 1 : end
        1.0f, 1.0f,-1.0f, // triangle 2 : begin
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f, // triangle 2 : end
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f
    };            
                  
    static const GLfloat g_color_buffer_data[] = {
        0.583f,  0.771f,  0.014f,
        0.609f,  0.115f,  0.436f,
        0.327f,  0.483f,  0.844f,
        0.822f,  0.569f,  0.201f,
        0.435f,  0.602f,  0.223f,
        0.310f,  0.747f,  0.185f,
        0.597f,  0.770f,  0.761f,
        0.559f,  0.436f,  0.730f,
        0.359f,  0.583f,  0.152f,
        0.483f,  0.596f,  0.789f,
        0.559f,  0.861f,  0.639f,
        0.195f,  0.548f,  0.859f,
        0.014f,  0.184f,  0.576f,
        0.771f,  0.328f,  0.970f,
        0.406f,  0.615f,  0.116f,
        0.676f,  0.977f,  0.133f,
        0.971f,  0.572f,  0.833f,
        0.140f,  0.616f,  0.489f,
        0.997f,  0.513f,  0.064f,
        0.945f,  0.719f,  0.592f,
        0.543f,  0.021f,  0.978f,
        0.279f,  0.317f,  0.505f,
        0.167f,  0.620f,  0.077f,
        0.347f,  0.857f,  0.137f,
        0.055f,  0.953f,  0.042f,
        0.714f,  0.505f,  0.345f,
        0.783f,  0.290f,  0.734f,
        0.722f,  0.645f,  0.174f,
        0.302f,  0.455f,  0.848f,
        0.225f,  0.587f,  0.040f,
        0.517f,  0.713f,  0.338f,
        0.053f,  0.959f,  0.120f,
        0.393f,  0.621f,  0.362f,
        0.673f,  0.211f,  0.457f,
        0.820f,  0.883f,  0.371f,
        0.982f,  0.099f,  0.879f
    };            
                  
    GLuint vertexbuffer, colorbuffer;
                  
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
                  
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
                  
                  
    GLuint MatrixID = glGetUniformLocation(ProgramID, "MVP");
                  
    GUIInitialize(DeviceContext, Window);
    
    // NOTE(zaklaus): Build fonts.
    font_style StandardFontStyle  = {0};
    GUICreateFont("Courier New", 12, FW_BOLD, StandardFontStyle);
    GUICreateFont("Times New Roman", 12, FW_DONTCARE, StandardFontStyle);
    
    while(Running)
    {             
        r64 NewTime = TimeGet();
        r64 DeltaTime = NewTime - OldTime;
        {         
            WindowUpdate();
            {     
                s32 WindowWidth = WindowArea.Width;
                s32 WindowHeight = WindowArea.Height;
                  
                { 
                     Dim = WindowGetClientRect(Window);
                    ResizeResult = WindowResize(Dim.X, Dim.Y, WindowArea, 1);
                    glViewport(0, 0, WindowArea.Width, WindowArea.Height);
                    WindowArea = ResizeResult;
                } 
                  
                glClearColor(1.f, 1.f, 1.f, 0.f);
                glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
                  
                glUseProgram(ProgramID);
                  
                  
                m4 Projection = MathPerspective(45.f, WindowArea.Width / (real32)WindowArea.Height, .1f, 100.f);
                  
                v3 Pos = {4, 3, 7};
                v3 Target = {0};
                v3 Up = {0, 1, 0};
                  
                m4 View = MathLookAt(Pos, Target, Up);
                m4 Model = MathMat4d(1.f);
                  
                mat4 MVP = MathMultiplyMat4(Projection, View);   
                MVP = MathMultiplyMat4(MVP, Model);
                  
                glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP.Elements[0][0]);
                  
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
                  
                glEnableVertexAttribArray(1);
                glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
                glVertexAttribPointer(
                    1,
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stride
                    (void*)0            // array buffer offset
                    );
                // Draw the triangle !
                glDrawArrays(GL_TRIANGLES, 0, 12*3); // Starting from vertex 0; 3 vertices total -> 1 triangle
                glDisableVertexAttribArray(1);
                glDisableVertexAttribArray(0);
                
                local_persist r32 yflt = 200.f;
local_persist r32 xflt = 50.f;
                local_persist r32 smallxflt = .4f;
                
                // NOTE(zaklaus): GUI Test
                {
                    v2 WPos = {100, 10};
                    v2 WRes = {320, 500};
                    //WRes.X = yflt;
                    //WRes.Y = yflt;
                    //WPos.X = xflt;
                    v3 Color = {0.12, 0.56, 0.43};
                    GUIBeginWindow("Handmade FTW", WPos, WRes, Color, &ShowTestWindow);
                    {
                        local_persist u32 cnt = 0;
                        char buffer[256];
                        sprintf(buffer, "Counter: %d", cnt++);
                        GUILabel(buffer, MathVec2(10, 20), 12, MathVec3(.33,.33,.33), "Courier New");
                    }
                    GUIEndWindow();
                    
                    v2 AnotherPos = {140, 10};
                    v2 AnotherRes = {540, 230};
                    v3 AnotherColor = {.78f, 0.f, 0.f};
                    GUIBeginWindow("Another Window", AnotherPos, AnotherRes, AnotherColor, 0);
                    {
                        
                    }
                    GUIEndWindow();
                    
                    xflt += sinf((r32)TimeGet());
                    smallxflt = sinf((r32)TimeGet());
                    yflt += sinf((r32)TimeGet())*2;
                }
                
                GUIDrawFrame();
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