#include "scene.h"

const int MAXDEPTH = 10;

scene::scene(int n_i, int n_l):
	num_item(n_i), num_light(n_l)
{
	itemlist = new item*[num_item];
	lightlist = new light*[num_light];
	cout<<"scene:item and light finish"<<endl;
	map = new photonmap(MAXPHOTON);
}

scene::~scene()
{
	delete cam;
	for(int i=0; i<num_item; i++)
		delete itemlist[i];
	delete []itemlist;
	for(int i=0; i<num_light; i++)
		delete lightlist[i];
	delete []lightlist;
	delete map;
}

item* scene::getNearestItem(Vec3 pos, Vec3 dir)
{
	item* result = NULL;
	int idx = -1;
	double min_dist = 9999.;
	for(int i=0; i<num_item; i++)
	{
		if(itemlist[i]->collide(pos, dir))
		{
			if(idx == -1)
			{
				idx = i;
				min_dist = itemlist[i]->dist;
				result = itemlist[i];
			}
			else if(itemlist[i]->dist < min_dist)
			{
				idx = i;
				min_dist = itemlist[i]->dist;
				result = itemlist[i];
			}
		}
	}
	return result;	
}

light* scene::getNearestLight(Vec3 pos, Vec3 dir)
{
	light* result = NULL;
	int idx = -1;
	double min_dist = 9999.;
	for(int i=0; i<num_light; i++)
	{
		if(lightlist[i]->collide(pos, dir))
		{
			if(idx == -1)
			{
				idx = i;
				min_dist = lightlist[i]->dist;
				result = lightlist[i];
			}
			else if(lightlist[i]->dist < min_dist)
			{
				idx = i;
				min_dist = lightlist[i]->dist;
				result = lightlist[i];
			}
		}
	}
	return result;
}

void scene::buildmap()
{
	double power = 0.;
	for(int i=0; i<num_light; i++)
		power += lightlist[i]->color.getmean();
	double photon_power = power / EMITPHOTON;
	for(int i=0; i<num_light; i++)
	{
		power = lightlist[i]->color.getmean();
		while(power >= photon_power)
		{
			if(map->isfull()) break;
			photon p = lightlist[i]->getPhoton();
			p.power = photon_power * p.power;
			locating(1, p);
			power -= photon_power;
		}
	}
}

void scene::locating(int depth, photon p)
{
	if(depth > MAXDEPTH) return;
	item* it = getNearestItem(p.pos, p.dir);
	if(it == NULL) return;
	p.pos = it->pos;
	Vec3 collide_pos = it->pos;
	Vec3 collide_normal = it->normal;
	bool collide_front = it->isfront;
	double collide_dist = it->dist;
	if(it->mate->diff > ACCEPTERROR)
		map->store(p);
	double prob_diff = it->mate->diff * it->mate->color.getmean();
	if((double(rand()) / double(RAND_MAX + 1)) < prob_diff)
	{
		p.power = mathmulti(p.power, it->mate->color / it->mate->color.getmean());
		Vec3 newdir = p.dir.randomunit();		
		while(newdir * collide_normal < ACCEPTERROR)
			newdir = newdir.randomunit();
		p.dir = newdir;
		//print(p.power);	
		locating(depth+1, p);
	}
	else
	{
		double prob_reflex = it->mate->reflex * it->mate->color.getmean();
		if((double(rand()) / double(RAND_MAX + 1)) * (1. - prob_diff) < prob_reflex)
		{
			double l_line = 2 * (-p.dir) * collide_normal;
			p.dir = p.dir + l_line * collide_normal;
			p.power = mathmulti(p.power, it->mate->color / it->mate->color.getmean());
			//print(p.power);
			locating(depth+1, p);
		}
		else
		{
			double prob_refrac = it->mate->refrac;
			if(!collide_front)
			{
				Vec3 absorb = vexp(-collide_dist * it->mate->absorb);
				prob_refrac *= absorb.getmean();
				p.power = mathmulti(p.power, absorb / absorb.getmean());
				//print(p.power);
			}
			if((double(rand()) / double(RAND_MAX + 1)) * (1. - prob_diff - prob_reflex) < prob_refrac)
			{
				double r = collide_front? it->mate->r : (1. / it->mate->r);
				Vec3 input = p.dir.unitization();
				double l_iy = -input * collide_normal;
				Vec3 iy = l_iy * collide_normal;
				Vec3 ix = input + l_iy * collide_normal;
				double l_ix = ix.norm();
				double l_rx = l_ix / r;
				if(l_rx < 1.)
				{
					Vec3 rx = l_rx * ix.unitization();
					Vec3 ry = -sqrt(1.-l_rx*l_rx) * collide_normal;
					p.dir = ry + rx;
					locating(depth+1, p);
				}
				else
				{ 
					double l_line = 2 * (-input) * collide_normal;
					p.dir = input + l_line * collide_normal;
					locating(depth+1, p); 
				}
			}
		}
	}
}
