#include "master.h"

const int MAXDEPTH = 10;

void master::setScene(scene* s)
{
	sce = s;
	H = sce->getCam()->getH();
	W = sce->getCam()->getW();
	result = new Vec3*[H];
	for(int i=0; i<H; i++)
		result[i] = new Vec3[W];
}

master::~master()
{
	delete sce;
	for(int i=0; i<H; i++)
		delete []result[i];
	delete []result;
}

void master::run()
{
	cout<<"master run"<<endl;
	sce->buildmap();
	cout<<"scene buildmap"<<endl;
	camera* cam = sce->getCam();
	Vec3 start = cam->getpos();
	for(int i=0; i<H; i++)
	{
		cout<<"i="<<i<<endl;
		for(int j=0; j<W; j++)
		{
			Vec3 dir = cam->emit(i, j);
			//cout<<i<<' '<<j<<"  emit"<<endl;
			//print(dir);
			Vec3 color = getColor(start, dir, 1);
			//print(color);
			result[i][j] = color;
		}
	}
	cout<<"finish master run"<<endl;
}

Vec3 master::getColor(Vec3 start, Vec3 dir, int depth)
{
	if(depth > MAXDEPTH)
		return Vec3();
	//cout<<"getcolor depth:"<<depth<<endl;
	Vec3 color;
	item* it = sce->getNearestItem(start, dir);
	light* li = sce->getNearestLight(start, dir);
	if(li != NULL)
	{
		//cout<<"add light power"<<endl;
		if(it == NULL || it->dist >= li->dist)
			color = li->color.cut(1.0);
	}
	if(it != NULL)
	{
		//cout<<"add item power"<<endl;
		Vec3 collide_pos = it->pos;
		Vec3 collide_normal = it->normal;
		double collide_dist = it->dist;
		bool collide_front = it->isfront;
		//print(collide_pos);
		if(it->mate->diff > ACCEPTERROR)
		{
			//cout<<"add diff power"<<endl;
			Vec3 mcolor = it->mate->diff * it->mate->color;
			if(it->mate->texture != NULL)
				mcolor = mathmulti(mcolor, it->getTexture(collide_pos));
			camera* cam = sce->getCam();
			//cout<<"start get map power"<<endl;
			Vec3 newcolor = sce->getmap()->getPower(collide_pos, collide_normal, SAMPLE, SAMPLE_DIST, EMITPHOTON);
			//cout<<"getcolor:";
			//print(newcolor);
			//cout<<"finish get map power"<<endl;
			color = color + Vec3(mcolor.x * newcolor.x, mcolor.y * newcolor.y, mcolor.z * newcolor.z);
		}
		if(it->mate->refrac > ACCEPTERROR)
		{
			//cout<<"add refrac power"<<endl;
			double r = collide_front? it->mate->r: (1. / it->mate->r);
			Vec3 input = dir.unitization();
			double l_iy = -input * collide_normal;
			Vec3 iy = l_iy * collide_normal;
			Vec3 ix = input + l_iy * collide_normal;
			double l_ix = ix.norm();
			double l_rx = l_ix / r;
			Vec3 newdir;
			if(l_rx < 1.)
			{
				Vec3 rx = l_rx * ix.unitization();
				Vec3 ry = -sqrt(1.-l_rx*l_rx) * collide_normal;
				newdir = ry + rx;
			}
			else
			{
				double l_line = 2 * (-input) * collide_normal;
				newdir = input + l_line * collide_normal;
			}
			Vec3 newcolor = getColor(collide_pos, newdir, depth+1);
			if(collide_front) 
				color = color + it->mate->refrac * newcolor;
			else
			{
				Vec3 absorb = vexp(-collide_dist * it->mate->absorb);
				color = color + it->mate->refrac * Vec3(absorb.x * newcolor.x, absorb.y * newcolor.y, absorb.z * newcolor.z);
			}
		}
		if(it->mate->reflex > ACCEPTERROR)
		{
			//cout<<"add reflex power"<<endl;
			double l_line = 2 * (-dir) * collide_normal;
			Vec3 newdir = dir + l_line * collide_normal;
			Vec3 newcolor = getColor(collide_pos, newdir, depth+1);
			Vec3 mcolor = it->mate->reflex * it->mate->color;
			color = color + Vec3(mcolor.x * newcolor.x, mcolor.y * newcolor.y, mcolor.z * newcolor.z);
		}
	}
	return depth==1? color.cut(1.0): color;
}

void master::output()
{
	pic out(H, W);
	out.outputFromColor(result);
}