// (c) 2016 ZaKlaus; All Rights Reserved

#if !defined(HFTW_PLATFORM_H)

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#ifdef _WIN32
#include <malloc.h>
#endif

#if 0

doc(PlatformMemAlloc)
doc_string(Allocates memory using platform-specific call.)

internal void *
PlatformMemAlloc(
size_t Size)  // Requested memory size.
{
    void *Result = 0;
    
    #ifdef _WIN32
    //Result = VirtualAlloc(0, Size, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
    Result = mmalloc(Size);
    #else
    Result = mmalloc(Size);
    #endif
    
    return(Result);
}

doc_cat(Platform)

doc(PlatformMemFree)
doc_string(Free memory using platform-specific call.)

internal void
PlatformMemFree(
void *Ptr)   // Pointer to allocated block of memory.
{
    #ifdef _WIN32
    //VirtualFree(Ptr, 0, MEM_RELEASE);
    free(Ptr);
    #else
    free(Ptr);
    #endif
}

doc(PlatformMemRealloc)
doc_string(Reallocates memory by requested size using platform-specific call.)

internal void *
PlatformMemRealloc(void *Ptr,   // Pointer to allocated block of memory.
                   size_t Size) // Requested memory size.
{
    void *Result = Ptr;
    Result = mrealloc(Result, Size);
    return(Result);
}

#endif

#define PlatformMemAlloc mmalloc
#define PlatformMemFree mfree
#define PlatformMemRealloc mrealloc

doc(PlatformMemMove)
doc_string(Moves the content of the memory to a new block.)

internal void *
PlatformMemMove(void *Ptr, ms NewSize)
{
    void *Result = PlatformMemAlloc(NewSize);
    Copy(NewSize, Ptr, Result);
    PlatformMemFree(Ptr);
    
    return(Result);
}

#ifdef _WIN32
#pragma comment(lib,"user32.lib") 
#pragma comment(lib,"gdi32.lib") 
#include "platform/win32_time.h"
#include "platform/win32_window.h"
#include "platform/win32_opengl.h"
#include "platform/win32_crt_math.cpp"
#include "platform/win32_crt_float.cpp"
#include "platform/win32_crt_memory.cpp"

internal LONGLONG 
GetFilePointerEx(HANDLE FileHandle)
{
    LARGE_INTEGER Offset={0};
    LARGE_INTEGER Position={0};
    SetFilePointerEx(FileHandle, Offset, &Position, FILE_CURRENT);
    return(Position.QuadPart);
}
#endif

#define HFTW_PLATFORM_H
#endif