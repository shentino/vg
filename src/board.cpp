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
#include <cstdint>
#include <cstring>
#include <cstdlib>

#include "board.h"
#include "gc.h"

board::board(int x, int y)
:sx(x),sy(y)
{
	int i, sz;

	cells = new uint8_t[sx * sy];

	memset(cells, 0, sx * sy);

	for (i = 0, sz = (sx * sy); i < sz; i++) {
		cells[i] = lrand48() & 0xff;
	}
}

board::~board()
{
	delete[] cells;
	sx = sy = -1;
}

void board::draw(gc *gc, int cellsize, int ox, int oy, int cc) const
{
	for (int y = 0; y < sy; y++) {
		for (int x = 0; x < sx; x++) {
			uint16_t r, g, b, c;

			c = cells[y * sx + x];

			r = (c * 0x00 + (0xff - c) * 0xaa) / 255;
			g = (c * 0xff + (0xff - c) * 0x55) / 255;
			b = (c * 0x00 + (0xff - c) * 0x00) / 255;

			gc->box(x * cellsize + ox, y * cellsize + oy, cellsize, cellsize, ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff));
		}
	}
}
