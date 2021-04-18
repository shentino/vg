#ifndef AGENT_H
#define AGENT_H

class board;
class gc;

class agent
{
	float x, y;
	float health;

public:
	agent()
	:x(-1),y(-1),health(-1)
	{
	}

	virtual ~agent();

	void set_position(float nx, float ny)
	{
		x = nx;
		y = ny;
	}
	void get_position(float &ox, float &oy)
	{
		ox = x;
		oy = y;
	}
	void set_health(float nh)
	{
		health = nh;
	}
	void get_health(float &oh)
	{
		oh = health;
	}

	void seek(board *b, float &dx, float &dy); // check the board, return which way we want to go
	void tick(); // burn health
	void draw(gc *gc, int cellsize, int ox, int oy, int cc) const;
};

#endif
