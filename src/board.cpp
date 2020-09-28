#include <cstdint>

#include "board.h"
#include "gc.h"

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

void board::draw(gc *gc, int cellsize, int ox, int oy, int cc) const
{
	for (int y = 0; y < sy; y++) {
		for (int x = 0; x < sx; x++) {
			uint16_t r, g, b, c;

			c = cells[y * sx + x];

			r = (c * 0x00 + (0xff - c) * 0xaa) / 255;
			g = (c * 0xff + (0xff - c) * 0x55) / 255;
			b = (c * 0x00 + (0xff - c) * 0x00) / 255;

			gc->box(x * cellsize + ox, y * cellsize + oy, cellsize, cellsize, ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff), 0);
		}
	}
}

