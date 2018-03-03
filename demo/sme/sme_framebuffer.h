#ifndef _SME_FRAMEBUFFER_H_
#define _SME_FRAMEBUFFER_H_

#include "genesis.h"

extern u8* smeFRAMEBUFFER;
extern u16 smeFRAMEBUFFER_Offsets[256];

void smeFRAMEBUFFER_Initialize();
void smeFRAMEBUFFER_Finalize();
void smeFRAMEBUFFER_Update();

#endif
