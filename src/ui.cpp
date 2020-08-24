#include <cmath>

#include "ui.h"
#include "gc.h"
#include "board.h"

const float pi = atanf(1.0f) * 4.0f; 

ui::ui()
:cx(0),cy(0),zoom(32),cursorframe(0),lag(0)
{
}

void ui::draw(gc *g, const board *b)
{
	int red, green, blue;

	red = green = blue = ((cursorframe / 15) & 1) ? 0xff : 0x00;

	g->prepare();
	g->clear(0x0055aa + (lag << 16)); // ocean
	b->draw(g, zoom, cx, cy, blue | (green << 8) | (red << 16));
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

void ui::lagged()
{
	lag = 0xff;
}
