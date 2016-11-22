// (c) 2016 ZaKlaus; All Rights Reserved

#if !defined(HFTW_MEM_H)

typedef struct
{
    memory_index Size;
    uint8 *Base;
    memory_index Used;

    int32 TempCount;
} Memory_Arena;

typedef struct
{
    Memory_Arena *Arena;
    memory_index Used;
} Temp_Memory;

inline void
InitializeArena(Memory_Arena *Arena, memory_index Size, void *Base)
{
    Arena->Size = Size;
    Arena->Base = (uint8 *)Base;
    Arena->Used = 0;
    Arena->TempCount = 0;
}

inline void
BuildArena(Memory_Arena *Arena, memory_index Size)
{
    void *Base = PlatformMemAlloc(Size);
    InitializeArena(Arena, Size, Base);
}

inline memory_index
GetAlignmentOffset(Memory_Arena *Arena, memory_index Alignment)
{
    memory_index AlignmentOffset = 0;
    
    memory_index ResultPointer = (memory_index)Arena->Base + Arena->Used;
    memory_index AlignmentMask = Alignment - 1;
    if(ResultPointer & AlignmentMask)
    {
        AlignmentOffset = Alignment - (ResultPointer & AlignmentMask);
    }

    return(AlignmentOffset);
}

typedef enum
{
    ArenaFlag_ClearToZero = 0x1,
} arena_push_flag;

typedef struct
{
    u32 Flags;
    u32 Alignment;
} arena_push_params;

inline arena_push_params
DefaultArenaParams(void)
{
    arena_push_params Params;
    Params.Flags = ArenaFlag_ClearToZero;
    Params.Alignment = 4;
    return(Params);
}

inline arena_push_params
AlignNoClear(u32 Alignment)
{
    arena_push_params Params = DefaultArenaParams();
    Params.Flags &= ~ArenaFlag_ClearToZero;
    Params.Alignment = Alignment;
    return(Params);
}

inline arena_push_params
Align(u32 Alignment, b32 Clear)
{
    arena_push_params Params = DefaultArenaParams();
    if(Clear)
    {
        Params.Flags |= ArenaFlag_ClearToZero;
    }
    else
    {
        Params.Flags &= ~ArenaFlag_ClearToZero;
    }
    Params.Alignment = Alignment;
    return(Params);
}

inline arena_push_params
NoClear(void)
{
    arena_push_params Params = DefaultArenaParams();
    Params.Flags &= ~ArenaFlag_ClearToZero;
    return(Params);
}

inline memory_index
GetArenaSizeRemaining(Memory_Arena *Arena, arena_push_params Params)
{
    memory_index Result = Arena->Size - (Arena->Used + GetAlignmentOffset(Arena, Params.Alignment));

    return(Result);
}

// TODO(casey): Optional "clear" parameter!!!!
#define PushStruct(Arena, type, ...) (type *)PushSize_(Arena, sizeof(type), ## __VA_ARGS__)
#define PushArray(Arena, Count, type, ...) (type *)PushSize_(Arena, (Count)*sizeof(type), ## __VA_ARGS__)
#define PushSize(Arena, Size, ...) PushSize_(Arena, Size, ## __VA_ARGS__)
#define PushCopy(Arena, Size, Source, ...) Copy(Size, Source, PushSize_(Arena, Size, ## __VA_ARGS__))
#define PushType PushStruct
inline memory_index
GetEffectiveSizeFor(Memory_Arena *Arena, memory_index SizeInit, arena_push_params Params)
{
    memory_index Size = SizeInit;
        
    memory_index AlignmentOffset = GetAlignmentOffset(Arena, Params.Alignment);
    Size += AlignmentOffset;

    return(Size);
}

inline b32
ArenaHasRoomFor(Memory_Arena *Arena, memory_index SizeInit, arena_push_params Params)
{
    memory_index Size = GetEffectiveSizeFor(Arena, SizeInit, Params);
    b32 Result = ((Arena->Used + Size) <= Arena->Size);
    return(Result);
}

inline void *
PushSize_(Memory_Arena *Arena, memory_index SizeInit, arena_push_params Params)
{
    memory_index Size = GetEffectiveSizeFor(Arena, SizeInit, Params);
    
    Assert((Arena->Used + Size) <= Arena->Size);

    memory_index AlignmentOffset = GetAlignmentOffset(Arena, Params.Alignment);
    void *Result = Arena->Base + Arena->Used + AlignmentOffset;
    Arena->Used += Size;

    Assert(Size >= SizeInit);

    if(Params.Flags & ArenaFlag_ClearToZero)
    {
        ZeroSize(SizeInit, Result);
    }
    
    return(Result);
}

// NOTE(casey): This is generally not for production use, this is probably
// only really something we need during testing, but who knows
inline char *
PushString(Memory_Arena *Arena, char *Source)
{
    u32 Size = 1;
    for(char *At = Source;
        *At;
        ++At)
    {
        ++Size;
    }
    
    char *Dest = (char *)PushSize_(Arena, Size, NoClear());
    for(u32 CharIndex = 0;
        CharIndex < Size;
        ++CharIndex)
    {
        Dest[CharIndex] = Source[CharIndex];
    }

    return(Dest);
}

inline char *
PushAndNullTerminate(Memory_Arena *Arena, u32 Length, char *Source)
{
    char *Dest = (char *)PushSize_(Arena, Length + 1, NoClear());
    for(u32 CharIndex = 0;
        CharIndex < Length;
        ++CharIndex)
    {
        Dest[CharIndex] = Source[CharIndex];
    }
    Dest[Length] = 0;

    return(Dest);
}

inline Temp_Memory
BeginTemporaryMemory(Memory_Arena *Arena)
{
    Temp_Memory Result;

    Result.Arena = Arena;
    Result.Used = Arena->Used;

    ++Arena->TempCount;

    return(Result);
}

inline void
EndTemporaryMemory(Temp_Memory TempMem)
{
    Memory_Arena *Arena = TempMem.Arena;
    Assert(Arena->Used >= TempMem.Used);
    Arena->Used = TempMem.Used;
    Assert(Arena->TempCount > 0);
    --Arena->TempCount;
}

inline void
Clear(Memory_Arena *Arena)
{
    InitializeArena(Arena, Arena->Size, Arena->Base);
}

inline void
CheckArena(Memory_Arena *Arena)
{
    Assert(Arena->TempCount == 0);
}

inline void
SubArena(Memory_Arena *Result, Memory_Arena *Arena, memory_index Size, arena_push_params Params)
{
    Result->Size = Size;
    Result->Base = (uint8 *)PushSize_(Arena, Size, Params);
    Result->Used = 0;
    Result->TempCount = 0;
}

inline void *
Copy(memory_index Size, void *SourceInit, void *DestInit)
{
    u8 *Source = (u8 *)SourceInit;
    u8 *Dest = (u8 *)DestInit;
    while(Size--) {*Dest++ = *Source++;}

    return(DestInit);
}

inline void
CleanUnusedArenaRoom(Memory_Arena * Arena)
{
    u8 * Base = Arena->Base + Arena->Used - 1;
    
    while(Base < (Arena->Base + Arena->Size))
    {
        *(Base++) = 0;
    }
}

inline void
FreeArena(Memory_Arena * Arena)
{
    Assert(Arena->TempCount == 0);
    
    PlatformMemFree(Arena->Base);
    Arena->Used = Arena->Size = 0;
}

#define HFTW_MEM_H
#endif