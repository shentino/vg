#include "agent.h"
#include "board.h"
#include "gc.h"

// check the board, return which way we want to go
void agent::seek(board *b, float &dx, float &dy)
{
}

// burn health
void agent::tick()
{
	health -= 1.0 / 60.0;
}

void agent::draw(gc *gc, int cellsize, int ox, int oy, int cc) const
{
	int x, y;

	gc->box(
		x * cellsize + ox,
		y * cellsize + oy,
		cellsize, cellsize,
		0x000000
	);
}
