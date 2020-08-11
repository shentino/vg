#include <stdexcept>

#include <SDL2/SDL.h>

#include "sdlgc.h"

using namespace std;

sdlgc::sdlgc()
{
}

sdlgc::sdlgc(const char *title)
{
	win = SDL_CreateWindow(title
		, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED
		, 640, 480, SDL_WINDOW_RESIZABLE
	);

	if (!win) {
		throw runtime_error(SDL_GetError());
	}
}

sdlgc::~sdlgc()
{
	SDL_DestroyWindow(win);
}

/* frame management */

void sdlgc::prepare()
{
	scr = SDL_GetWindowSurface(win);
	width = scr->w;
	height = scr->h;
}

void sdlgc::render()
{
	scr = NULL; // disallow further drawing until we prepare again

	SDL_UpdateWindowSurface(win);
}

/* drawing primitives */

void sdlgc::clear(int c)
{
	SDL_FillRect(scr, NULL, SDL_MapRGB(scr->format, (c >> 16) & 0xff, (c >> 8) & 0xff, c & 0xff));
}

void sdlgc::box(int x, int y, int w, int h, int c, int lw)
{
	SDL_Rect r;

	if (lw == 0) {
		r.x = x;
		r.y = y;
		r.w = w;
		r.h = h;

		SDL_FillRect(scr, &r, SDL_MapRGB(scr->format, (c >> 16) & 0xff, (c >> 8) & 0xff, c & 0xff));
	} else {
		// horizontal
		r.w = w;
		r.h = lw;
		r.x = x;

		r.y = y;
		SDL_FillRect(scr, &r, SDL_MapRGB(scr->format, (c >> 16) & 0xff, (c >> 8) & 0xff, c & 0xff));
		r.y = y + h - lw;
		SDL_FillRect(scr, &r, SDL_MapRGB(scr->format, (c >> 16) & 0xff, (c >> 8) & 0xff, c & 0xff));

		// vertical
		r.w = lw;
		r.h = h;
		r.y = y;

		r.x = x;
		SDL_FillRect(scr, &r, SDL_MapRGB(scr->format, (c >> 16) & 0xff, (c >> 8) & 0xff, c & 0xff));
		r.x = x + w - lw;
		SDL_FillRect(scr, &r, SDL_MapRGB(scr->format, (c >> 16) & 0xff, (c >> 8) & 0xff, c & 0xff));
	}
}

