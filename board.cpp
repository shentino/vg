#include <cstdint>

#include "board.h"
#include "gc.h"

int board::cellsize = 32;

board::board(int x, int y)
:sx(x),sy(y)
{
	cells = new uint8_t[sx * sy];
}

board::~board()
{
	delete[] cells;
	sx = sy = -1;
}

void board::draw(gc *gc, int cx, int cy, int cc)
{
	int gx, gy;
	int ox, oy;

	gx = gc->get_width();
	gy = gc->get_height();

	ox = offset(cellsize, gx, sx * cellsize, cx * cellsize + cellsize / 2);
	oy = offset(cellsize, gy, sy * cellsize, cy * cellsize + cellsize / 2);

	for (int y = 0; y < sy; y++) {
		for (int x = 0; x < sx; x++) {
			uint16_t r, g, b, c;

			c = cells[y * sx + x];

			r = (c * 0x00 + (0xff - c) * 0xaa) / 255;
			g = (c * 0xff + (0xff - c) * 0x55) / 255;
			b = (c * 0x00 + (0xff - c) * 0x00) / 255;

			gc->box(x * cellsize + ox, y * cellsize + oy, cellsize, cellsize,
				((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff), 0
			);
		}
	}

	gc->box(cx * cellsize + ox, cy * cellsize + oy, cellsize, cellsize, cc, cellsize / 8);
}

