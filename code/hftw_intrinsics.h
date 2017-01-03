// (c) 2016 ZaKlaus; All Rights Reserved

#if !defined(HFTW_INTRINSICS_H)


internal int32
SignOf(int32 Value)
{
    int32 Result = (Value >= 0) ? 1 : -1;
    return(Result);
}

internal real32
SignOf_r32(real32 Value)
{
    real32 Result = (Value >= 0) ? 1.0f : -1.0f;
    return(Result);
}

internal real32
AbsoluteValue(real32 Real32)
{
    real32 Result = (real32)fabs(Real32);
    return(Result);
}

internal uint32
RotateLeft(uint32 Value, int32 Amount)
{
#if COMPILER_MSVC
    uint32 Result = _rotl(Value, Amount);
#else
    // TODO(casey): Actually port this to other compiler platforms!
    Amount &= 31;
    uint32 Result = ((Value << Amount) | (Value >> (32 - Amount)));
#endif

    return(Result);
}

internal uint32
RotateRight(uint32 Value, int32 Amount)
{
#if COMPILER_MSVC
    uint32 Result = _rotr(Value, Amount);
#else
    // TODO(casey): Actually port this to other compiler platforms!
    Amount &= 31;
    uint32 Result = ((Value >> Amount) | (Value << (32 - Amount)));
#endif

    return(Result);
}

internal int32
RoundReal32ToInt32(real32 Real32)
{
    int32 Result = (int32)roundf(Real32);
    return(Result);
}

internal uint32
RoundReal32ToUInt32(real32 Real32)
{
    uint32 Result = (uint32)roundf(Real32);
    return(Result);
}

internal int32 
FloorReal32ToInt32(real32 Real32)
{
    int32 Result = (int32)floorf(Real32);
    return(Result);
}

internal int32 
CeilReal32ToInt32(real32 Real32)
{
    int32 Result = (int32)ceilf(Real32);
    return(Result);
}

internal int32
TruncateReal32ToInt32(real32 Real32)
{
    int32 Result = (int32)Real32;
    return(Result);
}

internal real32
Sin(real32 Angle)
{
    real32 Result = sinf(Angle);
    return(Result);
}

internal real32
Cos(real32 Angle)
{
    real32 Result = cosf(Angle);
    return(Result);
}

internal real32
ATan2(real32 Y, real32 X)
{
    real32 Result = atan2f(Y, X);
    return(Result);
}

typedef struct
{
    bool32 Found;
    uint32 Index;
} bit_scan_result;

internal bit_scan_result
FindLeastSignificantSetBit(uint32 Value)
{
    bit_scan_result Result = {0};

#if COMPILER_MSVC
    Result.Found = _BitScanForward((unsigned long *)&Result.Index, Value);
#else
    for(uint32 Test = 0;
        Test < 32;
        ++Test)
    {
        if(Value & (1 << Test))
        {
            Result.Index = Test;
            Result.Found = 1;
            break;
        }
    }
#endif
    
    return(Result);
}


#define HFTW_INTRINSICS_H
#endif
