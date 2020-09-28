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

	void draw(gc *gc, int cellsize, int ox, int oy, int cc) const;

	uint8_t &cellat(int x, int y)
	{
		using namespace std;

		if (x < 0 || x >= sx || y < 0 || y >= sy) {
			throw out_of_range("out of bounds");
		}

		return cells[sx * y + x];
	}
};

#endif

