// (c) 2016 ZaKlaus; All Rights Reserved

#if !defined(HFTW_H)

#define introspect(IGNORED)
#define counted_pointer(IGNORED)

// NOTE(zaklaus): These are used for documentating the code.
#define doc_ignore()
#define doc(...)
#define doc_cat(...)
#define doc_string(...)
#define doc_end
#define doc_example(...)
#define doc_ret(...) 
#define doc_tags(...)
#define doc_embed(...)
//#define doc_sig(...) __VA_ARGS__
#define doc_sep()
#define doc_hunt()

/*
  NOTE(casey):

  HANDMADE_INTERNAL:
    0 - Build for public release
    1 - Build for developer only

  HANDMADE_SLOW:
    0 - Not slow code allowed!
    1 - Slow code welcome.
*/

#define Minimum(A, B) ((A < B) ? (A) : (B))
#define Maximum(A, B) ((A > B) ? (A) : (B))

#define ZeroStruct(Instance) ZeroSize(sizeof(Instance), &(Instance))
#define ZeroArray(Count, Pointer) ZeroSize(Count*sizeof((Pointer)[0]), Pointer)

#define CSize(type, Count) sizeof(type)*Count
#define Count(array) sizeof(array)/sizeof(array[0])

//
// NOTE(casey): Compilers
//

#ifdef _MSC_VER
#define COMPILER_MSVC 1
#elif __clang__
#define COMPILER_LLVM 1
#endif

#if COMPILER_MSVC
#include <intrin.h>
#elif COMPILER_LLVM
#include <x86intrin.h>
#else
#error SEE/NEON optimizations are not available for this compiler yet!!!!
#endif
    
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include <float.h>

doc_ignore()

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef int32 bool32;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef intptr_t intptr;
typedef uintptr_t uintptr;

typedef size_t memory_index;
typedef size_t mi;
    typedef size_t ms;
    typedef size_t ml;

typedef float real32;
typedef double real64;

typedef int8 s8;
typedef int8 s08;
typedef int16 s16;
typedef int32 s32;
typedef int64 s64;
typedef bool32 b32;

typedef uint8 u8;
typedef uint8 u08;
typedef uint16 u16;
typedef uint32 u32;
typedef uint64 u64;

typedef real32 r32;
typedef real64 r64;
    typedef r32 f32;
    typedef r64 f64;

typedef uintptr_t umm;

doc_hunt()

#define U32FromPointer(Pointer) ((u32)(memory_index)(Pointer))
#define PointerFromU32(type, Value) (type *)((memory_index)Value)

#define Real32Maximum FLT_MAX
#define Real32Minimum -FLT_MAX

#if !defined(internal)
#define internal static
#endif
#define local_persist static
#define global_variable static

#define Pi32 3.14159265359f
#define Tau32 6.28318530717958647692f

#ifdef HANDMADE_SLOW
    
#ifdef COMPILER_MSVC
#define TRAP() __debugbreak()
#elif COMPILER_LLVM
#define TRAP() __builtin_trap()
#else
#define TRAP() volatile *(int *)0 = 0
#endif

#define mmalloc(Size) malloc(Size);fprintf(stderr, "malloc: %zu\t<%s@%d:%s>\n", (ms)Size, __FILE__, __LINE__, __FUNCTION__)
#define mfree(Ptr) free(Ptr);fprintf(stderr, "free: \t\t<%s@%d:%s>\n", __FILE__, __LINE__, __FUNCTION__)
#define mrealloc(Ptr, Size) realloc(Ptr, Size);fprintf(stderr, "realloc: %zu\t<%s@%d:%s>\n", (ms)Size, __FILE__, __LINE__, __FUNCTION__)

#define Assert(Expression) if(!(Expression)) {TRAP();}
#else
#define Assert(Expression)
#define mmalloc(Size) malloc(Size)
#define mfree(Ptr) free(Ptr)
#define mrealloc(Ptr, Size) realloc(Ptr, Size)
#endif

#define FSM(name) switch(name)
#define State(name) s##name :
#define NextState(name) goto s##name

#define InvalidCodePath Assert(!"InvalidCodePath")
#define InvalidDefaultCase default: {InvalidCodePath;} break

#define Kilobytes(Value) ((Value)*1024LL)
#define Megabytes(Value) (Kilobytes(Value)*1024LL)
#define Gigabytes(Value) (Megabytes(Value)*1024LL)
#define Terabytes(Value) (Gigabytes(Value)*1024LL)

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))
#define StringCase(Name) case Name: {return(#Name);}
#define Min(a,b) (a < b) ? a : b
#define Max(a,b) (a > b) ? a : b
#define Swap(a,b) a = a ^ b; b = a ^ b; a = a ^ b

#define SwapTypeDeclare(type) global_variable type __HFTW_SWAP##type
#define SwapType(a,b,type, tname) __HFTW_SWAP##type = a; a = b; b = __HFTW_SWAP##type
    
#define AlignPow2(Value, Alignment) ((Value + ((Alignment) - 1)) & ~((Alignment) - 1))
#define Align4(Value) ((Value + 3) & ~3)
#define Align8(Value) ((Value + 7) & ~7)
#define Align16(Value) ((Value + 15) & ~15)
#define Maximum64Value 0xFFFFFFFF
    #define Maximum32Value 0xFFFF
#define Maximum16Value 0xFF
#define Maximum8Value 0xF

internal uint32
SafeTruncateUInt64(uint64 Value)
{
    Assert(Value <= Maximum64Value);
    uint32 Result = (uint32)Value;
    return(Result);
}

internal u16
SafeTruncateToU16(uint32 Value)
{
    Assert(Value <= Maximum32Value);
    u16 Result = (u16)Value;
    return(Result);
}


internal void
ZeroSize(memory_index Size, void *Ptr)
{
    uint8 *Byte = (uint8 *)Ptr;
    while(Size--)
    {
        *Byte++ = 0;
    }
}

#if COMPILER_MSVC
#define CompletePreviousReadsBeforeFutureReads _ReadBarrier()
#define CompletePreviousWritesBeforeFutureWrites _WriteBarrier()
internal uint32 AtomicCompareExchangeUInt32(uint32 volatile *Value, uint32 New, uint32 Expected)
{
    uint32 Result = _InterlockedCompareExchange((long volatile *)Value, New, Expected);

    return(Result);
}
internal u64 AtomicExchangeU64(u64 volatile *Value, u64 New)
{
    u64 Result = _InterlockedExchange64((__int64 volatile *)Value, New);

    return(Result);
}
internal u64 AtomicAddU64(u64 volatile *Value, u64 Addend)
{
    // NOTE(casey): Returns the original value _prior_ to adding
    u64 Result = _InterlockedExchangeAdd64((__int64 volatile *)Value, Addend);

    return(Result);
}    
internal u32 GetThreadID(void)
{
    u8 *ThreadLocalStorage = (u8 *)__readgsqword(0x30);
    u32 ThreadID = *(u32 *)(ThreadLocalStorage + 0x48);

    return(ThreadID);
}

#elif COMPILER_LLVM
#define CompletePreviousReadsBeforeFutureReads asm volatile("" ::: "memory")
#define CompletePreviousWritesBeforeFutureWrites asm volatile("" ::: "memory")
internal uint32 AtomicCompareExchangeUInt32(uint32 volatile *Value, uint32 New, uint32 Expected)
{
    uint32 Result = __sync_val_compare_and_swap(Value, Expected, New);

    return(Result);
}
internal u64 AtomicExchangeU64(u64 volatile *Value, u64 New)
{
    u64 Result = __sync_lock_test_and_set(Value, New);

    return(Result);
}
internal u64 AtomicAddU64(u64 volatile *Value, u64 Addend)
{
    // NOTE(casey): Returns the original value _prior_ to adding
    u64 Result = __sync_fetch_and_add(Value, Addend);

    return(Result);
}    
internal u32 GetThreadID(void)
{
    u32 ThreadID;
#if defined(__APPLE__) && defined(__x86_64__)
    asm("mov %%gs:0x00,%0" : "=r"(ThreadID));
#elif defined(__i386__)
    asm("mov %%gs:0x08,%0" : "=r"(ThreadID));
#elif defined(__x86_64__)
    asm("mov %%fs:0x10,%0" : "=r"(ThreadID));
#else
#error Unsupported architecture
#endif

    return(ThreadID);
}
#else
#endif

#include "hftw_math.h"
#include "hftw_random.h"
#include "hftw_shared.h"
#include "hftw_platform.h"
#include "hftw_node.h"
#include "hftw_mem.h"
#include "hftw_intrinsics.h"
#include "hftw_simd.h"
#include "hftw_string.h"
#include "hftw_io.h"
#include "hftw_henc.h"

#define HFTW_H
#endif
