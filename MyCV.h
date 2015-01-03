#pragma once

#include <vector>
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\features2d\features2d.hpp"
#include "opencv2\nonfree\features2d.hpp"
#include "opencv2\opencv_modules.hpp"
#include "opencv2\nonfree\nonfree.hpp"
#include "opencv2\ml\ml.hpp"
#include "opencv\cv.h"

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
	void detectSIFT();
	void extractBOW();
	void readImage(std::string fileName);
	void readFrame(Mat frame);
	void img_preproc();
	void set_bg_frame();
	void setBOWExtractor(Mat vocabulary);
	Mat getImage();
	vector<float> getHuVector();
	vector<float> getSiftVector();
	Mat getSiftDescriptor();
	void clear();

private:
	Mat cvImage;
	Mat first_frame;
	Mat siftDescriptor;
	vector<float> huVector;
	vector<float> siftVector;
	Ptr<BOWImgDescriptorExtractor> bowExtractor;
};