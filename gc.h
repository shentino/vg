#ifndef GC_H
#define GC_H

class gc
{
	gc();
	gc(const gc &);
	gc &operator=(const gc &);

public:
	virtual ~gc();
};

#endif
