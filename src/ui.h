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

	uint8_t lag;
	uint8_t cursorframe; // which frame out of 60
	// cursor flashes black and white twice a second
	// 15 frames white, 15 frames black, repeat

public:
	int cx, cy; // cursor position
	int mx, my; // mouse position
	int sx, sy; // window size
	uint32_t mcolor; // mouse color
	int zoom; // zoom factor

	ui();

	void draw(gc *g, const board *b);
	void tick(); // once per frame
	void click(int x, int y); // mouse click
	void lagged(); // flag a lag so we can send an "ouch" color
};

#endif
