#ifndef PHOTON
#define PHOTON

#include "kdTree.h"

class photonmap
{
private:
	int max_photon, stored;
	photon *photons;
	kdTree *map;
	bool built_tree;
	Vec3 min_border, max_border;
	void buildTree();
public:
	photonmap(int max_size);
	~photonmap();
	bool isfull() { return stored == max_photon? 1 : 0; }
	void store(photon p);
	Vec3 getPower(Vec3 pos, Vec3 norm, int sample, double maxdist, int emit_photons);
};

#endif