#include <cstddef>
#include <sys/time.h>
#include <unistd.h>

#include "framer.h"

framer::framer()
{
	frame = 0;
	framerate = 0;
}

void framer::set_framerate(int newrate)
{
	framerate = newrate;

	reset();
}

void framer::reset()
{
	frame = 0;

	gettimeofday(&future, NULL);
	frametime = future;
}

void framer::next()
{
	frame++;

	while (frame > framerate) {
		frame -= framerate;
		frametime.tv_sec++;
	}

	// first, calculate when we should resume
	int usec = (1000000 * frame / framerate) + frametime.tv_usec;
	int sec = frametime.tv_sec;

	sec += usec / 1000000;
	usec %= 1000000;

	future.tv_sec = sec;
	future.tv_usec = usec;

	// second, calculate how long to delay
	timeval now;
	gettimeofday(&now, NULL);

	usec = ((int)future.tv_sec - (int)now.tv_sec) * 1000000;
	usec += ((int)future.tv_usec - (int)now.tv_usec);

	if (usec < 0) {
		// underrun
		frametime = now;
		frame = 0;
	} else if (usec > 1500000 / framerate) {
		// overrun, someone's probably tinkering with the system clock
		// make sure we don't stall forever
		usec = now.tv_usec;
		sec = now.tv_sec;

		usec += 1000000 / framerate;
		sec += usec / 1000000;
		usec %= 1000000;

		frametime.tv_sec = sec;
		frametime.tv_usec = usec;
		frame = 0;

		usleep(1000000 / framerate);
	} else {
		usleep(usec);
	}
}