#include "genesis.h"
#include "sme/sme_input.h"
#include "gfx.h"

int main()
{
    VDP_init();
    VDP_setHInterrupt(0);
    VDP_setHilightShadow(0);
    VDP_setScreenWidth320();
    VDP_setScreenHeight224();
    smeINPUTS_Initialize();
    
    VDP_setPaletteColors(16, menu_image.palette->data, 32);
    VDP_drawImageEx(BPLAN, &menu_image, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, TILE_USERINDEX), 0, 0, FALSE, TRUE);

    while(!smeINPUT_IsJustPressed(smeINPUT_START))
    {
        smeINPUTS_Update();
    
        VDP_waitVSync();
    }

    return 0;
}


