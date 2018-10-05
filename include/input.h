#ifndef INPUT_H
#define INPUT_H

#define JOYST_1		0x0001
#define JOYST_2		0x0002
#define	JOYST_FIRE	0x0100
#define	JOYST_LEFT	0x0200
#define	JOYST_RIGHT	0x0400
#define	JOYST_DOWN	0x0800
#define	JOYST_UP	0x1000

/*
// Address to set the scan mode (see KSCAN_MODE_xxx defines)
#define KSCAN_MODE	*((volatile unsigned char*)0x8374)

// Address to read back the detected key. 0xFF if no key was pressed.
#define KSCAN_KEY	*((volatile unsigned char*)0x8375)

// Address to read back the joystick X axis (scan modes 1 and 2 only)
#define KSCAN_JOYY	*((volatile unsigned char*)0x8376)

// Address to read back the joystick Y axis (scan modes 1 and 2 only)
#define KSCAN_JOYX	*((volatile unsigned char*)0x8377)

// Address to check the status byte. KSCAN_MASK is set if a key was pressed
#define KSCAN_STATUS *((volatile unsigned char*)0x837c)
#define KSCAN_MASK	0x20
*/

unsigned int 	read_joyst(int joystick_id);
unsigned char 	read_spacebar();
//unsigned char 	read_keyboard();

#endif