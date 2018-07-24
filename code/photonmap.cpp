#include "photonmap.h"

photonmap::photonmap(int max_size)
{
	cout<<"start new photomap"<<endl;
	max_photon = max_size;
	built_tree = 0;
	stored = 0;
	min_border = Vec3(1e8, 1e8, 1e8);
	max_border = Vec3(-1e8, -1e8, -1e8);
	photons = new photon[max_photon+1];
	cout<<"finish new photonmap"<<endl;
}

photonmap::~photonmap()
{
	delete []photons;
	delete map;
}

void photonmap::store(photon p)
{
	if(stored >= max_photon) return;
	min_border = getmin(min_border, p.pos);
	max_border = getmax(max_border, p.pos);
	photons[++stored] = p;
	if(stored % 1000 == 0)
		cout<<"stored:"<<stored<<endl;
}

void photonmap::buildTree()
{
	map = new kdTree(stored, photons);
	cout<<stored<<endl;
	map->mergebuilt(1, 1, stored, min_border, max_border);
	built_tree = 1;
}

Vec3 photonmap::getPower(Vec3 pos, Vec3 norm, int sample, double maxdist, int emit_photons)
{
	if(!built_tree)
	{
		cout<<"start build KDtree"<<endl;
		buildTree();
		cout<<"finish build KDtree"<<endl;
	}
	Vec3 power;
	//cout<<"start build heap"<<endl;
	heap pheap(sample, maxdist, pos, map);
	//cout<<"finish build heap"<<endl;
	int found = pheap.numFound();
	if(found <= 5) return power;
	//cout<<found<<endl;
	for(int i=1; i<=found; i++)
	{
		if(pheap.getphoton(i).dir * norm < 0)
		{
			//cout<<"heap power";
			//print(pheap.getphoton(i).power);
			power = power + pheap.getphoton(i).power;		
		}
	}
	//print(power);
	//cout<<"maxdist:"<<pheap.getmaxdist()<<endl;
	power = power / (3.1415926 * pheap.getmaxdist() * pheap.getmaxdist());
	//cout<<"power:"<<endl;
	//print(power);
	return power;
}