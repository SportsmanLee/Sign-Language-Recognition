#pragma once

#include <vector>
#include "include\opencv2\core\core.hpp"
#include "include\opencv2\highgui\highgui.hpp"
#include "include\opencv2\imgproc\imgproc.hpp"
#include "include\opencv2\features2d\features2d.hpp"
#include "include\opencv2\nonfree\features2d.hpp"
#include "include\opencv2\nonfree\nonfree.hpp"
#include "include\opencv2\ml\ml.hpp"
#include "include\opencv\cv.h"

using namespace std;
using namespace cv;

class MyCV
{
public:
	MyCV();
	System::Drawing::Bitmap^ getBitmap();
	System::Drawing::Bitmap^ getOtherBitmap(cv::Mat cvImage);
	void RGBtoYCbCr(IplImage *img);
	void Skin_Color_Detection(IplImage *img);
	void HuMoment();
	void calHistogram(int histSize, const float* histRange);
	void detectSIFT();
	void readImage(std::string fileName);
	Mat getImage();
	vector<std::string> getFiles();
	vector<float> getHuVector();
	vector<float> getHistVector();
	vector<float> getSiftVector();
	void setFiles(std::string filepath);
	void clear();
	void clearFiles();

private:
	Mat cvImage;
	vector<std::string> files;
	CvHuMoments HuMoments; 
	vector<float> huVector;
	vector<float> histVector;
	vector<float> siftVector;
};