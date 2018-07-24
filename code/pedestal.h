#ifndef PEDESTAL
#define PEDESTAL

#include "item.h"
#include <Eigen\dense>

using namespace Eigen;
const int ITER_DEPTH = 160;
const double ITER_END = 0.0001;

class pedestal:public item
{
	Vec3 o;
	double rate;
	double r[6];
	double z[6];
	double bernstein(int i, double t);
	double delta_bern(int i, double t);
	double getR(double t);
	double getZ(double t);
	double getDR(double t);
	double getDZ(double t);
	int factorial(int i);
	Matrix3d getInverseJ(double t, double theta, double l, Vec3 start, Vec3 dir);
public:
	pedestal(Vec3 O, double _r=1.0);
	~pedestal();
	bool collide(Vec3 start, Vec3 direction);
	Vec3 getTexture(Vec3 p);
};
#endif