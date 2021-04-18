#include <stdexcept>
#include <iostream>
#include <set>

#include <sys/time.h>
#include <unistd.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "framer.h"
#include "board.h"
#include "sdlgc.h"
#include "sdlglgc.h"
#include "ui.h"
#include "agent.h"

using namespace std;

extern void handle_events();

extern const int boardsize = 64;

static const int framerate = 60;
static short delta[boardsize * boardsize];
static framer f;
static set<agent *> agents;

static Mix_Chunk *oceansound; // background sound

ui ui;
board *b;
bool quit = false;
Mix_Chunk *clicksound; // the cursor moved
Mix_Chunk *bumpsound; // either the cursor or the zoom hit a barrier
Mix_Chunk *zoomsound; // we zoomed in or out

Uint32 sdlwinid;
Uint32 openglwinid;

static int value(int x, int y)
{
	int highest = 0;

	int lx = x > 0 ? x - 1 : 0;
	int ly = y > 0 ? y - 1 : 0;
	int hx = x < boardsize - 1 ? x + 1 : boardsize - 1;
	int hy = y < boardsize - 1 ? y + 1 : boardsize - 1;

	for (y = ly; y <= hy; y++) {
		for (x = lx; x <= hx; x++) {
			if (b->cellat(x, y) > highest) {
				highest = b->cellat(x, y);
			}
		}
	}

	return highest - 0x11;
}

static void calculate()
{
	for (int i = 0; i < boardsize * boardsize; i++) {
		delta[i] = 0;
	}

	for (int y = 0; y < boardsize; y++) {
		for (int x = 0; x < boardsize; x++) {
			delta[y * boardsize + x] = value(x, y);
		}
	}
}

static void commit()
{
	for (int y = 0; y < boardsize; y++) {
		for (int x = 0; x < boardsize; x++) {
			if (b->cellat(x, y) > delta[y * boardsize + x] && b->cellat(x, y) > 0) {
				b->cellat(x, y)--;
			} else if (b->cellat(x, y) < delta[y * boardsize + x] && b->cellat(x, y) < 0xff) {
				b->cellat(x, y)++;
			}
		}
	}
}

int main(int argc, char *argv[], char *envp[])
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		throw runtime_error(SDL_GetError());
	}

	b = new board(boardsize, boardsize);

	sdlgc sgc("SDL Video Game Prototype - SDL");
	sdlglgc ggc("SDL Video Game Prototype - GL");

	sdlwinid = sgc.getwindowid();
	openglwinid = ggc.getwindowid();

	Mix_Init(MIX_INIT_FLAC);

	if (Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 1, 512) < 0) {
		throw runtime_error(Mix_GetError());
	}

	Mix_AllocateChannels(2);
	// 0 = music
	// 1 = ui

	clicksound = Mix_LoadWAV("sound/click.wav");
	zoomsound = Mix_LoadWAV("sound/zoom.wav");
	oceansound = Mix_LoadWAV("sound/ocean.wav");
	bumpsound = Mix_LoadWAV("sound/bump.wav");

	Mix_Volume(0, MIX_MAX_VOLUME / 2);
	Mix_PlayChannel(0, oceansound, -1);

	/* first frame */
	handle_events();

	if (quit) {
		goto quit;
	}

	// draw initial board
	ui.draw_board(&sgc, b);
	ui.draw_board(&ggc, b);

	f.set_framerate(framerate);

	ggc.maximize();

	for (;;) {
		handle_events();

		if (quit) {
			goto quit;
		}

		// game logic
		uint8_t &c = b->cellat(ui.cx, ui.cy);

		for (int i = 0; i < 10; i++) {
			if (c < 0xff) {
				c++;
			} else {
				break;
			}
		}

		calculate();
		commit();

		ui.tick();
		ui.draw_board(&sgc, b);
		ui.draw_board(&ggc, b);
		ui.render(&sgc);
		ui.render(&ggc);

		f.next();
	}

quit:
	Mix_CloseAudio();
	Mix_Quit();
	SDL_Quit();
}
