#ifndef VEC
#define VEC

#include <cmath>
#include <random>
#include <iostream>

using namespace std;

#define	ACCEPTERROR 1e-6
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#define MIN(a,b) (((a) < (b)) ? (a) : (b))

class Vec3
{
public:
	double x, y, z;
	Vec3(double _x = 0., double _y = 0., double _z = 0.): x(_x), y(_y), z(_z) {}
	~Vec3() {}
	friend Vec3 operator - (const Vec3& A, const Vec3& B);
	friend Vec3 operator + (const Vec3& A, const Vec3& B);
	friend Vec3 operator * (const double& A, const Vec3& B);
	friend double operator * (const Vec3& A, const Vec3& B);
	friend Vec3 operator - (const Vec3& A);
	friend Vec3 operator / (const Vec3& A, const double& B);
	friend Vec3 mathmulti(const Vec3& A, const Vec3 &B);
	friend Vec3 vexp(const Vec3& A);
	Vec3 unitization();
	double norm();
	double distance(const Vec3& A);
	double& getbyAxis(int axis);
	friend Vec3 getmin(const Vec3& A, const Vec3& B);
	friend Vec3 getmax(const Vec3& A, const Vec3& B);
	friend void print(const Vec3& A);
	Vec3 randomunit();
	double getmean();
	Vec3 cut(double limit);
};

#endif