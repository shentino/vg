#include <iostream>
#include <stdexcept>

#include <SDL2/SDL.h>

int main(int argc, char *argv[], char *envp[])
{
	using namespace std;

	cout << "Hello world" << endl;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		throw runtime_error(SDL_GetError());
	}

	cout << "SDL Initialized" << endl;

	SDL_Delay(1000);

	SDL_Quit();
}
