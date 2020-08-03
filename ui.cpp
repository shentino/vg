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
	g->clear(0x0055aa); // ocean
	b->draw(
	g->render();
}