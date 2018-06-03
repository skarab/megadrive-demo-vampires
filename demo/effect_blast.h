#include "genesis.h"
#include "sme/sme.h"
#include "music.h"
#include "res/resources.h"

void EFFECT_Blast(int end)
{
	VDP_clearPlan(BPLAN, 0);

	int k;
	for (k=0 ; k<16 ; ++k)
        	VDP_setPaletteColor(k, 0);
   
	//VDP_setPaletteColors(0, blast.palette->data, 16);

	u16 w = title.w;
    	u16 h = title.h;
    	VDP_loadBMPTileData((u32*)blast.image, 1, w/8, h/8, w/8);
	int ys;
    	for (ys=0 ; ys<VDP_getPlanHeight()/(h/8); ++ys)
        	VDP_fillTileMapRectInc(APLAN, TILE_ATTR_FULL(PAL0, 0, 0, 0, 1), 0, ys*(h/8), w/8, h/8);

	int curY;
	int ii = 0;

	void vblank()
    	{
        	curY = 0;
		ii+=5;
		if (ii>VDP_getPlanHeight()*2) ii = 0;
        } 

    	void hblank()
    	{      
		vu16 *pw = (u16 *) GFX_DATA_PORT;
        	vu32 *pl = (u32 *) GFX_CTRL_PORT;
        	int x;
        	for (x=0 ; x<21 ; ++x)
        	{
			int val = sinFix16(curY*10+ii)*10;
            		u16 addr = (x&0x1F)*4;
            		*pl = GFX_WRITE_VSRAM_ADDR(addr);
            		*pw = val;
        	}    
		
        	curY++;
    	} 

	smeVDP_SetScrollMode(smeVDP_HSCROLL_Plane, smeVDP_VSCROLL_Column);
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
		if (time-start_time<200) fade = (time-start_time)*255/200;
		else if (time>end-200) fade = 255-(time-(end-200))*255/200;
		if (fade<0) fade = 0;

		m = m*fade/255;

		int k;
		for (k=0 ; k<16 ; ++k)
            	{
                	int r = ((u16*)blast.palette->data)[k]&15;
                	int g = (((u16*)blast.palette->data)[k]>>4)&15;
                	int b = (((u16*)blast.palette->data)[k]>>8)&15;
             
			r = (r*m)/255;
			g = (g*m)/255;
			b = (b*m)/255;

			VDP_setPaletteColor(k, r|(g<<4)|(b<<8));
		}

        	sme_Update();
    	}
	while (time<end);

	SYS_setVIntCallback(0);
    	SYS_setHIntCallback(0); 
	smeVDP_SetScrollMode(smeVDP_HSCROLL_Plane, smeVDP_HSCROLL_Plane);
    	VDP_setHorizontalScroll(PLAN_A,  0);
}
