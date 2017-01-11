// (c) 2016 ZaKlaus; All Rights Reserved

#if !defined(HFTW_IO_H)

#include <stdio.h>

#if 0
doc(seek_origin)
doc_cat(IO)
doc_string(Origin states used by IOFileSeek.)

typedef enum
{
    SeekOrigin_Set = 0,     // Seek from the start of the file.
    SeekOrigin_Cursor = 1,  // Seek from the current position.
    SeekOrigin_End = 2      // Seek from the end of the file backwards.
} seek_origin;
#else
typedef enum
{
    SeekOrigin_Set = SEEK_SET,        // Seek from the start of the file.
    SeekOrigin_Cursor = SEEK_CUR,  // Seek from the current position.
    SeekOrigin_End = SEEK_END         // Seek from the end of the file backwards.
} seek_origin;
#endif
#define MAX_HANDLES 128
global_variable FILE *FileHandles[MAX_HANDLES] = {0};


internal s32
IOFindHandle(void)
{
    s32 HandleIdx = -1;
    for(s32 Idx = 0;
        Idx < MAX_HANDLES;
        Idx++)
    {
        if(FileHandles[Idx] == 0)
        {
            HandleIdx = Idx;
            break;
        }
    }
    Assert(HandleIdx != -1);
    return(HandleIdx);
}

doc(IOFileLength)
doc_string(Retrieves the file length.)

internal ms
IOFileLength(FILE *File)
{
    #if 0
    LARGE_INTEGER FileSizeEx;
    GetFileSizeEx(File, &FileSizeEx);
    ms FileSize = (ms)FileSizeEx.QuadPart;
    return(FileSize);
    #else
     ms Pos, End;
    
    Pos = ftell(File);
    
    fseek(File, 0, SEEK_END);
    
    End = ftell(File);
    fseek(File, (s32)Pos, SEEK_SET);
    
    return(End);
    #endif
}

doc(IOFileOpenRead)
doc_string(Opens file for read access.)

internal s32
IOFileOpenRead(s8 *Path, // Path to the file.
               ms *Size) // [OUT][OPT] File size
{
    s32 HandleIdx = IOFindHandle();
    Assert(!FileHandles[HandleIdx]);
    
#if 0
    FileHandles[HandleIdx] = CreateFileA((const char *)Path, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
#else
    FileHandles[HandleIdx] = fopen((const char *)Path, "rb");
#endif
    
    if(!FileHandles[HandleIdx])
    {
        //fprintf(stderr, "Could not open file for read!\n");
        if(Size)
        {
            *Size = 0;
        }
        return(-1);
    }
    
    if(Size)
    {
        *Size = IOFileLength(FileHandles[HandleIdx]);
    }
    
    return(HandleIdx);
}

doc(IOFileOpenWrite)
doc_string(Opens file for write access.)

internal s32
IOFileOpenWrite(s8 *Path) // Path to the file.
{
    s32 HandleIdx = IOFindHandle();
    Assert(!FileHandles[HandleIdx]);
    
    #if 0
    FileHandles[HandleIdx] = CreateFileA((const char *)Path, GENERIC_WRITE, FILE_SHARE_WRITE, 0, CREATE_ALWAYS, 0, 0);
    #else
    FileHandles[HandleIdx] = fopen((const char *)Path, "wb");
    #endif
    
    if(!FileHandles[HandleIdx])
    {
        //fprintf(stderr, "Could not open file for write!\n");
        return(-1);
    }
    return(HandleIdx);
}

doc(IOFileClose)
doc_string(Closes the file handle.)

internal void
IOFileClose(s32 HandleIdx) // The ID of the file handle.
{
    Assert(HandleIdx >= 0 && HandleIdx < MAX_HANDLES);
    
    if(FileHandles[HandleIdx])
    {
        #if 0
        CloseHandle(FileHandles[HandleIdx]);
        #else
        fclose(FileHandles[HandleIdx]);
        #endif
        FileHandles[HandleIdx] = 0;
    }
}

doc(IOFileSeek)
doc_string(Moves the position of the cursor in the file.)

internal void
IOFileSeek(s32 HandleIdx,      // The ID of the file handle.
           s32 Position,       // The offset to travel.
           seek_origin Origin) // [OPT] Specifies the origin from where to travel.
{
    Assert(HandleIdx >= 0 && HandleIdx < MAX_HANDLES);
    
    if(!Origin)
    {
        Origin = SeekOrigin_Set;
    }
    
    if(FileHandles[HandleIdx])
    {
        #if 0
        LARGE_INTEGER PositionEx = {0};
        PositionEx.QuadPart = Position;
        SetFilePointerEx(FileHandles[HandleIdx], PositionEx, 0, Origin);
        #else
        fseek(FileHandles[HandleIdx], Position, Origin);
        #endif
    }
}

doc(IOFileRead)
doc_string(Reads the data from the file.)

internal ms
IOFileRead(s32 HandleIdx,  // The ID of the file handle.
           void *Dest,     // Destination to write read data to.
           ms Size)        // The size to read.
{
    Assert(HandleIdx >= 0 && HandleIdx < MAX_HANDLES && Dest);
    Assert(FileHandles[HandleIdx]);
    
    #if 0
    ms BytesRead;
    ReadFile(FileHandles[HandleIdx], Dest, (WORD)Size, (LPDWORD)&BytesRead, 0);
    #else
    ms BytesRead = fread(Dest, 1, Size, FileHandles[HandleIdx]);
    #endif
    return(BytesRead);
}

doc(IOFileWrite)
doc_string(Writes the data to the file.)

internal ms
IOFileWrite(s32 HandleIdx, // The ID of the file handle.
            void *Src,     // Source to read the data from.
            ms Size)       // The size to be written.
{
    Assert(HandleIdx >= 0 && HandleIdx < MAX_HANDLES && Src);
    Assert(FileHandles[HandleIdx]);
    
#if 0
    ms BytesWritten;
    WriteFile(FileHandles[HandleIdx], Src, (WORD)Size, (LPDWORD)&BytesWritten, 0);
#else
    ms BytesWritten = fwrite(Src, 1, Size, FileHandles[HandleIdx]);
#endif
    return(BytesWritten);
}

doc(IOGetStringLength)
doc_string(Retrieves length of a null-terminated string in stream.)

internal u32
IOGetStringLength(s32 HandleIdx) // The ID of the file handle.
{
    Assert(HandleIdx >= 0 && HandleIdx < MAX_HANDLES);
    Assert(FileHandles[HandleIdx]);
    
#if 0
    #define SZTELL GetFilePointerEx
    #else
    #define SZTELL ftell
    #endif
    
    u8 C;
    
    u32 Start = (u32)SZTELL(FileHandles[HandleIdx]);
        
        do
        {
            IOFileRead(HandleIdx, &C, sizeof(u8));
        } while(C != 0);
        u32 End = (u32)SZTELL(FileHandles[HandleIdx]);
        
        IOFileSeek(HandleIdx, Start, SeekOrigin_Set);
        return(End - Start);
        #undef SZTELL
}

#define IOConsolePrint(out, format, ...) fprintf(out, format, __VA_ARGS__)

#define HFTW_IO_H
#endif
