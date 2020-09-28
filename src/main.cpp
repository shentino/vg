#include <stdexcept>
#include <iostream>

#include <sys/time.h>
#include <unistd.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "board.h"
#include "sdlgc.h"
#include "sdlglgc.h"
#include "ui.h"

using namespace std;

static const int framerate = 60;
static const int boardsize = 32;
static short delta[boardsize * boardsize];
static board b(boardsize, boardsize);
static bool quit = false;
static ui ui;

static Mix_Chunk *oceansound; // background sound

static Mix_Chunk *ticksound; // the cursor moved
static Mix_Chunk *zoomsound; // we zoomed in or out
Mix_Chunk *bumpsound; // either the cursor or the zoom hit a barrier

static Uint32 sdlwinid;
static Uint32 openglwinid;

static int value(int x, int y)
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
			if (b.cellat(x, y) > delta[y * boardsize + x] && b.cellat(x, y) > 0) {
				b.cellat(x, y)--;
			} else if (b.cellat(x, y) < delta[y * boardsize + x] && b.cellat(x, y) < 0xff) {
				b.cellat(x, y)++;
			}
		}
	}
}

static void handle_keydown(SDL_Event &e)
{
	switch(e.key.keysym.scancode) {
	/* zooming */
	case SDL_SCANCODE_KP_PLUS:
		if (ui.zoom < 64) {
			Mix_PlayChannel(1, zoomsound, 0);
			ui.zoom++;
		} else {
			Mix_PlayChannel(1, bumpsound, 0);
		}
		break;

	case SDL_SCANCODE_KP_MINUS:
		if (ui.zoom > 1) {
			Mix_PlayChannel(1, zoomsound, 0);
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

static void handle_mousemotion(SDL_Event &e)
{
	if (e.motion.windowID == sdlwinid) {
		ui.mcolor = 0xaa0000;
	} else if (e.motion.windowID == openglwinid) {
		ui.mcolor = 0x00aa00;
	} else {
		throw logic_error("Received an event for unexpected window");
	}

	ui.mx = e.motion.x;
	ui.my = e.motion.y;
}

static void handle_mousebutton(SDL_Event &e)
{
	ui.click(e.button.x, e.button.y);
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

		case SDL_WINDOWEVENT:
			// something's going on with one of our windows
			switch(e.window.event) {
			case SDL_WINDOWEVENT_CLOSE:
				// close means quit
				quit = true;
				return;

			case SDL_WINDOWEVENT_RESIZED:
				{
					SDL_Window *buddy;

					Uint32 wid = e.window.windowID;
					int w = e.window.data1;
					int h = e.window.data2;

					if (wid == sdlwinid) {
						buddy = SDL_GetWindowFromID(openglwinid);
					} else if (wid = openglwinid) {
						buddy = SDL_GetWindowFromID(sdlwinid);
					} else {
						throw runtime_error("Unknown window in SDL event");
					}

					if (!buddy) {
						throw runtime_error(SDL_GetError());
					}

					int cw, ch;

					SDL_GetWindowSize(buddy, &cw, &ch);

					// don't redundantly resize to avoid an infinite resize event loop
					if (w != cw || h != ch) {
						SDL_SetWindowSize(buddy, w, h);
					}
				}
				break;
			}
			break;

		case SDL_KEYDOWN:
			handle_keydown(e);
			break;

		case SDL_MOUSEMOTION:
			handle_mousemotion(e);
			break;

		case SDL_MOUSEBUTTONDOWN:
			handle_mousebutton(e);
			break;
		}
	}
}

int main(int argc, char *argv[], char *envp[])
{
	struct timeval now;
	struct timeval frametime;
	struct timeval future;

	int frame = 0; /* number of frames since last checkpoint */

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		throw runtime_error(SDL_GetError());
	}

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

	ticksound = Mix_LoadWAV("sound/tick.wav");
	zoomsound = Mix_LoadWAV("sound/zoom.wav");
	oceansound = Mix_LoadWAV("sound/ocean.wav");
	bumpsound = Mix_LoadWAV("sound/bump.wav");

	Mix_Volume(0, MIX_MAX_VOLUME / 2);
	Mix_PlayChannel(0, oceansound, -1);

	// set timestamp of first frame
	gettimeofday(&now, NULL);
	frametime = now;
	frame = 0;

	/* first frame */
	handle_events();
	if (quit) {
		goto quit;
	}

	// draw initial board
	ui.draw(&sgc, &b);
	ui.draw(&ggc, &b);

	/* end of first frame */

	for (;;) {
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
		ui.draw(&sgc, &b);
		ui.draw(&ggc, &b);

		frame++;

		if (frame >= framerate) {
			frame -= framerate;
			frametime.tv_sec++;
		}

		// wait for the next frame

		// first, calculate when we should resume
		int usec = (1000000 * frame / framerate) + frametime.tv_usec;
		int sec = frametime.tv_sec;

		sec += usec / 1000000;
		usec %= 1000000;

		future.tv_sec = sec;
		future.tv_usec = usec;

		// second, calculate how long to delay
		gettimeofday(&now, NULL);

		int usecs = ((int)future.tv_sec - (int)now.tv_sec) * 1000000;
		usecs += ((int)future.tv_usec - (int)now.tv_usec);

		if (usecs < 0) {
			// underrun
			frametime = now;
			frame = 0;
		} else if (usecs > 1500000 / framerate) {
			// overrun, someone's probably tinkering with the system clock
			// make sure we don't stall forever
			usec = now.tv_usec;
			sec = now.tv_sec;

			usec += 1000000 / framerate;
			sec += usec / 1000000;
			usec %= 1000000;

			frametime.tv_sec = sec;
			frametime.tv_usec = usec;
			frame = 0;

			usleep(1000000 / framerate);
		} else {
			usleep(usecs);
		}
	}

quit:
	Mix_CloseAudio();
	Mix_Quit();
	SDL_Quit();
}
