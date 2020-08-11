#include "ui.h"
#include "gc.h"
#include "board.h"

ui::ui()
:cx(0),cy(0),zoom(1),cursorframe(0)
{
}

void ui::draw(gc *g, const board *b)
{
	g->prepare();
	g->clear(0x0055aa + (lag << 16)); // ocean
	b->draw(g, zoom, cx, cy, (cursorframe / 15) & 1 ? 0xffffff : 0x000000);
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
