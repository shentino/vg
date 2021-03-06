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
#include <iostream>
#include <set>

#include <sys/time.h>
#include <unistd.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "framer.h"
#include "board.h"
#include "sdlgc.h"
#include "sdlglgc.h"
#include "ui.h"
#include "agent.h"
#include "audio.h"

using namespace std;

extern void handle_events();

extern const int boardsize = 64;

static const int framerate = 60;
static short delta[boardsize * boardsize];
static framer f;
static set<agent *> agents;

ui ui;
board *b;
bool quit = false;

Uint32 sdlwinid;
Uint32 openglwinid;

static void randomize()
{
	timeval tv;

	gettimeofday(&tv, NULL);

	srand48(tv.tv_sec ^ tv.tv_usec);
	srandom(tv.tv_sec ^ tv.tv_usec);
}

static int value(int x, int y)
{
	int highest = 0;

	int lx = x > 0 ? x - 1 : 0;
	int ly = y > 0 ? y - 1 : 0;
	int hx = x < boardsize - 1 ? x + 1 : boardsize - 1;
	int hy = y < boardsize - 1 ? y + 1 : boardsize - 1;

	for (y = ly; y <= hy; y++) {
		for (x = lx; x <= hx; x++) {
			if (b->cellat(x, y) > highest) {
				highest = b->cellat(x, y);
			}
		}
	}

	return highest - 0x11;
}

static void calculate()
{
	for (int i = 0; i < boardsize * boardsize; i++) {
		delta[i] = 0;
	}

	for (int y = 0; y < boardsize; y++) {
		for (int x = 0; x < boardsize; x++) {
			delta[y * boardsize + x] = value(x, y);
		}
	}
}

static void commit()
{
	for (int y = 0; y < boardsize; y++) {
		for (int x = 0; x < boardsize; x++) {
			if (b->cellat(x, y) > delta[y * boardsize + x] && b->cellat(x, y) > 0) {
				b->cellat(x, y)--;
			} else if (b->cellat(x, y) < delta[y * boardsize + x] && b->cellat(x, y) < 0xff) {
				b->cellat(x, y)++;
			}
		}
	}
}

static void create_agent()
{
	agent *a = new agent;

	a->set_position(lrand48() % (boardsize - 1), lrand48() % (boardsize - 1));
	a->set_health(5.0 + drand48());

	agents.insert(a);
}

static void process_agents()
{
	set<agent *>::iterator ab, ae;

	set<agent *> hearse;

	ab = agents.begin();
	ae = agents.end();

	while (ab != ae) {
		agent *a;
		float health;
		float x, y;

		a = *ab;

		a->get_position(x, y);

		uint8_t &f = b->cellat(x, y);

		a->get_health(health);

		if (health > 0.0) {
			while (f > 0 && health < 7.0) {
				f--;
				health += 0.03;
			}
		}

		a->set_health(health);
		a->tick();

		if (health < -5.0) {
			hearse.insert(a);
		}

		ab++;
	}

	ab = hearse.begin();
	ae = hearse.end();

	while (ab != ae) {
		agent *corpse = *ab;

		agents.erase(corpse);

		delete corpse;

		ab++;
	}

	while (agents.size() < 20) {
		create_agent();
	}
}

static void draw_agents(gc *g)
{
	set<agent *>::iterator ab, ae;

	ab = agents.begin();
	ae = agents.end();

	while (ab != ae) {
		ui.draw_agent(g, *ab);
		ab++;
	}
}

static void draw_frame(gc *g)
{
	ui.draw_board(g, b);
	draw_agents(g);
	ui.render(g);
}

int main(int argc, char *argv[], char *envp[])
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		throw runtime_error(SDL_GetError());
	}

	b = new board(boardsize, boardsize);

	sdlgc sgc("SDL Video Game Prototype - SDL");
	sdlglgc ggc("SDL Video Game Prototype - GL");

	sdlwinid = sgc.getwindowid();
	openglwinid = ggc.getwindowid();

	setup_mixer();
	load_sounds();
	start_music();

	/* first frame */
	handle_events();

	if (quit) {
		goto quit;
	}

	while (agents.size() < 20) {
		create_agent();
	}

	// draw initial board
	ui.draw_board(&sgc, b);
	ui.draw_board(&ggc, b);

	f.set_framerate(framerate);

	if (random() & 0x1) {
		ggc.maximize();
	} else {
		sgc.maximize();
	}

	for (;;) {
		handle_events();

		if (quit) {
			goto quit;
		}

		// game logic
		uint8_t &c = b->cellat(ui.cx, ui.cy);

		for (int i = 0; i < 10; i++) {
			if (c < 0xff) {
				c++;
			} else {
				break;
			}
		}

		calculate();
		commit();
		process_agents();

		ui.tick();

		draw_frame(&sgc);
		draw_frame(&ggc);

		f.next();
	}

quit:
	Mix_CloseAudio();
	Mix_Quit();
	SDL_Quit();
}
