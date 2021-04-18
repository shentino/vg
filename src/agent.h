#ifndef AGENT_H
#define AGENT_H

class agent
{
	float x, y;
	float health;

public:
	agent();
	virtual ~agent();

	void move(float nx, float ny)
	{
	}
	void get_position(float &ox, float &oy)
	{
		ox = x;
		oy = y;
	}
	void get_health(float &oh)
	{
		oh = health;
	}

	void seek(board *b, float &dx, float &dy); // check the board, return which way we want to go
	void tick(); // burn health
};

#endif
