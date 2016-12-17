// (c) 2016 ZaKlaus; All Rights Reserved
/*
  PAK File Format
Usage: Archive format
SW: Quake series
*/
#if !defined(HFORMAT_PAK_H)

#pragma pack(push, 1)
typedef struct
{
    u8 Signature[4]; // NOTE(zaklaus): Should be "PACK"
    s32 DirectoryOffset;
    s32 DirectoryLength;
} hformat_pak_header;

typedef struct
{
    u8 FileName[56];
    s32 FilePosition;
    s32 FileLength;
} hformat_pak_file;
#pragma pack(pop)

typedef struct
{
    hformat_pak_header Header;
     hformat_pak_file *Files;
    s32 FileCount;
} hformat_pak;

internal hformat_pak *
HFormatLoadPakArchive(s32 HandleIdx)
{
    hformat_pak *Pak = PlatformMemAlloc(sizeof(hformat_pak));
    {
        IOFileRead(HandleIdx, &Pak->Header, sizeof(hformat_pak_header));
        
        if(!StringsAreEqualA(4, (s8 *)Pak->Header.Signature, "PACK"))
        {
            fprintf(stderr, "Not a valid PAK format!\n");
            PlatformMemFree(Pak);
            return(0);
        }
        
        Pak->FileCount = Pak->Header.DirectoryLength / sizeof(hformat_pak_file);
        
        Pak->Files = PlatformMemAlloc(sizeof(hformat_pak_file)*Pak->FileCount);
        
        IOFileSeek(HandleIdx, Pak->Header.DirectoryOffset, SeekOrigin_Set);
        
        IOFileRead(HandleIdx, Pak->Files, Pak->Header.DirectoryLength);
    }
    return(Pak);
}

internal hformat_pak *
HFormatReleasePakArchive(hformat_pak *Pak)
{
    PlatformMemFree(Pak->Files);
    PlatformMemFree(Pak);
    Pak = 0;
    return(Pak);
}

#define HFORMAT_PAK_H
#endif