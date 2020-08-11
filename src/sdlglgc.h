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
};

#endif
