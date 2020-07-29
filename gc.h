/* graphics context */
/* defines basic API for drawing */
/* a derived class is intended to actually implement the API */

/* drawable objects usually have a draw() method that accepts a gc argument */

#ifndef GC_H
#define GC_H

class gc
{
private:
	gc(const gc &); // copy constructor forbidden
	gc &operator=(const gc &); // assignment forbidden

protected:
	int width;
	int height;

	/* using the base class is forbidden */
	gc()
	{
	}

public:
	virtual ~gc() = 0;

	/** api **/

	/* dimension queries */

	/* guaranteed to stay fixed during the frame, drawables should query this during their draw routine */
	int get_width()
	{
		return width;
	}
	int get_height()
	{
		return height;	
	}

	/* frame */
	virtual void prepare() = 0; // prepare for rendering.  this MUST set width and height
	virtual void render() = 0; // we're done drawing, render

	/* drawing primitives */
	virtual void clear(int c) = 0;
	virtual void box(int x, int y, int w, int h, int c, int lw) = 0;
};

#endif
