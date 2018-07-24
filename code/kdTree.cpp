#include "kdTree.h"
#include <queue>

kdTree::kdTree(int _size, photon* p):
	size(_size)
{
	finished = 0;
	photons = new photon[_size*2];
	sortp = new photon[_size*2];
	for(int i=0; i<=_size; i++)
	{
		sortp[i] = p[i];
		photons[i] = p[i];
	}
}

kdTree::~kdTree()
{
	delete []sortp;
	delete []photons;
}

photon kdTree::getbyindex(int index)
{
	return photons[index]; 
}

void kdTree::mergebuilt(int index, int st, int ed, Vec3 min_b, Vec3 max_b)
{
	finished++;
	if(finished % 500000 == 0) cout<<"finished:"<<finished<<endl;
	if(st >= ed)
	{
		photons[index] = sortp[st];
		photons[index].axis = 0;
		return;
	}
	//cout<<index<<endl;
	int mid = ((ed - st) >> 1) + st;
	int axis = 0;
	//print(min_b);
	//print(max_b);
	double x_dist = max_b.x - min_b.x, y_dist = max_b.y - min_b.y, z_dist = max_b.z - min_b.z;
	if(x_dist < y_dist && z_dist <= y_dist) axis = 1;
	else if(x_dist < z_dist && y_dist < z_dist) axis = 2;
	sort(st, ed, mid, axis);
	photons[index] = sortp[mid];
	photons[index].axis = axis;
	//cout<<photons[index].axis<<endl;
	if(mid > st)
	{
		Vec3 border = max_b;
		border.getbyAxis(axis) = photons[index].pos.getbyAxis(axis);
		photons[index].hasleft = 1;
		//if(2*index == 604863) cout<<"left   "<<index<<' '<<st<<' '<<ed<<endl;
		mergebuilt(2*index, st, mid-1, min_b, border);
	}
	if(mid < ed)
	{
		Vec3 border = min_b;
		border.getbyAxis(axis) = photons[index].pos.getbyAxis(axis);
		photons[index].hasright = 1;
		//if(2*index+1 == 604863) cout<<"right   "<<index<<' '<<st<<' '<<ed<<endl;
		mergebuilt(2*index+1, mid+1, ed, border, max_b);
	}
}

void kdTree::sort(int st, int ed, int mid, int axis)
{
	while(st < ed)
	{
		int xx = st - 1, yy = ed;
		photon k = sortp[ed];
		while(1)
		{
			while(sortp[++xx].pos.getbyAxis(axis) < k.pos.getbyAxis(axis));
			while(sortp[--yy].pos.getbyAxis(axis) > k.pos.getbyAxis(axis) && yy > xx);
			if(xx >= yy) break;
			photon temp = sortp[xx];
			sortp[xx] = sortp[yy];
			sortp[yy] = temp;
		}
		photon temp = sortp[xx];
		sortp[xx] = sortp[ed];
		sortp[ed] = temp;
		if(xx >= mid) ed = xx - 1;
		if(xx <= mid) st = xx + 1;
	}
}

int kdTree::getsize()
{
	return size;
}

heap::heap(int _size, double maxd, Vec3 _pos, kdTree *tree):
	size(_size), max_dist(maxd), stored(0), pos(_pos)
{
	photons = new photon[size+1];
	dist = new double[size+1];
	int n = tree->getsize();
	int index = 1;
	std::queue<int> indexq;
	//print(pos);
	while(index <= n)
	{
		//cout<<"search  "<<index<<endl;
		photon p = tree->getbyindex(index);
		//print(p.pos);
		//cout<<p.axis<<endl;
		double d = p.pos.distance(pos);
		if(max_dist >= d)
		{
			if(stored < size)
			{
				photons[++stored] = p;
				dist[stored] = d;
				int i = stored;
				while((i>>1)>=1)
				{
					if(dist[i] > dist[i>>1])
					{
						photons[i] = photons[i>>1];
						dist[i] = dist[i>>1];
						photons[i>>1] = p;
						dist[i>>1] = d;
						i = (i >> 1);
					}
					else
						break;
				}
			}
			else
			{	
				
				int i = 1, j = 2;
				while((i<<1) <= size)
				{
					if(j+1 <= size && dist[j] < dist[j+1]) j++;
					if(d <= dist[i] && d >= dist[j])
						break;
					else
					{
						dist[i] = dist[j];
						photons[i] = photons[j];
						i = j;
						j = i << 1;
					}
				}
				dist[i] = d;
				photons[i] = p;
				max_dist = dist[1];
			}
		}
		double d0 = pos.getbyAxis(p.axis) - p.pos.getbyAxis(p.axis);
		if(d0 < 0)
		{
			if(p.hasleft)
				indexq.push(index << 1);
			if(d0 * d0 < max_dist * max_dist && p.hasright)
			{
				/*cout<<endl<<index<<endl;
				cout<<"maxdist:"<<max_dist<<"   axis:"<<p.axis<<"    now pos:";
				print(p.pos);
				cout<<"next pos:";
				print(tree->getbyindex(1+(index<<1)).pos);*/
				indexq.push((index << 1) + 1);
			}
		}
		else
		{
			if(d0 * d0 < max_dist * max_dist && p.hasleft)
				indexq.push(index << 1);
			if(p.hasright)
				indexq.push((index << 1) + 1);
		}
		if(indexq.empty())
			break;
		index = indexq.front();
		indexq.pop();
	}
}

heap::~heap()
{
	delete []photons;
	delete []dist;
}

photon heap::getphoton(int index)
{
	return photons[index];
}

double heap::getmaxdist()
{
	return max_dist;
}

int heap::numFound()
{
	return stored;
}