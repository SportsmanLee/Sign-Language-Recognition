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
	void detectSkin();
	void HuMoment();
	void calHistogram(int histSize, const float* histRange);
	void detectSIFT();
	void extractBOW();
	void regionGrowing(int x, int y, int regionLabel);
	vector<cv::Point> contourGrowing(cv::Point seedPoint, Mat& borderMap);
	vector<double> calcCurvature(vector<cv::Point>& contour);
	void setROI(int regionLabel);
	void regionCut();
	void readImage(std::string fileName);
	void readFrame(Mat frame);
	void img_preproc();
	void set_bg_frame();
	void setBOWExtractor(Mat vocabulary);
	Mat getImage();
	Mat getSkinImage();
	Mat getRegionMap();
	vector<float> getHuVector();
	vector<float> getHistVector();
	vector<float> getSiftVector();
	Mat getSiftDescriptor();
	void clear();

private:
	Mat cvImage;
	Mat skinImage;
	Mat first_frame;
	Mat siftDescriptor;
	Mat regionMap;
	vector<float> huVector;
	vector<float> histVector;
	vector<float> siftVector;
	Ptr<BOWImgDescriptorExtractor> bowExtractor;
};