#include "genesis.h"
#include "sme/sme.h"
#include "music.h"
#include "res/resources.h"

void EFFECT_Scroll(int end, int speed, int mode)
{
	smeVDP_SetScrollMode(smeVDP_HSCROLL_Line, smeVDP_VSCROLL_Plane);
        
    	int xs=0, ys=0;
    
	int k;
	for (k=0 ; k<16 ; ++k)
        {
        	VDP_setPaletteColor(k+32, 0);
		VDP_setPaletteColor(k+16, 0);
		VDP_setPaletteColor(k, 0);
	}
	
    	u16 w = axelay_up.w;
    	u16 h = axelay_up.h;
    	VDP_loadBMPTileData((u32*)axelay_up.image, 1, w / 8, h / 8, w/8 );
    	for (ys=0 ; ys<VDP_getPlanHeight()/(h/8); ++ys)
    	for (xs=0 ; xs<VDP_getPlanWidth()/(w/8); ++xs)
        	VDP_fillTileMapRectInc(APLAN, TILE_ATTR_FULL(PAL2, 0, 0, 0, 1), xs*(w/8), ys*(h/8), w / 8, h / 8);
    
    	w = cylinder_back.w;
    	h = cylinder_back.h;
    	VDP_loadBMPTileData((u32*)cylinder_back.image, 257, w / 8, h / 8, w/8 );
    	for (ys=0 ; ys<VDP_getPlanHeight()/(h/8); ++ys)
    	for (xs=0 ; xs<VDP_getPlanWidth()/(w/8); ++xs)
       		VDP_fillTileMapRectInc(BPLAN, TILE_ATTR_FULL(PAL1, 0, 0, 0, 257), xs*(w/8), ys*(h/8), w / 8, h / 8);
    
    	int m = 0;

	u32 time;
	u32 start_time = smeSOUND_GetTime(music);
    
    	u32 t=0;
    	u32 t2=0;

    	do
    	{
        	time = smeSOUND_GetTime(music);
        
		int height = VDP_getPlanHeight()*4;
		s16 values1[height];
		s16 values2[height];
		int i;
		for (i=0 ; i<height ; ++i)
		{
			if (i>t-20 && i<t+20)
			{
				values1[i] = sinFix16((i-(t-20))*1024/40+time+t*4)/4;
			}
			else
			{
				values1[i] = 0;
			}
			
			if (i>t-30 && i<t+30)
			{
				values2[i] = sinFix16((i-(t-30))*1024/40+time+t*4)/4;
			}
			else
			{
				values2[i] = 0;
			}

			values1[i] += -t2;
			values2[i] += -t2*2;
		}
		VDP_setHorizontalScrollLine(PLAN_A, 0, values1, height, 0);
		VDP_setHorizontalScrollLine(PLAN_B, 0, values2, height, 0);
		VDP_setVerticalScroll(PLAN_A, -t/2);
        	t += 7;
		if (t>height) t=0;
		t2 += speed*4;
		
		u8 data = music[time];

		m = (((int)data)*9+m)/10;

		int n = m*7;
		if (n>255) n = 255;
	
		int fade = 255;
		if (time-start_time<100) fade = (time-start_time)*255/100;
	
		n = n*fade/255;

		int n1 = n;
		int n2 = n/2+127;

		for (k=0 ; k<16 ; ++k)
        	{
               		int r, g, b;

			if (mode==0)
			{
                		r = ((u16*)axelay_up.palette->data)[k]&15;
                		g = (((u16*)axelay_up.palette->data)[k]>>4)&15;
                		b = (((u16*)axelay_up.palette->data)[k]>>8)&15;
             		}
			else
			{
				r = ((u16*)bar.palette->data)[k]&15;
                		g = (((u16*)bar.palette->data)[k]>>4)&15;
                		b = (((u16*)bar.palette->data)[k]>>8)&15;
             		}

			r = (r*n2)/255;
			g = (g*n2)/255;
			b = (b*n2)/255;

			VDP_setPaletteColor(k+32, r|(g<<4)|(b<<8));

			if (mode==0)
			{
				r = ((u16*)cylinder_back.palette->data)[k]&15;
                		g = (((u16*)cylinder_back.palette->data)[k]>>4)&15;
                		b = (((u16*)cylinder_back.palette->data)[k]>>8)&15;
             		}
			else
			{
				r = ((u16*)bar.palette->data)[k]&15;
                		g = (((u16*)bar.palette->data)[k]>>4)&15;
                		b = (((u16*)bar.palette->data)[k]>>8)&15;
             		}
             
			r = (r*n1)/255;
			g = (g*n1)/255;
			b = (b*n1)/255;

			VDP_setPaletteColor(k+16, r|(g<<4)|(b<<8));
		}
    	}
    	while (time<end);

	smeVDP_SetScrollMode(smeVDP_HSCROLL_Plane, smeVDP_VSCROLL_Plane);
        
	VDP_clearPlan(APLAN, 1);
	VDP_waitDMACompletion();
	VDP_clearPlan(BPLAN, 1);
	VDP_waitDMACompletion();

    	VDP_setHorizontalScroll(PLAN_A, 0);
	VDP_setVerticalScroll(PLAN_A, 0);
        VDP_setHorizontalScroll(PLAN_B, 0);
   	VDP_setVerticalScroll(PLAN_B, 0);
}
