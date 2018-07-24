#include "master.h"
#include "sphere.h"
#include "plane.h"
#include "pedestal.h"

int main()
{
	srand(19980407);
	cout<<"start"<<endl;
	pic* pic1 = new pic(".\\text.png");
	pic* pic2 = new pic(".\\ped.png");
	camera* cam = new camera(Vec3(0,-4,0), Vec3(0,1,0), Vec3(1,0,0), Vec3(0,0,1), 1000, 1000);
	light* li = new pointlight(Vec3(320, 320, 320), Vec3(0, 1.5, 1.8));
	item* sphere1 = new sphere(Vec3(-1, 3, -1.099), 1.3);
	sphere1->setmate(Vec3(1, 1, 1), Vec3(), 1, 0, 0, 0);
	item* sphere2 = new sphere(Vec3(1.0, 0.8, -1.3), 1.0);
	sphere2->setmate(Vec3(), Vec3(), 0, 1, 0, 1.7);
	item* ped = new pedestal(Vec3(-1, 3, -3.5), 0.6);
	ped->setmate(Vec3(1,1,1), Vec3(), 0, 0, 1, 0, pic2);
	item* pla1 = new plane(Vec3(0,0,1), -4, Vec3(8, 0, 0), Vec3(0, 8, 0));
	pla1->setmate(Vec3(1,1,1), Vec3(), 0, 0, 1, 0);
	item* pla2 = new plane(Vec3(0,0,-1), -5, Vec3(8, 0, 0), Vec3(0, 8, 0));
	pla2->setmate(Vec3(0.7,0.7,0.7), Vec3(), 0, 0, 1, 0);
	item* pla3 = new plane(Vec3(1,0,0), -3.5, Vec3(0, 8, 0), Vec3(0, 0, 8));
	pla3->setmate(Vec3(0.7,0.2,0.2), Vec3(), 0, 0, 1, 0);
	item* pla4 = new plane(Vec3(-1,0,0), -3.5, Vec3(0, 8, 0), Vec3(0, 0, 8));
	pla4->setmate(Vec3(0.2,0.2,0.7), Vec3(), 0, 0, 1, 0);
	item* pla5 = new plane(Vec3(0,-1,0), -6, Vec3(3.7, 0, 0), Vec3(0, 0, -5.1));
	pla5->setmate(Vec3(1,1,1), Vec3(), 0, 0, 1, 0, pic1);
	cout<<"set item finish"<<endl;
	scene* sce = new scene(8, 1); 
	cout<<"new scene finish"<<endl;
	sce->setcamera(cam);
	sce->setlight(0, li);
	sce->setitem(6, sphere1);
	sce->setitem(7, sphere2);
	sce->setitem(5, ped);
	sce->setitem(0, pla1);
	sce->setitem(1, pla2);
	sce->setitem(2, pla3);
	sce->setitem(3, pla4);
	sce->setitem(4, pla5);
	master* ma = new master();
	ma->setScene(sce);
	ma->run();
	ma->output();
	return 0;
}