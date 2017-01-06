// (c) 2016 ZaKlaus; All Rights Reserved

#if !defined(HFTW_GUI_H)

// TOP

#define GUI_MAX_WINDOWS 128
#define GUI_MAX_FONTS 32
#define GUI_MAX_LABELS 256

#define GUI_WINDOW_PANEL_HEIGHT 50
#define GUI_WINDOW_PANEL_BUTTON_DIM GUI_WINDOW_PANEL_HEIGHT

#define GUI_DEFAULT_WIDTH 800

typedef struct
{
    HWND Window;
    UINT Message;
    WPARAM WParam;
    LPARAM LParam;
    
    b32 MouseDown;
    b32 MousePress;
    b32 MouseUp;
} gui_wndmsg;

typedef struct
{
    font_data Fonts[GUI_MAX_FONTS];
    char *FontNames[GUI_MAX_FONTS];
} gui_fonts;

global_variable HWND GUIGlobalWindowHandle = 0;
global_variable HDC GUIGlobalDeviceContext = 0;
global_variable gui_fonts GUIGlobalFonts = {0};
global_variable gui_wndmsg GlobalGUIWindowMessage = {0};

//
// NOTE(zaklaus): Widgets
//

typedef struct gui_window_proto
{
    char *Title;
    v2 Pos, Res;
    v3 Color;
    
    b32 Used;
    b32 *Visible;
    b32 Overriden;
    b32 Rendered;
    b32 Moving;
    b32 MightClose;
    
    struct gui_window_proto *Parent;
    struct gui_window_proto *Children[GUI_MAX_WINDOWS];
} gui_window;

typedef struct
{
    char *Text;
    char *FontName;
    v2 Pos;
    v3 Color;
    r32 Scale;
    
    b32 Used;
    gui_window *Window;
} gui_label;

global_variable gui_window *GlobalGUICurrentWindow = 0;
global_variable gui_window GlobalGUIWindows[GUI_MAX_WINDOWS] = {0};
global_variable gui_label GlobalGUILabels[GUI_MAX_LABELS] = {0};

internal void
GUIAdjustBounds(gui_window *Window, v2 *Pos, v2 *Res);

internal void
GUIAdjustBoundsPos(gui_window *Window, v2 *Pos, r32 Scale);

internal void
GUIBeginWindow(char *Title, v2 Pos, v2 Res, v3 Color, b32 *Visible)
{
    gui_window *Window = GlobalGUIWindows;
    s32 Slot = -1;
    for(s32 Idx = 0;
        Idx < GUI_MAX_WINDOWS;
        ++Idx)
    {
        Window = GlobalGUIWindows + Idx;
        if(!Window->Used && Slot == -1)
        {
            Slot = Idx;
        }
        
        if(StringsAreEqual(Window->Title, Title))
        {
            Slot = Idx;
            break;
        }
    }
    
    Assert(Slot != -1);
    
    Window = GlobalGUIWindows + Slot;
    
    if(!GlobalGUICurrentWindow)
    {
        GlobalGUICurrentWindow = Window;
    }
    else
    {
        
        GUIAdjustBounds(GlobalGUICurrentWindow, &Pos, &Res);
        Window->Parent = GlobalGUICurrentWindow;
        
        s32 Idx;
        for(Idx = 0;
            Idx < GUI_MAX_WINDOWS;
            ++Idx)
        {
            if(GlobalGUICurrentWindow->Children[Idx])
            {
                if(StringsAreEqual(GlobalGUICurrentWindow->Children[Idx]->Title, Title))
                {
                    break;
                }
            }
            else
            {
                GlobalGUICurrentWindow->Children[Idx] = Window;
                break;
            }
        }
        
        GlobalGUICurrentWindow = Window;
    }
    
    Window->Title = Title;
    if(!Window->Overriden)
    {
        Window->Pos = Pos;
    }
    Window->Res = Res;
    Window->Color = Color;
    Window->Used = 1;
    Window->Visible = Visible;
    
    if(Window->Parent)
    {
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

internal void
GUILabel(char *Text, v2 Pos, r32 Scale, v3 Color, char *FontName)
{
    s32 Slot = -1;
    for(s32 Idx = 0;
        Idx < GUI_MAX_LABELS;
        ++Idx)
    {
        if(!GlobalGUILabels[Idx].Used)
        {
            Slot = Idx;
            break;
        }
    }
    
    Assert(Slot != -1);
    
    gui_label *Label = GlobalGUILabels + Slot;
    
    GUIAdjustBoundsPos(GlobalGUICurrentWindow, &Pos, Scale);
    
    Label->Text = Text;
    Label->Pos = Pos;
    Label->Scale = Scale;
    Label->Color = Color;
    Label->FontName = FontName;
    Label->Used = 1;
    Label->Window = GlobalGUICurrentWindow;
}

//
// NOTE(zaklaus): Internals
//

internal void
GUIAdjustBounds(gui_window *Window, v2 *Pos, v2 *Res)
{
    if(Window)
    {
        Pos->Y += GUI_WINDOW_PANEL_HEIGHT/2;
        
        if(Pos->Y + Res->Y > GUI_WINDOW_PANEL_HEIGHT/2)
        {
            Pos->Y += GUI_WINDOW_PANEL_HEIGHT/2;
        }
        
        if(Res->X + Pos->X > Window->Res.X)
        {
            r32 ddR = Res->X + Pos->X - Window->Res.X;
            Res->X -= ddR;
        }
        
        if(Pos->Y + Res->Y > Window->Res.Y)
        {
            r32 ddR = Pos->Y + Res->Y - Window->Res.Y;
            Res->Y -= ddR;
        }
        *Pos = MathAddVec2(Window->Pos, *Pos);
    }
}

internal void
GUIAdjustBoundsPos(gui_window *Window, v2 *Pos, r32 Scale)
{
    if(Window)
    {
        Pos->Y += GUI_WINDOW_PANEL_HEIGHT/2 + Scale;
        *Pos = MathAddVec2(Window->Pos, *Pos);
    }
}

internal void
GUICreateFont(char *FontName, 
              u32 FontHeight, u32 FontWeight, 
              font_style FontStyle)
{
    s32 Idx;
    for(Idx = 0;
        Idx < GUI_MAX_FONTS;
        ++Idx)
    {
        char *TargetFontName = GUIGlobalFonts.FontNames[Idx];
        
        if(!TargetFontName)
        {
            GUIGlobalFonts.FontNames[Idx] = FontName;
            
            font_data FontData = {0};
            FontBuild(FontName, FontHeight, 
                      FontWeight, FontStyle,
                      GUIGlobalDeviceContext,
                      &FontData);
            
            GUIGlobalFonts.Fonts[Idx] = FontData;
            return;
        }
    }
}

internal void
GUIInitialize(HDC DeviceContext, HWND Window)
{
    GUIGlobalWindowHandle = Window;
    GUIGlobalDeviceContext = DeviceContext;
    
    font_style FontStyle = {0, 0, 0};
    GUICreateFont("Courier New", 12, 0, FontStyle);
}

internal font_data *
GUISelectFont(char *FontName)
{
    s32 Idx;
    for(Idx = 0;
        Idx < GUI_MAX_FONTS;
        ++Idx)
    {
        char **TargetFontName = &GUIGlobalFonts.FontNames[Idx];
        
        if(TargetFontName)
        {
            if(StringsAreEqual(*TargetFontName, FontName))
            {
                return(&GUIGlobalFonts.Fonts[Idx]);
            }
        }
    }
    return(0);
}

internal b32
GUIProcessFrame(HWND Window,
                UINT Message,
                WPARAM WParam,
                LPARAM LParam)
{
    b32 Result = 0;
    
    GlobalGUIWindowMessage.Window = Window;
    GlobalGUIWindowMessage.Message = Message;
    GlobalGUIWindowMessage.WParam = WParam;
    GlobalGUIWindowMessage.LParam = LParam;
    
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

internal void
GUIDrawText(v2 Pos, r32 Scale, v3 Color)
{
    window_dim Dimensions = WindowGetClientRect(GUIGlobalWindowHandle);
    Scale /= 20 * 10;
    
    Scale /= Dimensions.X / GUI_DEFAULT_WIDTH;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho(-100, 100, -100, 100, 0, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    r32 AspectRatio = (real32)Dimensions.X / Dimensions.Y;
    glTranslatef(Pos.X, Pos.Y, 0.f);
    glScalef(Scale / AspectRatio, Scale, 1.f);
    
    glColor3f(Color.R, Color.G, Color.B);
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

internal b32
GUICheckPointIn2D(v2 Min, v2 Max, v2 Pos)
{
    b32 Result = 
        Min.X < Pos.X && Max.X > Pos.X &&
        Min.Y < Pos.Y && Max.Y > Pos.Y;
    
    return(Result);
}

// NOTE(zaklaus): Should only be called by GUIDrawFrame!
internal void
GUIUpdateFrame(void)
{
    gui_wndmsg *Wnd = &GlobalGUIWindowMessage;
    
    switch(Wnd->Message)
    {
        case WM_LBUTTONDOWN:
        {
            Wnd->MouseDown = 1;
            Wnd->MousePress = 1;
            Wnd->MouseUp = 0;
        }break;
        
        case WM_LBUTTONUP:
        {
            Wnd->MouseDown = 0;
            Wnd->MouseUp = 1;
        }break;
    }
    
    POINT CursorPoint;
    GetCursorPos(&CursorPoint);
    ScreenToClient(GUIGlobalWindowHandle, &CursorPoint);
    
    v2 CursorPos;
    CursorPos.X = (r32)CursorPoint.x;
    CursorPos.Y = (r32)CursorPoint.y;
    
    s32 Idx;                         
    for(Idx = GUI_MAX_WINDOWS-1;       
        Idx >= 0;                     
        --Idx)                       
    {                                
        gui_window *Window = GlobalGUIWindows + Idx;
        
        if(Window->Used)             
        {                            
            v2 Min = Window->Pos;    
            v2 Max = MathAddVec2(Window->Pos, Window->Res);
            
            if(Window->Moving)
            {
                goto case_is_moving;
            }
            
            if(GUICheckPointIn2D(Min, Max, CursorPos))
            {
                if(Wnd->MousePress)
                {
                    s32 Slot = -1;
                    for(s32 Idx2 = GUI_MAX_WINDOWS - 1;
                        Idx2 >= 0;
                        --Idx2)
                    {
                        gui_window *SecondWindow = GlobalGUIWindows + Idx2;
                        if(!SecondWindow->Used)
                        {
                            Slot = Idx2;
                        }
                        else
                        {
                            break;
                        }
                    }
                    
                    gui_window Window_ = *Window;
                    gui_window Empty_ = {0};
                    *Window = *(GlobalGUIWindows + Slot);
                    *(GlobalGUIWindows + Slot) = Window_;
                    
                    for(s32 Idx2 = Idx;
                        Idx2 < Slot;
                        ++Idx2)
                    {
                        *(GlobalGUIWindows + Idx2) = *(GlobalGUIWindows + Idx2 + 1);
                        *(GlobalGUIWindows + Idx2 + 1) = Empty_;
                    }
                }
                
                v2 ButtonMin = Window->Pos;
                ButtonMin.X += Window->Res.X - GUI_WINDOW_PANEL_BUTTON_DIM/2.f;
                v2 ButtonMax;
                ButtonMax.X = ButtonMin.X + GUI_WINDOW_PANEL_BUTTON_DIM/2.f;
                ButtonMax.Y = ButtonMin.Y + GUI_WINDOW_PANEL_BUTTON_DIM/2.f;
                if(GUICheckPointIn2D(ButtonMin, ButtonMax, CursorPos))
                {
                    if(Window->Visible)
                    {
                    if(Wnd->MouseDown && Window->Visible)
                    {
                        Window->MightClose = 1;
                    }
                    
                    if(Wnd->MouseUp && Window->MightClose)
                    {
                        *Window->Visible = 0;
                    }
                }
                break;
                }
                else
                {
                    if(Wnd->MouseUp)
                    {
                        Window->MightClose = 0;
                    }
                }
                
                v2 PanelMin = Window->Pos;
                v2 PanelMax;
                PanelMax.X = Window->Pos.X + Window->Res.X;
                PanelMax.Y = Window->Pos.Y + GUI_WINDOW_PANEL_HEIGHT/2.f;
                
                if(GUICheckPointIn2D(PanelMin, PanelMax, CursorPos))
                {
                    case_is_moving:
                    if(!Wnd->MouseDown)
                    {
                        Window->Moving = 0;
                    }
                    if(Wnd->MouseDown)
                    {
                        Window->Moving = 1;
                        v2 Res = Window->Res;
                        Res.X /= 2.f;
                        Res.Y = GUI_WINDOW_PANEL_HEIGHT/4.f;
                        Window->Pos = MathSubtractVec2(CursorPos, Res);
                        Window->Overriden = 1;
                    }
                }
                
                break;
            }
        }
    }
    
    Wnd->MousePress = 0;
}

internal void
GUIDrawWindow(gui_window *Window, b32 IgnoreParent)
{
    if(!Window)
    {
        return;
    }
    
        if(Window->Used && (!Window->Parent || IgnoreParent))
        {
            if(Window->Visible 
               && !*Window->Visible)
            {
                return;
            }
            
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            
            v2 Position = Window->Pos;
            v2 Resolution = Window->Res;
            {
                Resolution.X *= 2;
                Resolution.Y *= 2;
            }
            
            v2 Position2 = GUIAdjustRes(Position, Resolution);
            GUIDrawRectangle(GUIADJP(Position), Position2, Window->Color);
            
            v3 ColorDir = {.1, .1, .1};
            
            // NOTE(zaklaus): Draw window panel.
            {
                v2 PanelMin = Position;
                r32 PanelHeight = GUI_WINDOW_PANEL_HEIGHT; 
                v2 PanelRes;
                PanelRes.X = Resolution.X;
                PanelRes.Y = PanelHeight;
                
                v3 NewColor =MathAddVec3(Window->Color, MathMultiplyVec3f(ColorDir, 4));
                
                GUIDrawRectangle(GUIADJP(PanelMin), GUIAdjustRes(PanelMin, PanelRes), NewColor);
            }
            
            // NOTE(zaklaus): Draw panel button.
            {
                v2 ButtonMin = Position;
                ButtonMin.X += Resolution.X/2.f;
                v2 ButtonRes;
                ButtonRes.X = -GUI_WINDOW_PANEL_BUTTON_DIM;
                ButtonRes.Y = GUI_WINDOW_PANEL_BUTTON_DIM;
                
                v3 NewColor =  MathSubtractVec3(Window->Color, MathMultiplyVec3f(ColorDir, 4));
                
                GUIDrawRectangle(GUIADJP(ButtonMin), GUIAdjustRes(ButtonMin, ButtonRes), NewColor);
            }
            
            // NOTE(zaklaus): Draw title.
            {
                r32 ColorAccu = MathSquareRoot(MathDot(Window->Color, Window->Color));
                
                font_data *Font = GUISelectFont("Courier New");
                v2 TextPos = Position;
                TextPos.Y += 20;
                
                v3 TextColor = {1.f, 1.f, 1.f};
                
                if(ColorAccu < .75f)
                {
                    TextColor.X = TextColor.Y = TextColor.Z = 0.f;
                }
                
                GUIDrawText(GUIADJP(TextPos), 20, TextColor);
                FontPrintf(Font, Window->Title, Resolution.X - GUI_WINDOW_PANEL_BUTTON_DIM, 20);
            }
            
            
            // NOTE(zaklaus): Labels
            {
                s32 WIdx;
                for(WIdx = 0;
                    WIdx < GUI_MAX_LABELS;
                    ++WIdx)
                {
                    gui_label *Label = GlobalGUILabels + WIdx;
                    if(Label->Used && Label->Window == Window)
                    {
                        
                        gui_label Label_ = {0};
                        
                        font_data *Font = GUISelectFont(Label->FontName);
                        GUIDrawText(GUIADJP(Label->Pos), Label->Scale, Label->Color);
                        FontPrintf(Font, Label->Text, 0, Label->Scale);
                        
                        *Label = Label_;
                    }
                }
            }
        }
    }

    
internal void
GUIDrawFrame(void)
{
    GUIUpdateFrame();
    
    glDisable(GL_DEPTH_TEST);
    glUseProgram(0);
    
    // NOTE(zaklaus): Window widget.
    {
        
        for(s32 Idx = 0;
            Idx < GUI_MAX_WINDOWS;
            ++Idx)
        {
            GUIDrawWindow(&GlobalGUIWindows[Idx], 0);
    }
    
    ZeroMemory(GlobalGUILabels, sizeof(gui_label)*GUI_MAX_LABELS);
}
    glEnable(GL_DEPTH_TEST);
}

// BOTTOM

#define HFTW_GUI_H
#endif