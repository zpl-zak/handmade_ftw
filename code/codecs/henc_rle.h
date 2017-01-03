// (c) 2016 ZaKlaus; All Rights Reserved

#if !defined(HENC_RLE_H)

// NOTE(zaklaus): can be changed.
#define HENC_RLE_MAX_LITERAL_COUNT 255
#define HENC_RLE_MAX_RUN_COUNT 255
#define HENC_RLE_MAGIC 0x454c5248

typedef henc_data henc_rle;

COMPRESSION_SIZE_HANDLER(HENCGetOptimalRLESize)
{
    return(256 + 2*SrcSize + 8);
}

COMPRESSION_HANDLER(HENCCompressRLEMemory)
{
    henc_rle Result = {0};
    
    u8 *Src = SrcInit;
    ms TotalSize = HENCGetOptimalRLESize(Src, SrcSize);
    u8 *Dst = (u8 *)PlatformMemAlloc(TotalSize);
    u8 *DstInit = Dst;
    *(u32 *)Dst = HENC_RLE_MAGIC; // NOTE(zaklaus): "HRLE" signature
    Dst += 4;
    
    // TODO(zaklaus): Support files bigger than 4GiB.
    *(u32 *)Dst = (u32)SrcSize;
    Dst += 4;
    
    u8 Literals[HENC_RLE_MAX_LITERAL_COUNT];
       s32 LiteralCount = 0;
    
    u8 *End = Src + SrcSize;
    
    while(Src < End)
    {
        u8 Value = *Src;
          ms Run = 1;
        while((Run < (ms)(End - Src)) &&
              (Run < HENC_RLE_MAX_RUN_COUNT) &&
              (Src[Run] == Value))
        {
            ++Run;
        }
        
        if((Run > 1) || (LiteralCount == HENC_RLE_MAX_LITERAL_COUNT))
        {
            Assert(LiteralCount == (u8)LiteralCount);
            Assert(Run == (u8)Run);
            
            *Dst++ = (u8)LiteralCount;
            for(s32 LiteralIdx = 0;
                LiteralIdx < LiteralCount;
                ++LiteralIdx)
            {
                *Dst++ = Literals[LiteralIdx];
            }
            LiteralCount = 0;
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
    
    ms ResultSize = Dst - DstInit;
    u8 *ResultPtr = (u8 *)PlatformMemMove(DstInit, ResultSize);
    
    Result.MemorySize = ResultSize;
    Result.Memory = ResultPtr;
    
    return(Result);
}

DECOMPRESSION_HANDLER(HENCDecompressRLEMemory)
{
    henc_rle Result = {0};
    
    if(*(u32 *)Src == HENC_RLE_MAGIC) // NOTE(zaklaus): "HRLE" signature
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

doc(RLE Compression)
doc_string(To be documented.)
doc_embed(docs/rle_doc.html)

#define HENC_RLE_H
#endif