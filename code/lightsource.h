#ifndef LIGHT
#define LIGHT

#include "item.h"
#include "kdTree.h"

class light
{	
public:
	Vec3 color;
	double dist;
	light(Vec3 c): color(c) {}
	~light() {}
	virtual photon getPhoton() = 0;
	virtual bool collide(Vec3 start, Vec3 direction) = 0;
};

class reclight: public light
{
public:
	Vec3 pos, dx, dy, normal;
	reclight(Vec3 c, Vec3 p, Vec3 n, Vec3 x, Vec3 y): light(c), pos(p), normal(n), dx(x), dy(y) {}
	~reclight() {}
	photon getPhoton();
	bool collide(Vec3 start, Vec3 direction);
};


class pointlight: public light
{
public:
	Vec3 pos;
	pointlight(Vec3 c, Vec3 p): light(c), pos(p) {}
	~pointlight() {}
	photon getPhoton();
	bool collide(Vec3 start, Vec3 direction);
};

#endif