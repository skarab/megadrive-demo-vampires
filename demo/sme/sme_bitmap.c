#include "sme_bitmap.h"

void smeBITMAP_Acquire(smeBitmap* bitmap)
{
    smeBITMAP_AcquireBuffer(bitmap, (u8*)MEM_alloc(bitmap->Depth*2+bitmap->Width*bitmap->Height*2));
}

void smeBITMAP_Release(smeBitmap* bitmap)
{
    MEM_free(bitmap->Unpacked);
    smeBITMAP_ReleaseBuffer(bitmap);
}

void smeBITMAP_AcquireBuffer(smeBitmap* bitmap, u8* buffer)
{
    decompress_slz(buffer, bitmap->Packed);
    bitmap->Palette = (u16*)buffer;
    bitmap->Image = (u16*)(buffer+bitmap->Depth*2);
    bitmap->Unpacked = buffer;        
}

void smeBITMAP_ReleaseBuffer(smeBitmap* bitmap)
{
    bitmap->Unpacked = NULL;
    bitmap->Image = NULL;
    bitmap->Palette = NULL;
}
