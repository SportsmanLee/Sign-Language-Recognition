#include "stdafx.h"
#include <iostream>
#include "MyCV.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

MyCV::MyCV()
{
	histVector.clear();
	siftVector.clear();
}

System::Drawing::Bitmap^ MyCV::getBitmap()
{
	if (cvImage.type() != CV_8UC3)
	{
		throw gcnew NotSupportedException("Only images of type CV_8UC3 are supported for conversion to Bitmap");
	}

	//create the bitmap and get the pointer to the data
	System::Drawing::Imaging::PixelFormat fmt(System::Drawing::Imaging::PixelFormat::Format24bppRgb);
	Bitmap^ bmpImage = gcnew Bitmap(cvImage.cols, cvImage.rows, fmt);

	System::Drawing::Imaging::BitmapData ^data = bmpImage->LockBits(System::Drawing::Rectangle(0, 0, cvImage.cols, cvImage.rows), System::Drawing::Imaging::ImageLockMode::WriteOnly, fmt);

	unsigned char* dstData = reinterpret_cast<unsigned char*>(data->Scan0.ToPointer());

	unsigned char* srcData = cvImage.data;

	for (int row = 0; row < data->Height; ++row)
	{
		memcpy(reinterpret_cast<void*>(&dstData[row * data->Stride]), reinterpret_cast<void*>(&srcData[row * cvImage.step]), cvImage.cols * cvImage.channels());
	}

	bmpImage->UnlockBits(data);

	return bmpImage;
}

void MyCV::calHistogram(int histSize, const float* histRange)
{
	Mat grayImage;	cvtColor(cvImage, grayImage, CV_RGB2GRAY);

	Mat hist, n_hist;
	/// Compute the histogram
	calcHist(&grayImage, 1, 0, Mat(), hist, 1, &histSize, &histRange);

	// Record the histogram values
	histVector.clear();

	for (int i = 0; i < hist.rows; ++i) {
		histVector.push_back((double)hist.at<float>(i));
	}

	// Draw the histogram for intensity
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound( (double) hist_w/histSize );

	Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

	/// Normalize the result to [ 0, histImage.rows ]
	normalize(hist, n_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

	/// Draw
	for(int i = 1; i < histSize; ++i) {
		line( histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(n_hist.at<float>(i-1)) ) ,
						cv::Point( bin_w*(i), hist_h - cvRound(n_hist.at<float>(i)) ),
						Scalar( 255, 255, 255), 2, 8, 0  );
	}

	/// Display
	if (histVector.size() == 16) {
		imshow("Histogram", histImage );
		waitKey(0);
	}
}

void MyCV::detectSIFT()
{
	Mat grayImage;	cvtColor(cvImage, grayImage, CV_RGB2GRAY);

	// Detect the keypoints using SIFT Detector
	SiftFeatureDetector detector(50, 3, 0.04, 10, 1.6f);
	vector<KeyPoint> keypoints;

	detector.detect(grayImage, keypoints);

	// Draw keypoints
	Mat keypointsImg;
	drawKeypoints( cvImage, keypoints, keypointsImg, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
	cv::putText(keypointsImg, std::to_string(keypoints.size()), cv::Point(100, 100), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0));

	for (unsigned int i = 0; i < 50; ++i) {
		siftVector.push_back((double)keypoints[i].pt.x);
		siftVector.push_back((double)keypoints[i].pt.y);
	}

	imshow("SIFT Features", keypointsImg );
	waitKey(0);
}

void MyCV::readImage(std::string fileName)
{
	cvImage = imread(fileName, CV_LOAD_IMAGE_COLOR);
}

Mat MyCV::getImage()
{
	return cvImage;
}