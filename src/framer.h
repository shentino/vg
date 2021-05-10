#ifndef FRAMER_H
#define FRAMER_H

#include <sys/time.h>

class framer
{
	int framerate; // frames per second
	int frame; // frame of second

	timeval future; // when we should resume
	timeval frametime; // time of frame zero

public:
	framer();

	void set_framerate(int newrate);

	void reset();
	void next();
};

#endif
