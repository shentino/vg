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
#include <stdexcept>

#include <GL/gl.h>
#include <GL/glu.h>

#include <SDL2/SDL.h>

#include "sdlglgc.h"

using namespace std;

sdlglgc::sdlglgc(const char *title)
{
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	win = SDL_CreateWindow(title
		, 64 + 320, 32
		, 320, 240, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL
	);

	if (!win) {
		throw runtime_error(SDL_GetError());
	}

	glc = SDL_GL_CreateContext(win);
}

sdlglgc::~sdlglgc()
{
	SDL_GL_DeleteContext(glc);
}

void sdlglgc::prepare()
{
	sdlgc::prepare();

	SDL_GL_MakeCurrent(win, glc);

	glLoadIdentity();
	glViewport(0, 0, width, height);
	glOrtho(0, width, height, 0, 0, 1);
}

void sdlglgc::render()
{
	SDL_GL_SwapWindow(win);
}

void sdlglgc::clear(int c)
{
	int r = (c >> 16) & 0xff;
	int g = (c >> 8) & 0xff;
	int b = c & 0xff;

	glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, 1);
	glClear(GL_COLOR_BUFFER_BIT);
}

void sdlglgc::box(int x, int y, int w, int h, int c)
{
	glColor3ub((c >> 16) & 0xff, (c >> 8) & 0xff, c & 0xff);

	glBegin(GL_TRIANGLE_FAN);
		glVertex2i(x, y);
		glVertex2i(x + w, y);
		glVertex2i(x + w, y + h);
		glVertex2i(x, y + h);
	glEnd();
}
