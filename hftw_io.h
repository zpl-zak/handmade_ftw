// (c) 2016 ZaKlaus; All Rights Reserved

#if !defined(HFTW_IO_H)

#include <stdio.h>

typedef enum
{
    SeekOrigin_Set = SEEK_SET,
    SeekOrigin_Cursor = SEEK_CUR,
    SeekOrigin_End = SEEK_END
} seek_origin;

#define MAX_HANDLES 32
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

internal ms
IOFileLength(FILE *File)
{
     ms Pos, End;
    
    Pos = ftell(File);
    
    fseek(File, 0, SEEK_END);
    
    End = ftell(File);
    fseek(File, (s32)Pos, SEEK_SET);
    
    return(End);
}

internal s32
IOFileOpenRead(s8 *Path, ms *Size)
{
    s32 HandleIdx = IOFindHandle();
    Assert(!FileHandles[HandleIdx]);
    FileHandles[HandleIdx] = fopen(Path, "rb");
    
    if(!FileHandles[HandleIdx])
    {
        fprintf(stderr, "Could not open file for read!\n");
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

internal s32
IOFileOpenWrite(s8 *Path)
{
    s32 HandleIdx = IOFindHandle();
    Assert(!FileHandles[HandleIdx]);
    FileHandles[HandleIdx] = fopen(Path, "wb");
    
    if(!FileHandles[HandleIdx])
    {
        fprintf(stderr, "Could not open file for write!\n");
        return(-1);
    }
    return(HandleIdx);
}

internal void
IOFileClose(s32 HandleIdx)
{
    Assert(HandleIdx >= 0 && HandleIdx < MAX_HANDLES);
    
    if(FileHandles[HandleIdx])
    {
        fclose(FileHandles[HandleIdx]);
        FileHandles[HandleIdx] = 0;
    }
}

internal void
IOFileSeek(s32 HandleIdx, s32 Position, seek_origin Origin)
{
    Assert(HandleIdx >= 0 && HandleIdx < MAX_HANDLES);
    
    if(FileHandles[HandleIdx])
    {
        fseek(FileHandles[HandleIdx], Position, Origin);
    }
}

internal ms
IOFileRead(s32 HandleIdx, void *Dest, ms Size)
{
    Assert(HandleIdx >= 0 && HandleIdx < MAX_HANDLES && Dest);
    Assert(FileHandles[HandleIdx]);
    
    ms BytesRead = fread(Dest, 1, Size, FileHandles[HandleIdx]);
    return(BytesRead);
}


internal ms
IOFileWrite(s32 HandleIdx, void *Src, ms Size)
{
    Assert(HandleIdx >= 0 && HandleIdx < MAX_HANDLES && Src);
    Assert(FileHandles[HandleIdx]);
    
     ms BytesWritten = fwrite(Src, 1, Size, FileHandles[HandleIdx]);
    return(BytesWritten);
}



#define HFTW_IO_H
#endif
