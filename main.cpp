#include <stdexcept>
#include <iostream>

#include <sys/time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "board.h"
#include "sdlgc.h"

using namespace std;

static const int framerate = 60;
static const int boardsize = 32;
static short delta[boardsize * boardsize];
static board b(boardsize, boardsize);

static inline int cursorcolor(int tick)
{
	int r, g, b;

	r = tick * 0x1;
	g = tick * 0x1 + 0x55;
	b = tick * 0x1 + 0xaa;

	return ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
}

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

int main(int argc, char *argv[], char *envp[])
{
	int frame;
	int t;
	int cx, cy;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		throw runtime_error(SDL_GetError());
	}

	cx = 0;
	cy = 0;

	sdlgc gc("SDL Video Game Prototype");

	Mix_Init(MIX_INIT_FLAC);

	if (Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 1, 512) < 0) {
		throw runtime_error(Mix_GetError());
	}

	Mix_AllocateChannels(3);

	Mix_Chunk *ticksound = Mix_LoadWAV("tick.wav");
	Mix_Chunk *zoominsound = Mix_LoadWAV("zoomin.wav");
	Mix_Chunk *zoomoutsound = Mix_LoadWAV("zoomout.wav");
	Mix_Chunk *oceansound = Mix_LoadWAV("ocean.wav");
	Mix_Chunk *bumpsound = Mix_LoadWAV("bump.wav");

	Mix_PlayChannel(2, oceansound, -1);

	for (;;) {
		SDL_Event e;

		// event processing
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				// don't waste time doing anything else if we're quitting
				// destructor for gc will handle our cleanup
				goto quit;

			case SDL_KEYDOWN:
				switch(e.key.keysym.scancode) {
				/* zooming */
				case SDL_SCANCODE_KP_PLUS:
					if (board::cellsize < 64) {
						Mix_PlayChannel(1, zoominsound, 0);
						board::cellsize++;
					} else {
						Mix_PlayChannel(1, bumpsound, 0);
					}
					break;

				case SDL_SCANCODE_KP_MINUS:
					if (board::cellsize > 0) {
						Mix_PlayChannel(1, zoomoutsound, 0);
						board::cellsize--;
					} else {
						Mix_PlayChannel(1, bumpsound, 0);
					}
					break;

				/* cursor movement */
				case SDL_SCANCODE_RIGHT:
					if (cx < boardsize - 1) {
						Mix_PlayChannel(0, ticksound, 0);
						cx++;
					} else {
						Mix_PlayChannel(1, bumpsound, 0);
					}
					break;

				case SDL_SCANCODE_LEFT:
					if (cx > 0) {
						Mix_PlayChannel(0, ticksound, 0);
						cx--;
					} else {
						Mix_PlayChannel(1, bumpsound, 0);
					}
					break;

				case SDL_SCANCODE_DOWN:
					if (cy < boardsize - 1) {
						Mix_PlayChannel(0, ticksound, 0);
						cy++;
					} else {
						Mix_PlayChannel(1, bumpsound, 0);
					}
					break;

				case SDL_SCANCODE_UP:
					if (cy > 0) {
						Mix_PlayChannel(0, ticksound, 0);
						cy--;
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
						b.cellat(x, cy) = 0xff;
					}
					break;

				case SDL_SCANCODE_D:
					for (int y = 0; y < boardsize; y++) {
						b.cellat(cx, y) = 0xff;
					}
					break;
				}
			}
		}

		// game logic
		uint8_t &c = b.cellat(cx, cy);

		for (int i = 0; i < 10; i++) {
			if (c < 0xff) {
				c++;
			} else {
				break;
			}
		}

		calculate();
		commit();

		// rendering
		gc.prepare();
		gc.clear(0x0055aa); // ocean
		b.draw(&gc, cx, cy, cursorcolor(t));
		gc.render();

		// delay
		SDL_Delay(1000 / framerate);

		t += 0x11;
		t &= 0xff;
	}

quit:
	Mix_CloseAudio();
	Mix_Quit();
	SDL_Quit();
}

