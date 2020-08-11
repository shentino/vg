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
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	win = SDL_CreateWindow(title
		, 64 + 320, 32
		, 320, 240, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL
	);

	if (!win) {
		throw runtime_error(SDL_GetError());
	}
}

sdlglgc::~sdlglgc()
{
}
