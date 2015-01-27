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
	int avg_cb = 120;  //YCbCr顏色空間膚色cb的平均值
	int avg_cr = 155;  //YCbCr顏色空間膚色cr的平均值
	int skinRange = 18;  //YCbCr顏色空間膚色的範圍
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

	skinImage = resultImg;
	imshow("Skin", skinImage);
	waitKey(10);
}

void MyCV::HuMoment()
{
	Mat skinGray = skinImage.clone();

	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7));
	cv::morphologyEx(skinGray, skinGray, cv::MORPH_DILATE, element);

    Mat canny_output;
    vector<vector<cv::Point> > contours;
    vector<Vec4i> hierarchy;

    // Detect edges using canny
    //Canny( skinImage, canny_output, thresh, thresh*2, 3 );
    /// Find contours
    findContours( skinGray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

	/*
    /// Draw contours
    Mat drawing = Mat::zeros( skinImage.size(), CV_8UC3 );
    for( int i = 0; i< contours.size(); i++ )
    {
		Scalar color = Scalar( 255,0,0);
		drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, cv::Point() );
    }
	*/
	/// Get the moments
	vector<Moments> mu(contours.size() );
    mu[0] = moments( contours[0], false ); 

	/* //計算質心
	vector<Point2f> mc( contours.size() );
	for( int i = 0; i < contours.size(); i++ )
      mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
	*/

	cv::Moments mom = cv::moments(contours[0]); 
	double hu[7];
	cv::HuMoments(mom, hu);
	
	huVector.push_back((float)hu[1]);
	huVector.push_back((float)hu[2]);
	huVector.push_back((float)hu[3]);
	huVector.push_back((float)hu[4]);
	huVector.push_back((float)hu[5]);
	huVector.push_back((float)hu[6]);
	huVector.push_back((float)hu[7]);

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
	SiftFeatureDetector detector(30, 3, 0.04, 10.0, 1.6f);
	SiftDescriptorExtractor extractor(128, 3, 0.04, 10.0, 1.6f);
	vector<KeyPoint> keypoints;
	
	Mat skinColor = cvImage.clone();
	/*Mat skinColor(skinImage.size(), CV_8UC3, Scalar(0, 0, 0));

	int nRows = skinImage.rows;
    int nCols = skinImage.cols * skinImage.channels();
    if (skinImage.isContinuous()) {
        nCols *= nRows;
        nRows = 1;
    }
	
	// Reserve skin color pixels with a 3-channels Mat
	uchar* skinPtr;
	Vec3b* dstPtr, * originPtr;
	for (int y = 0; y < nRows; ++y)
	{
		skinPtr = skinImage.ptr<uchar>(y);
		dstPtr = skinColor.ptr<Vec3b>(y);
		originPtr = cvImage.ptr<Vec3b>(y);
		for (int x = 0; x < nCols; ++x)
		{
			if (skinPtr[x] > 10)
				dstPtr[x] = originPtr[x];
			else
				dstPtr[x] = Vec3b(0, 0, 0);
		}
	}*/

	// Detect SIFT keypoints & make sure the length of 30
	detector.detect(skinColor, keypoints);
	if (keypoints.empty()) {
		// Debug
		imshow("SIFT Empty!", skinColor);
		waitKey();
	}
	while (keypoints.size() < 30) {
		keypoints.push_back(keypoints[keypoints.size() - 1]);
	}
	while (keypoints.size() > 30) {
		keypoints.pop_back();
	}
	
	// Extract the SIFT Descriptor
	extractor.compute(skinColor, keypoints, siftDescriptor);

	// Draw keypoints
	/*Mat keypointsImg;
	drawKeypoints( skinImage, keypoints, keypointsImg, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
	cv::putText(keypointsImg, std::to_string(keypoints.size()), cv::Point(100, 100), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0));
	cv::putText(keypointsImg, std::to_string(siftDescriptor.rows), cv::Point(150, 100), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 0));
	cv::putText(keypointsImg, std::to_string(siftDescriptor.cols), cv::Point(200, 100), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255));
	imshow("keypoints", keypointsImg);
	waitKey();*/
}

void MyCV::extractBOW()
{
	// Detect the keypoints using SIFT Detector
	SiftFeatureDetector detector(30, 3, 0.04, 10.0, 1.6f);
	SiftDescriptorExtractor extractor(128, 3, 0.04, 10.0, 1.6f);
	vector<KeyPoint> keypoints;

	Mat skinColor = cvImage.clone();
	/*Mat skinColor(skinImage.size(), CV_8UC3, Scalar(0, 0, 0));

	int nRows = skinImage.rows;
    int nCols = skinImage.cols * skinImage.channels();
    if (skinImage.isContinuous()) {
        nCols *= nRows;
        nRows = 1;
    }

	// Reserve skin color pixels with a 3-channels Mat
	uchar* skinPtr;
	Vec3b* dstPtr, * originPtr;
	for (int y = 0; y < nRows; ++y) {
		skinPtr = skinImage.ptr<uchar>(y);
		dstPtr = skinColor.ptr<Vec3b>(y);
		originPtr = cvImage.ptr<Vec3b>(y);
		for (int x = 0; x < nCols; ++x)	{
			if (skinPtr[x] > 10)
				dstPtr[x] = originPtr[x];
			else
				dstPtr[x] = Vec3b(0, 0, 0);
		}
	}*/

	// Detect SIFT keypoints & make sure the length of 30
	detector.detect(skinColor, keypoints);
	if (keypoints.empty()) {
		// Debug
		imshow("SIFT Empty!", skinColor);
		waitKey();
	}
	while (keypoints.size() < 30) {
		keypoints.push_back(keypoints[keypoints.size() - 1]);
	}
	while (keypoints.size() > 30) {
		keypoints.pop_back();
	}

	// Extract the Bag-Of-Word
	bowExtractor->compute(skinColor, keypoints, siftDescriptor);

	// Fill in the feature vector
	for (int i = 0; i < siftDescriptor.rows; ++i) {
		for (int j = 0; j < siftDescriptor.cols; ++j) {
			siftVector.push_back(siftDescriptor.ptr<float>(i)[j]);
		}
	}
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
						checkList.push_back(cv::Point(n, m));
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
	cv::normalize(regionMap, regionMap, 0, 255, CV_MINMAX);
	Mat debug; regionMap.convertTo(debug, CV_8UC1);
	imshow("regionMap", debug);
	waitKey(10);
}

void MyCV::img_preproc()
{
	/*================================================
	Use the third frame as the background frame(BG), in case the frames are blurred at the beginning
	
	Do Frame-Difference using frame(now) and BG
	Convert the result into gray image(FD)
	Do thresholding to FD, if the pixel value > threshold, fill in the original pixel value
	Do skin-color detection to FD
	Then do morphological operation to denoise
	Thresholding again, and fill in original pixel value to get a clear image
	================================================*/

	// Skin Detection
	int avg_cb = 120;  //YCbCr顏色空間膚色cb的平均值
	int avg_cr = 155;  //YCbCr顏色空間膚色cr的平均值
	int skinRange = 18;  //YCbCr顏色空間膚色的範圍
	
	Mat FD,skinMask;;
	Mat filtered_image(first_frame.size(), CV_8UC3, Scalar(0, 0, 0));

	absdiff(cvImage,first_frame,filtered_image);
		
	cvtColor(filtered_image,FD,CV_RGB2GRAY);
	for (int x = 0; x < FD.rows; ++x)
	{
		for (int y = 0; y < FD.cols; ++y)
		{
			if(FD.at<uchar>(x,y) > 20)
				filtered_image.at<Vec3b>(x, y) = cvImage.at<Vec3b>(x, y);
			else
				filtered_image.at<Vec3b>(x, y) = Vec3b(0,0,0);
		}
	}
	//imshow("filtered_image", filtered_image);
		
	Mat YImage, skinImage(cvImage.size(), CV_8UC3, Scalar(0, 0, 0));
	cvtColor(filtered_image, YImage, CV_BGR2YCrCb);
	for (int x = 0; x < cvImage.rows; ++x)
	{
		for (int y = 0; y < cvImage.cols; ++y)
		{
			int Cr = YImage.at<Vec3b>(x, y).val[1], Cb = YImage.at<Vec3b>(x, y).val[2];

			if((Cb > avg_cb-skinRange && Cb < avg_cb+skinRange) && (Cr > avg_cr-skinRange && Cr < avg_cr+skinRange))
				skinImage.at<Vec3b>(x, y) = cvImage.at<Vec3b>(x, y);
			else
				skinImage.at<Vec3b>(x, y) = Vec3b(0, 0, 0);
		}
	}
	/*
	//get the frame number and write it on the current frame
    rectangle(first_frame, cv::Point(10, 2), cv::Point(100,20),cv::Scalar(255,255,255), -1);
	std::string frameNumberString = std::to_string(fn++);
    putText(first_frame, frameNumberString.c_str(), cv::Point(15, 15),FONT_HERSHEY_SIMPLEX, 0.5 , cv::Scalar(0,0,0));
	*/

	//morphological operation
	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7));
	cv::morphologyEx(skinImage, skinImage, cv::MORPH_ELLIPSE, element);
	cv::morphologyEx(skinImage, skinImage, cv::MORPH_DILATE, element);

	cvtColor(skinImage, skinMask, CV_RGB2GRAY);

	for (int x = 0; x < skinImage.rows; ++x)
	{
		for (int y = 0; y < skinImage.cols; ++y)
		{
			if(skinMask.at<uchar>(x,y) > 0)
				skinImage.at<Vec3b>(x, y) = cvImage.at<Vec3b>(x, y);
			else
				skinImage.at<Vec3b>(x, y) = Vec3b(0,0,0);
		}
	}
	cvImage = skinImage;
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
	cv::resize(cvImage, cvImage, cv::Size(960, 540));
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