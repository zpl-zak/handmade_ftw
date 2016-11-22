// (c) 2016 ZaKlaus; All Rights Reserved

#if !defined(HFTW_MEM_H)

typedef struct
{
    s32 Size;
    u32 Tag;
    u32 Offset;
} arena_header;

LinkedList(arena_header)

typedef struct
{
    memory_index Size;
    uint8 *Base;
    memory_index Used;
    u8 Flags;
    b32 WasExpanded;

    s32 TempCount;
    
    Node_arena_header *Header, *HeaderEnd;
} Memory_Arena;

typedef enum
{
    ArenaFlag_AllowRealloc = 0x1,
    ArenaFlag_AllowHeaders = 0x01,
} arena_flags;

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
    Arena->WasExpanded = 0;
    Arena->TempCount = 0;
    Arena->Flags = 0;
    Arena->Header = 0;
        Arena->HeaderEnd = 0;
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
    ArenaPushFlag_ClearToZero = 0x1,
} arena_push_flag;

typedef struct
{
    u32 Flags;
    u32 Alignment;
    u32 Expectation;
    u32 Tag;
} arena_push_params;

inline arena_push_params
DefaultArenaParams(void)
{
    arena_push_params Params;
    Params.Flags = ArenaPushFlag_ClearToZero;
    Params.Alignment = 4;
    Params.Expectation = 0;
    Params.Tag = 0;
    return(Params);
}

inline arena_push_params
AlignNoClear(u32 Alignment)
{
    arena_push_params Params = DefaultArenaParams();
    Params.Flags &= ~ArenaPushFlag_ClearToZero;
    Params.Alignment = Alignment;
    return(Params);
}

inline arena_push_params
Align(u32 Alignment, b32 Clear)
{
    arena_push_params Params = DefaultArenaParams();
    if(Clear)
    {
        Params.Flags |= ArenaPushFlag_ClearToZero;
    }
    else
    {
        Params.Flags &= ~ArenaPushFlag_ClearToZero;
    }
    Params.Alignment = Alignment;
    return(Params);
}

inline arena_push_params
NoClear(void)
{
    arena_push_params Params = DefaultArenaParams();
    Params.Flags &= ~ArenaPushFlag_ClearToZero;
    return(Params);
}

inline arena_push_params
Expect(u32 Expectation, b32 Clear)
{
    arena_push_params Params = Align(4, Clear);
    Params.Expectation = Expectation;
    return(Params);
}

inline arena_push_params
AlignExpect(u32 Alignment, u32 Expectation, b32 Clear)
{
    arena_push_params Params = Align(Alignment, Clear);
    Params.Expectation = Expectation;
    return(Params);
}

inline arena_push_params
Tag(u32 Tag, arena_push_params Rest)
{
    arena_push_params Params = Rest;
    Params.Tag = Tag;
    return(Params);
}

inline memory_index
GetArenaSizeRemaining(Memory_Arena *Arena, arena_push_params Params)
{
    memory_index Result = Arena->Size - (Arena->Used + GetAlignmentOffset(Arena, Params.Alignment));

    return(Result);
}


#define GetBlock(arena, type, idx) \
(type *)&(arena->Base)[sizeof(type)*idx]

inline void *
GetBlockByRecord(Memory_Arena *Arena, size_t Index)
{
    Assert(Arena->Header && "Arena headers aren't enabled!");
      u8 *Result = Arena->Base;
    size_t Idx = 0;
    for(Node_arena_header *Record = Arena->Header->Next;
        Record;
        Record = Record->Next)
{
    if(Idx++ == Index)
        break;
    Result += Record->Value.Size;
}
return(Result);
}

typedef struct
{
    u8 *Value;
    Node_arena_header *Node;
} tag_scan_result;

inline tag_scan_result
DefaultTagScan(void)
{
    tag_scan_result Result = {0};
    return(Result);
}

inline tag_scan_result
GetBlockByTagAndRecord(Memory_Arena *Arena, tag_scan_result scan, u32 Tag)
{
    if(!scan.Node)
        scan.Node = Arena->Header;
    Assert(scan.Node && "Arena headers aren't enabled!");
    tag_scan_result NewResult = {0};
    u8 *Result = Arena->Base + scan.Node->Value.Offset;
    for(Node_arena_header *Record = scan.Node;
        Record;
        Record = Record->Next)
    {
        if(Record->Value.Tag == Tag)
        {
            NewResult.Value = Result;
            NewResult.Node = Record->Next;
            break;
        }
        Result += Record->Value.Size;
    }
    return(NewResult);
}

#define GetVaryBlock(arena, type, idx) \
(type *)(GetBlockByRecord(arena,(size_t)idx))

#define GetVaryBlockTagValue(arena, scan, type, tag) \
(type *)(GetBlockByTagAndRecord(arena, scan, tag).Value)

#define GetVaryBlockTagResult(arena, scan, tag) \
 GetBlockByTagAndRecord(arena, scan, tag)


#define PushStruct(Arena, type, ...) (type *)PushSize_(Arena, sizeof(type), ## __VA_ARGS__)
#define PushArray(Arena, Count, type, ...) (type *)PushSize_(Arena, (Count)*sizeof(type), ## __VA_ARGS__)
#define PushSize(Arena, Size, ...) PushSize_(Arena, Size, ## __VA_ARGS__)
#define PushCopy(Arena, Size, Source, ...) Copy(Size, Source, PushSize_(Arena, Size, ## __VA_ARGS__))
#define PushType PushStruct
#define PushValue(Arena, type, Value, ...) *((type *) PushType(Arena, type, ## __VA_ARGS__)) = Value

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

inline void
ArenaExpand(Memory_Arena *Arena, memory_index Size)
{
    if(!((Arena->Used + Size) <= Arena->Size))
    {
        if(Arena->Flags & ArenaFlag_AllowRealloc)
        {
            Arena->Base = PlatformMemRealloc(Arena->Base, Arena->Size + Size);
            Arena->Size += Size;
            Arena->WasExpanded = 1;
            Assert((Arena->Used + Size) <= Arena->Size);
        }
        else
        {
            Assert(!"Not enough memory in Memory_Arena!");
        }
    }
    else
    {
        Arena->WasExpanded = 0;
    }
}

inline void *
PushSize_(Memory_Arena *Arena, memory_index SizeInit, arena_push_params Params)
{
    memory_index Size = GetEffectiveSizeFor(Arena, SizeInit, Params);
    
    ArenaExpand(Arena, Size + Params.Expectation);

    memory_index AlignmentOffset = GetAlignmentOffset(Arena, Params.Alignment);
    void *Result = Arena->Base + Arena->Used + AlignmentOffset;
    Arena->Used += Size;

    Assert(Size >= SizeInit);

    if(Params.Flags & ArenaPushFlag_ClearToZero)
    {
        ZeroSize(SizeInit, Result);
    }
    
    if(Params.Flags & ArenaFlag_AllowHeaders)
    {
        arena_header header = {0};
        header.Tag = Params.Tag;
        header.Size = (s32)SizeInit;
        header.Offset = (u32)Arena->Used - (u32)Size;
        if(!Arena->Header)
        {
            arena_header dummy = {0};
            Arena->Header = NewNode_arena_header(dummy);
            Arena->HeaderEnd = NewNode_arena_header(header);
            Arena->Header->Next = Arena->HeaderEnd;
        }
        else
        {
            Arena->HeaderEnd = AddNode_arena_header(Arena->HeaderEnd, header);
        }
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