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
	static int cellsize;

	board(int x, int y);
	virtual ~board();

	void draw(gc *gc, int cx, int cy, int cc);

	uint8_t &cellat(int x, int y)
	{
		using namespace std;

		if (x < 0 || x >= sx || y < 0 || y >= sy) {
			throw out_of_range("out of bounds");
		}

		return cells[sx * y + x];
	}

	// c - collar size
	// w - window size
	// b - board size
	// f - focus point

	// returns: where to draw the home end of the board
	static int offset(int c, int w, int b, int f)
	{
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
};

#endif

