#include "genesis.h"
#include "sme/sme.h"
#include "music.h"
#include "res/resources.h"
#include "effect_warning.h"
#include "effect_sega.h"
#include "effect_kstorm.h"
#include "effect_title.h"
#include "effect_cylinder.h"
#include "effect_axelay.h"
#include "effect_video.h"
#include "effect_credits.h"
#include "effect_bat.h"
#include "effect_blast.h"
#include "effect_blast2.h"
#include "effect_blast3.h"
#include "effect_boobs.h"
#include "effect_bar.h"
#include "effect_bar2.h"
#include "effect_ball.h"
#include "effect_scroll.h"

int main()
{
	sme_Initialize();

	EFFECT_Warning();

	SND_startPlay_PCM(music, sizeof(music), SOUND_RATE_11025, SOUND_PAN_CENTER, 0); 

	EFFECT_Sega(3000);
	EFFECT_Kstorm(8000);
	EFFECT_Title(12500);

	EFFECT_Video(19500, 0);
	EFFECT_Scroll(21500, 1, 0);
	EFFECT_Bar(23500);
	EFFECT_Ball(29000, 4);
	EFFECT_Axelay(33000, 1, 4, 0);

	EFFECT_Bar2(40000);
	EFFECT_Blast(44000);	
	
	EFFECT_Video(52000, 1);
	EFFECT_Scroll(54500, 1, 2);
	EFFECT_Blast3(57000);	
	EFFECT_Ball(59000, 4);
	EFFECT_Cylinder(64000, -10, 0);
	EFFECT_Axelay(67000, 1, -4, 0);

	EFFECT_Bat(1);
	EFFECT_Cylinder(77000, -10, 1);
	EFFECT_Boobs(81000);
	EFFECT_Axelay(85000, -1, 4, 1);

	EFFECT_Video(89000, 2);
	EFFECT_Scroll(91000, 3, 1);
	EFFECT_Bar(95000);
	EFFECT_Cylinder(97000, 10, 1);
	EFFECT_Blast2(102000);
	EFFECT_Axelay(108000, 1, 4, 0);

	EFFECT_Bat(2);
	EFFECT_Cylinder(110000, -12, 1);
	EFFECT_Blast(114000);
	EFFECT_Ball(117000, -8);
	EFFECT_Blast(120000);
	EFFECT_Cylinder(123000, -14, 1);
	EFFECT_Blast(126000);
	EFFECT_Cylinder(132000, -16, 0);
	EFFECT_Bat(0);

	EFFECT_Credits();

    	SND_stopPlay_PCM();

	sme_Finalize();
    	return 0;
}
