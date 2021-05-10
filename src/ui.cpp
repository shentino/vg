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
#include <cmath>

#include <SDL2/SDL_mixer.h>

#include "ui.h"
#include "agent.h"
#include "gc.h"
#include "board.h"

const float pi = atanf(1.0f) * 4.0f; 

extern Mix_Chunk *clicksound; // cursor moved
extern Mix_Chunk *bumpsound; // error

ui::ui()
:cx(0),cy(0),zoom(8),cursorframe(0)
{
}

// static int offset(int c, int w, int b, int f);

// c - collar size
// w - window size
// b - board size
// f - focus point

void ui::draw_board(gc *g, const board *b)
{
	g->prepare();

	sx = g->get_width();
	sy = g->get_height();

	bx = b->get_width();
	by = b->get_height();

	int ox = offset(zoom, sx, bx * zoom, cx * zoom + zoom / 2);
	int oy = offset(zoom, sy, by * zoom, cy * zoom + zoom / 2);

	g->clear(0x0055aa); // ocean
	b->draw(g, zoom, ox, oy, 0xffffff);

	if ((cursorframe / 15) & 1) {
		g->box(cx * zoom + ox, cy * zoom + oy, zoom, zoom, 0xffffff);
	}
}

void ui::draw_agent(gc *g, const agent *a)
{
	int ox = offset(zoom, sx, bx * zoom, cx * zoom + zoom / 2);
	int oy = offset(zoom, sy, by * zoom, cy * zoom + zoom / 2);

	a->draw(g, zoom, ox, oy, 0x000000);
}

void ui::render(gc *g)
{
	g->render();
}

void ui::tick()
{
	cursorframe++;
	cursorframe %= 60;
}

void ui::click(int mx, int my)
{
	int ox, oy;
	int tx, ty;

	ox = offset(zoom, sx, bx * zoom, cx * zoom + zoom / 2);
	oy = offset(zoom, sy, by * zoom, cy * zoom + zoom / 2);

	tx = (mx - ox);
	ty = (my - oy);

	if (tx < 0 || tx >= bx * zoom || ty < 0 || ty >= by * zoom) {
		Mix_PlayChannel(1, bumpsound, 0);
		return;
	}

	Mix_PlayChannel(1, clicksound, 0);

	cx = tx / zoom;
	cy = ty / zoom;
}
