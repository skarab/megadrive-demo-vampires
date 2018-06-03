#include "genesis.h"
#include "sme/sme.h"
#include "music.h"
#include "res/resources.h"

void EFFECT_Sega(int end)
{
	int k;
	for (k=0 ; k<16 ; ++k)
        	VDP_setPaletteColor(k, 0);
    		
	//VDP_setPaletteColors(0, sega.palette->data, 16);

	u16 w = title.w;
    	u16 h = title.h;
    	VDP_loadBMPTileData((u32*)sega.image, 1, w/8, h/8, w/8);
	int ys;
    	for (ys=0 ; ys<VDP_getPlanHeight()/(h/8); ++ys)
        	VDP_fillTileMapRectInc(APLAN, TILE_ATTR_FULL(PAL0, 0, 0, 0, 1), 0, ys*(h/8), w/8, h/8);

	int m = 0;
	u32 time;
	int start_time = smeSOUND_GetTime(music);
		
    	do
    	{
		time = smeSOUND_GetTime(music);
		u8 data = music[time];

		m = (((int)data)*9+m)/10;

		int fade = 255;
		if (time-start_time<200) fade = (time-start_time)*255/200;
		else if (time>3000-200) fade = 255-(time-(3000-200))*255/200;
		
		m = m*fade/255;

		int k;
		for (k=0 ; k<16 ; ++k)
            	{
                	int r = ((u16*)sega.palette->data)[k]&15;
                	int g = (((u16*)sega.palette->data)[k]>>4)&15;
                	int b = (((u16*)sega.palette->data)[k]>>8)&15;
             
			r = (r*m)/255;
			g = (g*m)/255;
			b = (b*m)/255;

			VDP_setPaletteColor(k, r|(g<<4)|(b<<8));
		}

		int y;
		for (y=4 ; y<16 ; ++y)
            	{
                	u32 col = sinFix16(-y*60+time*10)/20;
                	col = (col/20)&15;
                	VDP_setPaletteColor(y, col);
            	}
            
        	sme_Update();
    	}
	while (time<end);
}
