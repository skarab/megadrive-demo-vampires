#include "genesis.h"
#include "sme/sme.h"
#include "music.h"
#include "res/resources.h"

void EFFECT_Credits()
{
	int k;
	for (k=0 ; k<16 ; ++k)
        	VDP_setPaletteColor(k, 0);
    		
	//VDP_setPaletteColors(0, credits.palette->data, 16);

	smeVDP_SetScrollMode(smeVDP_HSCROLL_Plane, smeVDP_HSCROLL_Plane);
        VDP_setHorizontalScroll(PLAN_A, 0);
	VDP_setVerticalScroll(PLAN_A, 0);
        VDP_setHorizontalScroll(PLAN_B, 0);
	   VDP_setVerticalScroll(PLAN_B, 0);
	VDP_clearPlan(APLAN, 0);
	VDP_clearPlan(BPLAN, 0);

	u16 w = credits.w;
    	u16 h = credits.h;
    	VDP_loadBMPTileData((u32*)credits.image, 1, w/8, h/8, w/8);
	int ys;
    	for (ys=0 ; ys<VDP_getPlanHeight()/(h/8); ++ys)
        	VDP_fillTileMapRectInc(APLAN, TILE_ATTR_FULL(PAL0, 0, 0, 0, 1), 0, ys*(h/8), w/8, h/8);

	int m = 0;
	
    	while (1)
    	{
		u32 time = smeSOUND_GetTime(music);
		if (time<0x22155F)
		{
			u8 data = music[time];
			m = (((int)data)*9+m)/10;
		}
		else m = 0;

		int k;
		for (k=0 ; k<16 ; ++k)
            	{
                	int r = ((u16*)credits.palette->data)[k]&15;
                	int g = (((u16*)credits.palette->data)[k]>>4)&15;
                	int b = (((u16*)credits.palette->data)[k]>>8)&15;
             
			r = (r*m)/255;
			g = (g*m)/255;
			b = (b*m)/255;

			VDP_setPaletteColor(k, r|(g<<4)|(b<<8));
		}

        	sme_Update();
    	}
}
