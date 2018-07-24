#ifndef KDTREE
#define KDTREE

#include "vec.h"

class photon
{
public:
	Vec3 pos, dir;
	Vec3 power;
	int axis;
	bool hasleft, hasright;
	photon(): hasleft(0), hasright(0){}
};


class kdTree
{
private:
	int size;
	int finished;
	photon *photons, *sortp;
	Vec3 min_b, max_b;
	void sort(int st, int ed, int mid, int axis);
public:
	kdTree(int size, photon *p);
	~kdTree();
	int getsize();
	void mergebuilt(int index, int st, int ed, Vec3 min_b, Vec3 max_b);
	photon getbyindex(int index);
};

class heap
{
private:
	int size, stored;
	double max_dist;
	photon *photons;
	double *dist;
	Vec3 pos;
public:
	heap(int _size, double maxd, Vec3 _pos, kdTree *tree);
	~heap();
	photon getphoton(int index);
	double getmaxdist();
	int numFound();
};

#endif