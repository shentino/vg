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
#ifndef SDLGC_H
#define SDLGC_H

#include <SDL2/SDL.h>

#include "gc.h"

class sdlgc: public gc
{
	SDL_Surface *scr;

protected:
	SDL_Window *win;

	sdlgc();

public:
	sdlgc(const char *title);
	virtual ~sdlgc();

	Uint32 getwindowid();

	void maximize();

	virtual void prepare();
	virtual void render();

	virtual void clear(int c);
	virtual void box(int x, int y, int w, int h, int c);
};

#endif
