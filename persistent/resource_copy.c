/********************************************/
/* Resource copy routines - ROM to (V)RAM	*/
/* 2015 - Danny Lousberg   					*/
/********************************************/

#include "utils.h"
#include "vdp.h"
#include "bankswitch.h"

#define _binary_start_bank	5

// Copy from cartridge ROM to a section in RAM
void rom_to_ram(unsigned long start, unsigned long end, unsigned char *dst)
{
	unsigned int srcoffset 	= (unsigned int)start % BANKSIZE;
	unsigned int length		= (unsigned int)(end - start);
	unsigned int srcbank	= _binary_start_bank + ((start - srcoffset) / (BANKSIZE) );

	// Do we need to read across different banks?
	while ( (srcoffset + length) > BANKSIZE )
	{
		unsigned int templength = BANKSIZE - srcoffset;

		// activate source bank
		switchtobank(srcbank);

		// copy contents until end of bank
		memcpy(dst, (void*)(BANKADDRESS + srcoffset), templength);

		// update bank, offset and remaining length
		length -= templength;
		srcbank++;				// prime for next bank
		srcoffset = 0;			// start at start of bank
		dst += templength;		// Move destination pointer
	}

	// Copy contents of one (last) bank
	switchtobank(srcbank);
	memcpy(dst, (void*)(BANKADDRESS + srcoffset), length);

	// Restore bank
	switchtobank(currentbank);
}

// Copy from cartridge ROM to a section in VRAM
void rom_to_vram(unsigned long start, unsigned long end, unsigned int dst)
{
	unsigned int srcoffset 	= (unsigned int)start % BANKSIZE;
	unsigned int length		= (unsigned int)(end - start);
	unsigned int srcbank	= _binary_start_bank + ((start - srcoffset) / (BANKSIZE) );

	// Do we need to read across different banks?
	while ( (srcoffset + length) > BANKSIZE )
	{
		unsigned int templength = BANKSIZE - srcoffset;

		// activate source bank
		switchtobank(srcbank);

		// copy contents until end of bank
		vdpmemcpy(dst, (unsigned char*)(BANKADDRESS + srcoffset), templength);

		// update bank, offset and remaining length
		length -= templength;
		srcbank++;				// prime for next bank
		srcoffset = 0;			// start at start of bank
		dst += templength;		// Move destination pointer
	}

	// Copy contents of one (last) bank
	switchtobank(srcbank);
	vdpmemcpy(dst, (void*)(BANKADDRESS + srcoffset), length);

	// Restore bank
	switchtobank(currentbank);
}
