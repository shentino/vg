/*
 * Video Game Prototype
 * Copyright (C) 2021  Raymond Jennings
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
// this is the opengl on sdl version of the gc
#ifndef SDLGLGC_H
#define SDLGLGC_H

#include <SDL2/SDL.h>

#include "sdlgc.h"

class sdlglgc: public sdlgc
{
	SDL_GLContext glc;

public:
	sdlglgc(const char *title);
	virtual ~sdlglgc();

	virtual void prepare();
	virtual void render();

	virtual void clear(int c);
	virtual void box(int x, int y, int w, int h, int c);
};

#endif
