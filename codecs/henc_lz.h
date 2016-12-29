// (c) 2016 ZaKlaus; All Rights Reserved

#if !defined(HENC_LZ_H)

// NOTE(zaklaus): can be changed.
#define HENC_LZ_MAX_LITERAL_COUNT 255
#define HENC_LZ_MAX_RUN_COUNT 255
#define HENC_LZ_MAX_LOOKBACK_COUNT 255
#define HENC_LZ_MAGIC 0x305a4c48

typedef henc_data henc_lz;

COMPRESSION_SIZE_HANDLER(HENCGetOptimalLZSize)
{
    return(256 + 2*SrcSize + 8);
}

COMPRESSION_HANDLER(HENCCompressLZMemory)
{
    henc_lz Result = {0};
    
    u8 *Src = SrcInit;
    
    ms TotalSize = HENCGetOptimalLZSize(Src, SrcSize);
    u8 *Dst = (u8 *)PlatformMemAlloc(TotalSize);
    u8 *DstInit = Dst;
    *(u32 *)Dst = HENC_LZ_MAGIC; // NOTE(zaklaus): "HLZ0" signature
    Dst += 4;
    
    // TODO(zaklaus): Support files bigger than 4GiB.
    *(u32 *)Dst = (u32)SrcSize;
    Dst += 4;
    
    u8 Literals[HENC_LZ_MAX_LITERAL_COUNT];
    s32 LiteralCount = 0;
    
    u8 *End = Src + SrcSize;
    
    while(Src < End)
    {
         ms MaxLookback = Src - SrcInit;
        if(MaxLookback > HENC_LZ_MAX_LOOKBACK_COUNT)
        {
            MaxLookback = HENC_LZ_MAX_LOOKBACK_COUNT;
        }
        
        ms BestRun = 0;
        ms BestDistance = 0;
        
        for(u8 *WindowStep = Src - MaxLookback;
            WindowStep < Src;
            ++WindowStep)
        {
             ms WindowSize = End - WindowStep;
            if(WindowSize > HENC_LZ_MAX_RUN_COUNT)
            {
                WindowSize = HENC_LZ_MAX_RUN_COUNT;
            }
            u8 *WindowEnd = WindowStep + WindowSize;
            u8 *Test = Src;
            u8 *Window = WindowStep;
            
            u8 TestRun = 0;
            while((Window < WindowEnd) &&
                  (*Test++ == *Window++))
            {
                ++TestRun;
            }
            
            if(BestRun < TestRun)
            {
                BestRun = TestRun;
                BestDistance = Src - WindowStep;
            }
        }
        
        b32 OutputRun = 0;
        
        if(LiteralCount)
        {
            OutputRun = (BestRun > 4);
        }
        else
        {
            OutputRun = (BestRun > 2);
        }
        
        if(OutputRun || (LiteralCount == HENC_LZ_MAX_LITERAL_COUNT))
        {
            Assert(LiteralCount == (u8)LiteralCount);
            if(LiteralCount)
            {
                *Dst++ = (u8)LiteralCount;
                *Dst++ = 0; // NOTE(zaklaus): 0 copy count.
                
            for(s32 LiteralIdx = 0;
                LiteralIdx < LiteralCount;
                ++LiteralIdx)
            {
                *Dst++ = Literals[LiteralIdx];
            }
            LiteralCount = 0;
        }
        
        if(OutputRun)
        {
            Assert(BestRun == (u8)BestRun);
            *Dst++ = (u8)BestRun;
            
            Assert(BestDistance == (u8)BestDistance);
            *Dst++ = (u8)BestDistance;
            
            Src += BestRun;
        }
        }
        else
        {
            Literals[LiteralCount++] = *Src++;
        }
    }
    
    Assert(Src == End);
    
    Result.MemorySize = Dst - DstInit;
    Result.Memory = DstInit;
    
    return(Result);
}

DECOMPRESSION_HANDLER(HENCDecompressLZMemory)
{
    henc_lz Result = {0};
    
    if(*(u32 *)Src == HENC_LZ_MAGIC) // NOTE(zaklaus): "HLZ0" signature
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
            s32 Count = *Src++;
            u8 CopyDistance = *Src++;
            u8 *Source = (Dst - CopyDistance);
                
            if(CopyDistance == 0)
                {
                    Source = Src;
                    Src += Count;
                }
                
            while(Count--)
            {
                *Dst++ = *Source++;
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

doc(LZ Compression)
doc_string(To be documented.)
doc_embed(docs/lz_doc.html)

#define HENC_LZ_H
#endif