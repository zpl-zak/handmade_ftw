// (c) 2016 ZaKlaus; All Rights Reserved

#include "hftw.h"

int
main(void)
{
    
#if 1
    {
        // NOTE(zaklaus): RLE test code!
        ms FileSize;
        s32 FileIndex2 = IOFileOpenRead("data/test1.hha", &FileSize);
        {
            u8 *FileData = PlatformMemAlloc(FileSize);
            IOFileRead(FileIndex2, FileData, FileSize);
            henc_rle Data = HENCCompressRLEMemory(FileData, FileSize);
            
            s32 OutFile = IOFileOpenWrite("data/test.rle");
            IOFileWrite(OutFile, Data.Memory, Data.MemorySize);
            IOFileClose(OutFile);
            PlatformMemFree(Data.Memory);
            
            ms InSize;
            s32 InFile = IOFileOpenRead("data/test.rle", &InSize);
            FileData = PlatformMemAlloc(InSize);
            IOFileRead(InFile, FileData, InSize);
            henc_rle BMP = HENCDecompressRLEMemory(FileData, InSize);
            
            OutFile = IOFileOpenWrite("data/test_rle.hha");
            IOFileWrite(OutFile, BMP.Memory, BMP.MemorySize);
            IOFileClose(OutFile);
        }
        IOFileClose(FileIndex2);
    }
#endif
    
#if 1
    {
        // NOTE(zaklaus): LZ test code!
        ms FileSize;
        s32 FileIndex2 = IOFileOpenRead("data/test1.hha", &FileSize);
        {
            u8 *FileData = PlatformMemAlloc(FileSize);
            IOFileRead(FileIndex2, FileData, FileSize);
            henc_lz Data = HENCCompressLZMemory(FileData, FileSize);
            
            s32 OutFile = IOFileOpenWrite("data/test.lz");
            IOFileWrite(OutFile, Data.Memory, Data.MemorySize);
            IOFileClose(OutFile);
            PlatformMemFree(Data.Memory);
            
            ms InSize;
            s32 InFile = IOFileOpenRead("data/test.lz", &InSize);
            FileData = PlatformMemAlloc(InSize);
            IOFileRead(InFile, FileData, InSize);
            henc_lz BMP = HENCDecompressLZMemory(FileData, InSize);
            
            OutFile = IOFileOpenWrite("data/test_lz.hha");
            IOFileWrite(OutFile, BMP.Memory, BMP.MemorySize);
            IOFileClose(OutFile);
        }
        IOFileClose(FileIndex2);
    }
#endif
    return(0);
}