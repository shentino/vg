// this is the opengl on sdl version of the gc
#ifndef SDLGLGC_H
#define SDLGLGC_H

#include <SDL2/SDL.h>

#include "sdlgc.h"

class sdlglgc: public sdlgc
{
public:
	sdlglgc(const char *title);
	virtual ~sdlglgc();

	virtual void prepare();
	virtual void render();

	virtual void clear(int c);
	virtual void box(int x, int y, int w, int h, int c, int lw);
};

#endif
