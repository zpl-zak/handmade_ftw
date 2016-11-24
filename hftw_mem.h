// (c) 2016 ZaKlaus; All Rights Reserved

#if !defined(HFTW_MEM_H)

doc_cat(Memory Arena)

doc(arena_header)
doc_string(Describes arena elements.)
doc_sig(
typedef struct
{
    s32 Size;    // Size of the element.
    u32 Tag;     // Element's tag used for identification.
    u32 Offset;  // Memory offset from the base.
} arena_header;
)

LinkedList(arena_header)

doc(memory_arena)
doc_string(Memory arena holding contigous block of memory and tracking of pushed objects.)
doc_sig(
typedef struct
{
    memory_index Size;             // Allocated memory size. 
    uint8 *Base;                   // Memory base pointer. (Points to a contigous block of memory.)
    memory_index Used;             // Memory already used by elements.
    u8 Flags;                      // Arena's flags.
    b32 WasExpanded;               // If reallocation is allowed, this signals us whether memory expansion has happened.

    s32 TempCount;                 // Counts how many times is our arena used by temp_memory.
    
    Node_arena_header *Header;    // Linked list of tracked elements.
    Node_arena_header *HeaderEnd;
} memory_arena;
)

doc(arena_flags)
doc_string(Arena flags.)
doc_sig(
typedef enum
{
    ArenaFlag_AllowRealloc = 0x1,    // Allows re-allocation of the memory, if expansion is required.
    ArenaFlag_DisallowHeaders = 0x2, // Disallows tracking of elements. Useful if elements are of uniform size.
} arena_flags;
)

doc(temp_memory)
doc_string(Used to temporarily store short-living data in arena.)
doc_sig(
typedef struct
{
    memory_arena *Arena;  // Memory Arena to be used.
    memory_index Used;    // This holds the Arena's original used size.
} temp_memory;
)

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

doc(ArenaInitialize)
doc_string(Initializes Memory Arena with default values.)
doc_example(ArenaInitialize(Arena, 128, MemBlock);)
doc_sig(
inline void
ArenaInitialize(memory_arena *Arena, // Arena to be initialized.
                memory_index Size,   // Size of the allocated memory.
                void *Base)          // Memory block to be used with Arena.
)
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


doc(ArenaBuild)
doc_string(Uses internal allocator to build new arena.)
doc_example(ArenaBuild(Arena, 128);)
doc_sig(
inline void
ArenaBuild(memory_arena *Arena, // Arena to be built.
           memory_index Size)   // The size to be pre-allocated.
)
{
    void *Base = PlatformMemAlloc(Size);
    ArenaInitialize(Arena, Size, Base);
}

doc(ArenaGetAlignmentOffset)
doc_string(Calculates the offset required by specified alignment.)
doc_example(ArenaGetAlignmentOffset(Arena, 8);)
doc_sig(
inline memory_index
ArenaGetAlignmentOffset(memory_arena *Arena,     // Target Arena.
                        memory_index Alignment)  // Desired alignment
)
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

doc(ArenaDefaultParams)
doc_string(Returns default push state params.)
inline arena_push_params
ArenaDefaultParams(void)
{
    arena_push_params Params;
    Params.Flags = ArenaPushFlag_ClearToZero;
    Params.Alignment = 4;
    Params.Expectation = 0;
    Params.Tag = 0;
    return(Params);
}

inline arena_push_params
ArenaAlignNoClear(u32 Alignment)
{
    arena_push_params Params = ArenaDefaultParams();
    Params.Flags &= ~ArenaPushFlag_ClearToZero;
    Params.Alignment = Alignment;
    return(Params);
}

inline arena_push_params
ArenaAlign(u32 Alignment, b32 Clear)
{
    arena_push_params Params = ArenaDefaultParams();
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
ArenaNoClear(void)
{
    arena_push_params Params = ArenaDefaultParams();
    Params.Flags &= ~ArenaPushFlag_ClearToZero;
    return(Params);
}

inline arena_push_params
ArenaExpect(u32 Expectation, b32 Clear)
{
    arena_push_params Params = ArenaAlign(4, Clear);
    Params.Expectation = Expectation;
    return(Params);
}

inline arena_push_params
ArenaAlignExpect(u32 Alignment, u32 Expectation, b32 Clear)
{
    arena_push_params Params = ArenaAlign(Alignment, Clear);
    Params.Expectation = Expectation;
    return(Params);
}

inline arena_push_params
ArenaTag(u32 Tag, arena_push_params Rest)
{
    arena_push_params Params = Rest;
    Params.Tag = Tag;
    return(Params);
}

inline memory_index
ArenaGetSizeRemaining(memory_arena *Arena, arena_push_params Params)
{
    memory_index Result = Arena->Size - (Arena->Used + ArenaGetAlignmentOffset(Arena, Params.Alignment));

    return(Result);
}


#define ArenaGetBlock(arena, type, idx) \
(type *)&(arena->Base)[sizeof(type)*idx]

inline void *
ArenaGetBlockByRecord(memory_arena *Arena, size_t Index)
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

inline tag_scan_result
ArenaGetBlockByTagAndRecord(memory_arena *Arena, tag_scan_result scan, u32 Tag)
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

#define ArenaGetVaryBlock(arena, type, idx) \
(type *)(ArenaGetBlockByRecord(arena,(size_t)idx))

#define ArenaGetVaryBlockTagValue(arena, scan, type, tag) \
(type *)(ArenaGetBlockByTagAndRecord(arena, scan, tag).Value)

#define ArenaGetVaryBlockTagResult(arena, scan, tag) \
ArenaGetBlockByTagAndRecord(arena, scan, tag)


#define ArenaPushStruct(Arena, type, ...) (type *)ArenaPushSize_(Arena, sizeof(type), ## __VA_ARGS__)
#define ArenaPushArray(Arena, Count, type, ...) (type *)ArenaPushSize_(Arena, (Count)*sizeof(type), ## __VA_ARGS__)
#define ArenaPushSize(Arena, Size, ...) ArenaPushSize_(Arena, Size, ## __VA_ARGS__)
#define ArenaPushCopy(Arena, Size, Source, ...) Copy(Size, Source, ArenaPushSize_(Arena, Size, ## __VA_ARGS__))
#define ArenaPushType ArenaPushStruct
#define ArenaPushValue(Arena, type, Value, ...) *((type *) ArenaPushType(Arena, type, ## __VA_ARGS__)) = Value

inline memory_index
ArenaGetEffectiveSizeFor(memory_arena *Arena, memory_index SizeInit, arena_push_params Params)
{
    memory_index Size = SizeInit;
        
    memory_index AlignmentOffset = ArenaGetAlignmentOffset(Arena, Params.Alignment);
    Size += AlignmentOffset;

    return(Size);
}

inline b32
ArenaHasRoomFor(memory_arena *Arena, memory_index SizeInit, arena_push_params Params)
{
    memory_index Size = ArenaGetEffectiveSizeFor(Arena, SizeInit, Params);
    b32 Result = ((Arena->Used + Size) <= Arena->Size);
    return(Result);
}

inline void
ArenaExpand(memory_arena *Arena, memory_index Size)
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
            Assert(!"Not enough memory in memory_arena!");
        }
    }
    else
    {
        Arena->WasExpanded = 0;
    }
}

inline void *
ArenaPushSize_(memory_arena *Arena, memory_index SizeInit, arena_push_params Params)
{
    memory_index Size = ArenaGetEffectiveSizeFor(Arena, SizeInit, Params);
    
    ArenaExpand(Arena, Size + Params.Expectation);

    memory_index AlignmentOffset = ArenaGetAlignmentOffset(Arena, Params.Alignment);
    void *Result = Arena->Base + Arena->Used + AlignmentOffset;
    Arena->Used += Size;

    Assert(Size >= SizeInit);

    if(Params.Flags & ArenaPushFlag_ClearToZero)
    {
        ZeroSize(SizeInit, Result);
    }
    
    if(!(Arena->Flags & ArenaFlag_DisallowHeaders))
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

inline char *
ArenaPushString(memory_arena *Arena, const char *Source)
{
    u32 Size = 1;
    for(const char *At = Source;
        *At;
        ++At)
    {
        ++Size;
    }
    
    char *Dest = (char *)ArenaPushSize_(Arena, Size, ArenaNoClear());
    for(u32 CharIndex = 0;
        CharIndex < Size;
        ++CharIndex)
    {
        Dest[CharIndex] = Source[CharIndex];
    }

    return(Dest);
}

inline char *
ArenaPushAndNullTerminate(memory_arena *Arena, u32 Length, const char *Source)
{
    char *Dest = (char *)ArenaPushSize_(Arena, Length + 1, ArenaNoClear());
    for(u32 CharIndex = 0;
        CharIndex < Length;
        ++CharIndex)
    {
        Dest[CharIndex] = Source[CharIndex];
    }
    Dest[Length] = 0;

    return(Dest);
}

inline temp_memory
ArenaBeginTemporaryMemory(memory_arena *Arena)
{
    temp_memory Result;

    Result.Arena = Arena;
    Result.Used = Arena->Used;

    ++Arena->TempCount;

    return(Result);
}

inline void
ArenaEndTemporaryMemory(temp_memory TempMem)
{
    memory_arena *Arena = TempMem.Arena;
    Assert(Arena->Used >= TempMem.Used);
    Arena->Used = TempMem.Used;
    Assert(Arena->TempCount > 0);
    --Arena->TempCount;
}

inline void
ArenaClear(memory_arena *Arena)
{
    ArenaInitialize(Arena, Arena->Size, Arena->Base);
}

inline void
ArenaCheck(memory_arena *Arena)
{
    Assert(Arena->TempCount == 0);
}

inline void
ArenaSub(memory_arena *Result, memory_arena *Arena, memory_index Size, arena_push_params Params)
{
    Result->Size = Size;
    Result->Base = (uint8 *)ArenaPushSize_(Arena, Size, Params);
    Result->Used = 0;
    Result->TempCount = 0;
}

inline void *
Copy(memory_index Size, const void *SourceInit, void *DestInit)
{
    u8 *Source = (u8 *)SourceInit;
    u8 *Dest = (u8 *)DestInit;
    while(Size--) {*Dest++ = *Source++;}

    return(DestInit);
}

inline void
ArenaCleanUnusedRoom(memory_arena * Arena)
{
    u8 * Base = Arena->Base + Arena->Used - 1;
    
    while(Base < (Arena->Base + Arena->Size))
    {
        *(Base++) = 0;
    }
}

inline void
ArenaFree(memory_arena * Arena)
{
    Assert(Arena->TempCount == 0);
    
    PlatformMemFree(Arena->Base);
    Arena->Used = Arena->Size = 0;
}

#define HFTW_MEM_H
#endif