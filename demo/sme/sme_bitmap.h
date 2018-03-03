#ifndef _SME_BITMAP_H_
#define _SME_BITMAP_H_

#include "genesis.h"
#include "slz.h"

typedef struct
{
    u16 Width;
    u16 Height;
    u8 Depth;
    u16* Palette;
    u16* Image;
    u8* Unpacked;
    const u8* Packed; 
} smeBitmap;

extern u8* DecompressSlz(u8* output, const u8* input);

void smeBITMAP_Acquire(smeBitmap* bitmap);
void smeBITMAP_Release(smeBitmap* bitmap);
void smeBITMAP_AcquireBuffer(smeBitmap* bitmap, u8* buffer);
void smeBITMAP_ReleaseBuffer(smeBitmap* bitmap);

#endif
