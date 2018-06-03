#include "genesis.h"
#include "sme/sme.h"
#include "music.h"
#include "res/resources.h"

void EFFECT_Warning()
{
	VDP_setPaletteColors(0, warning.palette->data, 16);

	u16 w = warning.w;
    	u16 h = warning.h;
    	VDP_loadBMPTileData((u32*)warning.image, 1, w/8, h/8, w/8);
	int ys;
    	for (ys=0 ; ys<VDP_getPlanHeight()/(h/8); ++ys)
        	VDP_fillTileMapRectInc(APLAN, TILE_ATTR_FULL(PAL0, 0, 0, 0, 1), 0, ys*(h/8), w/8, h/8);

	int time = 0;

    	do
    	{
		++time;
        	sme_Update();
    	}
	while (time<80000);
}
