#include <stdexcept>
#include <iostream>

#include <sys/time.h>
#include <unistd.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "board.h"
#include "sdlgc.h"
#include "ui.h"

using namespace std;

static const int framerate = 60;
static const int boardsize = 32;
static short delta[boardsize * boardsize];
static board b(boardsize, boardsize);
static bool quit = false;
static ui ui;

static Mix_Chunk *ticksound;
static Mix_Chunk *zoominsound;
static Mix_Chunk *zoomoutsound;
static Mix_Chunk *oceansound;
static Mix_Chunk *bumpsound;

static inline int value(int x, int y)
{
	int highest = 0;

	int lx = x > 0 ? x - 1 : 0;
	int ly = y > 0 ? y - 1 : 0;
	int hx = x < boardsize - 1 ? x + 1 : boardsize - 1;
	int hy = y < boardsize - 1 ? y + 1 : boardsize - 1;

	for (y = ly; y <= hy; y++) {
		for (x = lx; x <= hx; x++) {
			if (b.cellat(x, y) > highest) {
				highest = b.cellat(x, y);
			}
		}
	}

	return highest - 0x11;
}

static inline void calculate()
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

static inline void commit()
{
	for (int y = 0; y < boardsize; y++) {
		for (int x = 0; x < boardsize; x++) {
			if (b.cellat(x, y) > delta[y * boardsize + x] && b.cellat(x, y) > 0) {
				b.cellat(x, y)--;
			} else if (b.cellat(x, y) < delta[y * boardsize + x] && b.cellat(x, y) < 0xff) {
				b.cellat(x, y)++;
			}
		}
	}
}

static void handle_events()
{
	SDL_Event e;

	// event processing
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_QUIT:
			// don't waste time doing anything else if we're quitting
			// destructor for gc will handle our cleanup
			quit = true;
			return;

		case SDL_KEYDOWN:
			switch(e.key.keysym.scancode) {
			/* zooming */
			case SDL_SCANCODE_KP_PLUS:
				if (ui.zoom < 64) {
					Mix_PlayChannel(1, zoominsound, 0);
					ui.zoom++;
				} else {
					Mix_PlayChannel(1, bumpsound, 0);
				}
				break;

			case SDL_SCANCODE_KP_MINUS:
				if (ui.zoom > 1) {
					Mix_PlayChannel(1, zoomoutsound, 0);
					ui.zoom--;
				} else {
					Mix_PlayChannel(1, bumpsound, 0);
				}
				break;

			/* cursor movement */
			case SDL_SCANCODE_RIGHT:
				if (ui.cx < boardsize - 1) {
					Mix_PlayChannel(1, ticksound, 0);
					ui.cx++;
				} else {
					Mix_PlayChannel(1, bumpsound, 0);
				}
				break;

			case SDL_SCANCODE_LEFT:
				if (ui.cx > 0) {
					Mix_PlayChannel(1, ticksound, 0);
					ui.cx--;
				} else {
					Mix_PlayChannel(1, bumpsound, 0);
				}
				break;

			case SDL_SCANCODE_DOWN:
				if (ui.cy < boardsize - 1) {
					Mix_PlayChannel(1, ticksound, 0);
					ui.cy++;
				} else {
					Mix_PlayChannel(1, bumpsound, 0);
				}
				break;

			case SDL_SCANCODE_UP:
				if (ui.cy > 0) {
					Mix_PlayChannel(1, ticksound, 0);
					ui.cy--;
				} else {
					Mix_PlayChannel(1, bumpsound, 0);
				}
				break;

			/* commands */
			case SDL_SCANCODE_A:
				for (int y = 0; y < boardsize; y++) {
					for (int x = 0; x < boardsize; x++) {
						b.cellat(x, y) = 0xff;
					}
				}
				break;

			case SDL_SCANCODE_B:
				for (int y = 0; y < boardsize; y++) {
					for (int x = 0; x < boardsize; x++) {
						b.cellat(x, y) = 0x55;
					}
				}
				break;

			case SDL_SCANCODE_C:
				for (int x = 0; x < boardsize; x++) {
					b.cellat(x, ui.cy) = 0xff;
				}
				break;

			case SDL_SCANCODE_D:
				for (int y = 0; y < boardsize; y++) {
					b.cellat(ui.cx, y) = 0xff;
				}
				break;
			}
		}
	}
}

int main(int argc, char *argv[], char *envp[])
{
	struct timeval frametime;
	int frame = 0; /* number of frames since last checkpoint */

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		throw runtime_error(SDL_GetError());
	}

	sdlgc gc("SDL Video Game Prototype");

	Mix_Init(MIX_INIT_FLAC);

	if (Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 1, 512) < 0) {
		throw runtime_error(Mix_GetError());
	}

	Mix_AllocateChannels(2);
	// 0 = music
	// 1 = ui

	ticksound = Mix_LoadWAV("tick.wav");
	zoominsound = Mix_LoadWAV("zoomin.wav");
	zoomoutsound = Mix_LoadWAV("zoomout.wav");
	oceansound = Mix_LoadWAV("ocean.wav");
	bumpsound = Mix_LoadWAV("bump.wav");

	Mix_PlayChannel(0, oceansound, -1);

	gettimeofday(&frametime, NULL); // set timestamp of first frame

	/* first frame */
	handle_events();

	if (quit) {
		goto quit;
	}

	// draw initial board
	ui.draw(&gc, &b);

	for (;;) {
		struct timeval future;

		handle_events();		

		if (quit) {
			goto quit;
		}

		// game logic
		uint8_t &c = b.cellat(ui.cx, ui.cy);

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
		ui.draw(&gc, &b);

		frame++;

		/* calculate when this frame should end */
		future = frametime;

		int usecs = frametime.tv_usec;
		usecs += (1000000 * frame / 60);

		future.tv_sec += usecs / 1000000;
		future.tv_usec += usecs % 1000000;
		
		struct timeval now;
		
		gettimeofday(&now, NULL);

		usecs = ((int)future.tv_sec - (int)now.tv_sec) * 1000000;
		usecs += ((int)future.tv_usec - (int)now.tv_usec);

		if (usecs < 0) {
			// underrun
			frametime = now;
			frame = 0;
		} else {
			usleep(usecs);
		}
	}

quit:
	Mix_CloseAudio();
	Mix_Quit();
	SDL_Quit();
}

