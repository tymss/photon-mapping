#ifndef ITEM
#define ITEM

#include "vec.h"
#include "pic.h"

class material
{
public:
	Vec3 color, absorb;
	double reflex, refrac, diff;
	double r;
	pic *texture;
	material();
	~material();
};

class item
{
protected:
	Vec3 getTextColor(double u, double v);
public:
	material *mate;
	Vec3 normal, pos;
	bool isfront;
	double dist;
	item();
	~item();	
	void setmate(Vec3 c, Vec3 ab, double rl, double rf, double diff, double r, pic* text=NULL);
	virtual bool collide(Vec3 start, Vec3 direction) = 0;
	virtual Vec3 getTexture(Vec3 p) = 0;
};

#endif