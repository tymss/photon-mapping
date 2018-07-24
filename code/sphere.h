#ifndef SPHERE
#define SPHERE

#include "item.h"

class sphere:public item
{
	Vec3 o;
	double R;
	Vec3 dz, dx;
public:
	sphere(Vec3 _o, double r, Vec3 _dz=Vec3(0, 0, 1), Vec3 _dx=Vec3(1, 0, 0));
	~sphere() {}
	bool collide(Vec3 start, Vec3 direction);
	Vec3 getTexture(Vec3 p);
};

#endif