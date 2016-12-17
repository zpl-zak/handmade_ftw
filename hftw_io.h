// (c) 2016 ZaKlaus; All Rights Reserved

#if !defined(HFTW_IO_H)

#include <stdio.h>

//doc(seek_origin)
//doc_cat(IO)
//doc_string(Origin states used by IOFileSeek.)
doc_sig(
typedef enum
{
    SeekOrigin_Set = SEEK_SET,     // Seek from the start of the file.
    SeekOrigin_Cursor = SEEK_CUR,  // Seek from the current position.
    SeekOrigin_End = SEEK_END      // Seek from the end of the file backwards.
} seek_origin;
)

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

//doc(IOFileOpenRead)
//doc_string(Opens file for read access.)
doc_sig(
internal s32
IOFileOpenRead(s8 *Path, // Path to the file.
               ms *Size) // [OUT] File size. (if Size is not NULL)
)
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

//doc(IOFileOpenWrite)
//doc_string(Opens file for write access.)
doc_sig(
internal s32
IOFileOpenWrite(s8 *Path) // Path to the file.
)
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

//doc(IOFileClose)
//doc_string(Closes the file handle.)
doc_sig(
internal void
IOFileClose(s32 HandleIdx) // The ID of the file handle.
)
{
    Assert(HandleIdx >= 0 && HandleIdx < MAX_HANDLES);
    
    if(FileHandles[HandleIdx])
    {
        fclose(FileHandles[HandleIdx]);
        FileHandles[HandleIdx] = 0;
    }
}

//doc(IOFileSeek)
//doc_string(Moves the position of the cursor in the file.)
doc_sig(
internal void
IOFileSeek(s32 HandleIdx,      // The ID of the file handle.
           s32 Position,       // The offset to travel.
           seek_origin Origin) // Specifies the origin from where to travel.
)
{
    Assert(HandleIdx >= 0 && HandleIdx < MAX_HANDLES);
    
    if(FileHandles[HandleIdx])
    {
        fseek(FileHandles[HandleIdx], Position, Origin);
    }
}

//doc(IOFileRead)
//doc_string(Reads the data from the file.)
doc_sig(
internal ms
IOFileRead(s32 HandleIdx,  // The ID of the file handle.
           void *Dest,     // Destination to write read data to.
           ms Size)        // The size to read.
)
{
    Assert(HandleIdx >= 0 && HandleIdx < MAX_HANDLES && Dest);
    Assert(FileHandles[HandleIdx]);
    
    ms BytesRead = fread(Dest, 1, Size, FileHandles[HandleIdx]);
    return(BytesRead);
}

//doc(IOFileWrite)
//doc_string(Writes the data to the file.)
doc_sig(
internal ms
IOFileWrite(s32 HandleIdx, // The ID of the file handle.
            void *Src,     // Source to read the data from.
            ms Size)       // The size to be written.
)
{
    Assert(HandleIdx >= 0 && HandleIdx < MAX_HANDLES && Src);
    Assert(FileHandles[HandleIdx]);
    
     ms BytesWritten = fwrite(Src, 1, Size, FileHandles[HandleIdx]);
    return(BytesWritten);
}



#define HFTW_IO_H
#endif
