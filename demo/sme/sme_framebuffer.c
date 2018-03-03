#include "sme_framebuffer.h"

u8* smeFRAMEBUFFER;
u16 smeFRAMEBUFFER_Height;
u16 smeFRAMEBUFFER_Offsets[256];

void smeFRAMEBUFFER_Initialize()
{
    BMP_init(FALSE, PAL0, FALSE);
    smeFRAMEBUFFER = BMP_getWritePointer(0, 0);
    
    u16 i;
    for (i=0 ; i<BMP_HEIGHT ; ++i)
        smeFRAMEBUFFER_Offsets[i] = i*BMP_WIDTH;
}

void smeFRAMEBUFFER_Finalize()
{
    BMP_end();
}

void smeFRAMEBUFFER_Update()
{
    BMP_flip(TRUE);
    smeFRAMEBUFFER = BMP_getWritePointer(0, 0);
}
