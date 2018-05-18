#include "sme_vdp.h"

u16 smeSCREEN_Width2;
u16 smeSCREEN_Height2;
u16 smeSCREEN_Width8;
u16 smeSCREEN_Height8;
u16 smeSCREEN_Width16;
u16 smeSCREEN_Height16;
u16 smePLAN_WIDTH;
u16 smePLAN_HEIGHT;

void smeVDP_Initialize()
{
    //VDP_init();

    VDP_setHInterrupt(0);
    VDP_setHilightShadow(0);
    VDP_setScreenWidth256();

    smeSCREEN_Width2 = screenWidth>>1;
    smeSCREEN_Height2 = screenHeight>>1;
    smeSCREEN_Width8 = screenWidth>>3;
    smeSCREEN_Height8 = screenHeight>>3;
    smeSCREEN_Width16 = screenWidth>>4;
    smeSCREEN_Height16 = screenHeight>>4;
    
    smePLAN_WIDTH = VDP_getPlanWidth();
    smePLAN_HEIGHT = VDP_getPlanHeight();
}

void smeVDP_Finalize()
{
}

void smeVDP_SetScrollMode(u8 horizontal, u8 vertical)
{
    VDP_setReg(0x0b, horizontal|vertical);
} 

