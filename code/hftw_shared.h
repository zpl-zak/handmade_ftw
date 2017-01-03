// (c) 2016 ZaKlaus; All Rights Reserved

#if !defined(HFTW_SHARED_H)

internal void *
Copy(memory_index Size, const void *SourceInit, void *DestInit)
{
    u8 *Source = (u8 *)SourceInit;
    u8 *Dest = (u8 *)DestInit;
    while(Size--) {*Dest++ = *Source++;}
    
    return(DestInit);
}

internal b32
IsEndOfLine(char C)
{
    b32 Result = ((C == '\n') ||
                  (C == '\r'));

    return(Result);
}

internal b32
IsWhitespace(char C)
{
    b32 Result = ((C == ' ') ||
                  (C == '\t') ||
                  (C == '\v') ||
                  (C == '\f') ||
                  IsEndOfLine(C));

    return(Result);
}

internal b32
StringsAreEqual(char *A, char *B)
{
    b32 Result = (A == B);

    if(A && B)
    {
        while(*A && *B && (*A == *B))
        {
            ++A;
            ++B;
        }

        Result = ((*A == 0) && (*B == 0));
    }
    
    return(Result);
}

internal b32
StringsAreEqualA(umm ALength, char *A, char *B)
{
    b32 Result = 0;
    
    if(B)
    {
        char *At = B;
        for(umm Index = 0;
            Index < ALength;
            ++Index, ++At)
        {
            if((*At == 0) ||
               (A[Index] != *At))
            {
               return(0);
            }        
        }
            
        Result = (*At == 0);
    }
    else
    {
        Result = (ALength == 0);
    }
    
    return(Result);
}

internal b32
StringsAreEqualAB(memory_index ALength, char *A, memory_index BLength, char *B)
{
    b32 Result = (ALength == BLength);

    if(Result)
    {
        Result = 1;
        for(u32 Index = 0;
            Index < ALength;
            ++Index)
        {
            if(A[Index] != B[Index])
            {
                Result = 0;
                break;
            }
        }
    }

    return(Result);
}

internal s32
S32FromZInternal(char **AtInit)
{
    s32 Result = 0;
    
    char *At = *AtInit;
    while((*At >= '0') &&
          (*At <= '9'))
    {
        Result *= 10;
        Result += (*At - '0');
        ++At;
    }
    
    *AtInit = At;
    
    return(Result);
}

internal s32
S32FromZ(char *At)
{
    char *Ignored = At;
    s32 Result = S32FromZInternal(&At);
    return(Result);
}

#define HFTW_SHARED_H
#endif