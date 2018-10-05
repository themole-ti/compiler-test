// Tursi's compressed VGM player -- allows interrupt-based playback
// of custom compressed VGM files (you'll need the compressor tool)
// Player uses a workspace at >8322 (32-bytes), as well as its own
// block of RAM for decompression. 

//*********************
// Use these functions if you want music AND sound effects. If you only
// want music, use player.h which will use less memory and CPU time.
//*********************

//*********************
// Player Data
//*********************

// pointers into the song data - all include the command nibble. Assumes player uses workspace at >8322
extern volatile unsigned int * const pVoice;	// four words, one each for channel pitch (mangled, see below)
extern volatile unsigned char * const pVol;		// four bytes, one each for channel volume
extern volatile unsigned char * const pDone;	// single byte, non-zero if playing, 0x00 when done - is a bitmask of channels still playing

// note, the pitch bytes are reversed, and as written to the sound chip. This means:
// for a tone channel, 0x2381 is a pitch value (on channel 0) of 0x0123
// for noise, 0x??E1 is a noise type 1 (and the ?? byte is not guaranteed)

//*********************
// Player functions
//*********************

// stinitsfx - initialize a song stream playback
// Inputs: pSong: points to the compressed song data (in CPU memory)
//         index: index of the song to play (0 if not a multi-bank song)
void stinitsfx(const void *pSong, const int index);

// ststopsfx - stop playback of the song stream
void ststopsfx();

// sfxinitsfx - initialize a sound effect playback
// Inputs: pSong: points to the compressed song data (in CPU memory)
//         index: index of the sfx to play (0 if not a multi-bank)
//		   priority: priority of this sfx. If less than currently playing,
//					 this one will be ignored, else the other is replaced
//					 sfx always has priority over music
void sfxinitsfx(const void *pSong, const int index, const int priority);

// sfxstopsfx - stop playback of current sfx
void sfxstopsfx();

// allstopsfx - stops music and sfx both
void allstopsfx();

// stplaysfx - play one tick (song and sfx)
// You must call this function once every 60hz interrupt. It is acceptable
// to load it directly into the VDP interrupt hook.
void stplaysfx();

// stcount - returns how many songs are in a pack
// inputs - pSong - pointer to song data
// returns - count (which is just the table pointers subtracted and divided)
inline unsigned int stcount(const void *pSong) 
{
	unsigned int ret;

	// using inline assembly because I don't want gcc to
	// pull in division just for this simple piece of math
	__asm__(
		"mov *%1+,r2\n\t"
		"mov *%1,r3\n\t"
		"s r2,r3\n\t"
		"clr r2\n\t"
		"li %1,24\n\t"
		"div %1,r2\n\t"
		"mov r2,%0\n\t"
		 : "=r"(ret) : "r"(pSong) : "r2", "r3"		// todo: unfortunate to force these, how do we specify 2 regs for div?
	);

	return ret;
}

// Helpers for direct and console interrupt sound processing

//*********************
// direct sound chip access
//*********************
#define SOUND		*((volatile unsigned char*)0x8400)

//*********************
// console sound interrupt pointers
//*********************
// pointer to load the address (in VDP or GROM) of your sound list
#define SOUND_PTR	*((volatile unsigned int*)0x83cc)

// countdown byte - set to '1' to start processing on the next interrupt
// You can also read this back, if it is zero, then the list is complete.
#define SOUND_CNT	*((volatile unsigned char*)0x83ce)

// Flag byte - set SOUND_VDP_MASK to indicate a sound list is in VDP instead of GROM
#define SOUND_VDP	*((volatile unsigned char*)0x83fd)
#define SOUND_VDP_MASK	0x01

// Command nibbles
#define TONE1_FREQ	0x80
#define TONE1_VOL	0x90
#define TONE2_FREQ	0xA0
#define TONE2_VOL	0xB0
#define TONE3_FREQ	0xC0
#define TONE3_VOL	0xD0
#define NOISE_MODE	0xE0
#define NOISE_VOL	0xF0

//*********************
// console sound interrupt helpers (remember you still have to enable interrupts in your main loop!)
//*********************

// set the address of your sound list
inline void SET_SOUND_PTR(unsigned int x)	{	SOUND_PTR = x;					}

// set that the sound list is in VDP
inline void SET_SOUND_VDP()					{	SOUND_VDP |= SOUND_VDP_MASK;	}

// set that the sound list is in GROM
inline void SET_SOUND_GROM()				{	SOUND_VDP &= ~SOUND_VDP_MASK;	}

// start processing sound list on the next interrupt
inline void START_SOUND()					{	SOUND_CNT = 1;					}

// mute all channels
inline void MUTE_SOUND()					{ SOUND=TONE1_VOL|0x0f; SOUND=TONE2_VOL|0x0f; SOUND=TONE3_VOL|0x0f; SOUND=NOISE_VOL|0x0f; }
