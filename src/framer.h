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
#ifndef FRAMER_H
#define FRAMER_H

#include <sys/time.h>

class framer
{
	int framerate; // frames per second
	int frame; // frame of second

	timeval future; // when we should resume
	timeval frametime; // time of frame zero

public:
	framer();

	void set_framerate(int newrate);

	void reset();
	void next();
};

#endif
