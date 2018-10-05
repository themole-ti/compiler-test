/********************************/
/* VDP access routines	   		*/
/* Copied from Tursi's libti99  */
/********************************/
#include "vdp.h"

unsigned char gSaveIntCnt;	// console interrupt count byte

void vdpwaitvint() 
{
	VDP_INT_ENABLE; 

	while ( VDP_INT_COUNTER == gSaveIntCnt ) 
	{ 
	}
	 
	gSaveIntCnt = VDP_INT_COUNTER; 

	VDP_INT_DISABLE; 
}

void vdpmemcpy(int pAddr, const unsigned char *pSrc, int cnt) 
{
	VDP_SET_ADDRESS_WRITE(pAddr);

	while (cnt--) 
		VDPWD=*(pSrc++);
}

void vdpmemset(int pAddr, int ch, int cnt) 
{
	VDP_SET_ADDRESS_WRITE(pAddr);

	while (cnt--) 
		VDPWD = ch;
}

void vdpchar(int pAddr, int ch) 
{
	VDP_SET_ADDRESS_WRITE(pAddr);

	VDPWD=ch;
}

void vdpwriteinc(int pAddr, int nStart, int cnt) 
{
	VDP_SET_ADDRESS_WRITE(pAddr);

	while (cnt--) 
		VDPWD=nStart++;
}