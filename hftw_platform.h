// (c) 2016 ZaKlaus; All Rights Reserved

#if !defined(HFTW_PLATFORM_H)

// NOTE(zaklaus): Lazy solution for MM.
#ifdef PLATFORM_ALLOC
#error Platform-specific MM is not yet implemented
#else//LIBC_MALLOC
#include <malloc.h>

doc(PlatformMemAlloc)
doc_string(Allocates memory using platform-specific call.)
doc_sig(
inline void *
PlatformMemAlloc(
size_t Size)  // Requested memory size.
)
{
    void *Result = 0;
    Result = malloc(Size);
    
    return(Result);
}

doc_cat(Platform)

doc(PlatformMemFree)
doc_string(Free memory using platform-specific call.)
doc_sig(
inline void
PlatformMemFree(
void *Ptr)   // Pointer to allocated block of memory.
)
{
    free(Ptr);
}

doc(PlatformMemRealloc)
doc_string(Reallocates memory by requested size using platform-specific call.)
doc_sig(
inline void *
PlatformMemRealloc(void *Ptr,   // Pointer to allocated block of memory.
                   size_t Size) // Requested memory size.
)
{
    void *Result = Ptr;
    Result = realloc(Result, Size);
    return(Result);
}
#endif

#define HFTW_PLATFORM_H
#endif