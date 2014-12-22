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
	int skinRange = 18;  //YCbCr顏色空間膚色的範圍
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

	CvMemStorage* storage = cvCreateMemStorage(0);
	CvMemStorage* storage1 = cvCreateMemStorage(0);

	CvSeq* contour = 0;
	CvSeq* cont;
	CvSeq* mcont;
	
	tmp = cvCloneImage(&(IplImage)cvImage);
		
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
	Skin_Color_Detection(YCbCr_img);	//===========YCbCr轉灰階=============
	gray_img = cvCreateImage(cvGetSize(tmp), IPL_DEPTH_8U, 1);
	dst = cvCreateImage(cvGetSize(tmp), tmp->depth, tmp->nChannels);

	cvCvtColor( YCbCr_img, gray_img, CV_RGB2GRAY );
	//===========Find Contours & Draw=============
	cvFindContours(gray_img, storage, &contour, sizeof(CvContour), CV_RETR_EXTERNAL , CV_CHAIN_APPROX_SIMPLE);

	if(contour)
	{
		CvTreeNodeIterator it;
		cvInitTreeNodeIterator(&it, contour, 1);
		while(0 != (cont = (CvSeq*)cvNextTreeNode (&it)))
		{
			mcont = cvApproxPoly(cont, sizeof(CvContour), storage1, CV_POLY_APPROX_DP, cvContourPerimeter(cont)*0.02, 0);
			cvDrawContours(dst, mcont, CV_RGB(0,255,0), CV_RGB(0,0,100), 1, 2, 8, cvPoint(0,0));
		}
	}

	CvMoments Moments;
	cvContourMoments(mcont,&Moments);
	cvGetHuMoments(&Moments, &HuMoments);
	
	huVector.push_back((float)HuMoments.hu1);
	huVector.push_back((float)HuMoments.hu2);
	huVector.push_back((float)HuMoments.hu3);
	huVector.push_back((float)HuMoments.hu4);
	huVector.push_back((float)HuMoments.hu5);
	huVector.push_back((float)HuMoments.hu6);
	huVector.push_back((float)HuMoments.hu7);

	cvReleaseImage(&gray_img);
	cvReleaseImage(&dst);
	cvReleaseImage(&YCbCr_img);
	cvReleaseImage(&tmp);
	cvClearMemStorage(storage);
	cvClearMemStorage(storage1);
	cvReleaseMemStorage(&storage);
	cvReleaseMemStorage(&storage1);
	//cvClearSeq(contour);
	//cvClearSeq(cont);
	//cvClearSeq(mcont);
	//cvReleaseMemStorage(&contour->storage);
	//cvReleaseMemStorage(&cont->storage);
	//cvReleaseMemStorage(&mcont->storage);
}

void MyCV::calHistogram(int histSize, const float* histRange)
{
	Mat grayImage;	cvtColor(cvImage, grayImage, CV_RGB2GRAY);

	Mat hist, n_hist;
	/// Compute the histogram
	calcHist(&grayImage, 1, 0, Mat(), hist, 1, &histSize, &histRange);

	for (int i = 0; i < hist.rows; ++i) {
		histVector.push_back(hist.at<float>(i));
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
	/*if (histVector.size() == 16) {
		imshow("Histogram", histImage );
		waitKey(0);
	}*/
}

void MyCV::detectSIFT()
{
	// Detect the keypoints using SIFT Detector
	SiftFeatureDetector detector(30, 3, 0.04, 10.0, 1.6f);
	SiftDescriptorExtractor extractor(128, 3, 0.04, 10.0, 1.6f);
	vector<KeyPoint> keypoints;

	Mat YImage, skinImage(cvImage.size(), CV_8UC3, Scalar(0, 0, 0));
	cvtColor(cvImage, YImage, CV_BGR2YCrCb);

	// Skin Detection
	int avg_cb = 120;  //YCbCr顏色空間膚色cb的平均值
	int avg_cr = 155;  //YCbCr顏色空間膚色cr的平均值
	int skinRange = 22;  //YCbCr顏色空間膚色的範圍
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

	Mat descriptor;
	extractor.compute(skinImage, keypoints, descriptor);

	for (int i = 0; i < descriptor.rows; ++i) {
		for (int j = 0; j < descriptor.cols; ++j) {
			siftVector.push_back(descriptor.at<float>(i, j));
		}
	}
	
	// Draw keypoints
	/*Mat keypointsImg;
	drawKeypoints( skinImage, keypoints, keypointsImg, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
	cv::putText(keypointsImg, std::to_string(keypoints.size()), cv::Point(100, 100), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0));
	cv::putText(keypointsImg, std::to_string(descriptor.rows), cv::Point(150, 100), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 0));
	cv::putText(keypointsImg, std::to_string(descriptor.cols), cv::Point(200, 100), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255));
	imshow("keypoints", keypointsImg);
	waitKey();*/
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

vector<float> MyCV::getHistVector()
{
	return histVector;
}

vector<float> MyCV::getSiftVector()
{
	return siftVector;
}

void MyCV::clear()
{
	histVector.clear();
	huVector.clear();
	siftVector.clear();
	cvImage.release();
}

void MyCV::set_bg_frame()
{
	first_frame = cvImage.clone();
}