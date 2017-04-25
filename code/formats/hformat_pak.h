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
    u64 *Checksums;
    u32 FileCount;
} hformat_pak;

internal hformat_pak *
HFormatLoadPakArchive(s32 HandleIdx)
{
    hformat_pak *Pak = (hformat_pak *)PlatformMemAlloc(sizeof(hformat_pak));
    {
        IOFileRead(HandleIdx, &Pak->Header, sizeof(hformat_pak_header));
        
        if(!StringsAreEqualA(4, (char *)Pak->Header.Signature, "PACK"))
        {
            fprintf(stderr, "Not a valid PAK format!\n");
            PlatformMemFree(Pak);
            return(0);
        }
        
        Pak->FileCount = Pak->Header.DirectoryLength / sizeof(hformat_pak_file);
        
        Pak->Files = (hformat_pak_file *)PlatformMemAlloc(sizeof(hformat_pak_file)*Pak->FileCount);
        Pak->Checksums = (u64 *)PlatformMemAlloc(sizeof(u64)*Pak->FileCount);
        
        IOFileSeek(HandleIdx, Pak->Header.DirectoryOffset, SeekOrigin_Set);
        
        IOFileRead(HandleIdx, Pak->Files, Pak->Header.DirectoryLength);
        
        for(mi Idx = 0;
            Idx < Pak->FileCount;
            Idx++)
        {
            IOFileSeek(HandleIdx, Pak->Files[Idx].FilePosition, SeekOrigin_Set);
            
            s8 *FileData = (s8 *)PlatformMemAlloc(Pak->Files[Idx].FileLength);
            IOFileRead(HandleIdx, FileData, Pak->Files[Idx].FileLength);
            
            Pak->Checksums[Idx] = 0;
            
            u64 Pos = Pak->Files[Idx].FilePosition;
            u64 End = Pak->Files[Idx].FilePosition + Pak->Files[Idx].FileLength;
            s8 *DataPtr = FileData;
            while(Pos++ != End)
            {
                Pak->Checksums[Idx] += *DataPtr++;
            }
            
            PlatformMemFree(FileData);
        }
        
        IOFileSeek(HandleIdx, Pak->Header.DirectoryOffset, SeekOrigin_Set);
    }
    IOFileClose(HandleIdx);
    return(Pak);
}

internal s8 *
HFormatLoadPakFile(s8 *FileName, hformat_pak *Pak, ms *Size)
{
#if 0
    for(mi Idx = 0;
        Idx < Pak->FileCount;
        Idx++)
    {
        if(StringsAreEqual((char *)Pak->Files[Idx].FileName, (char *)FileName))
        {
            IOFileSeek(Pak->PakHandle, Pak->Files[Idx].FilePosition, (seek_origin)0);
            
            s8 *FileData = (s8 *)PlatformMemAlloc(Pak->Files[Idx].FileLength);
            IOFileRead(Pak->PakHandle, FileData, Pak->Files[Idx].FileLength);
            
            if(Size)
            {
                *Size = Pak->Files[Idx].FileLength;
            }
            return(FileData);
        }
    }
#endif
    if(Size)
    {
        *Size = 0;
    }
    return(0);
}

internal hformat_pak *
HFormatReleasePakArchive(hformat_pak *Pak)
{
    PlatformMemFree(Pak->Files);
    PlatformMemFree(Pak->Checksums);
    PlatformMemFree(Pak);
    Pak = 0;
    return(Pak);
}

internal s8 *
HFormatReleasePakFile(s8 *File)
{
    PlatformMemFree(File);
    File = 0;
    return(File);
}

#define HFORMAT_PAK_H
#endif