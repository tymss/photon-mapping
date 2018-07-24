#include "item.h"
#include <iostream>

material::material()
{
	color = Vec3();
	absorb = Vec3();
	r = reflex = refrac = diff;
	texture = NULL;
}

material::~material()
{
	if(texture != NULL)
		delete texture;
}

item::item()
{
	mate = new material;
}

item::~item()
{
	delete mate;
}

void item::setmate(Vec3 c, Vec3 ab, double rl, double rf, double dif, double _r, pic* text)
{
	mate->color = c;
	mate->absorb = ab;
	mate->reflex = rl;
	mate->refrac = rf;
	mate->diff = dif;
	mate->r = _r;
	mate->texture = text;
}

Vec3 item::getTextColor(double u, double v)
{
	if(u < 0.) u = 0.;
	if(u > 1.) u = 1.;
	if(v < 0.) v = 0.;
	if(v > 1.) v = 1.;
	double w = int(u * (mate->texture->getW() - 1));
	double h = int(v * (mate->texture->getH() - 1));
	double delta_w = w - double(int(w));
	double delta_h = h - double(int(h));
	int w1 = int(w), h1 = int(h);
	int w2 = w1 + 1, h2 = h1 + 1;
	Vec3 result;
	result = result + (1 - delta_w) * (1 - delta_h) * mate->texture->inputFromPic(h1, w1);
	if(h2 < mate->texture->getH()) result = result + (1 - delta_w) * delta_h * mate->texture->inputFromPic(h2, w1);
	if(w2 < mate->texture->getW()) result = result + (1 - delta_h) * delta_w * mate->texture->inputFromPic(h1, w2);
	if(w2 < mate->texture->getW() && h2 < mate->texture->getH()) 
		result = result + delta_h * delta_w * mate->texture->inputFromPic(h2, w2);
	return result;
}