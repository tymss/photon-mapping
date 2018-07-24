#ifndef MASTER
#define MASTER

#include "pic.h"
#include "scene.h"

class master
{
	scene* sce;
	Vec3** result;
	int W, H;
	Vec3 getColor(Vec3 start, Vec3 dir, int depth);
public:
	master(): sce(NULL) {}
	~master();
	void setScene(scene* s);
	void output();
	void run();
};

#endif