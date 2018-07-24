#ifndef SCENE
#define SCENE

#include "camera.h"
#include "lightsource.h"
#include "item.h"
#include "photonmap.h"

const int MAXPHOTON = 20000000;
const int EMITPHOTON = 16000000;
const int SAMPLE = 10000;
const double SAMPLE_DIST = 0.16;

class scene
{
	camera *cam;
	item **itemlist;
	light **lightlist;
	int num_item, num_light;
	photonmap *map;
public:
	scene(int n_i, int n_l);
	~scene();
	void setcamera(camera *ca) { cam = ca; }
	void setitem(int idx, item* it) { itemlist[idx] = it; }
	void setlight(int idx, light* li) { lightlist[idx] = li; }
	camera* getCam() { return cam; }
	photonmap* getmap() { return map; }
	item* getNearestItem(Vec3 pos, Vec3 dir);
	light* getNearestLight(Vec3 pos, Vec3 dir);
	void buildmap();
	void locating(int depth, photon p);
};

#endif