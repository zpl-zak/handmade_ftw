// (c) 2016 ZaKlaus; All Rights Reserved

#if !defined(HFTW_STRING_H)

typedef struct
{
    memory_arena Arena;
    u32 Length;
    s32 StringHash;
    b32 IsModified;
} string;

inline s32
StringCalcHash(char *Source, u32 Length)
{
    s32 Hash = 0;
    char *Ptr = Source;
    char C = 0;
    while((C = *(Ptr++))!= 0)
    {
        Hash += ('z' - C) * 11 + 'a' * C + Length;
    }
    Assert(Hash < INT32_MAX);
    return(Hash);
}

inline u32
StringGetLengthFromArray(char *Src)
{
    u32 Length = 0;
    while(*(Src++))
    {
        ++Length;
    }
    return(Length);
}

inline string
StringCreate(char *Source, u32 Length)
{
    string Str = {0};
    memory_arena StringArena = {0};
    ArenaBuild(&StringArena, Length);
    StringArena.Flags = ArenaFlag_AllowRealloc;
    ArenaPushAndNullTerminate(&StringArena, Length, (char *)Source);
    Str.Arena = StringArena;
    Str.Length = Length;
    Str.StringHash = StringCalcHash(Source, Str.Length);
    
    return(Str);
}

inline void
StringDestroy(string *Str)
{
    ArenaFree(&Str->Arena);
}

// NOTE(zaklaus): Read-Only
inline const char *
StringGetRaw(string *Str)
{
    Assert(!Str->IsModified);
    const char *Source = (char *)Str->Arena.Base;
    return(Source);
}

inline b32
StringCompare(string *Str1, string *Str2)
{
    b32 Result = Str1->StringHash == Str2->StringHash;
    return(Result);
}

inline b32
StringCompareRaw(string *Str1, string *Str2)
{
    b32 Result = StringsAreEqual((char *)Str1->Arena.Base, (char *)Str2->Arena.Base);
    return(Result);
}

inline char *
StringBeginEdit(string *Str)
{
    Assert(!Str->IsModified);
    char * Result = (char *)Str->Arena.Base;
    Str->IsModified = 1;
    return(Result);
}

inline void
StringEndEdit(string *Str)
{
    Assert(Str->IsModified);
    char *Source = (char *)Str->Arena.Base;
    Str->Length = StringGetLengthFromArray(Source);
    Str->StringHash = StringCalcHash(Source, Str->Length);
    Str->IsModified = 0;
}

inline u32
StringGetLength(string *Str)
{
    Assert(!Str->IsModified);
    u32 Length = Str->Length;
    return(Length);
}

inline u32
StringGetAllocatedLength(string *Str)
{
    Assert(!Str->IsModified);
    u32 Length = (u32)Str->Arena.Size;
    return(Length);
}

inline void
StringCheck(string *Str)
{
    Assert(!Str->IsModified);
    char *Source = (char *)Str->Arena.Base;
    u32 Length = Str->Length;
    s32 CompareHash = StringCalcHash(Source, Length);
    Assert(CompareHash == Str->StringHash);
}

inline string
StringDuplicate(string *Str)
{
    string Result = {0};
    memory_arena NewArena;
    
    // NOTE(zaklaus): Build memory arena ourselves
    u8 *Buffer = PlatformMemAlloc(CSize(u8, Str->Arena.Size));
    Copy(Str->Length, Str->Arena.Base, Buffer);
    ArenaInitialize(&NewArena, Str->Arena.Size, Buffer);
    NewArena.Flags = ArenaFlag_AllowRealloc;
    NewArena.Used = Str->Arena.Used;
    Result.Length = Str->Length;
    Result.Arena = NewArena;
    Result.StringHash = Str->StringHash;
    return(Result);
}

inline void
StringAppend(string *StrA, string *StrB)
{
     char *Dest = ArenaPushSize_(&(StrA->Arena), StrB->Length, ArenaNoClear());
    Copy(1, StringGetRaw(StrB), Dest);
    
    StrA->Length += StrB->Length;
}

#define literal(Literal) Literal, sizeof(Literal)/sizeof(u8)-1

#define HFTW_STRING_H
#endif