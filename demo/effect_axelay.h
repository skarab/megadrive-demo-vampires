#include "genesis.h"
#include "sme/sme.h"
#include "music.h"
#include "res/resources.h"

void EFFECT_Axelay(int end, int sens, int speed, int mode)
{
    u16* scrollTable;
	u16 curY;
    u16 ttta=0;
    int vscroll;

    void initTable()
    {
        int h = 180;
        scrollTable = (u16*)MEM_alloc(h*sizeof(u16));
        vscroll=0x840; 
        int k;
        int p = (h/2)/8;
        
        int sub = 0;
        int subb[h];
        for (k=0 ; k<h ; ++k)
        {
            int m = k;

		int mod = 0;
		int u;
		for (u=0 ; u<m ; ++u)
		{
			++mod;
			if (mod==p)
				mod = 0;
		}

            if (mod==0)
                sub += p*(m/p);
            subb[k] = sub;
        }

        for (k=0 ; k<h ; ++k)
        {
            if (k<h/2)
            {
                int m = sinFix16(k*255/(h/2))*(h/2)/64;
                vscroll = m*(7-m/p)+subb[m];
                
                //vscroll = k*(4-k/p)-subb[h-k-1];
            }
            else
            {
                //vscroll = 0;
                vscroll --;
            }
            
            scrollTable[k] = vscroll;
        }
        
        for (k=2 ; k<h-2 ; ++k)
        {
            scrollTable[k] = (scrollTable[k-2]+scrollTable[k-1]*2+scrollTable[k]*3+scrollTable[k+1]*2+scrollTable[k+2])/9;
        }
    }

    int posY = 0;
    int incPosY = speed;

    void vblank()
    {
        ++ttta;
        curY = 0;
        posY += incPosY;
    } 

    void hblank()
    {      
        VDP_setVerticalScroll(PLAN_A, scrollTable[curY]-posY-ttta/4);
        VDP_setVerticalScroll(PLAN_B, scrollTable[curY]-(posY+ttta/4)/2);
        
        curY++;
    } 

    int incPosX = sens*speed;
    int posX = 0;    
    
    smeVDP_SetScrollMode(smeVDP_HSCROLL_Plane, smeVDP_HSCROLL_Plane);
    
    initTable();   
    
    int xs=0, ys=0;
    u16 w = axelay.w;
    u16 h = axelay.h;
    
	int k;
	for (k=0 ; k<16 ; ++k)
        {
        	VDP_setPaletteColor(k+32, 0);
		VDP_setPaletteColor(k+16, 0);
	}

    //VDP_setPalette(PAL2, axelay.palette->data);
    VDP_loadBMPTileData((u32*) axelay.image, 257, w / 8, h / 8, w/8 );
    for (ys=0 ; ys<VDP_getPlanHeight()/(h/8); ++ys)
    for (xs=0 ; xs<VDP_getPlanWidth()/(w/8); ++xs)
        VDP_fillTileMapRectInc(BPLAN, TILE_ATTR_FULL(PAL2, 0, 0, 0, 257), xs*(w/8), ys*(h/8), w / 8, h / 8);
       
    w = axelay_up.w;
    h = axelay_up.h;
        
    //VDP_setPalette(PAL1, axelay_up.palette->data);
    VDP_loadBMPTileData((u32*) axelay_up.image, 1, w / 8, h / 8, w/8 );
    for (ys=0 ; ys<VDP_getPlanHeight()/(h/8); ++ys)
    for (xs=0 ; xs<VDP_getPlanWidth()/(w/8); ++xs)
        VDP_fillTileMapRectInc(APLAN, TILE_ATTR_FULL(PAL1, 0, 0, 0, 1), xs*(w/8), ys*(h/8), w / 8, h / 8);
    
    SYS_setVIntCallback(vblank);
    SYS_setHIntCallback(hblank);
    VDP_setHIntCounter(1);
    VDP_setHInterrupt(1); 
    
    curY = 0;
    
	int m = 0;

	u32 time;
u32 start_time = smeSOUND_GetTime(music);
    
    do
    {
        time = smeSOUND_GetTime(music);
        u8 data = music[time];

	int fade = 255;
	if (time-start_time<100) fade = (time-start_time)*255/100;
	else if (time>end-300) fade = 255-(time-(end-300))*255/300;
	if (fade<0) fade = 0;

	m = (((int)data)*9+m)/10;
	int n = m*7;
	if (n>255) n = 255;
	
	int n1 = n*fade/255;
	int n2 = (n/2+127)*fade/255;

	for (k=0 ; k<16 ; ++k)
        {
		int r, g, b;

		r = ((u16*)axelay.palette->data)[k]&15;
               	g = (((u16*)axelay.palette->data)[k]>>4)&15;
               	b = (((u16*)axelay.palette->data)[k]>>8)&15;
      
		r = (r*n2)/255;
		g = (g*n2)/255;
		b = (b*n2)/255;

		VDP_setPaletteColor(k+32, r|(g<<4)|(b<<8));

		r = ((u16*)axelay_up.palette->data)[k]&15;
               	g = (((u16*)axelay_up.palette->data)[k]>>4)&15;
               	b = (((u16*)axelay_up.palette->data)[k]>>8)&15;

		r = (r*n1)/255;
		g = (g*n1)/255;
		b = (b*n1)/255;

		VDP_setPaletteColor(k+16, r|(g<<4)|(b<<8));
	}

        posX = time*incPosX/8;
        
        VDP_setHorizontalScroll(PLAN_A, posX);
        VDP_setHorizontalScroll(PLAN_B, posX*3/4);
    }
    while (time<end);

    SYS_setVIntCallback(0);
    SYS_setHIntCallback(0); 
	    VDP_setHorizontalScroll(PLAN_A, 0);
	VDP_setVerticalScroll(PLAN_A, 0);
        VDP_setHorizontalScroll(PLAN_B, 0);
   VDP_setVerticalScroll(PLAN_B, 0);
    MEM_free(scrollTable);
}

