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
	huVector.clear();
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

void MyCV::RGBtoYCbCr(IplImage *img)
{
	CvScalar scalarImg;
	double cb, cr, y;
	for( int i = 0; i < img->height; i++ )
	for( int j = 0; j < img->width; j++ )
	{
		scalarImg = cvGet2D(img, i, j);   //從影像中取RGB值
		y =  (16 + scalarImg.val[2]*0.257 + scalarImg.val[1]*0.504 + scalarImg.val[0]*0.098);
		cb = (128 - scalarImg.val[2]*0.148 - scalarImg.val[1]*0.291 + scalarImg.val[0]*0.439);
		cr = (128 + scalarImg.val[2]*0.439 - scalarImg.val[1]*0.368 - scalarImg.val[0]*0.071);      
		cvSet2D(img, i, j, cvScalar( y, cr, cb));  //以YCbCr方式畫img
	}
}

void MyCV::Skin_Color_Detection(IplImage *img)
{
	//================
	int avg_cb = 120;  //YCbCr顏色空間膚色cb的平均值
	int avg_cr = 155;  //YCbCr顏色空間膚色cr的平均值
	int skinRange = 22;  //YCbCr顏色空間膚色的範圍
	//================

	CvScalar scalarImg;
	double cb, cr;
	for( int i = 0; i < img->height; i++ )
	{
		for( int j = 0; j < img->width; j++ )
		{
			scalarImg = cvGet2D(img, i, j);
			cr = scalarImg.val[1];
			cb = scalarImg.val[2];
			if((cb > avg_cb-skinRange && cb < avg_cb+skinRange) && (cr > avg_cr-skinRange && cr < avg_cr+skinRange))
				cvSet2D(img, i, j, cvScalar( 255, 255, 255));
			else
				cvSet2D(img, i, j, cvScalar( 0, 0, 0));
		}
	}
}

void MyCV::HuMoment()
{
	IplImage *tmp, *gray_img, *dst, *YCbCr_img; 

	CvMemStorage* storge = cvCreateMemStorage(0);
	CvMemStorage* storge1 = cvCreateMemStorage(0);

	CvSeq* contour = 0;
	CvSeq* cont;
	CvSeq* mcont;

	huVector.clear();

	tmp = &IplImage(cvImage);
		
	//Img = cvCreateImage(img_resize, tmp->depth, tmp->nChannels);
	//YCbCr_img = cvCreateImage(cvGetSize(Img), Img->depth, Img->nChannels);
	//cvResize(tmp, Img, CV_INTER_LINEAR);    //縮放來源影像到目標影像
	// cvSaveImage("00010 004.jpg", img);		 //儲存影像(覆蓋原檔)
	
	YCbCr_img = cvCreateImage(cvGetSize(tmp), tmp->depth, tmp->nChannels);

	//========RGB2YCrCb==========
	cvCopy(tmp, YCbCr_img, NULL);
	RGBtoYCbCr(YCbCr_img);
	//==========================

	//==========skin color detection=========
	Skin_Color_Detection(YCbCr_img);
	//===========YCbCr轉灰階=============
	gray_img = cvCreateImage(cvGetSize(tmp), IPL_DEPTH_8U, 1);
	dst = cvCreateImage(cvGetSize(tmp), tmp->depth, tmp->nChannels);

	cvCvtColor( YCbCr_img, gray_img, CV_RGB2GRAY );
	//===========Find Contours & Draw=============
	cvFindContours(gray_img, storge, &contour, sizeof(CvContour), CV_RETR_EXTERNAL , CV_CHAIN_APPROX_SIMPLE);

	if(contour)
	{
		CvTreeNodeIterator it;
		cvInitTreeNodeIterator(&it, contour, 1);
		while(0 != (cont = (CvSeq*)cvNextTreeNode (&it)))
		{
			mcont = cvApproxPoly(cont, sizeof(CvContour), storge1, CV_POLY_APPROX_DP, cvContourPerimeter(cont)*0.02, 0);
			cvDrawContours(dst, mcont, CV_RGB(0,255,0), CV_RGB(0,0,100), 1, 2, 8, cvPoint(0,0));
		}
	}

	CvMoments Moments;
	cvContourMoments(mcont,&Moments);
	cvGetHuMoments(&Moments, &HuMoments);
	
	huVector.push_back(HuMoments.hu1);
	huVector.push_back(HuMoments.hu2);
	huVector.push_back(HuMoments.hu3);
	huVector.push_back(HuMoments.hu4);
	huVector.push_back(HuMoments.hu5);
	huVector.push_back(HuMoments.hu6);
	huVector.push_back(HuMoments.hu7);
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

	siftVector.clear();

	detector.detect(grayImage, keypoints);

	// Draw keypoints
	Mat keypointsImg;
	drawKeypoints( cvImage, keypoints, keypointsImg, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
	cv::putText(keypointsImg, std::to_string(keypoints.size()), cv::Point(100, 100), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0));

	while (((int)keypoints.size() - 50) > 0) {
		keypoints.pop_back();
	}

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

vector<double> MyCV::getHuVector()
{
	return huVector;
}