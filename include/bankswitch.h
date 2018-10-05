#ifndef BANKSWITCH_H
#define BANKSWITCH_H

#define BANKSIZE	0x2000
#define BANKADDRESS 0x6000

// Variable to store our current bank, used by all trampoline functions
extern volatile unsigned int currentbank;

// Switching banks
// 		e.g.	switchtobank[2];
// This will switch to bank 2
#define switchtobank(x)	{ *((volatile unsigned char*)BANKADDRESS + (2 * x)) = 1; }

#endif