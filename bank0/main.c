/********************************************/
/* Alex Kidd - Cartridge Version			*/
/* 2015 - Danny Lousberg   					*/
/********************************************/

#include "bankswitch.h"
#include "vdp.h"
#include "globals.h"
#include "playersfx.h"
#include "input.h"
#include "resource_defs.h"
#include "resource_copy.h"

unsigned char			levelsounds[MAX_SOUNDLIST_SIZE];
volatile unsigned int 	currentbank = 0;

int main(int argc, char *argv[])
{
	unsigned int    joyst_result;

	// Initialize current bank to 0
	currentbank = 0;

	// set the interrupt routine
	VDP_INT_CTRL = VDP_INT_CTRL_DISABLE_ALL;
	VDP_INT_HOOK = stplaysfx;

	// Mute all playing stuff
	allstopsfx();
	MUTE_SOUND();

	// Load title music and sound effect into RAM
	rom_to_ram(RES_START(music_theme_1), RES_END(music_theme_1), (unsigned char*)levelsounds);

	// Play song
	stinitsfx(levelsounds, 0);

	// Print how many 'songs' we have in the loaded file?
	vdpchar(0x0000, 48 + stcount(levelsounds));

	while (1)
	{
		// wait for fire
		joyst_result = 0;
		while (!(joyst_result & JOYST_FIRE))
		{
			joyst_result = read_joyst(JOYST_1);

			// wait for vsync
			vdpwaitvint();
		}

		// Pressed fire, make noise!
		sfxinitsfx(levelsounds, 1, 0);
	}

	return 0;
}
