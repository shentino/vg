#ifndef GC_H
#define GC_H

class gc
{
	gc();
	gc(const gc &);
	gc &operator=(const gc &);

public:
	virtual ~gc();
	virtual void prepare() = 0; // prepares gc for drawing, fixes size
};

#endif
