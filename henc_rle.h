// (c) 2016 ZaKlaus; All Rights Reserved

#if !defined(HENC_RLE_H)

// NOTE(zaklaus): can be changed.
#define HENC_MAX_LITERAL_COUNT 255
#define HENC_MAX_RUN_COUNT 255

doc(henc_rle)
doc_string(Holds compressed/uncompressed data.<br/>NOTE: Caller has to free the memory.)
doc_cat(Compression: RLE)

typedef struct
{
    ms MemorySize;
    u8 *Memory;
} henc_rle;

doc_sep()

internal ms
HENCGetOptimalRLESize(u8 *Src, ms SrcSize)
{
    return(256 + 2*SrcSize + 8);
}

doc(RLECompressMemory)
doc_string(Compresses data using Run-Length Encoding technique.)

internal henc_rle
HENCCompressRLEMemory(u8 *Src, ms SrcSize)
{
    henc_rle Result = {0};
    
    ms TotalSize = HENCGetOptimalRLESize(Src, SrcSize);
    u8 *Dst = (u8 *)PlatformMemAlloc(TotalSize);
    u8 *DstInit = Dst;
    *(u32 *)Dst = 0x454c5248; // NOTE(zaklaus): "HRLE" signature
    Dst += 4;
    
    // TODO(zaklaus): Support files bigger than 4GiB.
    *(u32 *)Dst = (u32)SrcSize;
    Dst += 4;
    
    u8 Literals[HENC_MAX_LITERAL_COUNT];
       s32 LiteralCount = 0;
    
    u8 *End = Src + SrcSize;
    
    while(Src < End)
    {
        u8 Value = *Src;
          ms Run = 1;
        while((Run < (ms)(End - Src)) &&
              (Run < HENC_MAX_RUN_COUNT) &&
              (Src[Run] == Value))
        {
            ++Run;
        }
        
        if((Run > 1) || (LiteralCount == HENC_MAX_LITERAL_COUNT))
        {
            Assert(LiteralCount == (u8)LiteralCount);
            *Dst++ = (u8)LiteralCount;
            for(s32 LiteralIdx = 0;
                LiteralIdx < LiteralCount;
                ++LiteralIdx)
            {
                *Dst++ = Literals[LiteralIdx];
            }
            LiteralCount = 0;
            
            Assert(Run == (u8)Run);
            *Dst++ = (u8)Run;
            *Dst++ = Value;
            
            Src += Run;
        }
        else
        {
            Literals[LiteralCount++] = Value;
            ++Src;
        }
    }
    
    Assert(Src == End);
    
    Result.MemorySize = Dst - DstInit;
    Result.Memory = DstInit;
    
    return(Result);
}

doc(RLEDecompressMemory)
doc_string(Decompresses data using Run-Length Encoding technique.)

internal henc_rle
HENCDecompressRLEMemory(u8 *Src, ms SrcSize)
{
    henc_rle Result = {0};
    
    if(*(u32 *)Src == 0x454c5248) // NOTE(zaklaus): "HRLE" signature
    {
        Src += 4;
         ms DecompressedSize = *(u32 *)Src;
        Src += 4;
        
        u8 *Dst = (u8 *)PlatformMemAlloc(DecompressedSize);
        u8 *DstInit = Dst;
        ms Size = SrcSize-8;
        u8 *End = Src + Size;
        
        while(Src < End)
        {
            s32 LiteralCount = *Src++;
            while(LiteralCount--)
            {
                *Dst++ = *Src++;
            }
            
            s32 ReplicationCount = *Src++;
            u8 ReplicationValue = *Src++;
            while(ReplicationCount--)
            {
                *Dst++ = ReplicationValue;
            }
        }
        
        Assert(Src == End);
        
        Result.MemorySize = DecompressedSize;
        Result.Memory = DstInit;
    }
    else
    {
        fprintf(stderr, "Not a valid HRLE format!\n");
    }
    
    return(Result);
}

#define HENC_RLE_H
#endif