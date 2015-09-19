#include  <sys/time.h>
#include "n2DLib.h"
#include "shared.h"


extern short button_state[18];
extern unsigned char button_time[18];

unsigned int m_Flag;
unsigned int interval;

unsigned int gameCRC;
gamecfg GameConf;
char gameName[512];
char current_conf_app[MAX__PATH];
unsigned char vidBuf[256*256];

unsigned long nextTick, lastTick = 0, newTick, currentTick, wait;
int FPS = 60; 
int pastFPS = 0; 

Console* theConsole = (Console*) NULL;
Sound* theSDLSnd = (Sound*) NULL;
uInt8* filebuffer = 0;
uInt32 console_controleurtype = 0;

unsigned short stella_palette[256];

void graphics_paint(void) 
{
	unsigned short *buffer_scr = (unsigned short *)BUFF_BASE_ADDRESS;
	unsigned char *buffer_flip = (unsigned char *) &vidBuf;
	unsigned int W,H,ix,iy,x,y, xfp,yfp;
	static char buffer[32];

	if (GameConf.m_ScreenRatio) { // Full screen
		x=0;
		y=0; 
		W=320;
		H=240;
		ix=(SYSVID_WIDTH<<16)/W;
		iy=(SYSVID_HEIGHT<<16)/H;
		xfp = 300;yfp = 1;

		do   
		{
			unsigned char *buffer_mem=(buffer_flip+((y>>16)*SYSVID_WIDTH));
			W=320; x=0;
			do {
				*buffer_scr++=stella_palette[buffer_mem[x>>16]];
				x+=ix;
			} while (--W);
			y+=iy;
		} while (--H);
	}
	else  // Original show
	{
		x=((320 - SYSVID_WIDTH)/2);
		y=((240 - SYSVID_HEIGHT)/2); 
		W=SYSVID_WIDTH;
		H=SYSVID_HEIGHT;
		ix=(SYSVID_WIDTH<<16)/W;
		iy=(SYSVID_HEIGHT<<16)/H;
		xfp = (x+SYSVID_WIDTH)-20;yfp = y+1;
		
		buffer_scr += (y)*320;
		buffer_scr += (x);
		do   
		{
			unsigned char *buffer_mem=(buffer_flip+((y>>16)*SYSVID_WIDTH));
			W=SYSVID_WIDTH; x=((320 - SYSVID_WIDTH)/2);
			do {
				*buffer_scr++=stella_palette[buffer_mem[x>>16]];
				x+=ix;
			} while (--W);
			y+=iy;
			buffer_scr += (320) - 320 - SYSVID_WIDTH;
		} while (--H);
	}
	
	updateScreen();
}


void initSDL(void) {
	initBuffering();
}

int stella_init(char *filename) 
{
	unsigned int buffer_size=0;
  
	if (filebuffer != 0)
		free(filebuffer);

	if (theConsole) 
		delete theConsole;
	if (theSDLSnd) 
		delete theSDLSnd;
  
	theSDLSnd = new SoundSDL(0);
	
	theSDLSnd->setVolume(100);

	// Load the file
	FILE *romfile = fopen(filename, "r");
	
	fseek(romfile, 0, SEEK_END);
	
	buffer_size = ftell(romfile);
	
	rewind(romfile);
	
	filebuffer = (unsigned char *) malloc(buffer_size);
	
	fread(filebuffer, buffer_size, 1, romfile);
	
	fclose(romfile);

	// Init the emulation
	theConsole = new Console(filebuffer, buffer_size, "", *theSDLSnd);
	
	// Init palette
	const uInt32* gamePalette = theConsole->myMediaSource->palette();
	for(uInt32 i = 0; i < 256; i++)	{
		uInt8 r, g, b;
 
		r = (uInt8) ((gamePalette[i] & 0x00ff0000) >> 16);
		g = (uInt8) ((gamePalette[i] & 0x0000ff00) >> 8);
		b = (uInt8) ((gamePalette[i] & 0x000000ff) >> 0);
 
		//stella_palette[i]=PIX_TO_RGB(r,g,b);
		stella_palette[i] = (r >> 3) << 11 	| (g >> 2) << 5 | (b >> 3);
		
	}
	
	printf("done\n");
	
	return 1;
}

int main(int argc, char *argv[]) 
{
	unsigned int index;
	double period;

	// Get init file directory & name
#ifdef _TINSPIRE
	enable_relative_paths(argv);
#endif
	
	getcwd(current_conf_app, MAX__PATH);
	snprintf(current_conf_app, sizeof(current_conf_app), "./stella.cfg.tns",current_conf_app);
	
	// Init graphics & sound
	initSDL();
	
	m_Flag = GF_MAINUI;
	system_loadcfg(current_conf_app);

    //load rom file via args if a rom path is supplied
	if(argc > 1) {
		strcpy(gameName,argv[1]);
		m_Flag = GF_GAMEINIT;
	}

	while (m_Flag != GF_GAMEQUIT) {

		switch (m_Flag) {
			case GF_MAINUI:
				screen_showtopmenu();
				if (cartridge_IsLoaded()) {
					/*SDL_PauseAudio(0);*/
				}
				break;

			case GF_GAMEINIT:
				if (stella_init(gameName)) {
					m_Flag = GF_GAMERUNNING;
				}
				break;
		
			case GF_GAMERUNNING:
				
				// Update frame
				theConsole->update();

				// Draw frame
				graphics_paint();

				// Wait for keys
				theConsole->eventHandler().sendKeyEvent(StellaEvent::KCODE_SPACE, (button_state[4] == 1));
				
				theConsole->eventHandler().sendKeyEvent(StellaEvent::KCODE_UP,    (button_state[2] == 1));
				theConsole->eventHandler().sendKeyEvent(StellaEvent::KCODE_DOWN,  (button_state[3] == 1));
				theConsole->eventHandler().sendKeyEvent(StellaEvent::KCODE_LEFT,  (button_state[0] == 1));
				theConsole->eventHandler().sendKeyEvent(StellaEvent::KCODE_RIGHT, (button_state[1] == 1));
				
				theConsole->eventHandler().sendKeyEvent(StellaEvent::KCODE_F1, 0);
				theConsole->eventHandler().sendKeyEvent(StellaEvent::KCODE_F2, 0);
				theConsole->eventHandler().sendKeyEvent(StellaEvent::KCODE_F3, 0);
				theConsole->eventHandler().sendKeyEvent(StellaEvent::KCODE_F4, 0);
				theConsole->eventHandler().sendKeyEvent(StellaEvent::KCODE_F5, 0);
				theConsole->eventHandler().sendKeyEvent(StellaEvent::KCODE_F6, 0);

				
				if (button_state[12]) 
				{ 
					m_Flag = GF_MAINUI;
				}
				
				/*if ( (keys[SDLK_RETURN] == SDL_PRESSED) )  { theConsole->eventHandler().sendKeyEvent(StellaEvent::KCODE_F1, 1); }*/ // 
				
				nextTick += interval;
				break;
		}
	}
	
	deinitBuffering();
	
	exit(0);
}
