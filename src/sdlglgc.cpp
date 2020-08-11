#include <GL/gl.h>
#include <GL/glu.h>

#include <SDL2/SDL.h>

#include "sdlglgc.h"

#include <stdexcept>

#include <SDL2/SDL.h>

#include "sdlgc.h"

using namespace std;

sdlglgc::sdlglgc(const char *title)
{
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	win = SDL_CreateWindow(title
		, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED
		, 640, 480, SDL_WINDOW_RESIZABLE | SDL_OPENGL
	);

	if (!win) {
		throw runtime_error(SDL_GetError());
	}

	SDL_MaximizeWindow(win);
}
