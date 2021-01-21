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
		, 32, 32
		, 320, 240, SDL_WINDOW_RESIZABLE
	);

	if (!win) {
		throw runtime_error(SDL_GetError());
	}
}

sdlgc::~sdlgc()
{
	SDL_DestroyWindow(win);
}

Uint32 sdlgc::getwindowid()
{
	Uint32 id = SDL_GetWindowID(win);

	if (id == 0) {
		throw runtime_error(SDL_GetError());
	}

	return id;
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

void sdlgc::box(int x, int y, int w, int h, int c)
{
	SDL_Rect r;

	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;

	SDL_FillRect(scr, &r, SDL_MapRGB(scr->format, (c >> 16) & 0xff, (c >> 8) & 0xff, c & 0xff));
}
