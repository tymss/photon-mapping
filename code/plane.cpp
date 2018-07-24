#include "plane.h"

plane::plane(Vec3 n, double dist, Vec3 _dx, Vec3 _dy):
	norm(n), dis(dist), dx(_dx), dy(_dy) {}

bool plane::collide(Vec3 start, Vec3 direction)
{
	norm = norm.unitization();
	Vec3 ud = direction.unitization();
	Vec3 os = Vec3(0, 0, 0) - start;
	double ld = - (os * (norm));
	if(ld - dis > ACCEPTERROR && -(norm * ud) > ACCEPTERROR)
	{
		dist = (ld - dis) / (- (norm * ud));
		isfront = true;
	}
	else if(dis - ld > ACCEPTERROR && norm * ud > ACCEPTERROR)
	{
		dist = (dis - ld) / (norm * ud);
		isfront = false;
	}
	else
		return false;
	normal = isfront ? norm : -norm; 
	pos = start + dist * ud;
	return true;
}

Vec3 plane::getTexture(Vec3 p)
{
	double u = p * dx / dx.norm();
	double v = p * dy / dy.norm();
	//print(p);
	//cout<<u<<' '<<v<<endl;
	if(fabs(u) > dx.norm() || fabs(v) > dy.norm()) return Vec3();
	u = 0.5 + u / (2 * dx.norm());
	v = 0.5 + v / (2 * dy.norm());
	return getTextColor(u, v);
}