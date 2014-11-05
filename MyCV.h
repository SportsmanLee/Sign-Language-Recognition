#pragma once

#include <vector>
#include "include\opencv2\core\core.hpp"
#include "include\opencv2\highgui\highgui.hpp"
#include "include\opencv2\imgproc\imgproc.hpp"
#include "include\opencv2\features2d\features2d.hpp"
#include "include\opencv2\nonfree\features2d.hpp"
#include "include\opencv2\nonfree\nonfree.hpp"

using namespace std;
using namespace cv;

class MyCV
{
public:
	MyCV();
	System::Drawing::Bitmap^ getBitmap();
	void calHistogram(int histSize, const float* histRange);
	void detectSIFT();
	void readImage(std::string fileName);
	Mat getImage();

private:
	Mat cvImage;
	vector<double> histVector;
	vector<double> siftVector;
};