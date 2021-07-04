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
/* graphics context */
/* defines basic API for drawing */
/* a derived class is intended to actually implement the API */

/* drawable objects usually have a draw() method that accepts a gc argument */

#ifndef GC_H
#define GC_H

class gc
{
	gc(const gc &); // copy constructor forbidden
	gc &operator=(const gc &); // assignment forbidden

protected:
	int width;
	int height;

	/* using the base class is forbidden */
	gc()
	{
	}

public:
	virtual ~gc() = 0;

	/** api **/

	/* dimension queries */

	/* guaranteed to stay fixed during the frame, drawables should query this during their draw routine */
	int get_width()
	{
		return width;
	}
	int get_height()
	{
		return height;
	}

	/* frame */
	virtual void prepare() = 0; // prepare for rendering.  this MUST set width and height
	virtual void render() = 0; // we're done drawing, render

	/* drawing primitives */
	virtual void clear(int c) = 0;
	virtual void box(int x, int y, int w, int h, int c) = 0;
};

#endif
