// crt0.c - C runtime initialization code
#include "trampolines.h"
#include "vdp.h"

extern unsigned int  _text;
extern unsigned int  _text_end;
extern unsigned int  _bss;
extern unsigned int  _bss_end;
extern unsigned int  _persistent;
extern unsigned int  _persistent_src;
extern unsigned int  _persistent_end;
extern unsigned int  _scratchpad;
extern unsigned int  _scratchpad_src;
extern unsigned int  _scratchpad_end;
extern unsigned int  _bss_end;
extern unsigned int  _data_bank;

// Don't use variables here, they need the 32k!
// This means no loops either...
#define MEMEXP	*((volatile unsigned int*)0xa000)
void detect_32k()
{
	MEMEXP = 0xdead;

	if (MEMEXP != 0xdead)
	{
		VDP_SET_REGISTER( 7, 0x44);
		VDP_SET_ADDRESS_WRITE(0x0380);
		VDPWD = 0xf4;
		VDPWD = 0xf4;
		VDPWD = 0xf4;
		VDPWD = 0xf4;
		VDPWD = 0xf4;
		VDPWD = 0xf4;
		VDPWD = 0xf4;
		VDPWD = 0xf4;
		VDPWD = 0xf4;
		VDPWD = 0xf4;
		VDPWD = 0xf4;
		VDPWD = 0xf4;
		VDPWD = 0xf4;
		VDPWD = 0xf4;
		VDPWD = 0xf4;
		VDPWD = 0xf4;

		VDP_SET_ADDRESS_WRITE(0x0188);
		VDPWD = '3';
		VDPWD = '2';
		VDPWD = 'K';
		VDPWD = ' ';
		VDPWD = 'M';
		VDPWD = 'E';
		VDPWD = 'M';
		VDPWD = ' ';
		VDPWD = 'R';
		VDPWD = 'E';
		VDPWD = 'Q';
		VDPWD = 'U';
		VDPWD = 'I';
		VDPWD = 'R';
		VDPWD = 'E';
		VDPWD = 'D';
		VDPWD = '!';
		while (1)
		{
		}
	}
}

// Linker will look for _start symbol as the entry point of our program
void _start()
{
	// Start by turning off interupts, and setting the workspace pointer
	__asm__
	(
		"limi	0	\n\t"
		"lwpi	0x8300\n\t"
	);

	detect_32k();

	// The symbols starting with '_' are defined in the linker script
	// They point to the ROM locations for each section
	unsigned int *src = &_persistent_src;
	unsigned int *dst = &_persistent;

	// Copy persistent code (ie, non-bankswitchable) to lower memory expansion
	while (dst < &_persistent_end)
		*dst++ = *src++;

	// Copy scratchpad code (ie, non-bankswitchable super dupder fast stuff) to lower memory expansion
	unsigned int *src2 = &_scratchpad_src;
	unsigned int *dst2 = &_scratchpad;
	while (dst2 < &_scratchpad_end)
		*dst2++ = *src2++;

	// Copy initial data from ROM to higher memory expansion
	init_data_section();

	// Zero BSS
	for (dst = &_bss; dst < &_bss_end; dst++)
		*dst = 0;

	// Set stack
	__asm__
	(
		"li	sp, 0x4000\n\t"
	);

	// Start executing C program from main function
	__asm__
	(
		"b	@main\n\t"
	);
}
