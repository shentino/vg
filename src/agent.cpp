#include "agent.h"
#include "board.h"
#include "gc.h"

using namespace std;

agent::~agent()
{
}

// check the board, return which way we want to go
void agent::seek(board *b, float &dx, float &dy)
{
}

// burn health
void agent::tick()
{
	health -= 1.0 / 60.0;
}

void agent::draw(gc *g, int cellsize, int ox, int oy, int cc) const
{
	float lhealth;
	int c;

	lhealth = health;

	if (lhealth < -5.0) {
		lhealth = -5.0;
	} else if (lhealth > 5.0) {
		lhealth = 5.0;
	}

	if (lhealth > 0.0) {
		// white, decaying to black
		c = 0x010101 * (int)(lhealth * 255.0 / 5.0);
	} else {
		// black, decaying to red
		c = 0x010000 * (int)(-lhealth * 255.0 / 5.0);
	}

	g->box(
		x * cellsize + ox,
		y * cellsize + oy,
		cellsize, cellsize,
		c
	);
}
