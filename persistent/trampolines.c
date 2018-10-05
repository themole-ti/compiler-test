/********************************************/
/* Trampoline functions						*/
/* Use these to call functions in other		*/
/* banks. 									*/
/* 2015 - Danny Lousberg   					*/
/********************************************/
#include "bankswitch.h"

extern unsigned int  _data;
extern unsigned int  _data_end;

// We can't use "bankbase" here, since it hasn't been initialized yet.
#define BSWITCH_0 *((volatile unsigned int*)0x6000)
#define BSWITCH_4 *((volatile unsigned int*)0x6008)
void init_data_section()
{
	volatile unsigned int *src = (unsigned int*)0x6000;
	volatile unsigned int *dst = &_data;

	// Switch to bank containing initialization data
	BSWITCH_4 = 1;

	// Copy .data from ROM to higher memory expansion
	while (dst < &_data_end)
		*dst++ = *src++;

	// Switch back to bank 0
	BSWITCH_0 = 1;
}

