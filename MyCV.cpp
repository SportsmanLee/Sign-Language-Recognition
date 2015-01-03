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

void MyCV::HuMoment()
{
	int thresh = 100;
	// Skin Detection
	int avg_cb = 120;  //YCbCr顏色空間膚色cb的平均值
	int avg_cr = 155;  //YCbCr顏色空間膚色cr的平均值
	int skinRange = 18;  //YCbCr顏色空間膚色的範圍

	Mat dst = Mat::zeros(cvImage.rows, cvImage.cols, CV_8UC3);

	Mat YImage, skinImage(cvImage.size(), CV_8UC3, Scalar(0, 0, 0));
	cvtColor(cvImage, YImage, CV_BGR2YCrCb);
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
	cvtColor(skinImage,skinImage,CV_RGB2GRAY);

	for (int x = 0; x < cvImage.rows; ++x)
	{
		for (int y = 0; y < cvImage.cols; ++y)
		{
			if( skinImage.at<uchar>(x, y) > 0)
				skinImage.at<uchar>(x, y) = 255;
			else
				skinImage.at<uchar>(x, y) = 0;
		}
	}

	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7));
	cv::morphologyEx(skinImage, skinImage, cv::MORPH_DILATE, element);

    Mat canny_output;
    vector<vector<cv::Point> > contours;
    vector<Vec4i> hierarchy;

    // Detect edges using canny
    //Canny( skinImage, canny_output, thresh, thresh*2, 3 );
    /// Find contours
    findContours( skinImage, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

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

	CvHuMoments Hu;
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

void MyCV::detectSIFT()
{
	// Detect the keypoints using SIFT Detector
	SiftFeatureDetector detector(30, 3, 0.04, 10.0, 1.6f);
	SiftDescriptorExtractor extractor(128, 3, 0.04, 10.0, 1.6f);
	vector<KeyPoint> keypoints;

	Mat YImage;
	cvtColor(cvImage, YImage, CV_BGR2YCrCb);
	Mat skinImage = cvImage.clone();
	skinImage.create(cvImage.size(), CV_8UC3);

	// Skin Detection
	int avg_cb = 120;  //YCbCr顏色空間膚色cb的平均值
	int avg_cr = 155;  //YCbCr顏色空間膚色cr的平均值
	int skinRange = 18;  //YCbCr顏色空間膚色的範圍
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
	
	// Detect SIFT keypoints & make sure the length of 30
	detector.detect(skinImage, keypoints);
	if (keypoints.empty()) {
		while (keypoints.size() < 30) {
			keypoints.push_back(KeyPoint());
		}
	}
	while (keypoints.size() < 30) {
		keypoints.push_back(keypoints[keypoints.size() - 1]);
	}
	while (keypoints.size() > 30) {
		keypoints.pop_back();
	}
	
	// Extract the SIFT Descriptor
	extractor.compute(skinImage, keypoints, siftDescriptor);

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

	Mat YImage;
	cvtColor(cvImage, YImage, CV_BGR2YCrCb);
	Mat skinImage = cvImage.clone();

	// Skin Detection
	int avg_cb = 120;  //YCbCr顏色空間膚色cb的平均值
	int avg_cr = 155;  //YCbCr顏色空間膚色cr的平均值
	int skinRange = 18;  //YCbCr顏色空間膚色的範圍
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

	// Detect SIFT keypoints & make sure the length of 30
	detector.detect(skinImage, keypoints);
	if (keypoints.empty()) {
		while (keypoints.size() < 30) {
			keypoints.push_back(KeyPoint());
		}
	}
	while (keypoints.size() < 30) {
		keypoints.push_back(keypoints[keypoints.size() - 1]);
	}
	while (keypoints.size() > 30) {
		keypoints.pop_back();
	}

	// Extract the Bag-Of-Word
	bowExtractor->compute(skinImage, keypoints, siftDescriptor);

	// Fill in the feature vector
	for (int i = 0; i < siftDescriptor.rows; ++i) {
		for (int j = 0; j < siftDescriptor.cols; ++j) {
			siftVector.push_back(siftDescriptor.ptr<float>(i)[j]);
		}
	}
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
	int skinRange = 16;  //YCbCr顏色空間膚色的範圍
	
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

void MyCV::normalize()
{
	float min = 0, max = 0;
	float offset, d;

	// *****************7Hu Moments*****************
	for (unsigned int i = 0; i < huVector.size(); ++i) {
		if (min > huVector[i]) {
			min = huVector[i];
		}
		if (max < huVector[i]) {
			max = huVector[i];
		}
	}

	offset = -min;

	for (unsigned int i = 0; i < huVector.size(); ++i) {
		huVector[i] += offset;
	}
	
	d = max - min;

	for (unsigned int i = 0; i < huVector.size(); ++i) {
		huVector[i] /= d;
	}
	// *****************End*****************

	// SIFT feature vector is already normalized.
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
	cv::resize(cvImage, cvImage, cv::Size(cvRound(cvImage.cols / 2.0), cvRound(cvImage.rows / 2.0)));
}

void MyCV::readFrame(Mat frame)
{
	cvImage = frame.clone();
	cv::resize(cvImage, cvImage, cv::Size(cvRound(cvImage.cols / 2.0), cvRound(cvImage.rows / 2.0)));
}

Mat MyCV::getImage()
{
	return cvImage;
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
	first_frame.release();
	siftDescriptor.release();
	huVector.clear();
	siftVector.clear();
}

void MyCV::set_bg_frame()
{
	first_frame = cvImage.clone();
}