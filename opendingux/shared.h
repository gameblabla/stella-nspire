#ifndef SHARED_H
#define SHARED_H

//#include <stdbool.h>
//#include <malloc.h>
//#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

#include <SDL/SDL.h>

// defines and macros
#define MAX__PATH 1024
#define FILE_LIST_ROWS 19

#define SYSVID_WIDTH	160
#define SYSVID_HEIGHT	210

#define GF_GAMEINIT    1
#define GF_MAINUI      2
#define GF_GAMEQUIT    3
#define GF_GAMERUNNING 4

#ifndef O_BINARY
#define O_BINARY 0
#endif

#define PIX_TO_RGB(fmt, r, g, b) (((r>>fmt->Rloss)<<fmt->Rshift)| ((g>>fmt->Gloss)<<fmt->Gshift)|((b>>fmt->Bloss)<<fmt->Bshift))

// Stella dependencies
#include "Console.hxx"
#include "Joystick.hxx"
#include "MediaSrc.hxx"
#include "Paddles.hxx"
#include "Sound.hxx"
#include "SoundSDL.hxx"
#include "Event.hxx"
#include "StellaEvent.hxx"
#include "EventHandler.hxx"

extern Console* theConsole;
extern Sound* theSDLSnd;
extern uInt8* filebuffer;

#define cartridge_IsLoaded() (filebuffer != 0)

typedef struct {
  unsigned int sndLevel;
  unsigned int m_ScreenRatio; // 0 = original show, 1 = full screen
  unsigned int OD_Joy[12]; // each key mapping
  unsigned int m_DisplayFPS;
  char current_dir_rom[MAX__PATH];
} gamecfg;


#ifdef _TINSPIRE
	#define PAD_UP		KEY_NSPIRE_UP
	#define PAD_LEFT	KEY_NSPIRE_LEFT
	#define PAD_RIGHT	KEY_NSPIRE_RIGHT
	#define PAD_DOWN	KEY_NSPIRE_DOWN

	#define PAD_XUP		KEY_NSPIRE_UP
	#define PAD_XLEFT	KEY_NSPIRE_LEFT
	#define PAD_XRIGHT	KEY_NSPIRE_RIGHT
	#define PAD_XDOWN	KEY_NSPIRE_DOWN
	
	#define PAD_YUP		KEY_NSPIRE_8
	#define PAD_YLEFT	KEY_NSPIRE_4
	#define PAD_YRIGHT	KEY_NSPIRE_6
	#define PAD_YDOWN	KEY_NSPIRE_5
	
	#define PAD_A		KEY_NSPIRE_CTRL
	#define PAD_B		KEY_NSPIRE_SHIFT
	
	#define PAD_X		KEY_NSPIRE_VAR
	#define PAD_Y		KEY_NSPIRE_DEL
	
	#define PAD_L		KEY_NSPIRE_L
	#define PAD_R		KEY_NSPIRE_R

	#define PAD_START		KEY_NSPIRE_TAB
	#define PAD_SELECT		KEY_NSPIRE_MENU
	
	#define PAD_SLIDER		KEY_NSPIRE_ENTER
	
	#define PAD_QUIT		KEY_NSPIRE_ESC
#else
	#define PAD_XUP		SDLK_UP
	#define PAD_XLEFT	SDLK_LEFT
	#define PAD_XRIGHT	SDLK_RIGHT
	#define PAD_XDOWN	SDLK_DOWN
	#define PAD_YUP		SDLK_t
	#define PAD_YLEFT	SDLK_y
	#define PAD_YRIGHT	SDLK_u
	#define PAD_YDOWN	SDLK_i
	#define PAD_UP		SDLK_UP
	#define PAD_LEFT	SDLK_LEFT
	#define PAD_RIGHT	SDLK_RIGHT
	#define PAD_DOWN	SDLK_DOWN
	
	#define PAD_A		SDLK_LCTRL
	#define PAD_B		SDLK_LALT
	
	#define PAD_X		SDLK_LSHIFT
	#define PAD_Y		SDLK_SPACE
	
	#define PAD_L		SDLK_s
	#define PAD_R		SDLK_l
	
	#define PAD_START		SDLK_RETURN
	#define PAD_SELECT		SDLK_BACKSPACE
	
	#define PAD_SLIDER		0
	
	#define PAD_QUIT		SDLK_ESCAPE
#endif	
	
//typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int uint;

extern SDL_Surface* actualScreen;						// Main program screen

extern SDL_Event event;

extern gamecfg GameConf;

extern char gameName[512];
extern char current_conf_app[MAX__PATH];

extern unsigned int gameCRC;

extern void system_loadcfg(char *cfg_name);
extern void system_savecfg(char *cfg_name);

extern unsigned long crc32 (unsigned int crc, const unsigned char *buf, unsigned int len);


// menu
extern void screen_showtopmenu(void);
extern void print_string_video(int x, int y, const char *s);

#endif
