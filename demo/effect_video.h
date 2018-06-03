#include "genesis.h"
#include "sme/sme.h"
#include "music.h"
#include "res/resources.h"

void EFFECT_Video(int end, int mode)
{
	VDP_clearPlan(APLAN, 0);
	VDP_clearPlan(BPLAN, 0);

	int v;
	for (v=0 ; v<16 ; ++v)
        	VDP_setPaletteColor(v, 0);

	u16 w = video_1.w;
    	u16 h = video_1.h;
    	//VDP_setPalette(PAL0, video_1.palette->data);
    	VDP_loadBMPTileData((u32*)video_1.image, 1, w/8, h/8, w/8);
	int ys;
    	for (ys=0 ; ys<VDP_getPlanHeight()/(h/8)-2; ++ys)
        	VDP_fillTileMapRectInc(APLAN, TILE_ATTR_FULL(PAL0, 0, 0, 0, 1), 0, ys*(h/8)+4, w/8, h/8);

	if (mode==1)
	{
		w = vip.w;
    		h = vip.h;
    		VDP_setPalette(PAL1, vip.palette->data);
		VDP_loadBMPTileData((u32*)vip.image, 257, w/8, h/8, w/8);
    		for (ys=0 ; ys<VDP_getPlanHeight()/(h/8); ++ys)
        		VDP_fillTileMapRectInc(BPLAN, TILE_ATTR_FULL(PAL1, 0, 0, 0, 257), 0, ys*(h/8), w/8, h/8);
	}

	int k = 0;
	int m = 0;
	int old_m = 0;

	u32 time;
	u32 start_time = smeSOUND_GetTime(music);
	int t = 0;
    	do
    	{
		time = smeSOUND_GetTime(music);
		u8 data = music[time];

		t = (((int)data)*9+t)/10;
		t *= 5;

		if (t>255) t = 255;
		
		int fade = 255;
		if (time-start_time<200) fade = (time-start_time)*255/200;
		else if (time>end-200) fade = 255-(time-(end-200))*255/200;
		if (fade<0) fade = 0;

		t = t*fade/255;

		for (v=0 ; v<16 ; ++v)
            	{
                	int r = ((u16*)video_1.palette->data)[v]&15;
                	int g = (((u16*)video_1.palette->data)[v]>>4)&15;
                	int b = (((u16*)video_1.palette->data)[v]>>8)&15;
             
			r = (r*t)/255;
			g = (g*t)/255;
			b = (b*t)/255;

			VDP_setPaletteColor(v, r|(g<<4)|(b<<8));
		}

		if (old_m!=m)
		{
			old_m = m;

			if (m==0) VDP_loadBMPTileData((u32*)video_1.image, 1, w/8, h/8, w/8);
			else if (m==1) VDP_loadBMPTileData((u32*)video_2.image, 1, w/8, h/8, w/8);
			else if (m==2) VDP_loadBMPTileData((u32*)video_3.image, 1, w/8, h/8, w/8);
			else if (m==3) VDP_loadBMPTileData((u32*)video_4.image, 1, w/8, h/8, w/8);
			else if (m==4) VDP_loadBMPTileData((u32*)video_5.image, 1, w/8, h/8, w/8);
			else if (m==5) VDP_loadBMPTileData((u32*)video_6.image, 1, w/8, h/8, w/8);
			else if (m==6) VDP_loadBMPTileData((u32*)video_7.image, 1, w/8, h/8, w/8);
			else if (m==7) VDP_loadBMPTileData((u32*)video_8.image, 1, w/8, h/8, w/8);
			else if (m==8) VDP_loadBMPTileData((u32*)video_9.image, 1, w/8, h/8, w/8);
			else if (m==9) VDP_loadBMPTileData((u32*)video_10.image, 1, w/8, h/8, w/8);
			else if (m==10) VDP_loadBMPTileData((u32*)video_11.image, 1, w/8, h/8, w/8);
			else if (m==11) VDP_loadBMPTileData((u32*)video_12.image, 1, w/8, h/8, w/8);
		}

		++k;
		if (k==12*19) k = 0;
		m = k/19;

        	sme_Update();
    	}
	while (time<end);	
}
