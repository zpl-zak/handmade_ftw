// (c) 2016 ZaKlaus; All Rights Reserved
/*
 BMP File Format
Usage: Image format
SW: N/A
*/
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
     u8 *Data;
} hformat_bmp;

internal hformat_bmp *
HFormatLoadBMPImage(s32 HandleIdx)
{
    hformat_bmp *Image = PlatformMemAlloc(sizeof(hformat_bmp));
    {
        s32 ImageIdx = 0;
        u8 _Swap0 = 0;
        
        IOFileRead(HandleIdx, &Image->Header, sizeof(hformat_bmp_header));
        
        if(Image->Header.Signature != 0x4D42)
        {
            fprintf(stderr, "Not a valid BMP format!\n");
            PlatformMemFree(Image);
            return(0);
        }
        
        IOFileRead(HandleIdx, &Image->Info, sizeof(hformat_bmp_info));
        
        IOFileSeek(HandleIdx, Image->Header.DataOffset, SeekOrigin_Set);
        
        Image->Data = PlatformMemAlloc(Image->Info.ImageSize);
        
        IOFileRead(HandleIdx, Image->Data, Image->Info.ImageSize);
        
        for(mi Idx = 0;
            Idx < Image->Info.ImageSize;
            Idx += 3)
        {
            _Swap0 = Image->Data[Idx];
            Image->Data[Idx] = Image->Data[Idx + 2];
            Image->Data[Idx + 2] = _Swap0;
        }
    }
    return(Image);
}

internal hformat_bmp *
HFormatReleaseBMPImage(hformat_bmp *Image)
{
    PlatformMemFree(Image->Data);
    PlatformMemFree(Image);
    Image = 0;
    return(Image);
}

#define HFORMAT_BMP_H
#endif