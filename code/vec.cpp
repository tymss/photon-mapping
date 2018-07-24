#include "vec.h"

Vec3 operator - (const Vec3 &a, const Vec3 &b)
{
	return Vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vec3 operator + (const Vec3 &a, const Vec3 &b)
{
	return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vec3 operator * (const double &a, const Vec3 &b)
{
	return Vec3(a * b.x, a * b.y, a * b.z);
}

double operator * (const Vec3 &a, const Vec3 &b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 operator - (const Vec3 &a)
{
	return Vec3(-a.x, -a.y, -a.z);
}

Vec3 operator / (const Vec3 &a, const double &b)
{
	return Vec3(a.x / b, a.y / b, a.z / b);
}

Vec3 mathmulti(const Vec3 &a, const Vec3 &b)
{
	return Vec3(a.x * b.x, a.y * b.y, a.z * b.z);
}

Vec3 Vec3::unitization()
{
	double norm = this->norm();
	return Vec3(x/norm, y/norm, z/norm);
}

double Vec3::norm()
{
	return sqrt(x * x + y * y + z * z);
}

double Vec3::distance(const Vec3 &a)
{
	return (*this - a).norm();
}

double& Vec3::getbyAxis(int axis)
{
	if(axis == 0) return x;
	if(axis == 1) return y;
	if(axis == 2) return z;
}

Vec3 getmin(const Vec3& a, const Vec3& b)
{
	return Vec3(MIN(a.x, b.x), MIN(a.y, b.y), MIN(a.z, b.z));
}

Vec3 getmax(const Vec3& a, const Vec3& b)
{
	return Vec3(MAX(a.x, b.x), MAX(a.y, b.y), MAX(a.z, b.z));
}

Vec3 Vec3::randomunit()
{
	x = double(rand() - RAND_MAX/2);
	y = double(rand() - RAND_MAX/2);
	z = double(rand() - RAND_MAX/2);
	return unitization();
}

double Vec3::getmean()
{
	return (x + y + z) / 3.;
}

Vec3 vexp(const Vec3& a)
{
	return Vec3(exp(a.x), exp(a.y), exp(a.z));
}

Vec3 Vec3::cut(double limit)
{
	if(x > limit)
		x = limit;
	if(y > limit)
		y = limit;
	if(z > limit)
		z = limit;
	return Vec3(x, y, z);
}

void print(const Vec3& a)
{
	cout<<a.x<<' '<<a.y<<' '<<a.z<<endl;
}