#ifndef CAMERA
#define CAMERA

#include "vec.h"

class camera
{
	Vec3 pos, dir, dw, dh;
	int h, w;
public:
	camera(Vec3 p=Vec3(0,0,0), Vec3 d=Vec3(0,1,0), Vec3 _dw=Vec3(1,0,0),
		Vec3 _dh=Vec3(0,0,1), int _h=500, int _w=500);
	~camera() {}
	int getH() { return h; }
	int getW() { return w; }
	Vec3 emit(int i, int j);
	Vec3 getpos() { return pos; }
};

#endif