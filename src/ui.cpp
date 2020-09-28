#include <cmath>

#include <SDL2/SDL_mixer.h>

#include "ui.h"
#include "gc.h"
#include "board.h"

const float pi = atanf(1.0f) * 4.0f; 

extern Mix_Chunk *clicksound; // cursor moved
extern Mix_Chunk *bumpsound; // error

ui::ui()
:cx(0),cy(0),zoom(32),cursorframe(0),lag(0)
{
}

// static int offset(int c, int w, int b, int f);

// c - collar size
// w - window size
// b - board size
// f - focus point

void ui::draw(gc *g, const board *b)
{
	int red, green, blue;
	int ox, oy;

	red = green = blue = ((cursorframe / 15) & 1) ? 0xff : 0x00;

	g->prepare();

	// future clicks will be based on the frame we're currently drawing, capture the size now

	sx = g->get_width();
	sy = g->get_height();

	bx = b->get_width();
	by = b->get_height();

	ox = offset(zoom, sx, bx * zoom, cx * zoom + zoom / 2);
	oy = offset(zoom, sy, by * zoom, cy * zoom + zoom / 2);

	g->clear(0x0055aa + (lag << 16)); // ocean
	b->draw(g, zoom, ox, oy, blue | (green << 8) | (red << 16));
	g->box(cx * zoom + ox, cy * zoom + oy, zoom, zoom, ((red & 0xff) << 16) + ((green & 0xff) << 8) + (blue & 0xff), zoom / 8);
	g->box(mx - 2, my - 2, 5, 5, mcolor, 0);
	g->render();
}

void ui::tick()
{
	cursorframe++;
	cursorframe %= 60;

	if (lag) {
		lag--;
	}
}

void ui::click(int mx, int my)
{
	int ox, oy;
	int tx, ty;

	ox = offset(zoom, sx, bx * zoom, cx * zoom + zoom / 2);
	oy = offset(zoom, sy, by * zoom, cy * zoom + zoom / 2);

	tx = (mx - ox);
	ty = (my - oy);

	if (tx < 0 || tx >= bx * zoom || ty < 0 || ty >= by * zoom) {
		Mix_PlayChannel(1, bumpsound, 0);
		return;
	}

	Mix_PlayChannel(1, clicksound, 0);

	cx = tx / zoom;
	cy = ty / zoom;
}

void ui::lagged()
{
	lag = 0xff;
}
