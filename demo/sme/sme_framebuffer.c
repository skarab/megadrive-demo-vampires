#include "sme_framebuffer.h"
#include "sme_screen.h"

u16* smeFRAMEBUFFER;
const u16 smeFRAMEBUFFER_Width = 64;
const u16 smeFRAMEBUFFER_Height = 160;
u16* smeFRAMEBUFFER_Offsets;

void smeFRAMEBUFFER_Initialize()
{
    smeFRAMEBUFFER_Offsets = (u16*)MEM_alloc(smeFRAMEBUFFER_Height*sizeof(u16));
    u16 i;
    for (i=0 ; i<smeFRAMEBUFFER_Height ; ++i)
        smeFRAMEBUFFER_Offsets[i] = i*smeFRAMEBUFFER_Width;

    BMP_init(FALSE, PAL0, FALSE);
    smeSCREEN_SetScrollMode(smeSCREEN_HSCROLL_Plane, smeSCREEN_VSCROLL_Plane);
    smeFRAMEBUFFER = (u16*)BMP_getWritePointer(0, 0);
}

void smeFRAMEBUFFER_Finalize()
{
    MEM_free(smeFRAMEBUFFER_Offsets);
}

void smeFRAMEBUFFER_Update()
{
    BMP_flip(1);
    smeFRAMEBUFFER = (u16*)BMP_getWritePointer(0, 0);
}

