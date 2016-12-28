// (c) 2016 ZaKlaus; All Rights Reserved

#if !defined(HFTW_HENC_H)

doc(henc_data)
doc_string(Holds compressed/uncompressed data.<br/>NOTE: Caller has to free the memory.)
doc_cat(Codecs)

typedef struct
{
    ms MemorySize;
    u8 *Memory;
} henc_data;

doc_ignore()

#define COMPRESSION_HANDLER(name) internal henc_data name (u8 *SrcInit, ms SrcSize)

#define DECOMPRESSION_HANDLER(name) internal henc_data name (u8 *Src, ms SrcSize)

#define COMPRESSION_SIZE_HANDLER(name) internal ms name (u8 *SrcInit, ms SrcSize)

doc_hunt()

#include "codecs/henc_rle.h"
#include "codecs/henc_lz.h"

#define HFTW_HENC_H
#endif