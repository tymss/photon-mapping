#include "pic.h"

pic::pic(string f): filename(f)
{
	Mat text = imread(filename);
	if(text.empty())
		cout<<"failed to read pic"<<endl;
	else
		cout<<"read success"<<endl;
	Mat img;
	text.convertTo(img, CV_64FC3, 1./255.);
	H = img.rows;
	W = img.cols;
	cout<<"H="<<H<<"  W="<<W<<endl;
	data = new Vec3*[H];
	for(int i=0; i<H; i++)
	{
		data[i] = new Vec3[W];
		for(int j=0; j<W; j++)
		{
			data[i][j] = Vec3(img.at<Vec3d>(i, j)[2], img.at<Vec3d>(i, j)[1], img.at<Vec3d>(i, j)[0]);
		}
	}
}

pic::~pic()
{
	if(data != NULL)
	{
		for(int i=0; i<H; i++)
			delete []data[i];
		delete []data;
	}
}

void pic::outputFromColor(Vec3** data)
{
	Mat img(H, W, CV_64FC3, Scalar(0, 0, 0));
	for(int i=0; i<H; i++)
	{	
		for(int j=0; j<W; j++)
		{
			Vec3d p;
			p[2] = data[i][j].x;
			p[1] = data[i][j].y;
			p[0] = data[i][j].z;
			img.at<Vec3d>(i, j) = p;
		}
	}
	Mat src;
	img.convertTo(src, CV_8UC3, 255.0);
	imwrite(".\\result.png", src);
}

Vec3 pic::inputFromPic(int i, int j)
{
	return data[i][j];
}