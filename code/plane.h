#ifndef PLANE
#define PLANE

#include "item.h"

class plane:public item
{
	Vec3 norm;
	double dis;
	Vec3 dx, dy;
public:
	plane(Vec3 n, double dist, Vec3 _dx=Vec3(1, 0, 0), Vec3 _dy=Vec3(0, 1, 0));
	~plane() {}
	bool collide(Vec3 start, Vec3 direction);
	Vec3 getTexture(Vec3 p);
};

#endif