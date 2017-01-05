// (c) 2016 ZaKlaus; All Rights Reserved

#if !defined(HFTW_GUI_H)

#define GUI_MAX_WINDOWS 128
#define GUI_WINDOW_PANEL_HEIGHT 50
#define GUI_WINDOW_PANEL_BUTTON_DIM GUI_WINDOW_PANEL_HEIGHT

typedef struct gui_window_proto
{
    char *Title;
    v2 Pos, Res;
    v3 Color;
    
    b32 Used;
    b32 *Visible;
    
    struct gui_window_proto *Parent;
} gui_window;

global_variable HWND GUIGlobalWindowHandle = 0;
global_variable gui_window *GlobalGUICurrentWindow = 0;
global_variable gui_window GlobalGUIWindows[GUI_MAX_WINDOWS] = {0};

internal void
GUIBeginWindow(char *Title, v2 Pos, v2 Res, v3 Color, b32 *Visible)
{
     s32 Slot = -1;
    for(s32 Idx = 0;
        Idx < GUI_MAX_WINDOWS;
        ++Idx)
    {
        if(!GlobalGUIWindows[Idx].Used)
        {
            Slot = Idx;
            break;
        }
    }
    
    
    Assert(Slot != -1);
    
    gui_window *Window = GlobalGUIWindows + Slot;
    
    if(!GlobalGUICurrentWindow)
    {
        GlobalGUICurrentWindow = Window;
    }
    else
    {
        Pos = MathAddVec2(Pos, GlobalGUICurrentWindow->Pos);
        Window->Parent = GlobalGUICurrentWindow;
        GlobalGUICurrentWindow = Window;
    }
    
    Window->Title = Title;
    Window->Pos = Pos;
    Window->Res = Res;
    Window->Color = Color;
    Window->Used = 1;
    Window->Visible = Visible;
    
    // NOTE(zaklaus): Correct if within the bounds.
    if(Window->Parent)
    {
        if(Window->Parent->Pos.Y - Window->Pos.Y < GUI_WINDOW_PANEL_HEIGHT)
        {
            // NOTE(zaklaus): Window overlaps window panel.
            Window->Pos.Y = Window->Parent->Pos.Y + GUI_WINDOW_PANEL_HEIGHT;
        }
    if(Window->Pos.X*2 + Window->Res.X > Window->Parent->Res.X)
    {
        Window->Res.X = Window->Parent->Res.X - Window->Pos.X * 2;
    }
    
    if(Window->Pos.Y*2 + Window->Res.Y > Window->Parent->Res.Y)
    {
        Window->Res.Y = Window->Parent->Res.Y - Window->Pos.Y *2;
    }
    
    if(!Visible)
    {
        Window->Visible = Window->Parent->Visible;
    }
}
}

internal void
GUIEndWindow(void)
{
    Assert(GlobalGUICurrentWindow);
    
    GlobalGUICurrentWindow = GlobalGUICurrentWindow->Parent;
}

internal b32
GUIProcessFrame(HWND Window,
                UINT Message,
                WPARAM WParam,
                LPARAM LParam)
{
    b32 Result = 0;
    
    return(Result);
}

internal v2
GUIAdjustDim(v2 Dim, r32 NewRange, r32 NewMin, b32 FlipX, b32 FlipY)
{
    v2 Result = {0};
    window_dim Dimensions = WindowGetClientRect(GUIGlobalWindowHandle);
    
    if(FlipX)
    {
        Dim.X = Dimensions.X - Dim.X;
    }
    
    if(FlipY)
    {
        Dim.Y = Dimensions.Y - Dim.Y;
    }
    
    Result.X = ((Dim.X * NewRange) / (float)Dimensions.X) + NewMin;
    Result.Y = ((Dim.Y * NewRange) / (float)Dimensions.Y) + NewMin;
    
    return(Result);
}

internal r32
GUIAdjustDimf(r32 Value, r32 NewRange, r32 NewMin, b32 FlipX, b32 FlipY)
{
     r32 Result = 0;
    window_dim Dimensions = WindowGetClientRect(GUIGlobalWindowHandle);
    
    if(FlipX)
    {
        Value = Dimensions.X - Value;
    }
    
    if(FlipY)
    {
        Value = Dimensions.Y - Value;
    }
    
    Result = ((Value * NewRange) / (float)Dimensions.X) + NewMin;
    
    return(Result);
}

internal void
GUIDrawRectangle(v2 Min, v2 Max, v3 Color)
{
    glBegin(GL_QUADS);
    {
        glColor3f(Color.R, Color.G, Color.B);
        glVertex2f(Min.X, Min.Y);
        glVertex2f(Max.X, Min.Y);
        glVertex2f(Max.X, Max.Y);
        glVertex2f(Min.X, Max.Y);
    }
    glEnd();
}

#define GUIADJP(vector) GUIAdjustDim(vector, 2, -1, 0, 1)
#define GUIADJR(vector) GUIAdjustDim(vector, 1, 0, 0, 0)
#define GUIADJRF(value) GUIAdjustDimf(value, 1, 0, 0, 0)
#define GUIADJ(min,max) GUIADJP(min), GUIADJR(max)

internal v2
GUIAdjustRes(v2 Pos, v2 Res)
{
    v2 Result = {0};
    Pos = GUIADJP(Pos);
    Res = GUIADJR(Res);
    
    Result.X = Pos.X + Res.X;
    Result.Y = Pos.Y - Res.Y;
    
    return(Result);
}

internal void
GUIDrawFrame(HWND WindowHandle)
{
    glDisable(GL_DEPTH_TEST);
    glUseProgram(0);
    GUIGlobalWindowHandle = WindowHandle;
    
    // NOTE(zaklaus): Window widget.
    {
        for(s32 Idx = 0;
            Idx < GUI_MAX_WINDOWS;
            ++Idx)
        {
            gui_window *Window = GlobalGUIWindows + Idx;
            gui_window Window_ = {0};
            
            if(Window)
            {
                if(Window->Visible 
                   && !*Window->Visible)
                {
                        goto gui_window_end;
                }
                
                v2 Position = Window->Pos;
                {
                    Position.X *= 2;
                    Position.Y *= 2;
                }
                v2 Resolution = Window->Res;
                {
                    Resolution.X *= 2;
                    Resolution.Y *= 2;
                }
                
                v2 Position2 = GUIAdjustRes(Position, Resolution);
                GUIDrawRectangle(GUIADJP(Position), Position2, Window->Color);
                
                v3 ColorDir = MathNormalize(Window->Color);
                
                // NOTE(zaklaus): Draw window panel.
                {
                    v2 PanelMin = Position;
                    r32 PanelHeight = GUI_WINDOW_PANEL_HEIGHT; 
                    v2 PanelRes;
                    PanelRes.X = Resolution.X;
                    PanelRes.Y = PanelHeight;
                    
                v3 NewColor = MathAddVec3(Window->Color, MathMultiplyVec3f(ColorDir, .25));
                
                    GUIDrawRectangle(GUIADJP(PanelMin), GUIAdjustRes(PanelMin, PanelRes), NewColor);
            }
            
            // NOTE(zaklaus): Draw panel button.
            {
                v2 ButtonMin = Position;
                ButtonMin.X += Resolution.X/2.f;
                v2 ButtonRes;
                ButtonRes.X = -GUI_WINDOW_PANEL_BUTTON_DIM;
                ButtonRes.Y = GUI_WINDOW_PANEL_BUTTON_DIM;
                
                v3 NewColor = MathAddVec3(Window->Color, MathMultiplyVec3f(ColorDir, -.25));
                
                GUIDrawRectangle(GUIADJP(ButtonMin), GUIAdjustRes(ButtonMin, ButtonRes), NewColor);
            }
            
            gui_window_end:
                *Window = Window_;
            }
        }
    }
    
    glEnable(GL_DEPTH_TEST);
}

#define HFTW_GUI_H
#endif