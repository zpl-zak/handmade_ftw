// (c) 2016 ZaKlaus; All Rights Reserved

#if !defined(WIN32_OPENGL_H)

#include <gl/gl.h>
#include <gl/glu.h>

#include "win32_opengl_defs.h"

#ifdef OPENGL_VERBOSE_EXTENSIONS
#define PRINT_OPENGL_EXTENSIONS 1
#else
#define PRINT_OPENGL_EXTENSIONS 0
#endif

#ifndef OPENGL_SKIP_INFO_PRINT
#define PRINT_OPENGL_INFO 1
#else
#define PRINT_OPENGL_INFO 0
#define PRINT_OPENGL_EXTENSIONS 0
#endif

typedef struct
{
    b32 ModernContext;
    
    char *Vendor;
    char *Renderer;
    char *Version;
    char *ShadingLanguageVersion;
    char *Extensions;
    
    b32 GL_EXT_texture_sRGB;
    b32 GL_EXT_framebuffer_sRGB;
    b32 GL_ARB_framebuffer_object;
} opengl_info;

#define GL_FRAMEBUFFER_SRGB               0x8DB9
#define GL_SRGB8_ALPHA8                   0x8C43

#define GL_SHADING_LANGUAGE_VERSION       0x8B8C

#define GL_FRAMEBUFFER                    0x8D40
#define GL_COLOR_ATTACHMENT0              0x8CE0
#define GL_FRAMEBUFFER_COMPLETE           0x8CD5

// NOTE(casey): Windows-specific
#define WGL_CONTEXT_MAJOR_VERSION_ARB           0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB           0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB             0x2093
#define WGL_CONTEXT_FLAGS_ARB                   0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB            0x9126

#define WGL_CONTEXT_DEBUG_BIT_ARB               0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB  0x0002

#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB        0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002

#define WGL_DRAW_TO_WINDOW_ARB                  0x2001
#define WGL_ACCELERATION_ARB                    0x2003
#define WGL_SUPPORT_OPENGL_ARB                  0x2010
#define WGL_DOUBLE_BUFFER_ARB                   0x2011
#define WGL_PIXEL_TYPE_ARB                      0x2013

#define WGL_TYPE_RGBA_ARB                       0x202B
#define WGL_FULL_ACCELERATION_ARB               0x2027

#define WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB        0x20A9

typedef HGLRC WINAPI wgl_create_context_attribs_arb(HDC hDC, HGLRC hShareContext,
                                                    const int *attribList);

typedef BOOL WINAPI wgl_get_pixel_format_attrib_iv_arb(HDC hdc,
                                                       int iPixelFormat,
                                                       int iLayerPlane,
                                                       UINT nAttributes,
                                                       const int *piAttributes,
                                                       int *piValues);

typedef BOOL WINAPI wgl_get_pixel_format_attrib_fv_arb(HDC hdc,
                                                       int iPixelFormat,
                                                       int iLayerPlane,
                                                       UINT nAttributes,
                                                       const int *piAttributes,
                                                       FLOAT *pfValues);

typedef BOOL WINAPI wgl_choose_pixel_format_arb(HDC hdc,
                                                const int *piAttribIList,
                                                const FLOAT *pfAttribFList,
                                                UINT nMaxFormats,
                                                int *piFormats,
                                                UINT *nNumFormats);

typedef void WINAPI gl_bind_framebuffer(GLenum target, GLuint framebuffer);
typedef void WINAPI gl_gen_framebuffers(GLsizei n, GLuint *framebuffers);
typedef void WINAPI gl_framebuffer_texture_2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef GLenum WINAPI gl_check_framebuffer_status(GLenum target);
typedef void WINAPI gl_gen_vertex_arrays(GLsizei n, GLuint *arrays);
typedef void WINAPI gl_bind_vertex_array(GLuint array);
typedef void WINAPI gl_buffer_data(GLenum target,
                          GLsizei size,
                           const GLvoid * data,
                          GLenum usage);
typedef void WINAPI gl_buffer_sub_data(GLenum target,
                                       GLsizei offset,
                                   GLsizei size,
                                   const GLvoid * data,
                                   GLenum usage);
typedef void WINAPI gl_gen_buffers(GLsizei n, GLuint *buffers);
typedef void WINAPI gl_bind_buffer(GLenum target, GLuint buffer);
typedef void WINAPI gl_enable_vertex_attrib_array(GLuint index);
typedef void WINAPI gl_disable_vertex_attrib_array(GLuint index);
typedef void WINAPI gl_vertex_attrib_pointer(GLuint index,
                           GLint size,
                           GLenum type,
                           GLboolean normalized,
                           GLsizei stride,
                           const GLvoid * pointer);
typedef GLuint WINAPI gl_create_shader(GLenum shaderType);
typedef void WINAPI gl_attach_shader(GLuint program,
                                     GLuint shader);
typedef void WINAPI gl_compile_shader(GLuint shader);
typedef void WINAPI gl_shader_source(GLuint shader,
                                     GLsizei count,
                                     const char **string,
                                     const GLint *length);
typedef void WINAPI gl_delete_shader(GLuint shader);
typedef void WINAPI gl_detach_shader(GLuint program,
                                     GLuint shader);
typedef void WINAPI gl_get_shaderiv(GLuint shader,
                                    GLenum pname,
                                    GLint *params);
typedef GLuint WINAPI gl_create_program(void);
typedef void WINAPI gl_use_program(GLuint program);
typedef void WINAPI gl_link_program(GLuint program);
typedef void WINAPI gl_get_programiv(GLuint program,
                                     GLenum pname,
                                     GLint *params);
typedef void WINAPI gl_get_shader_info_log(GLuint shader,
                                           GLsizei maxLength,
                                           GLsizei *length,
                                           char *infoLog);typedef void WINAPI gl_get_program_info_log(GLuint program,
                                           GLsizei maxLength,
                                           GLsizei *length,
                                           char *infoLog);
typedef GLint WINAPI gl_get_uniform_location(GLuint program,
                                             const char *name);

typedef void WINAPI gl_uniform1f(GLint location,
                 GLfloat v0);

typedef void WINAPI gl_uniform2f(GLint location,
                 GLfloat v0,
                 GLfloat v1);

typedef void WINAPI gl_uniform3f(GLint location,
                 GLfloat v0,
                 GLfloat v1,
                 GLfloat v2);

typedef void WINAPI gl_uniform4f(GLint location,
                 GLfloat v0,
                 GLfloat v1,
                 GLfloat v2,
                 GLfloat v3);

typedef void WINAPI gl_uniform1i(GLint location,
                 GLint v0);

typedef void WINAPI gl_uniform2i(GLint location,
                 GLint v0,
                 GLint v1);

typedef void WINAPI gl_uniform3i(GLint location,
                 GLint v0,
                 GLint v1,
                 GLint v2);

typedef void WINAPI gl_uniform4i(GLint location,
                 GLint v0,
                 GLint v1,
                 GLint v2,
                 GLint v3);

typedef void WINAPI gl_uniform1fv(GLint location,
                  GLsizei count,
                  const GLfloat *value);

typedef void WINAPI gl_uniform2fv(GLint location,
                  GLsizei count,
                  const GLfloat *value);

typedef void WINAPI gl_uniform3fv(GLint location,
                  GLsizei count,
                  const GLfloat *value);

typedef void WINAPI gl_uniform4fv(GLint location,
                  GLsizei count,
                  const GLfloat *value);

typedef void WINAPI gl_uniform1iv(GLint location,
                  GLsizei count,
                  const GLint *value);

typedef void WINAPI gl_uniform2iv(GLint location,
                  GLsizei count,
                  const GLint *value);

typedef void WINAPI gl_uniform3iv(GLint location,
                  GLsizei count,
                  const GLint *value);

typedef void WINAPI gl_uniform4iv(GLint location,
                  GLsizei count,
                  const GLint *value);

typedef void WINAPI gl_uniform_matrix2fv(GLint location,
                                       GLsizei count,
                                       b32 transpose,
                                       const GLfloat *value);

typedef void WINAPI gl_uniform_matrix3fv(GLint location,
                                       GLsizei count,
                                       b32 transpose,
                                       const GLfloat *value);

typedef void WINAPI gl_uniform_matrix4fv(GLint location,
                                       GLsizei count,
                                       b32 transpose,
                                       const GLfloat *value);
typedef void WINAPI gl_active_texture(GLenum texture);


//
// NOTE(zaklaus): OpenGL Functions
//

global_variable gl_bind_framebuffer *glBindFramebuffer;
global_variable gl_gen_framebuffers *glGenFramebuffers;
global_variable gl_framebuffer_texture_2D *glFramebufferTexture2D;
global_variable gl_check_framebuffer_status *glCheckFramebufferStatus;
global_variable gl_gen_vertex_arrays *glGenVertexArrays;
global_variable gl_bind_vertex_array *glBindVertexArray;
global_variable gl_buffer_data *glBufferData;
global_variable gl_buffer_sub_data *glBufferSubData;
global_variable gl_gen_buffers *glGenBuffers;
global_variable gl_bind_buffer *glBindBuffer;
global_variable gl_enable_vertex_attrib_array *glEnableVertexAttribArray;
global_variable gl_disable_vertex_attrib_array *glDisableVertexAttribArray;
global_variable gl_vertex_attrib_pointer *glVertexAttribPointer;
global_variable gl_create_shader *glCreateShader;
global_variable gl_attach_shader *glAttachShader;
global_variable gl_compile_shader *glCompileShader;
global_variable gl_shader_source *glShaderSource;
global_variable gl_delete_shader *glDeleteShader;
global_variable gl_detach_shader *glDetachShader;
global_variable gl_get_shaderiv *glGetShaderiv;
global_variable gl_create_program *glCreateProgram;
global_variable gl_use_program *glUseProgram;
global_variable gl_link_program *glLinkProgram;
global_variable gl_get_programiv *glGetProgramiv;
global_variable gl_get_shader_info_log *glGetShaderInfoLog;
global_variable gl_get_program_info_log *glGetProgramInfoLog;
global_variable gl_get_uniform_location *glGetUniformLocation;

global_variable gl_uniform1f *glUniform1f;
global_variable gl_uniform2f *glUniform2f;
global_variable gl_uniform3f *glUniform3f;
global_variable gl_uniform4f *glUniform4f;
global_variable gl_uniform1i *glUniform1i;
global_variable gl_uniform2i *glUniform2i;
global_variable gl_uniform3i *glUniform3i;
global_variable gl_uniform4i *glUniform4i;
global_variable gl_uniform1fv *glUniform1fv;
global_variable gl_uniform2fv *glUniform2fv;
global_variable gl_uniform3fv *glUniform3fv;
global_variable gl_uniform4fv *glUniform4fv;
global_variable gl_uniform1iv *glUniform1iv;
global_variable gl_uniform2iv *glUniform2iv;
global_variable gl_uniform3iv *glUniform3iv;
global_variable gl_uniform4iv *glUniform4iv;
global_variable gl_uniform_matrix2fv *glUniformMatrix2fv;
global_variable gl_uniform_matrix3fv *glUniformMatrix3fv;
global_variable gl_uniform_matrix4fv *glUniformMatrix4fv;
global_variable gl_active_texture *glActiveTexture;

// ---

typedef BOOL WINAPI wgl_swap_interval_ext(int interval);
typedef const char * WINAPI wgl_get_extensions_string_ext(void);

global_variable wgl_create_context_attribs_arb *wglCreateContextAttribsARB;
global_variable wgl_choose_pixel_format_arb *wglChoosePixelFormatARB;
global_variable wgl_swap_interval_ext *wglSwapIntervalEXT;
global_variable wgl_get_extensions_string_ext *wglGetExtensionsStringEXT;
global_variable b32 OpenGLSupportsSRGBFramebuffer;
global_variable GLuint OpenGLDefaultInternalTextureFormat;
global_variable GLuint OpenGLReservedBlitTexture;
global_variable b32 FramebufferSupportsSRGB;

global_variable s32 Win32OpenGLAttribs[] =
{
    WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
    WGL_CONTEXT_MINOR_VERSION_ARB, 0,
    WGL_CONTEXT_FLAGS_ARB, 0 // NOTE(casey): Enable for testing WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB
#if HANDMADE_INTERNAL
    |WGL_CONTEXT_DEBUG_BIT_ARB
#endif
    ,
    WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
    0,
};


internal opengl_info
Win32OpenGLGetInfo(b32 ModernContext)
{
    opengl_info Result = {0};
    
    Result.ModernContext = ModernContext;
    Result.Vendor = (char *)glGetString(GL_VENDOR);
    Result.Renderer = (char *)glGetString(GL_RENDERER);
    Result.Version = (char *)glGetString(GL_VERSION);
    if(Result.ModernContext)
    {
        Result.ShadingLanguageVersion = (char *)glGetString(GL_SHADING_LANGUAGE_VERSION);
    }
    else
    {
        Result.ShadingLanguageVersion = "(none)";
    }
    
    Result.Extensions = (char *)glGetString(GL_EXTENSIONS);
    
    for(char *At = Result.Extensions;
        *At;
        )
    {
        while(IsWhitespace(*At)) {++At;}
        char *End = At;
        while(*End && !IsWhitespace(*End)) {++End;}
        
        umm Count = End - At;        
        
        if(0) {}
        else if(StringsAreEqualA(Count, At, "GL_EXT_texture_sRGB")) {Result.GL_EXT_texture_sRGB=1;}
        else if(StringsAreEqualA(Count, At, "GL_EXT_framebuffer_sRGB")) {Result.GL_EXT_framebuffer_sRGB=1;}
        else if(StringsAreEqualA(Count, At, "GL_ARB_framebuffer_sRGB")) {Result.GL_EXT_framebuffer_sRGB=1;}
        else if(StringsAreEqualA(Count, At, "GL_ARB_framebuffer_object")) {Result.GL_ARB_framebuffer_object=1;}
        // TODO(casey): Is there some kind of ARB string to look for that indicates GL_EXT_texture_sRGB?
        
        At = End;
    }
    
    char *MajorAt = Result.Version;
    char *MinorAt = 0;
    for(char *At = Result.Version;
        *At;
        ++At)
    {
        if(At[0] == '.')
        {
            MinorAt = At + 1;
            break;
        }
    }
    
    s32 Major = 1;
    s32 Minor = 0;
    if(MinorAt)
    {
        Major = S32FromZ(MajorAt);
        Minor = S32FromZ(MinorAt);
    }
    
    if((Major > 2) || ((Major == 2) && (Minor >= 1)))
    {
        // NOTE(casey): We _believe_ we have sRGB textures in 2.1 and above automatically.
        Result.GL_EXT_texture_sRGB = 1;
    }
    
    return(Result);
}

internal void
Win32SetPixelFormat(HDC WindowDC)
{
     s32 SuggestedPixelFormatIndex = 0;
    GLuint ExtendedPick = 0;
    if(wglChoosePixelFormatARB)
    {
        int IntAttribList[] =
        {
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
            WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
            WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
#if HANDMADE_STREAMING
            WGL_DOUBLE_BUFFER_ARB, GL_FALSE,
#else
            WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
#endif
            WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
            WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, GL_TRUE,
            0,
        };
        
        if(!OpenGLSupportsSRGBFramebuffer)
        {
            IntAttribList[10] = 0;
        }
        
        wglChoosePixelFormatARB(WindowDC, IntAttribList, 0, 1,
                                &SuggestedPixelFormatIndex, &ExtendedPick);
    }
    
    if(!ExtendedPick)
    {
    PIXELFORMATDESCRIPTOR DesiredPixelFormat = {0};
    DesiredPixelFormat.nSize = sizeof(DesiredPixelFormat);
    DesiredPixelFormat.nVersion = 1;
    DesiredPixelFormat.iPixelType = PFD_TYPE_RGBA;
    DesiredPixelFormat.iLayerType = PFD_MAIN_PLANE;
    DesiredPixelFormat.dwFlags = 
        PFD_SUPPORT_OPENGL
        |PFD_DRAW_TO_WINDOW
        |PFD_DOUBLEBUFFER;
    DesiredPixelFormat.cColorBits = 32;
    DesiredPixelFormat.cAlphaBits = 8;
    
     SuggestedPixelFormatIndex = ChoosePixelFormat(WindowDC, &DesiredPixelFormat);
}

PIXELFORMATDESCRIPTOR SuggestedPixelFormat;
DescribePixelFormat(WindowDC, SuggestedPixelFormatIndex, sizeof(SuggestedPixelFormat), &SuggestedPixelFormat);
SetPixelFormat(WindowDC, SuggestedPixelFormatIndex, &SuggestedPixelFormat);
}

internal void
Win32LoadOpenGLExtensions(HDC WindowDC)
{
    Win32SetPixelFormat(WindowDC);
    HGLRC OpenGLRC = wglCreateContext(WindowDC);
    if(wglMakeCurrent(WindowDC, OpenGLRC))
    {
        wglChoosePixelFormatARB =
            (wgl_choose_pixel_format_arb *)wglGetProcAddress("wglChoosePixelFormatARB");
        wglCreateContextAttribsARB =
            (wgl_create_context_attribs_arb *)wglGetProcAddress("wglCreateContextAttribsARB");
        wglSwapIntervalEXT = (wgl_swap_interval_ext *)wglGetProcAddress("wglSwapIntervalEXT");
        wglGetExtensionsStringEXT = (wgl_get_extensions_string_ext *)wglGetProcAddress("wglGetExtensionsStringEXT");
        
        if(wglGetExtensionsStringEXT)
        {
            char *Extensions = (char *)wglGetExtensionsStringEXT();
            char *At = Extensions;
            while(*At)
            {
                while(IsWhitespace(*At)) {++At;}
                char *End = At;
                while(*End && !IsWhitespace(*End)) {++End;}
                
                umm Count = End - At;
                
                if(0) {}
                else if(StringsAreEqualA(Count, At, "WGL_EXT_framebuffer_sRGB")) {OpenGLSupportsSRGBFramebuffer = 1;}
                else if(StringsAreEqualA(Count, At, "WGL_ARB_framebuffer_sRGB")) {OpenGLSupportsSRGBFramebuffer = 1;}
                
                At = End;
            }
            
            wglMakeCurrent(0, 0);
        }
        
        wglDeleteContext(OpenGLRC);
    }
}

internal PROC
Win32OpenGLLoadProc(char *Name)
{
     PROC Result = wglGetProcAddress(Name);
    
    if(!Result)
    {
        HMODULE OpenGL = LoadLibrary("opengl32.lib");
        Result = GetProcAddress(OpenGL, Name);
        FreeLibrary(OpenGL);
    }
    
    return(Result);
}

internal HGLRC
Win32InitOpenGL(HDC WindowDC, b32 *ModernContext)
{
    Win32LoadOpenGLExtensions(WindowDC);
    
    Win32SetPixelFormat(WindowDC);
    
    HGLRC OpenGLRC = 0;
    
    if(wglCreateContextAttribsARB)
    {
        OpenGLRC = wglCreateContextAttribsARB(WindowDC, 0, Win32OpenGLAttribs);
    }
    
    if(!OpenGLRC)
    {
        *ModernContext = 0;
        OpenGLRC = wglCreateContext(WindowDC);
    }
    
    if(wglMakeCurrent(WindowDC, OpenGLRC))
    {
        
        opengl_info Info = Win32OpenGLGetInfo(*ModernContext);
        
        // NOTE(zaklaus): OpenGL Info
            #if PRINT_OPENGL_INFO == 1
        {
            printf("OpenGL Context Load:\n===\n");
            printf("Vendor: %s\nRenderer: %s\nVersion: %s\n",
                   Info.Vendor, Info.Renderer, Info.Version);
            
            if(Info.ModernContext)
            {
                printf("Shading Language Version: %s\n",
                       Info.ShadingLanguageVersion);
            }
            
            #if PRINT_OPENGL_EXTENSIONS == 1
            {
                printf("OpenGL Extensions: %s\n", Info.Extensions);
            }
            #endif
            
            printf("===\n");
        }
            #endif
        // zLOAD
        if(Info.GL_ARB_framebuffer_object)
        {
            glBindFramebuffer = (gl_bind_framebuffer *)Win32OpenGLLoadProc("glBindFramebuffer");
            glGenFramebuffers = (gl_gen_framebuffers *)Win32OpenGLLoadProc("glGenFramebuffers");
            glFramebufferTexture2D = (gl_framebuffer_texture_2D *)Win32OpenGLLoadProc("glFramebufferTexture2D");
            glCheckFramebufferStatus = (gl_check_framebuffer_status *)Win32OpenGLLoadProc("glCheckFramebufferStatus");
            glGenVertexArrays = (gl_gen_vertex_arrays *)Win32OpenGLLoadProc("glGenVertexArrays");
            glBindVertexArray = (gl_bind_vertex_array *)Win32OpenGLLoadProc("glBindVertexArray");
            glBufferData = (gl_buffer_data *)Win32OpenGLLoadProc("glBufferData");
            glBufferSubData = (gl_buffer_sub_data *)Win32OpenGLLoadProc("glBufferSubData");
            glGenBuffers = (gl_gen_buffers *)Win32OpenGLLoadProc("glGenBuffers");
 glBindBuffer = (gl_bind_buffer *)Win32OpenGLLoadProc("glBindBuffer");
            
            glEnableVertexAttribArray = (gl_enable_vertex_attrib_array *)Win32OpenGLLoadProc("glEnableVertexAttribArray");
            glDisableVertexAttribArray = (gl_disable_vertex_attrib_array *)Win32OpenGLLoadProc("glDisableVertexAttribArray");
            glVertexAttribPointer = (gl_vertex_attrib_pointer *)Win32OpenGLLoadProc("glVertexAttribPointer");
            
            glCreateShader = (gl_create_shader *)Win32OpenGLLoadProc("glCreateShader");
            glAttachShader = (gl_attach_shader *)Win32OpenGLLoadProc("glAttachShader");
            glCompileShader = (gl_compile_shader *)Win32OpenGLLoadProc("glCompileShader");
            glShaderSource = (gl_shader_source *)Win32OpenGLLoadProc("glShaderSource");
            glDeleteShader = (gl_delete_shader *)Win32OpenGLLoadProc("glDeleteShader");
            glDetachShader = (gl_detach_shader *)Win32OpenGLLoadProc("glDetachShader");
            glGetShaderiv = (gl_get_shaderiv *)Win32OpenGLLoadProc("glGetShaderiv");
            glCreateProgram = (gl_create_program *)Win32OpenGLLoadProc("glCreateProgram");
            glUseProgram = (gl_use_program *)Win32OpenGLLoadProc("glUseProgram");
            glLinkProgram = (gl_link_program *)Win32OpenGLLoadProc("glLinkProgram");
            glGetProgramiv = (gl_get_programiv *)Win32OpenGLLoadProc("glGetProgramiv");
            glGetShaderInfoLog = (gl_get_shader_info_log *)Win32OpenGLLoadProc("glGetShaderInfoLog");glGetProgramInfoLog = (gl_get_shader_info_log *)Win32OpenGLLoadProc("glGetProgramInfoLog");
            glGetUniformLocation = (gl_get_uniform_location *)Win32OpenGLLoadProc("glGetUniformLocation");
            glActiveTexture = (gl_active_texture *)Win32OpenGLLoadProc("glActiveTexture");
            
            // NOTE(zaklaus): glUniform
            {
                glUniform1f = (gl_uniform1f *)Win32OpenGLLoadProc("glUniform1f");
                glUniform2f = (gl_uniform2f *)Win32OpenGLLoadProc("glUniform2f");
                glUniform3f = (gl_uniform3f *)Win32OpenGLLoadProc("glUniform3f");
                glUniform4f = (gl_uniform4f *)Win32OpenGLLoadProc("glUniform4f");
                glUniform1i = (gl_uniform1i *)Win32OpenGLLoadProc("glUniform1i");
                glUniform2i = (gl_uniform2i *)Win32OpenGLLoadProc("glUniform2i");
                glUniform3i = (gl_uniform3i *)Win32OpenGLLoadProc("glUniform3i");
                glUniform4i = (gl_uniform4i *)Win32OpenGLLoadProc("glUniform4i");
                glUniform1fv = (gl_uniform1fv *)Win32OpenGLLoadProc("glUniform1fv");
                glUniform2fv = (gl_uniform2fv *)Win32OpenGLLoadProc("glUniform2fv");
                glUniform3fv = (gl_uniform3fv *)Win32OpenGLLoadProc("glUniform3fv");
                glUniform4fv = (gl_uniform4fv *)Win32OpenGLLoadProc("glUniform4fv");
                glUniform1iv = (gl_uniform1iv *)Win32OpenGLLoadProc("glUniform1iv");
                glUniform2iv = (gl_uniform2iv *)Win32OpenGLLoadProc("glUniform2iv");
                glUniform3iv = (gl_uniform3iv *)Win32OpenGLLoadProc("glUniform3iv");
                glUniform4iv = (gl_uniform4iv *)Win32OpenGLLoadProc("glUniform4iv");
                glUniformMatrix2fv = (gl_uniform_matrix2fv *)Win32OpenGLLoadProc("glUniformMatrix2fv");
                glUniformMatrix3fv = (gl_uniform_matrix3fv *)Win32OpenGLLoadProc("glUniformMatrix3fv");
                glUniformMatrix4fv = (gl_uniform_matrix4fv *)Win32OpenGLLoadProc("glUniformMatrix4fv");
            }
        }
        
        if(wglSwapIntervalEXT)
        {
            wglSwapIntervalEXT(1);
        }
        
        // NOTE(casey): If we believe we can do full sRGB on the texture side
        // and the framebuffer side, then we can enable it, otherwise it is
        // safer for us to pass it straight through.
        OpenGLDefaultInternalTextureFormat = GL_RGBA8;
        if(FramebufferSupportsSRGB && Info.GL_EXT_texture_sRGB && Info.GL_EXT_framebuffer_sRGB)
        {
            OpenGLDefaultInternalTextureFormat = GL_SRGB8_ALPHA8;
            glEnable(GL_FRAMEBUFFER_SRGB);
        }
        
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    }
    else
    {
        InvalidCodePath;
    }
    
    
    return(OpenGLRC);
}

#define WIN32_OPENGL_H
#endif