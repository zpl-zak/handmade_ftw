// (c) 2016 ZaKlaus; All Rights Reserved

#if !defined(HFTW_FONT_H)

#include <stdarg.h>

#define FONT_MAX_CHARS 512

typedef struct
{
    GLuint FontHandle;
    GLYPHMETRICSFLOAT GMF[256];
    HFONT Font;
} font_data;

typedef struct
{
    b32 Italic;
    b32 Underline;
    b32 Strikeout;
} font_style;

internal void
FontBuild(char *FontName, 
          u32 FontHeight, u32 FontWeight, 
          font_style FontStyle, 
          HDC DeviceContext, 
             font_data *FontData)
{
    HFONT Font;
    
    FontData->FontHandle = glGenLists(256);
    Font = CreateFont(-(s32)FontHeight, 
                      0, 0, 0, 
                      FontWeight, 
                      FontStyle.Italic,
FontStyle.Underline,
FontStyle.Strikeout,
                      ANSI_CHARSET, // NOTE(zaklaus): Expose to API...
                      OUT_TT_PRECIS,
                      CLIP_DEFAULT_PRECIS,
                      ANTIALIASED_QUALITY,
                      DEFAULT_PITCH|FF_DONTCARE,
                       FontName);
    
    Assert(Font);
    FontData->Font = Font;
    
    SelectObject(DeviceContext, Font);
    wglUseFontOutlines(DeviceContext, 0, 255, 
                       FontData->FontHandle, 0.f, 0.2f, 
                       WGL_FONT_POLYGONS, FontData->GMF);
    
    //DeleteObject(Font);
}

internal void
FontSelect(font_data *FontData, HDC DeviceContext)
{
    SelectObject(DeviceContext, FontData->Font);
    wglUseFontOutlines(DeviceContext, 0, 255, 
                       FontData->FontHandle, 0.f, 0.2f, 
                       WGL_FONT_POLYGONS, FontData->GMF);
    
}

internal void
FontPrintf(font_data *FontData, char *Format, r32 Capacity, r32 Scale, ...)
{
    char Text[FONT_MAX_CHARS];
    va_list VarArgs;
    
    if(Format == 0)
    {
        return;
    }
    
    va_start(VarArgs, Format);
    {
        vsprintf(Text, Format, VarArgs);
    }
    va_end(VarArgs);
    
    GLsizei TextLength = (GLsizei)strlen(Text);
    if(Capacity)
    {
        r32 Width = 35*Scale / 20;
    if(TextLength*Width > Capacity)
    {
        TextLength = (GLsizei)floor(Capacity / Width);
    }
    Text[TextLength] = 0;
}

    glPushAttrib(GL_LIST_BIT);
    {
        glListBase(FontData->FontHandle);
        glCallLists(TextLength, GL_UNSIGNED_BYTE, Text);
    }
    glPopAttrib();
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLint Viewport[4];
    glGetIntegerv(GL_VIEWPORT, Viewport);
    glOrtho(Viewport[0], Viewport[1], Viewport[2], Viewport[3], 0, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

internal void
FontDestroy(font_data *FontData)
{
    glDeleteLists(FontData->FontHandle, 256);
}

#define HFTW_FONT_H
#endif