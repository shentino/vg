/*
the UI class is responsible for keeping track of all data related to the UI

this includes cursor position and zoom level and anything that the board and gc shouldn't have to worry about
*/
#ifndef UI_H
#define UI_H

class gc;
class board;

class ui
{
	ui(const ui &);
	ui &operator=(const ui &);

	int cx, cy; // cursor position
	int zoom; // zoom factor

public:
	void draw(gc *g, const board *b);
	void keyup(int key);
	void keydown(int key);
	void tick(); // once per frame
};

#endif
