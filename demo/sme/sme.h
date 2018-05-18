#ifndef _SME_H_
#define _SME_H_

#include "sme_maths.h"
#include "sme_vdp.h"
#include "sme_sound.h"
#include "sme_input.h"
#include "sme_time.h"
#include "sme_screen.h"
#include "sme_framebuffer.h"
#include "sme_mesh.h"
#include "sme_scene.h"
#include "sme_raster.h"

void sme_Initialize();
void sme_Finalize();
void sme_Update();

void* MEM_realloc(void* ptr, u16 old_size, u16 new_size);
void memcpy_bytes(void *to, const void *from, int len);

#endif
