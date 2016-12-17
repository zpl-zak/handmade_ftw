// (c) 2016 ZaKlaus; All Rights Reserved

#if !defined(HFORMAT_BMP_H)

#include "hftw.h"
#include <stdio.h>

#pragma pack(push, 1)
typedef struct
{
    u16 Signature; // NOTE(zaklaus): Should always be 'BM' (0x4D42) except OS/2
    u32 FileSize;
    u16 _Reserved0;
    u16 _Reserved1;
    u32 DataOffset;
} hformat_bmp_header;

typedef struct
{
    u32 Size;
    s32 Width;
    s32 Height;
    u16 Planes; // NOTE(zaklaus): Should always be 1
    u16 BitCount; // NOTE(zaklaus): Number of bits per pixel (BPP)
    u32 Compression;
    u32 ImageSize; // NOTE(zaklaus): Determined in bytes
    s32 XPixelsPerMeter; // NOTE(zaklaus): Number of pixels per meter in x axis
    s32 YPixelsPerMeter; // NOTE(zaklaus): Number of pixels per meter in y axis
    u32 ColorsUsed; // NOTE(zaklaus): Number of colors used by the bitmap
    u32 ColorsImportant; // NOTE(zaklaus): Number of colors that are important
} hformat_bmp_info;
#pragma pack(pop)

typedef struct
{
    hformat_bmp_header Header;
    hformat_bmp_info Info;
    memory_arena Data; // NOTE(zaklaus): DO NOT FORGET to call ArenaFree on the memory arena, once you don't need the BMP!
} hformat_bmp;

internal hformat_bmp *
HFormatLoadBMPImage(FILE *File)
{
    hformat_bmp *Image = PlatformMemAlloc(sizeof(hformat_bmp));
    {
        s32 ImageIdx = 0;
        u8 _Swap0 = 0;
        
        fread(&Image->Header, sizeof(hformat_bmp_header), 1, File);
        
        if(Image->Header.Signature != 0x4D42)
        {
            fprintf(stderr, "Not a valid BMP format!\n");
            PlatformMemFree(Image);
            return(0);
        }
        
        fread(&Image->Info, sizeof(hformat_bmp_info), 1, File);
        
        fseek(File, Image->Header.DataOffset, SEEK_SET);
        
        ArenaBuild(&Image->Data, Image->Info.ImageSize);
        
        fread(Image->Data.Base, sizeof(u8), Image->Info.ImageSize, File);
        
        for(mi Idx = 0;
            Idx < Image->Info.ImageSize;
            Idx += 3)
        {
            _Swap0 = Image->Data.Base[Idx];
            Image->Data.Base[Idx] = Image->Data.Base[Idx + 2];
            Image->Data.Base[Idx + 2] = _Swap0;
        }
    }
    return(Image);
}

#define HFORMAT_BMP_H
#endif