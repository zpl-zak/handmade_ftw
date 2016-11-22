// (c) 2016 ZaKlaus; All Rights Reserved

#if !defined(HFTW_H)

// TODO(casey): Have the meta-parser ignore its own #define
#define introspect(IGNORED)
#define counted_pointer(IGNORED)

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


#ifdef __cplusplus
extern "C" {
#endif

//
// NOTE(casey): Compilers
//
    
#if !defined(COMPILER_MSVC)
#define COMPILER_MSVC 0
#endif
    
#if !defined(COMPILER_LLVM)
#define COMPILER_LLVM 0
#endif
    
#if !COMPILER_MSVC && !COMPILER_LLVM
#if _MSC_VER
#undef COMPILER_MSVC
#define COMPILER_MSVC 1
#else
    // TODO(casey): Moar compilerz!!!
#undef COMPILER_LLVM
#define COMPILER_LLVM 1
#endif
#endif
    
#if COMPILER_MSVC
#include <intrin.h>
#elif COMPILER_LLVM
#include <x86intrin.h>
#else
#error SEE/NEON optimizations are not available for this compiler yet!!!!
#endif
    
//
// NOTE(casey): Types
//
#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include <float.h>

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

typedef uintptr_t umm;

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
// TODO(casey): Complete assertion macro - don't worry everyone!

#ifdef COMPILER_MSVC
#define TRAP() *(int *)0 = 0
#elif COMPILER_LLVM
#define TRAP() __builtin_trap()
#else
#define TRAP() volatile *(int *)0 = 0
#endif

#define Assert(Expression) if(!(Expression)) {TRAP();}
#else
#define Assert(Expression)
#endif

#define InvalidCodePath Assert(!"InvalidCodePath")
#define InvalidDefaultCase default: {InvalidCodePath;} break

#define Kilobytes(Value) ((Value)*1024LL)
#define Megabytes(Value) (Kilobytes(Value)*1024LL)
#define Gigabytes(Value) (Megabytes(Value)*1024LL)
#define Terabytes(Value) (Gigabytes(Value)*1024LL)

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))
// TODO(casey): swap, min, max ... macros???

#define AlignPow2(Value, Alignment) ((Value + ((Alignment) - 1)) & ~((Alignment) - 1))
#define Align4(Value) ((Value + 3) & ~3)
#define Align8(Value) ((Value + 7) & ~7)
#define Align16(Value) ((Value + 15) & ~15)

inline uint32
SafeTruncateUInt64(uint64 Value)
{
    // TODO(casey): Defines for maximum values
    Assert(Value <= 0xFFFFFFFF);
    uint32 Result = (uint32)Value;
    return(Result);
}

inline u16
SafeTruncateToU16(uint32 Value)
{
    // TODO(casey): Defines for maximum values
    Assert(Value <= 0xFFFF);
    u16 Result = (u16)Value;
    return(Result);
}


inline void
ZeroSize(memory_index Size, void *Ptr)
{
    // TODO(casey): Check this guy for performance
    uint8 *Byte = (uint8 *)Ptr;
    while(Size--)
    {
        *Byte++ = 0;
    }
}

#if COMPILER_MSVC
#define CompletePreviousReadsBeforeFutureReads _ReadBarrier()
#define CompletePreviousWritesBeforeFutureWrites _WriteBarrier()
inline uint32 AtomicCompareExchangeUInt32(uint32 volatile *Value, uint32 New, uint32 Expected)
{
    uint32 Result = _InterlockedCompareExchange((long volatile *)Value, New, Expected);

    return(Result);
}
inline u64 AtomicExchangeU64(u64 volatile *Value, u64 New)
{
    u64 Result = _InterlockedExchange64((__int64 volatile *)Value, New);

    return(Result);
}
inline u64 AtomicAddU64(u64 volatile *Value, u64 Addend)
{
    // NOTE(casey): Returns the original value _prior_ to adding
    u64 Result = _InterlockedExchangeAdd64((__int64 volatile *)Value, Addend);

    return(Result);
}    
inline u32 GetThreadID(void)
{
    u8 *ThreadLocalStorage = (u8 *)__readgsqword(0x30);
    u32 ThreadID = *(u32 *)(ThreadLocalStorage + 0x48);

    return(ThreadID);
}

#elif COMPILER_LLVM
// TODO(casey): Does LLVM have real read-specific barriers yet?
#define CompletePreviousReadsBeforeFutureReads asm volatile("" ::: "memory")
#define CompletePreviousWritesBeforeFutureWrites asm volatile("" ::: "memory")
inline uint32 AtomicCompareExchangeUInt32(uint32 volatile *Value, uint32 New, uint32 Expected)
{
    uint32 Result = __sync_val_compare_and_swap(Value, Expected, New);

    return(Result);
}
inline u64 AtomicExchangeU64(u64 volatile *Value, u64 New)
{
    u64 Result = __sync_lock_test_and_set(Value, New);

    return(Result);
}
inline u64 AtomicAddU64(u64 volatile *Value, u64 Addend)
{
    // NOTE(casey): Returns the original value _prior_ to adding
    u64 Result = __sync_fetch_and_add(Value, Addend);

    return(Result);
}    
inline u32 GetThreadID(void)
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
// TODO(casey): Other compilers/platforms??
#endif

#ifdef __cplusplus
}
#endif

#include "hftw_platform.h"
#include "hftw_node.h"

#include "hftw_mem.h"

#define HANDMADE_MATH_IMPLEMENTATION

#include "hftw_math.h"
#include "hftw_intrinsics.h"
#include "hftw_shared.h"

#include "hftw_string.h"

#define HFTW_H
#endif