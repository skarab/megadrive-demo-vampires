#include "genesis.h"
#include "sme/sme.h"
#include "music.h"
#include "res/resources.h"

void EFFECT_Bat(int mode)
{
	VDP_clearPlan(APLAN, 0);
	VDP_clearPlan(BPLAN, 0);

	u16 w = bat_1.w;
    	u16 h = bat_1.h;

	
    	VDP_setPalette(PAL0, bat_1.palette->data);

    	VDP_loadBMPTileData((u32*)bat_1.image, 1, w/8, h/8, w/8);
	int ys;
    	for (ys=0 ; ys<VDP_getPlanHeight()/(h/8)-2; ++ys)
        	VDP_fillTileMapRectInc(APLAN, TILE_ATTR_FULL(PAL0, 0, 0, 0, 1), 0, ys*(h/8)+4, w/8, h/8);

	int k = 0;
	int m = 0;
	int old_m = 0;
	int ii = 0;

	int t = 0;
    	while (1)
    	{
		u32 time = smeSOUND_GetTime(music);
		u8 data = music[time];

		t = (((int)data)*9+t)/10;
		t *= 5;

		if (t>255) t = 255;
		
		int v;
		for (v=0 ; v<16 ; ++v)
            	{
			int r, g, b;

			if (mode==0 || mode==2)
			{
                		r = ((u16*)bat_1.palette->data)[v]&15;
                		g = (((u16*)bat_1.palette->data)[v]>>4)&15;
                		b = (((u16*)bat_1.palette->data)[v]>>8)&15;
             		}
			else
			{
                		r = ((u16*)bar.palette->data)[v]&15;
                		g = (((u16*)bar.palette->data)[v]>>4)&15;
                		b = (((u16*)bar.palette->data)[v]>>8)&15;
             		}

			r = (r*t)/255;
			g = (g*t)/255;
			b = (b*t)/255;

			VDP_setPaletteColor(v, r|(g<<4)|(b<<8));
		}

		if (mode==2)
		{
			int height = VDP_getPlanHeight()*4;
			s16 values[height];
			int i;
			for (i=0 ; i<height ; ++i)
			{
				if (i>ii-20 && i<ii+20)
				{
					values[i] = sinFix16((i-(ii-20))*1024/40+time+ii);
				}
				else
				{
					values[i] = 0;
				}
			}
			VDP_setHorizontalScrollLine(PLAN_A, 0, values, height, 0);
		
			ii += 6;
			if (ii>height) ii=0;
		}

		if (old_m!=m)
		{
			old_m = m;

			if (m==0) VDP_loadBMPTileData((u32*)bat_1.image, 1, w/8, h/8, w/8);
			else if (m==1) VDP_loadBMPTileData((u32*)bat_2.image, 1, w/8, h/8, w/8);
			else if (m==2) VDP_loadBMPTileData((u32*)bat_3.image, 1, w/8, h/8, w/8);
			else if (m==3) VDP_loadBMPTileData((u32*)bat_4.image, 1, w/8, h/8, w/8);
			else if (m==4) VDP_loadBMPTileData((u32*)bat_5.image, 1, w/8, h/8, w/8);
			else if (m==5) VDP_loadBMPTileData((u32*)bat_6.image, 1, w/8, h/8, w/8);
			else if (m==6) VDP_loadBMPTileData((u32*)bat_7.image, 1, w/8, h/8, w/8);
			else if (m==7) VDP_loadBMPTileData((u32*)bat_8.image, 1, w/8, h/8, w/8);
			else if (m==8) VDP_loadBMPTileData((u32*)bat_9.image, 1, w/8, h/8, w/8);
			else if (m==9) VDP_loadBMPTileData((u32*)bat_10.image, 1, w/8, h/8, w/8);
			else if (m==10) VDP_loadBMPTileData((u32*)bat_11.image, 1, w/8, h/8, w/8);
			else if (m==11) VDP_loadBMPTileData((u32*)bat_12.image, 1, w/8, h/8, w/8);
			else if (m==12) VDP_loadBMPTileData((u32*)bat_13.image, 1, w/8, h/8, w/8);
			else if (m==13) VDP_loadBMPTileData((u32*)bat_14.image, 1, w/8, h/8, w/8);
			else if (m==14) VDP_loadBMPTileData((u32*)bat_15.image, 1, w/8, h/8, w/8);
			else if (m==15) VDP_loadBMPTileData((u32*)bat_16.image, 1, w/8, h/8, w/8);
			else if (m==16) VDP_loadBMPTileData((u32*)bat_17.image, 1, w/8, h/8, w/8);
			else if (m==17) VDP_loadBMPTileData((u32*)bat_18.image, 1, w/8, h/8, w/8);
			else if (m==18) VDP_loadBMPTileData((u32*)bat_19.image, 1, w/8, h/8, w/8);
			else if (m==19) VDP_loadBMPTileData((u32*)bat_20.image, 1, w/8, h/8, w/8);
		}

		int pp = 20;
		if (mode==2) pp = 10;

		++k;
		if (k==19*pp) break;;
		m = k/pp;

        	sme_Update();
    	}
}
