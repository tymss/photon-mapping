#include "pedestal.h"

#define PI 3.14159265359


pedestal::pedestal(Vec3 O, double _r):
	o(O), rate(_r)
{
	r[0] = 0, z[0] = 0;
	r[1] = 1, z[1] = 1;
	r[2] = 2, z[2] = 1.5;
	r[3] = 2, z[3] = 2.5;
	r[4] = 2, z[4] = 3;
	r[5] = 2, z[5] = 4;
}

int pedestal::factorial(int num)
{
	int result = 1;
	for(int i=0; i<num; i++)
		result *= i + 1;
	return result;
}

double pedestal::bernstein(int i, double t)
{
	return 120. / (double(factorial(i)) * double(factorial(5-i))) * pow(t, double(i)) * pow(1-t, double(5-i));
}

double pedestal::delta_bern(int i, double t)
{
	double coe = 120. / (double(factorial(i)) * double(factorial(5-i)));
	if(i == 0)
		return coe * 5 * pow(1-t, 4.);
	if(i == 5)
		return coe * 5 * pow(t, 4.);
	return coe * double(i) * double(5-i) * pow(t, double(i-1)) * pow(1-t, double(5-i-1));
}

double pedestal::getR(double t)
{
	double R = 0;
	for(int i=0; i<6; i++)
		R += r[i] * bernstein(i, t);
	return R * rate;
}

double pedestal::getZ(double t)
{
	double Z = 0;
	for(int i=0; i<6; i++)
		Z += z[i] * bernstein(i, t);
	return Z * rate;
}

double pedestal::getDZ(double t)
{
	double dz = 0;
	for(int i=0; i<6; i++)
		dz += z[i] * delta_bern(i, t);
	return dz * rate;
}

double pedestal::getDR(double t)
{
	double dr = 0;
	for(int i=0; i<6; i++)
		dr += r[i] * delta_bern(i, t);
	return dr * rate;
}

Matrix3d pedestal::getInverseJ(double t, double theta, double l, Vec3 start, Vec3 dir)
{
	Matrix3d J;
	J(0, 0) = dir.x;
	J(1, 0) = dir.y;
	J(2, 0) = dir.z;
	J(0, 1) = -cos(theta) * getDR(t);
	J(1, 1) = -sin(theta) * getDR(t);
	J(2, 1) = -getDZ(t);
	J(0, 2) = sin(theta) * getR(t);
	J(1, 2) = -cos(theta) * getR(t);
	J(2, 2) = 0;
	//cout<<J<<endl;
	return J.inverse();
}

bool pedestal::collide(Vec3 start, Vec3 dir)
{
	double theta0, t0, l0;
	//around box
	Vec3 pro_vec(1, 1, 0);
	Vec3 pro_s = mathmulti(pro_vec, start);
	Vec3 pro_dir = mathmulti(pro_vec, dir).unitization();
	Vec3 pro_O = mathmulti(o, pro_vec);
	Vec3 so = pro_O - pro_s;
	if(fabs(so * pro_dir) < ACCEPTERROR) return false;
	Vec3 so_ = so * pro_dir * pro_dir;
	double di = (so - so_).norm();
	if(rate * 2. - di < ACCEPTERROR) return false;
	double d0 = sqrt(4. * rate * rate - di * di);
	Vec3 pro_o;
	if(so * pro_dir + d0 < ACCEPTERROR) return false;
	if(so * pro_dir - d0 > ACCEPTERROR) pro_o = pro_s + (so * pro_dir - d0) * pro_dir;
	else pro_o = pro_s + (d0 + so * pro_dir) * pro_dir;
	l0 = (fabs(dir.x) > fabs(dir.y)) ? (pro_o.x - start.x) / dir.x : (pro_o.y - start.y) / dir.y; 
	Vec3 around_o = l0 * dir + start;
	if(start.z - 4 * rate - o.z > ACCEPTERROR)
	{
		if(dir.z >= 0) return false;
		double lm = (4 * rate + o.z - start.z) / dir.z;
		Vec3 am = start + lm * dir;
		double dd = pow(am.x - o.x, 2.) + pow(am.y - o.y, 2.);
		if(dd < 4 * rate * rate)
		{
			around_o = am;
			l0 = lm;
		}
		else
			if(around_o.z - 4 * rate - o.z > ACCEPTERROR || o.z - around_o.z > ACCEPTERROR) return false;
	}
	else if(o.z - start.z > ACCEPTERROR)
	{
		if(dir.z <= 0) return false;
		double lm = (o.z - start.z) / dir.z;
		Vec3 am = start + lm * dir;
		double dd = pow(am.x - o.x, 2.) + pow(am.y - o.y, 2.);
		if(dd < 4 * rate * rate)
		{
			around_o = am;
			l0 = lm;
		}
		else
			if(around_o.z - 4 * rate - o.z > ACCEPTERROR || o.z - around_o.z > ACCEPTERROR) return false;
	}
	else if(4 * rate + o.z - start.z > ACCEPTERROR && start.z - o.z >= ACCEPTERROR)
	{
		if(dir.z > 0 * rate * rate)
		{
			double lm = (4 * rate + o.z - start.z) / dir.z;
			Vec3 am = start + lm * dir;
			double dd = pow(am.x - o.x, 2.) + pow(am.y - o.y, 2.);
			if(dd < 4 * rate * rate)
			{
				around_o = am;
				l0 = lm;
			}
			else
				if(around_o.z - 4 * rate - o.z > ACCEPTERROR || o.z - around_o.z > ACCEPTERROR) return false;
		}
		else if(dir.z < 0 * rate * rate)
		{
			double lm = (o.z - start.z) / dir.z;
			Vec3 am = start + lm * dir;
			double dd = pow(am.x - o.x, 2.) + pow(am.y - o.y, 2.);
			if(dd < 4 * rate * rate)
			{
				around_o = am;
				l0 = lm;
			}
			else
				if(around_o.z - 4 * rate - o.z > ACCEPTERROR || o.z - around_o.z > ACCEPTERROR) return false;
		}
	}
	//start iteration
	double can_theta[4] = {PI/4., 0.75*PI, -PI/4., -0.75*PI};
	double can_t[4] = {0.2, 0.4, 0.6, 0.8};
	Vec3 res[16];
	Vec3 x[16];
	bool find[16] = {0};
	double can_l = l0;
	for(int q=0; q<16; q++)
	{
		theta0 = can_theta[q/4];
		t0 = can_t[q%4];
		l0 = can_l;
		//cout<<"l0="<<l0<<"   t0="<<t0<<"   theta0="<<theta0<<endl;
		for(int t=0; t<ITER_DEPTH; t++)
		{
			//cout<<"iter="<<t<<endl;
			Matrix3d IJ = getInverseJ(t0, theta0, l0, start, dir);
			double fx0 = start.x + dir.x * l0 - o.x - cos(theta0) * getR(t0);
			double fx1 = start.y + dir.y * l0 - o.y - sin(theta0) * getR(t0);
			double fx2 = start.z + dir.z * l0 - o.z - getZ(t0);
			//cout << "fx=" << fx0 << "   " << fx1 << "   " <<fx2<< "   "<<endl;
			//cout<<"l0="<<l0<<"   t0="<<t0<<"   theta0="<<theta0<<endl;
			double dx[3];
			for(int i=0; i<3; i++)
				dx[i] = fx0 * IJ(i, 0) + fx1 * IJ(i, 1) + fx2 * IJ(i, 2);
			l0 -= dx[0];
			t0 -= dx[1];
			theta0 -= dx[2];
			if(fabs(dx[0]) < ACCEPTERROR && fabs(dx[1]) < ACCEPTERROR && fabs(dx[2]) < ACCEPTERROR)
			{
				find[q] = 1;
				//cout << t << endl;
				break;
			}
		}
		//cout<<t0<<endl;
		if(t0<0 || t0 > 1 || l0 < 0) find[q] = 0;
		res[q] = start + l0 * dir;
		x[q] = Vec3(l0, t0, theta0);
	}
	double mindist = 1000.;
	int minindex = -1;
	for(int i=0; i<16; i++)
	{
		if(find[i])
		{//print(res[i]);
			double md = (res[i] - start).norm();
			if(md < mindist)
			{
				mindist = md;
				minindex = i;
			}
		}
	}
	if(minindex == -1) return false;
	pos = res[minindex];
	l0 = x[minindex].x;
	t0 = x[minindex].y;
	theta0 = x[minindex].z;
	double r0 = 1.;
	double r1 = -getDR(t0) / getDZ(t0);
	normal = Vec3(cos(theta0) * r0, sin(theta0) * r0, r1).unitization();
	if(dir * normal > 0) normal = -normal;
	//cout << "pos=";
	//print(pos);
	//cout <<"normal=";
	//print(normal);
	dist = (pos - start).norm();
	Vec3 h_op = mathmulti(Vec3(1, 1, 0), o - pos);
	Vec3 hnormal = mathmulti(Vec3(1, 1, 0), normal);
	if(h_op * hnormal > 0) isfront = 1;
	else isfront = 0;
	return true;
}

Vec3 pedestal::getTexture(Vec3 p)
{
	double dx = p.x - o.x;
	double dy = p.y - o.y;
	double dz = p.z - o.z;
	double v = dz / (4. * rate);
	double u = 0.5 * (atan2(dy,dx)/PI + 1.0);
	return getTextColor(u, v);
}