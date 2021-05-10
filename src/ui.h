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
/*
the UI class is responsible for keeping track of all data related to the UI

this includes cursor position and zoom level and anything that the board and gc shouldn't have to worry about
*/
#ifndef UI_H
#define UI_H

#include <cstdint>

class gc;
class board;
class agent;

class ui
{
	ui(const ui &);
	ui &operator=(const ui &);

	uint8_t cursorframe; // which frame out of 60
	// cursor flashes black and white twice a second
	// 15 frames white, 15 frames black, repeat

	int sx, sy; // window size, cached at draw time
	int bx, by; // board size, cached at draw time

public:
	int cx, cy; // cursor position
	int mx, my; // mouse position

	uint32_t mcolor; // mouse color
	int zoom; // zoom factor

	ui();

	static int offset(int c, int w, int b, int f)
	{
		// c - collar size
		// w - window size
		// b - board size
		// f - focus point
		int o;

		if (w >= (b + c * 2)) {
			// room for the whole thing
			return (w - b) / 2;
		}

		if (f + c < w / 2) {
			// focus is close enough to left edge
			return c;
		} else if (c + b - f < w / 2) {
			// focus is close enough to right edge
			return w - b - c;
		} else {
			// squeeze time
			return (w / 2) - f;
		}
	}

	void draw_board(gc *g, const board *b);
	void draw_agent(gc *g, const agent *a);
	void render(gc *g);

	void tick(); // once per frame
	void click(int x, int y); // mouse click
};

#endif
