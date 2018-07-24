#include "camera.h"

camera::camera(Vec3 p, Vec3 d, Vec3 _dw,
		Vec3 _dh, int _h, int _w): h(_h), w(_w)
{
	pos = p;
	dir = d;
	dw = _dw;
	dh = _dh;
}

Vec3 camera::emit(int i, int j)
{
	Vec3 delta = (1. - 2. * double(i) / double(h)) * dh + (2. * double(j) / double(w) - 1.) * dw;
	return dir + delta;
}