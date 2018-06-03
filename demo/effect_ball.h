#include "genesis.h"
#include "sme/sme.h"
#include "music.h"
#include "res/resources.h"

void EFFECT_Ball(int end, int speed)
{
	u16 j = 0;
    	u16* scrollTable;
    	u16 curY;
   	 int curX;
    	u16 ttt=0;
    	int vscroll;
    	u16 scr = 0;

    	void initTable()
    	{
        	scrollTable = (u16*)MEM_alloc(21*42*sizeof(u16));
        	vscroll=0x840; 
       		int k, h;
        	int center = 10;
        	for (k=0 ; k<42 ; ++k)
        	{
            		int count = (k*k)/10;
            		for (h=0 ; h<21 ; ++h)
            		{
				int z = (h-8)*(h-8);
                		int xv;
                		if (k<center) xv = z*((center-k)*(center-k))/60;
                		else xv = -z*((k-center)*(k-center))/60;
                		scrollTable[k*21+h] = 0x840-count+xv;
            		}
        	}
    	}

    	int posY = 0;
    	u16* scrollData = NULL;

    	void vblank()
    	{
        	++ttt;
        	curY = 0;
        	posY += speed;
        
        	scrollData = scrollTable;
    	} 

    	void hblank()
    	{
        	vu16 *pw = (u16 *) GFX_DATA_PORT;
        	vu32 *pl = (u32 *) GFX_CTRL_PORT;

		int x;
        	for (x=0 ; x<21 ; ++x)
        	{
			int val = -posY-ttt/4;
            		int x2 = 10-x;
			if (x2<0) x2 = x-10;
            		u16 addr = (x&0x1F)*4;
            		*pl = GFX_WRITE_VSRAM_ADDR(addr);
            		*pw = val+scrollData[x2]*2;
			*pl = GFX_WRITE_VSRAM_ADDR(addr+2);
            		*pw = val/2+scrollData[x2]*2;
        	}
        
        	scrollData += 21;
        	curY++;
    	} 

    	smeVDP_SetScrollMode(smeVDP_HSCROLL_Plane, smeVDP_VSCROLL_Column);
    
    	initTable();
    
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
    
    	w = ball.w;
    	h = ball.h;
    	VDP_loadBMPTileData((u32*)ball.image, 257, w / 8, h / 8, w/8 );
    	for (ys=0 ; ys<VDP_getPlanHeight()/(h/8); ++ys)
    	for (xs=0 ; xs<VDP_getPlanWidth()/(w/8); ++xs)
       		VDP_fillTileMapRectInc(BPLAN, TILE_ATTR_FULL(PAL1, 0, 0, 0, 257), xs*(w/8), ys*(h/8), w / 8, h / 8);
  
    	SYS_setVIntCallback(vblank);
    	SYS_setHIntCallback(hblank);
    	VDP_setHIntCounter(1);
    	VDP_setHInterrupt(1); 
    
    	curY = 0;
    
    	int m = 0;

	u32 time;
	u32 start_time = smeSOUND_GetTime(music);
    
    	u32 t=0;
    	do
    	{
        	time = smeSOUND_GetTime(music);
        
		u8 data = music[time];

		m = (((int)data)*9+m)/10;

		int n = m*7;
		if (n>255) n = 255;
	
		int fade = 255;
		if (time-start_time<100) fade = (time-start_time)*255/100;
	
		n = n*fade/255;

		for (k=0 ; k<16 ; ++k)
        	{
               		int r, g, b;

			r = ((u16*)axelay_up.palette->data)[k]&15;
                	g = (((u16*)axelay_up.palette->data)[k]>>4)&15;
                	b = (((u16*)axelay_up.palette->data)[k]>>8)&15;
             		
			r = (r*n)/255;
			g = (g*n)/255;
			b = (b*n)/255;

			VDP_setPaletteColor(k+32, r|(g<<4)|(b<<8));

			r = ((u16*)ball.palette->data)[k]&15;
                	g = (((u16*)ball.palette->data)[k]>>4)&15;
                	b = (((u16*)ball.palette->data)[k]>>8)&15;
             		
			r = (r*n)/255;
			g = (g*n)/255;
			b = (b*n)/255;

			VDP_setPaletteColor(k+16, r|(g<<4)|(b<<8));
		}
    	}
    	while (time<end);

    	SYS_setVIntCallback(0);
    	SYS_setHIntCallback(0);
	smeVDP_SetScrollMode(smeVDP_HSCROLL_Plane, smeVDP_HSCROLL_Plane);
    	VDP_setHorizontalScroll(PLAN_A, 0);
	VDP_setVerticalScroll(PLAN_A, 0);
        VDP_setHorizontalScroll(PLAN_B, 0);
   	VDP_setVerticalScroll(PLAN_B, 0);
         MEM_free(scrollTable);
	VDP_clearPlan(APLAN, 0);
	VDP_clearPlan(BPLAN, 0);
}
