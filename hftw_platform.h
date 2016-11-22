// (c) 2016 ZaKlaus; All Rights Reserved

#if !defined(HFTW_PLATFORM_H)

// NOTE(zaklaus): Lazy solution for MM.
#ifdef PLATFORM_ALLOC
#error Platform-specific MM is not yet implemented
#else//LIBC_MALLOC
#include <malloc.h>

inline void *
PlatformMemAlloc(size_t Size)
{
    void *Result = 0;
    Result = malloc(Size);
    
    return(Result);
}

inline void
PlatformMemFree(void *Ptr)
{
    free(Ptr);
}

inline void *
PlatformMemRealloc(void *Ptr, size_t Size)
{
    void *Result = Ptr;
    Result = realloc(Result, Size);
    return(Result);
}
#endif

#define HFTW_PLATFORM_H
#endif