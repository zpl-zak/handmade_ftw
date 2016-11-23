// (c) 2016 ZaKlaus; All Rights Reserved

#if !defined(HFTW_STRING_H)

typedef struct
{
    memory_arena Arena;
    u32 Length;
    s32 StringHash;
    b32 IsModified;
} String;

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

inline String
StringCreate(char *Source, u32 Length)
{
    String Str = {0};
    memory_arena StringArena = {0};
    ArenaBuild(&StringArena, Length+2);
    StringArena.Flags = ArenaFlag_AllowRealloc;
    ArenaPushAndNullTerminate(&StringArena, Length+1, (char *)Source);
    Str.Arena = StringArena;
    Str.Length = Length;
    Str.StringHash = StringCalcHash(Source, Str.Length);
    
    return(Str);
}

// NOTE(zaklaus): Read-Only
inline const char *
StringGetRaw(String *Str)
{
    Assert(!Str->IsModified);
    const char *Source = (char *)Str->Arena.Base;
    return(Source);
}

inline b32
StringCompare(String *Str1, String *Str2)
{
    b32 Result = Str1->StringHash == Str2->StringHash;
    return(Result);
}

inline b32
StringCompareRaw(String *Str1, String *Str2)
{
    b32 Result = StringsAreEqual((char *)Str1->Arena.Base, (char *)Str2->Arena.Base);
    return(Result);
}

inline char *
StringBeginEdit(String *Str)
{
    Assert(!Str->IsModified);
    char * Result = (char *)Str->Arena.Base;
    Str->IsModified = 1;
    return(Result);
}

inline void
StringEndEdit(String *Str)
{
    Assert(Str->IsModified);
    char *Source = (char *)Str->Arena.Base;
    Str->Length = StringGetLengthFromArray(Source);
    Str->StringHash = StringCalcHash(Source, Str->Length);
    Str->IsModified = 0;
}

inline u32
StringGetLength(String *Str)
{
    Assert(!Str->IsModified);
    u32 Length = Str->Length;
    return(Length);
}

inline u32
StringGetAllocatedLength(String *Str)
{
    Assert(!Str->IsModified);
    u32 Length = (u32)Str->Arena.Size;
    return(Length);
}

inline void
StringCheck(String *Str)
{
    Assert(!Str->IsModified);
    char *Source = (char *)Str->Arena.Base;
    u32 Length = Str->Length;
    s32 CompareHash = StringCalcHash(Source, Length);
    Assert(CompareHash == Str->StringHash);
}

inline void
StringAppend(String *StrA, String *StrB)
{
    
}

#define literal(Literal) Literal, sizeof(Literal)/sizeof(u8)-1

#define HFTW_STRING_H
#endif