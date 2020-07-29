#ifndef PVA_H
#define PVA_H

struct pva
{
	float pos;
	float vel;
	float acc;

	void advance(float t)
	{
		float av;

		av = vel + t * acc * 0.5f;

		pos += av * t;
		vel += acc * t;
	}
};

#endif
