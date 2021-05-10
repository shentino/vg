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
#ifndef AGENT_H
#define AGENT_H

class board;
class gc;

class agent
{
	float x, y;
	float health;

public:
	agent()
	:x(-1),y(-1),health(-1)
	{
	}

	virtual ~agent();

	void set_position(float nx, float ny)
	{
		x = nx;
		y = ny;
	}
	void get_position(float &ox, float &oy)
	{
		ox = x;
		oy = y;
	}
	void set_health(float nh)
	{
		health = nh;
	}
	void get_health(float &oh)
	{
		oh = health;
	}

	void seek(board *b, float &dx, float &dy); // check the board, return which way we want to go
	void tick(); // burn health
	void draw(gc *gc, int cellsize, int ox, int oy, int cc) const;
};

#endif
