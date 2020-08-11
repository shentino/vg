#ifndef SDLGC_H
#define SDLGC_H

#include <SDL2/SDL.h>

#include "gc.h"

class sdlgc: public gc
{
	SDL_Surface *scr;

protected:
	SDL_Window *win;

public:
	sdlgc(const char *title);
	virtual ~sdlgc();

	virtual void prepare();
	virtual void render();

	virtual void clear(int c);
	virtual void box(int x, int y, int w, int h, int c, int lw);
};

#endif

