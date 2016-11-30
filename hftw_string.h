// (c) 2016 ZaKlaus; All Rights Reserved

#if !defined(HFTW_STRING_H)

doc(string)
doc_cat(String)
doc_string(Describes string component.)
doc_sig(
typedef struct
{
    memory_arena Arena; // Memory arena used by the string.
    u32 Length;         // Length of the string.
    s32 StringHash;     // String's generated hash.
    b32 IsModified;     // Was our string modified?
} string;
)

doc_sep()

doc(StringCalcHash)
doc_string(Calculates string hash.)
doc_sig(
inline s32
StringCalcHash(char *Source, // Source of the raw string to be hashed.
               u32 Length)   // Its length.
)
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

doc(StringGetLengthFromArray)
doc_string(Returns length of raw null-terminated string.)
doc_sig(
inline u32
StringGetLengthFromArray(char *Src) // Source of the raw string to get length from.
)
{
    u32 Length = 0;
    while(*(Src++))
    {
        ++Length;
    }
    return(Length);
}

doc(StringCreate)
doc_string(Returns newly-created managed string.)
doc_example(StringCreate("Hello world!", 11);)
doc_sig(
inline string
StringCreate(char *Source, // Source of the raw string.
             u32 Length)   // Its length.
)
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

doc(StringDestroy)
doc_string(Destroys string and de-allocates its memory.)
doc_sig(
inline void
StringDestroy(string *Str) // Our string.
)
{
    ArenaFree(&Str->Arena);
}

// NOTE(zaklaus): Read-Only
doc(StringGetRaw)
doc_string(Returns raw string.)
doc_sig(
inline const char *
StringGetRaw(string *Str) // Our string.
)
{
    Assert(!Str->IsModified);
    const char *Source = (char *)Str->Arena.Base;
    return(Source);
}

doc(StringCompare)
doc_string(Compares two strings by hash.)
doc_ret(Returns 1 if string hashes are equal.)
doc_sig(
inline b32
StringCompare(string *Str1, // Our 1st string.
              string *Str2) // Our 2nd string.
)
{
    b32 Result = Str1->StringHash == Str2->StringHash;
    return(Result);
}

doc(StringCompareSlow)
doc_string(Compares two string by comparing their characters.)
doc_sig(
inline b32
StringCompareSlow(string *Str1, // Our 1st string.
                  string *Str2) // Our 2nd string.
)
{
    b32 Result = StringsAreEqual((char *)Str1->Arena.Base, (char *)Str2->Arena.Base);
    return(Result);
}

doc(StringBeginEdit)
doc_string(Tells the string we want to edit it. Therefore entering edit mode.)
doc_sig(
inline char *
StringBeginEdit(string *Str) // Our string.
)
{
    Assert(!Str->IsModified);
    char * Result = (char *)Str->Arena.Base;
    Str->IsModified = 1;
    return(Result);
}

doc(StringEndEdit)
doc_string(Tells the string we`re done with editation, therefore it recalculates string`s hash.)
doc_sig(
inline void
StringEndEdit(string *Str) // Our string.
)
{
    Assert(Str->IsModified);
    char *Source = (char *)Str->Arena.Base;
    Str->Length = StringGetLengthFromArray(Source);
    Str->StringHash = StringCalcHash(Source, Str->Length);
    Str->IsModified = 0;
}

doc(StringGetLength)
doc_string(Returns the length of our string.)
doc_sig(
inline u32
StringGetLength(
string *Str) // Our string.
)
{
    Assert(!Str->IsModified);
    u32 Length = Str->Length;
    return(Length);
}

doc(StringGetAllocatedLength)
doc_string(Returns the allocated memory for our string.)
doc_sig(
inline u32
StringGetAllocatedLength(
string *Str) // Our string.
)
{
    Assert(!Str->IsModified);
    u32 Length = (u32)Str->Arena.Size;
    return(Length);
}

doc(StringCheck)
doc_string(Checks whether our hash is correct.)
doc_sig(
inline void
StringCheck(
string *Str) // Our string.
)
{
    Assert(!Str->IsModified);
    char *Source = (char *)Str->Arena.Base;
    u32 Length = Str->Length;
    s32 CompareHash = StringCalcHash(Source, Length);
    Assert(CompareHash == Str->StringHash);
}

doc(StringDuplicate)
doc_string(Duplicates our string.)
doc_sig(
inline string
StringDuplicate(
string *Str) // Our string.
)
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

doc(StringAppend)
doc_string(Appends our string to our original string.)
doc_sig(
inline void
StringAppend(string *StrA, // Our string.
             string *StrB) // Our string.
)
{
     char *Dest = ArenaPushSize_(&(StrA->Arena), StrB->Length, ArenaNoClear());
    Copy(1, StringGetRaw(StrB), Dest);
    
    StrA->Length += StrB->Length;
}

#define literal(Literal) Literal, sizeof(Literal)/sizeof(u8)-1
#define hash_lit(Literal) StringCalcHash( Literal , sizeof( Literal )/sizeof(u8)-1)

#define HFTW_STRING_H
#endif                                                                   