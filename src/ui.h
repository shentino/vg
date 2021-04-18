/*
the UI class is responsible for keeping track of all data related to the UI

this includes cursor position and zoom level and anything that the board and gc shouldn't have to worry about
*/
#ifndef UI_H
#define UI_H

#include <cstdint>

class gc;
class board;

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
	void tick(); // once per frame
	void click(int x, int y); // mouse click
};

#endif
