/********************************************/
/* Keyboard and Joystick routines			*/
/* 2015 - Danny Lousberg   					*/
/********************************************/
#include "input.h"

unsigned int read_joyst(int joystick_id)
{
	unsigned int	result;
	
	__asm__
	(
		"li r12,>0024	\n\t"	
		"ai %1,5		\n\t"
		"swpb %1		\n\t"
		"ldcr %1,3		\n\t"
		"src r12,7		\n\t"
		"li r12,>0006	\n\t"
		"clr %0			\n\t"
		"stcr %0,8"
		:"=r"(result)
		:"r"(joystick_id)
		:"r12"
	);

	// results are inverted, a key/direction/button is pressed when the respective bit is zero
	// So we invert the result first
	return ~result;
}

unsigned char read_spacebar() 
{
	unsigned int col = 0, key;

	__asm__
	(
		"li r12,>0024	\n\t"
		"ldcr %1,3		\n\t"
		"src r12,7		\n\t"
		"li r12,>0006	\n\t"
		"clr %0			\n\t"
		"stcr %0,8" 
		: "=r"(key) 
		: "r"(col) 
		: "r12"
	);	// set cru, column, delay, read

	return !(key & 0x0200);
}

/*
// By columns, then rows. 8 Rows per column. No shift states
const unsigned char keymap[] = {
		61,32,13,255,1,2,3,255,
		'.','L','O','9','2','S','W','X',
		',','K','I','8','3','D','E','C',
		'M','J','U','7','4','F','R','V',
		'N','H','Y','6','5','G','T','B',
		'/',';','P','0','1','A','Q','Z'
};

unsigned char read_keyboard()
{
	unsigned int col, key, shift;

	for (col = 0; col < 0x0600; col += 0x0100) 
	{
		__asm__
		(
			"li r12,>0024	\n\t"
			"ldcr %1,3		\n\t"
			"src r12,7		\n\t"
			"li r12,>0006	\n\t"
			"clr %0			\n\t"
			"stcr %0,8" 
			: "=r"(key) 
			: "r"(col) 
			: "r12"
		);	// set cru, column, delay, read

		shift = 0x8000;
		for (int cnt = 7; cnt >= 0; cnt--) 
		{
			// a pressed key returns a 0 bit
			if (key & shift) 
			{
				shift >>= 1;
				continue;
			}

			// found one
			return keymap[(col>>5)+cnt];
		}
	}

	return 0;
}
*/
