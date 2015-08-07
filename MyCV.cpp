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
	clear();
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

System::Drawing::Bitmap^ MyCV::getOtherBitmap(Mat Image)
{
	if (Image.type() != CV_8UC3)
	{
		throw gcnew NotSupportedException("Only images of type CV_8UC3 are supported for conversion to Bitmap");
	}

	//create the bitmap and get the pointer to the data
	System::Drawing::Imaging::PixelFormat fmt(System::Drawing::Imaging::PixelFormat::Format24bppRgb);
	Bitmap^ bmpImage = gcnew Bitmap(Image.cols, Image.rows, fmt);

	System::Drawing::Imaging::BitmapData ^data = bmpImage->LockBits(System::Drawing::Rectangle(0, 0, Image.cols, Image.rows), System::Drawing::Imaging::ImageLockMode::WriteOnly, fmt);

	unsigned char* dstData = reinterpret_cast<unsigned char*>(data->Scan0.ToPointer());

	unsigned char* srcData = Image.data;

	for (int row = 0; row < data->Height; ++row)
	{
		memcpy(reinterpret_cast<void*>(&dstData[row * data->Stride]), reinterpret_cast<void*>(&srcData[row * Image.step]), Image.cols * Image.channels());
	}

	bmpImage->UnlockBits(data);

	return bmpImage;
}

void MyCV::detectSkin()
{
	// Skin Detection
	int avg_cb = 110;  //YCbCr顏色空間膚色cb的平均值
	int avg_cr = 155;  //YCbCr顏色空間膚色cr的平均值
	int skinRange = 26;  //YCbCr顏色空間膚色的範圍
	Mat resultImg = Mat::zeros(cvImage.size(), CV_8UC1);

	Mat YCrCbImage;
	vector<Mat> YCrCbMV;
	cvtColor(cvImage, YCrCbImage, CV_BGR2YCrCb);
	split(YCrCbImage, YCrCbMV);

	int nRows = YCrCbMV[1].rows;
    int nCols = YCrCbMV[1].cols * YCrCbMV[1].channels();
    if (YCrCbMV[1].isContinuous()) {
        nCols *= nRows;
        nRows = 1;
    }

	uchar* CrPtr, * CbPtr;
	uchar* resultPtr;
	for (int y = 0; y < nRows; ++y)
	{
		CrPtr = YCrCbMV[1].ptr<uchar>(y);	CbPtr = YCrCbMV[2].ptr<uchar>(y);
		resultPtr = resultImg.ptr<uchar>(y);
		for (int x = 0; x < nCols; ++x)
		{
			int Cr = CrPtr[x], Cb = CbPtr[x];
			if((Cb > avg_cb-skinRange && Cb < avg_cb+skinRange) && (Cr > avg_cr-skinRange && Cr < avg_cr+skinRange))
				resultPtr[x] = 255;
			else
				resultPtr[x] = 0;
		}
	}
	/*Mat resultImg = Mat::zeros(skinImage.size(), CV_8UC1);

	for (int i = 0; i < skinImage.rows; ++i) {
		for (int j = 0; j < skinImage.cols; ++j) {
			if (skinImage.ptr<uchar>(i)[j] > 5)
				resultImg.ptr<uchar>(i)[j] = 255;
			else
				resultImg.ptr<uchar>(i)[j] = 0;
		}
	}*/

	skinImage = resultImg;
	imshow("Skin", skinImage);
	waitKey(10);
}

void MyCV::HuMoment()
{
	Mat skinGray = skinImage.clone();

	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7));
	cv::morphologyEx(skinGray, skinGray, cv::MORPH_DILATE, element);

    vector<vector<cv::Point> > contours;
    vector<Vec4i> hierarchy;

    // Detect edges using canny
    //Canny( skinImage, canny_output, thresh, thresh*2, 3 );
    /// Find contours
    findContours( skinGray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );
    	
	int maxIdx = 0;
	for (size_t i = 0; i < contours.size(); ++i) {
		if (contours[maxIdx].size() < contours[i].size()) {
			maxIdx = i;
		}
	}
	
    /// Draw contours
    Mat drawing = Mat::zeros( skinImage.size(), CV_8UC3 );
	Scalar color = Scalar( 255,0,0);
	drawContours( drawing, contours, maxIdx, color, 2, 8, hierarchy, 0, cv::Point() );
	imshow("contour", drawing);
	waitKey(10);

	cv::Moments mom = cv::moments(contours[maxIdx]); 
	vector<double> hu;
	cv::HuMoments(mom, hu);
	
	for (size_t i = 0; i < hu.size(); ++i) {
		huVector.push_back(static_cast<float>(hu[i]));
	}

	cv::normalize(huVector, huVector, 0, 1, CV_MINMAX);
}

void MyCV::calHistogram(int histSize, const float* histRange)
{
	Mat grayImage;	cvtColor(cvImage, grayImage, CV_RGB2GRAY);

	Mat hist;
	/// Compute the histogram
	calcHist(&grayImage, 1, 0, Mat(), hist, 1, &histSize, &histRange);

	for (int i = 0; i < hist.rows; ++i) {
		histVector.push_back(hist.at<float>(i));
	}

	/// Normalize the result to [ 0, histImage.rows ]
	normalize(histVector, histVector, 0, 1, CV_MINMAX);
}

void MyCV::detectSIFT()
{
	// Detect the keypoints using SIFT Detector
	SiftFeatureDetector detector(50);
	SiftDescriptorExtractor extractor(128);
	vector<KeyPoint> keypoints;
	
	Mat skinColor = cvImage.clone();

	detector.detect(skinColor, keypoints);
	if (keypoints.empty()) {
		// Debug
		imshow("SIFT Empty!", skinColor);
		waitKey();
	}
	while (keypoints.size() < 50) {
		keypoints.push_back(keypoints[keypoints.size() - 1]);
	}
	while (keypoints.size() > 50) {
		keypoints.pop_back();
	}
	
	// Extract the SIFT Descriptor
	extractor.compute(skinColor, keypoints, siftDescriptor);

	// Draw keypoints
	Mat keypointsImg;
	drawKeypoints( skinImage, keypoints, keypointsImg, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
	cv::putText(keypointsImg, std::to_string(keypoints.size()), cv::Point(100, 100), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0));
	cv::putText(keypointsImg, std::to_string(siftDescriptor.rows), cv::Point(150, 100), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 0));
	cv::putText(keypointsImg, std::to_string(siftDescriptor.cols), cv::Point(200, 100), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255));
	imshow("keypoints", keypointsImg);
	waitKey(10);
}

void MyCV::extractBOW()
{
	// Detect the keypoints using SIFT Detector
	SiftFeatureDetector detector(50);
	vector<KeyPoint> keypoints;

	Mat skinColor = cvImage.clone();

	detector.detect(skinColor, keypoints);
	if (keypoints.empty()) {
		// Debug
		imshow("SIFT Empty!", skinColor);
		waitKey();
	}
	while (keypoints.size() < 50) {
		keypoints.push_back(keypoints[keypoints.size() - 1]);
	}
	while (keypoints.size() > 50) {
		keypoints.pop_back();
	}

	// Draw keypoints
	/*Mat keypointsImg;
	drawKeypoints( skinColor, keypoints, keypointsImg, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
	cv::putText(keypointsImg, std::to_string(keypoints.size()), cv::Point(100, 100), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0));
	imshow("keypoints", keypointsImg);
	waitKey(10);*/

	// Extract the Bag-Of-Word
	bowExtractor->compute(skinColor, keypoints, siftDescriptor);

	// Fill in the feature vector
	for (int i = 0; i < siftDescriptor.rows; ++i) {
		for (int j = 0; j < siftDescriptor.cols; ++j) {
			siftVector.push_back(siftDescriptor.ptr<float>(i)[j]);
		}
	}

	normalize(siftVector, siftVector, 0, 1, CV_MINMAX);
}

// Dynamic Programming
void MyCV::regionGrowing(int x, int y, int regionLabel)
{
	vector<cv::Point> checkList;
	checkList.push_back(cv::Point(x, y));
	Mat skinGray = skinImage.clone();	//cvtColor(skinImage, skinGray, CV_BGR2GRAY);

	// candidates growing & set region labels
	while (checkList.size() > 0) {
		cv::Point candidate = checkList.front();
		regionMap.ptr<ushort>(candidate.y)[candidate.x] = regionLabel;

		for (int m = candidate.y - 1; m < candidate.y + 2; ++m) {
				if (m < 1 || m >= skinGray.rows - 1)	continue;
			for (int n = candidate.x - 1; n < candidate.x + 2; ++n) {
				if (n < 1 || n >= skinGray.cols - 1)	continue;
				if (m != candidate.y && n != candidate.x)	continue;

				if (skinGray.ptr<uchar>(m)[n] > 10 && regionMap.ptr<ushort>(m)[n] == 0) {
					//regionGrowing(n, m, regionLabel);	// Seed growing
					cv::Point checkPoint(cv::Point(n, m));
					bool isNew = true;
					for (size_t j = 0; j < checkList.size(); ++j) {
						if (checkPoint.x == checkList[j].x && checkPoint.y == checkList[j].y) {
							isNew = false;
							break;
						}
					}
					if (isNew) {
						checkList.push_back(checkPoint);
					}
				}
			}
		}

		checkList.erase(checkList.begin());
	}
}

void MyCV::setROI(int regionLabel)
{
	int upperBound = -1, lowerBound = -1, leftBound = -1, rightBound = -1;
	int sumX = 0, sumY = 0;
	int pixelCount = 0;
	for (int i = 0; i < regionMap.rows; ++i) {
		for (int j = 0; j < regionMap.cols; ++j) {
			if (regionMap.ptr<ushort>(i)[j] == regionLabel) {
				++pixelCount;
				sumX += j;	sumY += i;
				
				if (upperBound == -1 || lowerBound == -1) {
					upperBound = i;	lowerBound = i;
				}
				if (leftBound == -1 || rightBound == -1) {
					leftBound = j;	rightBound = j;
				}

				if (i > lowerBound) {
					lowerBound = i;
				}
				if (j < leftBound) {
					leftBound = j;
				}
				if (j > rightBound) {
					rightBound = j;
				}
			}
		}
	}
	// expand the bounding box slightly
	leftBound = (leftBound - 3) < 0 ? 0 : (leftBound - 3);
	upperBound = (upperBound - 3) < 0 ? 0 : (upperBound - 3);
	rightBound = (rightBound + 3) >= regionMap.cols ? (regionMap.cols - 1) : (rightBound + 3);
	lowerBound = (lowerBound + 3) >= regionMap.rows ? (regionMap.rows - 1) : (lowerBound + 3);

	cv::Point tl(leftBound, upperBound), br(rightBound, lowerBound);
	Mat imageROI = cvImage(Rect(tl, br)).clone();

	// Reserve skin color pixels which have specific region label
	for (int i = 0; i < imageROI.rows; ++i) {
		for (int j = 0; j < imageROI.cols; ++j) {
			if (regionMap.ptr<ushort>(tl.y + i)[tl.x + j] != regionLabel)
				imageROI.ptr<Vec3b>(i)[j] = Vec3b(0, 0, 0);
		}
	}

	// Applying Gaussian blur
	GaussianBlur( imageROI, imageROI, cv::Size(3, 3), 0, 0);

	cvImage.release();	cvImage = imageROI;
	detectSkin();
}

void MyCV::regionCut()
{
	Mat skinGray = skinImage.clone();
	regionMap = Mat::zeros(skinGray.size(), CV_16U);
	// Label Regions
	int labelCount = 1, step = 8;
	for (int i = step; i < skinGray.rows - 1; i += step) {
		for (int j = step; j < skinGray.cols - 1; j += step) {
			if (skinGray.ptr<uchar>(i)[j] > 10 && regionMap.ptr<ushort>(i)[j] == 0) {
				regionGrowing(j, i, labelCount);	// Put a seed to grow
				++labelCount;
			}
		}
	}

	int regionCount[256] = {0}, maxRegionIdx = 0, maxRegionNum = 0;
	for (int i = 1; i < regionMap.rows - 1; ++i) {
		for (int j = 1; j < regionMap.cols - 1; ++j) {
			++regionCount[regionMap.ptr<ushort>(i)[j]];
		}
	}
	for (int i = 1; i < 256; ++i) {
		if (maxRegionNum < regionCount[i]) {
			maxRegionIdx = i;
			maxRegionNum = regionCount[i];
		}
	}

	setROI(maxRegionIdx);
	// Debug
	/*cv::normalize(regionMap, regionMap, 0, 255, CV_MINMAX);
	Mat debug; regionMap.convertTo(debug, CV_8UC1);
	imshow("regionMap", debug);
	waitKey(10);*/
	// Reserve skin color pixels which have specific region label
	/*for (int i = 0; i < cvImage.rows; ++i) {
		for (int j = 0; j < cvImage.cols; ++j) {
			if (regionMap.ptr<ushort>(i)[j] != maxRegionIdx)
				cvImage.ptr<Vec3b>(i)[j] = Vec3b(0, 0, 0);
		}
	}*/

	// Applying Gaussian blur
	/*GaussianBlur( cvImage, cvImage, cv::Size(3, 3), 0, 0);
	detectSkin();*/
}

void MyCV::setBOWExtractor(Mat vocabulary)
{
	Ptr<DescriptorExtractor> descExtractor = DescriptorExtractor::create("SIFT"); //引號裡面修改特徵種類。  
	Ptr<DescriptorMatcher> descMatcher = DescriptorMatcher::create("BruteForce"); //引號裡面修改匹配類型;  
	bowExtractor = new BOWImgDescriptorExtractor(descExtractor, descMatcher);

	bowExtractor->setVocabulary(vocabulary);
}

void MyCV::readImage(std::string fileName)
{
	cvImage = imread(fileName, CV_LOAD_IMAGE_COLOR);
	//cv::resize(cvImage, cvImage, cv::Size(960, 540));
	if (cvImage.rows >= cvImage.cols) {
		cv::resize(cvImage, cvImage, cv::Size(cvRound(cvImage.cols * 960 / cvImage.rows), 960));
	}
	else {
		cv::resize(cvImage, cvImage, cv::Size(960, cvRound(cvImage.rows * 960 / cvImage.cols)));
	}
	cvtColor(cvImage, skinImage, CV_BGR2GRAY);
}

void MyCV::readFrame(Mat frame)
{
	cvImage = frame.clone();
	cv::resize(cvImage, cvImage, cv::Size(960, 540));
}

Mat MyCV::getImage()
{
	return cvImage;
}

Mat MyCV::getSkinImage()
{
	return skinImage;
}

Mat MyCV::getRegionMap()
{
	return regionMap;
}

vector<float> MyCV::getHistVector()
{
	return histVector;
}

vector<float> MyCV::getHuVector()
{
	return huVector;
}

vector<float> MyCV::getSiftVector()
{
	return siftVector;
}

Mat MyCV::getSiftDescriptor()
{
	return siftDescriptor;
}

void MyCV::clear()
{
	cvImage.release();
	skinImage.release();
	first_frame.release();
	siftDescriptor.release();
	regionMap.release();
	histVector.clear();
	huVector.clear();
	siftVector.clear();
}

void MyCV::set_bg_frame()
{
	first_frame = cvImage.clone();
}