// (c) 2016 ZaKlaus; All Rights Reserved

#if !defined(HFTW_GUI_H)

#define GUI_MAX_WINDOWS 128
#define GUI_MAX_FONTS 32
#define GUI_MAX_LABELS 256

#define GUI_WINDOW_PANEL_HEIGHT 50
#define GUI_WINDOW_PANEL_BUTTON_DIM GUI_WINDOW_PANEL_HEIGHT

#define GUI_DEFAULT_WIDTH 800

typedef struct gui_window_proto
{
    char *Title;
    v2 Pos, Res;
    v3 Color;
    
    b32 Used;
    b32 *Visible;
    
    struct gui_window_proto *Parent;
} gui_window;

typedef struct
{
    char *Text;
    char *FontName;
    v2 Pos;
    v3 Color;
    r32 Scale;
    
    b32 Used;
} gui_label;

typedef struct
{
    font_data Fonts[GUI_MAX_FONTS];
    char *FontNames[GUI_MAX_FONTS];
} gui_fonts;

global_variable HWND GUIGlobalWindowHandle = 0;
global_variable HDC GUIGlobalDeviceContext = 0;
global_variable gui_fonts GUIGlobalFonts = {0};

global_variable gui_window *GlobalGUICurrentWindow = 0;
global_variable gui_window GlobalGUIWindows[GUI_MAX_WINDOWS] = {0};
global_variable gui_label GlobalGUILabels[GUI_MAX_LABELS] = {0};

internal void
GUIInitialize(HDC DeviceContext, HWND Window)
{
    GUIGlobalWindowHandle = Window;
    GUIGlobalDeviceContext = DeviceContext;
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

internal void
GUIAdjustBounds(gui_window *Window, v2 *Pos, v2 *Res)
{
    if(Window)
    {
        Pos->Y += GUI_WINDOW_PANEL_HEIGHT/2;
        
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
        GUIAdjustBounds(GlobalGUICurrentWindow, &Pos, &Res);
        Window->Parent = GlobalGUICurrentWindow;
        GlobalGUICurrentWindow = Window;
    }
    
    Window->Title = Title;
    Window->Pos = Pos;
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

internal void
GUIDrawFrame(void)
{
    glDisable(GL_DEPTH_TEST);
    glUseProgram(0);
    
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
            
            // NOTE(zaklaus): Draw title.
            {
                font_data *Font = GUISelectFont("Courier New");
                v2 TextPos = Position;
                TextPos.Y += 20;
                v3 TextColor = {1.f, 1.f, 1.f};
                GUIDrawText(GUIADJP(TextPos), 20, TextColor);
                FontPrintf(Font, Window->Title, Resolution.X - GUI_WINDOW_PANEL_BUTTON_DIM, 20);
            }
            
            gui_window_end:
                *Window = Window_;
            }
        }
        
        // NOTE(zaklaus): Labels
        {
            s32 Idx;
            for(Idx = 0;
                Idx < GUI_MAX_LABELS;
                ++Idx)
            {
                gui_label *Label = GlobalGUILabels + Idx;
                if(Label->Used)
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
    glEnable(GL_DEPTH_TEST);
}

#define HFTW_GUI_H
#endif