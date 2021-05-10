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
#ifndef BOARD_H
#define BOARD_H

#include <cstdint>
#include <stdexcept>

class gc;

class board
{
	uint8_t *cells;
	int sx, sy;

	board();

public:
	board(int x, int y);
	virtual ~board();

	int get_width() const
	{
		return sx;
	}

	int get_height() const
	{
		return sy;
	}

	uint8_t &cellat(int x, int y)
	{
		using namespace std;

		if (x < 0 || x >= sx || y < 0 || y >= sy) {
			throw out_of_range("out of bounds");
		}

		return cells[sx * y + x];
	}

	void draw(gc *gc, int cellsize, int ox, int oy, int cc) const;
};

#endif
