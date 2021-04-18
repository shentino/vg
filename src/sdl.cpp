#include <stdexcept>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "board.h"
#include "ui.h"

using namespace std;

extern ui ui;
extern board *b;
extern int sdlwinid;
extern int openglwinid;
extern const int boardsize;
extern bool quit;

extern Mix_Chunk *clicksound;
extern Mix_Chunk *bumpsound;
extern Mix_Chunk *zoomsound;

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
			Mix_PlayChannel(1, clicksound, 0);
			ui.cx++;
		} else {
			Mix_PlayChannel(1, bumpsound, 0);
		}
		break;

	case SDL_SCANCODE_LEFT:
		if (ui.cx > 0) {
			Mix_PlayChannel(1, clicksound, 0);
			ui.cx--;
		} else {
			Mix_PlayChannel(1, bumpsound, 0);
		}
		break;

	case SDL_SCANCODE_DOWN:
		if (ui.cy < boardsize - 1) {
			Mix_PlayChannel(1, clicksound, 0);
			ui.cy++;
		} else {
			Mix_PlayChannel(1, bumpsound, 0);
		}
		break;

	case SDL_SCANCODE_UP:
		if (ui.cy > 0) {
			Mix_PlayChannel(1, clicksound, 0);
			ui.cy--;
		} else {
			Mix_PlayChannel(1, bumpsound, 0);
		}
		break;

	/* commands */
	case SDL_SCANCODE_A:
		for (int y = 0; y < boardsize; y++) {
			for (int x = 0; x < boardsize; x++) {
				b->cellat(x, y) = 0xff;
			}
		}
		break;

	case SDL_SCANCODE_B:
		for (int y = 0; y < boardsize; y++) {
			for (int x = 0; x < boardsize; x++) {
				b->cellat(x, y) = 0x55;
			}
		}
		break;

	case SDL_SCANCODE_C:
		for (int x = 0; x < boardsize; x++) {
			b->cellat(x, ui.cy) = 0xff;
		}
		break;

	case SDL_SCANCODE_D:
		for (int y = 0; y < boardsize; y++) {
			b->cellat(ui.cx, y) = 0xff;
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

extern void handle_events()
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

			case SDL_WINDOWEVENT_ENTER:
				{
					SDL_Window *entered;

					entered = SDL_GetWindowFromID(e.window.windowID);

					if (!entered) {
						throw runtime_error("Unknown window in SDL event");
					}

					SDL_SetWindowInputFocus(entered);
				}
				break;

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