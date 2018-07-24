#ifndef PIC
#define PIC

#include "vec.h"
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

class pic
{
	int H, W;
	Vec3** data;
	string filename;
public:
	pic(string f);
	pic(int h, int w): H(h), W(w), data(NULL) {}
	~pic();
	void outputFromColor(Vec3** data);
	Vec3 inputFromPic(int i, int j);
	int getH() { return H; }
	int getW() { return W; }
};

#endif