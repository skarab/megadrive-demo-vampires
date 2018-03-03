#ifndef _SME_VDP_H_
#define _SME_VDP_H_

#include "genesis.h"

#define smeVDP_HSCROLL_Plane  0
#define smeVDP_HSCROLL_Row    2
#define smeVDP_HSCROLL_Line   3
#define smeVDP_VSCROLL_Plane  0
#define smeVDP_VSCROLL_Column 4

extern u16 smeSCREEN_Width2;
extern u16 smeSCREEN_Height2;
extern u16 smeSCREEN_Width8;
extern u16 smeSCREEN_Height8;
extern u16 smeSCREEN_Width16;
extern u16 smeSCREEN_Height16;

extern u16 smePLAN_WIDTH;
extern u16 smePLAN_HEIGHT;

void smeVDP_Initialize();
void smeVDP_Finalize();
void smeVDP_SetScrollMode(u8 horizontal, u8 vertical);

#endif
