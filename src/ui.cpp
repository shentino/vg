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
	float phase;
	uint8_t val;

	phase = (float)cursorframe; // extract which frame of this second we're in
	phase /= 60.0f; // normalize to [0, 1)
	phase *= pi * 2.0f; // convert to radians
	phase *= 2.0f; // two blinks per second
	phase = sinf(phase); // [-1, 1]
	phase *= 63.5f; // [-127.5, 127.5]
	phase += 127.5f; // [0.0, 255.0]
	val = (uint8_t)nearbyintf(phase)

	g->prepare();
	g->clear(0x0055aa + (lag << 16)); // ocean
	b->draw(g, zoom, cx, cy, val | (val << 8) | (val << 16));
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
