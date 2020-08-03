#ifndef SPRITE_H
#define SPRITE_H

#include <cstdint>

#include "pva.h"

class gc;

class sprite
{
	
public:
	pva x, y; // cells, relative to origin
	int color;
	float size;

	void draw(gc *g, int cx, int cy, 
};

#endif SPRITE_H
