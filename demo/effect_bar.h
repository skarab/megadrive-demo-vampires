#include "genesis.h"
#include "sme/sme.h"
#include "music.h"
#include "res/resources.h"

void EFFECT_Bar(int end)
{
	int curY;
	int tt = 0;
	
	void vblank()
    	{
        	curY = 0;
		tt += 20;
	} 

    	void hblank()
    	{
		int j = sinFix16(curY*4+tt);
		if (j<-223) j = -223;
		if (j>223) j = 223;	
        	VDP_setVerticalScroll(PLAN_A, (-curY+j+112));
		++curY;
	}
        
    	smeVDP_SetScrollMode(smeVDP_HSCROLL_Plane, smeVDP_VSCROLL_Plane);
    
    	u16 w = bar.w;
    	u16 h = bar.h;
    	VDP_setPalette(PAL0, bar.palette->data);
    	VDP_loadBMPTileData((u32*)bar.image, 1, w / 8, h / 8, w/8 );
    	int xs, ys;
	for (ys=0 ; ys<VDP_getPlanHeight()/(h/8); ++ys)
    	for (xs=0 ; xs<VDP_getPlanWidth()/(w/8); ++xs)
        	VDP_fillTileMapRectInc(APLAN, TILE_ATTR_FULL(PAL0, 0, 0, 0, 1), xs*(w/8), ys*(h/8), w / 8, h / 8);
    
	VDP_clearPlan(BPLAN, 0);

    	SYS_setVIntCallback(vblank);
    	SYS_setHIntCallback(hblank);
    	VDP_setHIntCounter(1);
    	VDP_setHInterrupt(1); 
    
    	int m = 0;

	u32 time;
	u32 start_time = smeSOUND_GetTime(music);
    	
    	do
    	{
		time = smeSOUND_GetTime(music);
        
		u8 data = music[time];

		m = (((int)data)*9+m)/10;
		
		int fade = 255;
		if (time-start_time<100) fade = (time-start_time)*255/100;
		else if (time>end-100) fade = 255-(time-(end-100))*255/100;
		
		m = m*fade/255;

		int k;
		for (k=0 ; k<16 ; ++k)
        	{
                	int r = ((u16*)bar.palette->data)[k]&15;
               	 	int g = (((u16*)bar.palette->data)[k]>>4)&15;
                	int b = (((u16*)bar.palette->data)[k]>>8)&15;
             
			r = (r*m)/255;
			g = (g*m)/255;
			b = (b*m)/255;

			VDP_setPaletteColor(k, r|(g<<4)|(b<<8));
		}

		VDP_waitVSync();
    	}
    	while (time<end);

    	SYS_setVIntCallback(0);
    	SYS_setHIntCallback(0);
	smeVDP_SetScrollMode(smeVDP_HSCROLL_Plane, smeVDP_HSCROLL_Plane);
    	VDP_setHorizontalScroll(PLAN_A, 0);
	VDP_setVerticalScroll(PLAN_A, 0);
        VDP_setHorizontalScroll(PLAN_B, 0);
   	VDP_setVerticalScroll(PLAN_B, 0);
}
