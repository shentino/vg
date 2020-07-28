#ifndef GC_H
#define GC_H

#include <cstdint>

class gc
{
	gc();
	gc(const gc &);
	gc &operator=(const gc &);

protected:
	uint16_t width, height;

public:
	virtual ~gc();
	virtual void prepare() = 0; // prepares gc for drawing, fixes size
};

#endif
