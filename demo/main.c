#include "genesis.h"
#include "sme/sme.h"
//#include "music.h"

int main(int hard)
{
	sme_Initialize(hard);

	//SND_startPlay_PCM(music, sizeof(music), SOUND_RATE_11025, SOUND_PAN_CENTER, 0);

	smeFRAMEBUFFER_Initialize();

    	while(1)
    	{
		int i;
		for (i=0 ; i<BMP_WIDTH*BMP_HEIGHT ; ++i)
			smeFRAMEBUFFER[i] = i%32;		

        	sme_Update();
		smeFRAMEBUFFER_Update();
    	}

	smeFRAMEBUFFER_Finalize();

    	//SND_stopPlay_PCM();

	sme_Finalize();

    	return 0;
}


