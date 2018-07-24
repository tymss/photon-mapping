#include "sphere.h"
#define PI 3.14159265359

sphere::sphere(Vec3 _o, double r, Vec3 _dz, Vec3 _dx):
	item(), o(_o), R(r), dz(_dz), dx(_dx) {}

bool sphere::collide(Vec3 start, Vec3 direction)
{
	Vec3 os = o - start;
	Vec3 ud = direction.unitization();
	double ld = os * ud;
	double ls = R * R - (os.norm() * os.norm() - ld * ld);
	if(ls < ACCEPTERROR)
		return false;
	else
	{
		ls = sqrt(ls);
		if(ls + ld < ACCEPTERROR)   //tangent
			return false;
		if(ld - ls > ACCEPTERROR)
		{
			isfront = true;
			dist = ld - ls;
		}
		else
		{
			isfront = false;
			dist = ld + ls;
		}
	}
	pos = start + dist * ud;
	normal = isfront ? (pos - o).unitization() : (o - pos).unitization();
	return true;
}

Vec3 sphere::getTexture(Vec3 p)
{
	Vec3 vec = p - o;
	Vec3 dy = (dx.y*dz.z-dx.z*dz.y, dx.x*dz.z-dx.z*dz.x, dx.x*dz.y-dx.y*dz.x);
	double z = vec * dz / R;
	double x = vec * dx / R;
	double y = vec * dy / R;
	double v = asin(z) / PI +0.5;
	double u = 0.5 * (atan2(y,x)/PI + 1.0);
	return getTextColor(u, v);
}