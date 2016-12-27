// (c) 2016 ZaKlaus; All Rights Reserved

#if !defined(HFTW_MEM_H)

//doc(arena_header)
//doc_string(Describes arena elements.)
doc_cat(Memory Arena)

typedef struct
{
    s32 Size;    // Size of the element.
    u32 Tag;     // Element's tag used for identification.
    u32 Offset;  // Memory offset from the base.
    b32 IsFree;  // Is this node free for use?
} arena_header;

LinkedList(arena_header)

//doc(memory_arena)
//doc_string(Memory arena holding contigous block of memory and tracking of pushed objects.)

typedef struct
{
    memory_index Size;             // Allocated memory size. 
    uint8 *Base;                   // Memory base pointer. (Points to a contigous block of memory.)
    memory_index Used;             // Memory already used by elements.
    u8 Flags;                      // Arena's flags.
    b32 WasExpanded;               // If reallocation is allowed, this signals us whether memory expansion has happened.

    s32 TempCount;                 // Counts how many times is our arena used by temp_memory.
    s32 NodeCount;                 // Counts how many nodes do we track in our arena header.
    
    Node_arena_header *Header;     // Linked list of tracked elements.
    Node_arena_header *HeaderEnd;
} memory_arena;

//doc(arena_flags)
//doc_string(Arena flags.)

typedef enum
{
    ArenaFlag_AllowRealloc = (1 << 1),    // Allows re-allocation of the memory, if expansion is required.
    ArenaFlag_DisallowHeaders = (1 << 2), // Disallows tracking of elements. Useful if elements are of uniform size. This also disables Arena memory reuse.
    ArenaFlag_DisallowReuse = (1 << 3),   // Disallows the ability to reuse freed memory in arena.
} arena_flags;

//doc(temp_memory)
//doc_string(Used to temporarily store short-living data in arena.)

typedef struct
{
    memory_arena *Arena;  // Memory Arena to be used.
    memory_index Used;    // This holds the Arena's original used size.
} temp_memory;

//doc(arena_push_flag)
//doc_string(Push flags used by arena allocator.)

typedef enum
{
    ArenaPushFlag_ClearToZero = 0x1,  // Tell the allocator to clear allocated memory.
} arena_push_flag;

//doc(arena_push_params)
//doc_string(Params used by allocator for customized allocation procedure.)

typedef struct
{
    u32 Flags;        // Specifies push flags. @see arena_push_flag
    u32 Alignment;    // Specifies memory alignment requirements.
    u32 Expectation;  // This tells the allocator to check whether we have enough reserved memory up-front.
    u32 Tag;          // Tag used for identification in pushed element.
} arena_push_params;

//doc(tag_scan_result)
//doc_string(Holds pointer to current element found by specified tag identification.)

typedef struct
{
    u8 *Value;                // Pointer to memory tagged by identification.
    Node_arena_header *Node;  // Pointer to found element.
} tag_scan_result;

doc_sep()

//doc(ArenaGetBlock)
//doc_string(Returns block of memory pointed to by index. *NOTE* Works only with arena`s elements of uniform size!)
doc_example((Type *) ArenaGetBlock(Arena, Type, Index))
#define ArenaGetBlock(arena, type, idx) \
(type *)&(arena->Base)[sizeof(type)*idx]

//doc(ArenaGetVaryBlock)
//doc_string(Returns element by index.)
doc_example((Type *) ArenaGetVaryBlock(Arena, Type, Index))
#define ArenaGetVaryBlock(arena, type, idx) \
(type *)(ArenaGetBlockByRecord(arena,(size_t)idx))

//doc(ArenaGetVaryBlockTagValue)
//doc_string(Returns element`s value by tag.)
doc_example((Type *) ArenaGetVaryBlockTagValue(Arena, Scan, Type, Tag))
#define ArenaGetVaryBlockTagValue(arena, scan, type, tag) \
(type *)(ArenaGetBlockByTagAndRecord(arena, scan, tag).Value)

//doc(ArenaGetVaryBlockTagResult)
//doc_string(Returns tag scan result by tag.)
doc_example(tag_scan_result ArenaGetVaryBlockTagResult(Arena, Scan, Tag))
#define ArenaGetVaryBlockTagResult(arena, scan, tag) \
ArenaGetBlockByTagAndRecord(arena, scan, tag)

//doc(ArenaPushStruct)
//doc_string(Push struct to the arena.)
doc_example((Type *) ArenaPushStruct(Arena, Type, ...))
#define ArenaPushStruct(Arena, type, ...) (type *)ArenaPushSize_(Arena, sizeof(type), ## __VA_ARGS__)

//doc(ArenaPushArray)
//doc_string(Push array to the arena.)
doc_example((Type *) ArenaPushArray(Arena, Count, Type, ...))
#define ArenaPushArray(Arena, Count, type, ...) (type *)ArenaPushSize_(Arena, (Count)*sizeof(type), ## __VA_ARGS__)

//doc(ArenaPushSize)
//doc_string(Push size to the arena.)
doc_example((void *) ArenaPushSize(Arena, Size, ...))
#define ArenaPushSize(Arena, Size, ...) ArenaPushSize_(Arena, Size, ## __VA_ARGS__)

//doc(ArenaPushCopy)
//doc_string(Push and copy size from the source.)
doc_example((void *) ArenaPushCopy(Arena, Size, Source, ...))
#define ArenaPushCopy(Arena, Size, Source, ...) Copy(Size, Source, ArenaPushSize_(Arena, Size, ## __VA_ARGS__))

#define ArenaPushType ArenaPushStruct

//doc(ArenaPushValue)
//doc_string(Push and set value to the arena.)
doc_example((none) 
            ArenaPushValue(Arena, Type, Value, ...))
#define ArenaPushValue(Arena, type, Value, ...) *((type *) ArenaPushType(Arena, type, ## __VA_ARGS__)) = Value

doc_sep()

//doc(ArenaDefaultTagScan)
//doc_string(Default scan setup for new scans.)

internal tag_scan_result
ArenaDefaultTagScan(void)
{
    tag_scan_result Result = {0};
    return(Result);
}


//doc(ArenaInitialize)
//doc_string(Initializes Memory Arena with default values.)
doc_example(ArenaInitialize(Arena, 128, MemBlock);)

internal void
ArenaInitialize(memory_arena *Arena, // Arena to be initialized.
                memory_index Size,   // Size of the allocated memory.
                void *Base)          // Memory block to be used with Arena.
{
    Arena->Size = Size;
    Arena->Base = (uint8 *)Base;
    Arena->Used = 0;
    Arena->WasExpanded = 0;
    Arena->TempCount = 0;
    Arena->Flags = 0;
    Arena->Header = 0;
    Arena->HeaderEnd = 0;
    Arena->NodeCount = 0;
}


//doc(ArenaBuild)
//doc_string(Uses internal allocator to build new arena.)
doc_example(ArenaBuild(Arena, 128);)

internal void
ArenaBuild(memory_arena *Arena, // Arena to be built.
           memory_index Size)   // The size to be pre-allocated.
{
    void *Base = PlatformMemAlloc(Size);
    ArenaInitialize(Arena, Size, Base);
}

//doc(ArenaGetAlignmentOffset)
//doc_string(Calculates the offset required by specified alignment.)
doc_example(ArenaGetAlignmentOffset(Arena, 8);)

internal memory_index
ArenaGetAlignmentOffset(memory_arena *Arena,     // Target Arena.
                        memory_index Alignment)  // Desired alignment
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

//doc(ArenaDefaultParams)
//doc_string(Returns default push state params.)

internal arena_push_params
ArenaDefaultParams(void)
{
    arena_push_params Params;
    Params.Flags = ArenaPushFlag_ClearToZero;
    Params.Alignment = 4;
    Params.Expectation = 0;
    Params.Tag = 0;
    return(Params);
}

//doc(ArenaAlignNoClear)
//doc_string(Returns alignment arena push params without clear flag.)

internal arena_push_params
ArenaAlignNoClear(
u32 Alignment)      // Memory alignment we require during allocation.
{
    arena_push_params Params = ArenaDefaultParams();
    Params.Flags &= ~ArenaPushFlag_ClearToZero;
    Params.Alignment = Alignment;
    return(Params);
}

//doc(ArenaAlign)
//doc_string(Returns alignment arena push params.)

internal arena_push_params
ArenaAlign(u32 Alignment, // Memory alignment we require during allocation.
           b32 Clear)     // Should allocator clear allocated memory?
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

//doc(ArenaNoClear)
//doc_string(Returns arena push params without clear flag.)

internal arena_push_params
ArenaNoClear(void)
{
    arena_push_params Params = ArenaDefaultParams();
    Params.Flags &= ~ArenaPushFlag_ClearToZero;
    return(Params);
}

//doc(ArenaExpect)
//doc_string(Returns arena push params with expected reserves.)

internal arena_push_params
ArenaExpect(u32 Expectation, // Expected memory to be in reserves.
            b32 Clear)       // Should allocator clear allocated memory?
{
    arena_push_params Params = ArenaAlign(4, Clear);
    Params.Expectation = Expectation;
    return(Params);
}

//doc(ArenaAlignExpect)
//doc_string(Returns arena push params with expected reserves and alignment.)

internal arena_push_params
ArenaAlignExpect(u32 Alignment,   // Memory alignment we require during allocation.
                 u32 Expectation, // Expected memory to be in reserves.
                 b32 Clear)       // Should allocator clear allocated memory?
{
    arena_push_params Params = ArenaAlign(Alignment, Clear);
    Params.Expectation = Expectation;
    return(Params);
}

//doc(ArenaTag)
//doc_string(Returns arena push params with tag property.)

internal arena_push_params
ArenaTag(u32 Tag,                 // Tag used for element lookup.
         arena_push_params Rest)  // Rest of the push params. @see arena_push_params
{
    arena_push_params Params = Rest;
    Params.Tag = Tag;
    return(Params);
}

//doc(ArenaGetSizeRemaining)
//doc_string(Returns the remaining unused space size in arena.)

internal memory_index
ArenaGetSizeRemaining(memory_arena *Arena,      // Target arena
                      arena_push_params Params) // Params to be used during the procedure.
{
    memory_index Result = Arena->Size - (Arena->Used + ArenaGetAlignmentOffset(Arena, Params.Alignment));

    return(Result);
}

//doc(ArenaGetBlockByRecord)
//doc_string(Returns block of memory pointed to by index.)

internal void *
ArenaGetBlockByRecord(memory_arena *Arena, // Target arena
                      size_t Index)        // Element index
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

//doc(ArenaGetBlockByTagAndRecord)
//doc_string(Returns tag scan result based on specified tag.)

internal tag_scan_result
ArenaGetBlockByTagAndRecord(memory_arena *Arena,  // Target arena
                            tag_scan_result scan, // Previous scan result. (Or DefaultTagScan if none. @see DefaultTagScan)
                            u32 Tag)              // Tag used for lookup.
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

//doc(ArenaGetEffectiveSizeFor)
//doc_string(Returns effective size based on push params for specified initial size.)

internal memory_index
ArenaGetEffectiveSizeFor(memory_arena *Arena,      // Target arena
                         memory_index SizeInit,    // Initial size required by callee.
                         arena_push_params Params) // Params to be used during the procedure.
{
    memory_index Size = SizeInit;
        
    memory_index AlignmentOffset = ArenaGetAlignmentOffset(Arena, Params.Alignment);
    Size += AlignmentOffset;

    return(Size);
}

//doc(ArenaHasRoomFor)
//doc_string(Returns truth value determining whether we have enough space in our arena depending on initial size requirements and push params.)

internal b32
ArenaHasRoomFor(memory_arena *Arena,       // Target arena
                memory_index SizeInit,     // Initial size required by callee.
                arena_push_params Params)  // Params to be used during the procedure.
{
    memory_index Size = ArenaGetEffectiveSizeFor(Arena, SizeInit, Params);
    b32 Result = ((Arena->Used + Size) <= Arena->Size);
    return(Result);
}

//doc(ArenaExpand)
//doc_string(Expands the arena if the requested size can`t make it into our arena.)

internal void
ArenaExpand(memory_arena *Arena, // Our arena.
            memory_index Size)   // The requested size.
{
    if(!((Arena->Used + Size) <= Arena->Size))
    {
        if(Arena->Flags & ArenaFlag_AllowRealloc)
        {
            Arena->Base = (u8 *)PlatformMemRealloc(Arena->Base, Arena->Size + Size);
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

internal void *
ArenaReuseMem(memory_arena *Arena,
              memory_index Size,
              b32 *IsReused,
              arena_push_params Params)
{
    void *Result = 0;
    *IsReused = 0;
    if(!(Arena->Flags & ArenaFlag_DisallowReuse))
        {
            for (Node_arena_header *Node = Arena->Header;
                 Node;
                 Node = Node->Next)
            {
                arena_header *E = &Node->Value;
                if(E->IsFree && E->Size >= Size)
                {
                    if(E->Size > Size)
                    {
                        Node_arena_header *NE = Node->Next;
                        s32 NewSize = E->Size - (s32)Size;
                        E->Size = (s32)Size;
                        
                        arena_header N = {0};
                        N.Size = NewSize;
                        N.Offset = E->Offset + E->Size;
                        N.IsFree = 1;
                        Node_arena_header *NewNode = AddNode_arena_header(Node, N);
                        NewNode->Next = NE;
                        ++Arena->NodeCount;
                    }
                    Result = Arena->Base + E->Offset;
                    E->Tag = Params.Tag;
                    E->IsFree = 0;
                    *IsReused = 1;
                    return(Result);
                }
            }
        }
    return(Result);
}

//doc(ArenaPushSize_)
//doc_string(Asks our arena for plotting a block of memory determined by initial size and params requirements.)

internal void *
ArenaPushSize_(memory_arena *Arena,         // Target arena
               memory_index SizeInit,       // Initial size required by callee.
               arena_push_params Params)    // Params to be used during the procedure.
{
    memory_index Size = ArenaGetEffectiveSizeFor(Arena, SizeInit, Params);
    b32 IsReused = 0;
    memory_index AlignmentOffset = ArenaGetAlignmentOffset(Arena, Params.Alignment);
    void *Result = ArenaReuseMem(Arena, Size, &IsReused, Params);
    
    if(!IsReused)
    {
        ArenaExpand(Arena, Size + Params.Expectation);
        Result = Arena->Base + Arena->Used + AlignmentOffset;
        Arena->Used += Size;
    }
    
    Assert(Size >= SizeInit);

    if(Params.Flags & ArenaPushFlag_ClearToZero)
    {
        ZeroSize(SizeInit, Result);
    }
    
    if(!(Arena->Flags & ArenaFlag_DisallowHeaders) && !IsReused)
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
        ++Arena->NodeCount;
    }
    
    return(Result);
}

//doc(ArenaPushString)
//doc_string(Pushes null-terminated string to the arena.)

internal char *
ArenaPushString(memory_arena *Arena, // Target arena
                const char *Source)  // String source
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

//doc(ArenaPushAndNullTerminate)
//doc_string(Pushes string of specified size to the arena and additionally null-terminates it.)

internal char *
ArenaPushAndNullTerminate(memory_arena *Arena, // Target arena
                          u32 Length,          // String length
                          const char *Source)  // String source
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

//doc(ArenaBeginTemporaryMemory)
//doc_string(Tells our arena we plan to store temporary memory in it.)

internal temp_memory
ArenaBeginTemporaryMemory(
memory_arena *Arena) // Target arena
{
    temp_memory Result;

    Result.Arena = Arena;
    Result.Used = Arena->Used;

    ++Arena->TempCount;

    return(Result);
}

//doc(ArenaEndTemporaryMemory)
//doc_string(Tells our arena we`re done serving temporary memory and resets it back to its original state.)

internal void
ArenaEndTemporaryMemory(
temp_memory TempMem) // Structure holding our arena's pointer and originally used size. @see temp_memory
{
    memory_arena *Arena = TempMem.Arena;
    Assert(Arena->Used >= TempMem.Used);
    Arena->Used = TempMem.Used;
    Assert(Arena->TempCount > 0);
    --Arena->TempCount;
}

//doc(ArenaClear)
//doc_string(Clears the whole arena.)

internal void
ArenaClear(
memory_arena *Arena) // Target arena
{
    ArenaInitialize(Arena, Arena->Size, Arena->Base);
}

//doc(ArenaCheck)
//doc_string(Checks whether our arena is still holding temporary data.)

internal void
ArenaCheck(memory_arena *Arena)
{
    Assert(Arena->TempCount == 0);
}

//doc(ArenaSub)
//doc_string(Creates sub-arena inside of our arena, with specified size and push params.)

internal void
ArenaSub(memory_arena *Result,     // Target SubArena
         memory_arena *Arena,      // Target Arena
         memory_index Size,        // Initial size required by callee.
         arena_push_params Params) // Params to be used during the procedure.
{
    Result->Size = Size;
    Result->Base = (uint8 *)ArenaPushSize_(Arena, Size, Params);
    Result->Used = 0;
    Result->TempCount = 0;
}

//doc(ArenaCleanUnusedRoom)
//doc_string(Clean unused reserved space in our arena.)

internal void
ArenaCleanUnusedRoom(
memory_arena * Arena) // Target arena
{
    u8 * Base = Arena->Base + Arena->Used - 1;
    
    while(Base < (Arena->Base + Arena->Size))
    {
        *(Base++) = 0;
    }
}

//doc(ArenaFree)
//doc_string(Deallocate our arena.)

internal void
ArenaFree(
memory_arena * Arena) // Target arena
{
    Assert(Arena->TempCount == 0);
    
    PlatformMemFree(Arena->Base);
    Arena->Used = Arena->Size = 0;
}

//doc(ArenaSerialize)
//doc_string(Serializes our arena)
doc_ret(Returns serialized data.)

internal u8 *
ArenaSerialize(
memory_arena * Arena, // Arena to be serialized.
 size_t * Size)         // Size of the serialized data.
{
    Assert(!Arena->TempCount);
    size_t AllocSize = Arena->Used + sizeof(mi)*2 + sizeof(u8) + sizeof(s32) + sizeof(arena_header)*Arena->NodeCount;
    u8 * Result = (u8 *)PlatformMemAlloc(AllocSize);
    u8 * Ptr = Result;
    
    // NOTE(zaklaus): size, used, flags, content
    
    *(mi *)Ptr = Arena->Size; Ptr += sizeof(mi);
    *(mi *)Ptr = Arena->Used; Ptr += sizeof(mi);
    *(u8 *)Ptr = Arena->Flags; Ptr += sizeof(u8);
    Copy(Arena->Used, Arena->Base, Ptr);
    Ptr += Arena->Used;
    
    // NOTE(zaklaus): Headers
    if(!(Arena->Flags & ArenaFlag_DisallowHeaders))
    {
        *(s32 *)Ptr = Arena->NodeCount; Ptr += sizeof(s32);
        
        for (Node_arena_header *Node = Arena->Header;
             Node;
             Node = Node->Next)
        {
            arena_header E = Node->Value;
            *(s32 *)Ptr = E.Size; Ptr += sizeof(s32);
            *(s32 *)Ptr = E.Tag; Ptr += sizeof(u32);
            *(s32 *)Ptr = E.Offset; Ptr += sizeof(u32);
            *(b32 *)Ptr = E.IsFree; Ptr += sizeof(b32);
        }
    }
    
*Size = AllocSize;
return(Result);
}

//doc(ArenaDeserialize)
//doc_string(Deserializes our packed data to our new arena.)

internal void
ArenaDeserialize(
memory_arena * Arena, // Our new arena to be used for unpacking.
u8 * Data)            // Source of our serialized data.
{
    Arena->Size = *(mi *)Data; Data += sizeof(mi);
    Arena->Used = *(mi *)Data; Data += sizeof(mi);
    Arena->Flags = *(u8 *)Data; Data += sizeof(u8);
    Arena->NodeCount = Arena->TempCount = 0;
    Arena->Base = (u8 *)PlatformMemAlloc(Arena->Size);
    Copy(Arena->Used, Data, Arena->Base);
    Data += Arena->Used;
    
    if(!(Arena->Flags & ArenaFlag_DisallowHeaders))
    {
        Arena->NodeCount = *(s32 *)Data; Data += sizeof(s32);
        
// NOTE(zaklaus): Include header as a part of the Linked-List.
        for (s32 Idx = 0;
             Idx < Arena->NodeCount + 1;
             Idx++)
        {
            arena_header header = {0};
            header.Size = *(s32 *)Data; Data += sizeof(s32);
            header.Tag = *(u32 *)Data; Data += sizeof(u32);
            header.Offset = *(u32 *)Data; Data += sizeof(u32);
            header.IsFree = *(b32 *)Data; Data += sizeof(b32);
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
    }
}

//doc(ArenaDuplicate)
//doc_string(Duplicates our arena.)

internal void
ArenaDuplicate(
memory_arena * A, // Arena to be duplicated.
memory_arena * B) // Arena to duplicate to.
{
    Assert(!A->TempCount);
    B->Size = A->Size;
    B->Used = A->Used;
    B->Flags = A->Flags;
    B->NodeCount = A->NodeCount;
    B->WasExpanded = A->WasExpanded;
    B->Base = (u8 *)PlatformMemAlloc(B->Size);
    Copy(B->Used, A->Base, B->Base);
    
    if(!(B->Flags & ArenaFlag_DisallowHeaders))
    {
        for (Node_arena_header *E = A->Header;
             E;
             E = E->Next)
        {
            arena_header header = E->Value;
            if (!B->Header)
            {
                arena_header dummy = {0};
                B->Header = NewNode_arena_header(dummy);
                B->HeaderEnd = NewNode_arena_header(header);
                B->Header->Next = B->HeaderEnd;
            }
            else
            {
                B->HeaderEnd = AddNode_arena_header(B->HeaderEnd, header);
            }
        }
    }
    else
    {
        B->Header = B->HeaderEnd = 0;
    }
}

internal void
ArenaFreeBlockID(
memory_arena *Arena,
mi Idx)
{
    mi OurIdx = 0;
    for(Node_arena_header *Node = Arena->Header;
        Node;
        Node = Node->Next
        )
    {
        if(OurIdx == Idx+1)
        {
            Node->Value.IsFree = 1;
            return;
        }
        ++OurIdx;
    }
}

internal void
ArenaFreeBlockTag(
memory_arena *Arena,
u32 Tag)
{
    for(Node_arena_header *Node = Arena->Header->Next;
        Node;
        Node = Node->Next
        )
{
    if(Node->Value.Tag == Tag)
    {
        Node->Value.IsFree = 1;
        return;
    }
}
}

#define HFTW_MEM_H
#endif
