#include "lightsource.h"

photon reclight::getPhoton()
{
	photon p;
	p.power = color / color.getmean();
	Vec3 delta_x = (double(rand() / (RAND_MAX + 1.0)) * 2 - 1.0) * dx;
	Vec3 delta_y = (double(rand() / (RAND_MAX + 1.0)) * 2 - 1.0) * dy;
	p.pos = pos + delta_x + delta_y;
	p.dir = p.dir.randomunit();
	/*
	cout<<"pos   ";
	print(p.pos);
	cout<<"dir   ";
	print(p.dir);
	cout<<"power   ";
	print(p.power);
	*/
	return p;
}

bool reclight::collide(Vec3 start, Vec3 direction)
{
	Vec3 dir = direction.unitization();
	Vec3 n = normal.unitization();
	double v_dist = n * dir;
	if(fabs(v_dist) < ACCEPTERROR) return false;
	Vec3 o_o = pos - start;
	double n_dist = o_o * n;
	if(v_dist > ACCEPTERROR && n_dist < ACCEPTERROR) return false;
	if(v_dist < -ACCEPTERROR && n_dist > -ACCEPTERROR) return false;
	Vec3 delta = (start + n_dist * n) - pos;
	if(fabs(delta * dx) > dx * dx) return false;
	if(fabs(delta * dy) > dy * dy) return false;
	dist = n_dist / v_dist;
	return true;
}

photon pointlight::getPhoton()
{
	photon p;
	p.power = color / color.getmean();
	p.pos = pos;
	p.dir = p.dir.randomunit();
	return p;
}

bool pointlight::collide(Vec3 start, Vec3 direction)
{
	return false;
}