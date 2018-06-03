#include "genesis.h"
#include "sme/sme.h"
#include "music.h"
#include "res/resources.h"

void EFFECT_Cylinder(int end, int speed, int mode)
{
	int sens = 1;
	
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
        	int center = 8;
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
	int ii = 0;

    	void vblank()
    	{
        	++ttt;
        	curY = 0;
        	posY += sens*speed;
        
        	scrollData = scrollTable;

		ii += 2;
    	} 

    	void hblank()
    	{
        	vu16 *pw = (u16 *) GFX_DATA_PORT;
        	vu32 *pl = (u32 *) GFX_CTRL_PORT;
        
		int c = cosFix16(ii);
        	int x;
        	for (x=0 ; x<21 ; ++x)
        	{
			int val = -posY-ttt/4+(x-10)*c/32+10;
            		
            		u16 addr = (x&0x1F)*4;
            		*pl = GFX_WRITE_VSRAM_ADDR(addr);
            		*pw = val+scrollData[x];
            		*pl = GFX_WRITE_VSRAM_ADDR(addr+2);
            		*pw = val/2+scrollData[x];
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
	
    	u16 w = cylinder.w;
    	u16 h = cylinder.h;
    	VDP_loadBMPTileData((u32*)cylinder.image, 1, w / 8, h / 8, w/8 );
    	for (ys=0 ; ys<VDP_getPlanHeight()/(h/8); ++ys)
    	for (xs=0 ; xs<VDP_getPlanWidth()/(w/8); ++xs)
        	VDP_fillTileMapRectInc(APLAN, TILE_ATTR_FULL(PAL2, 0, 0, 0, 1), xs*(w/8), ys*(h/8), w / 8, h / 8);
    
    	w = cylinder_back.w;
    	h = cylinder_back.h;
    	VDP_loadBMPTileData((u32*)cylinder_back.image, 257, w / 8, h / 8, w/8 );
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
        
        	VDP_setHorizontalScroll(PLAN_A, sinFix16(t*speed/20)*2);
        	VDP_setHorizontalScroll(PLAN_B, sinFix16(t*speed/20)*2);

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
                		r = ((u16*)cylinder.palette->data)[k]&15;
                		g = (((u16*)cylinder.palette->data)[k]>>4)&15;
                		b = (((u16*)cylinder.palette->data)[k]>>8)&15;
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

		char str[16];
	    	uintToStr(time, str, 1);
        	VDP_clearText(2, 1, 6);
    		VDP_drawText(str, 1, 1);
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
}
