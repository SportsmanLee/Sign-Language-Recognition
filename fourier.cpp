// fourier transform main function 
#include "stdafx.h"
#include "fourier.h"
#include <limits> 


using namespace cv;

fourier::fourier(){
	//nothing to do
}


void fourier::readimg(string path){
	img_path = path;
	img = imread(path, CV_LOAD_IMAGE_COLOR);
	img_gray = imread(path, CV_LOAD_IMAGE_GRAYSCALE);
	resize(img,img,cv::Size(img.cols/2,img.rows/2));
	resize(img_gray,img_gray,cv::Size(img_gray.cols/2,img_gray.rows/2));
	//imshow("",img);
	waitKey(0);
	if(img.empty())
		return ;

}

void fourier::RGBtoYCbCr(IplImage *img)
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

void fourier::Skin_Color_Detection(IplImage *img)
{
	//================
	int avg_cb = 110;  //YCbCr顏色空間膚色cb的平均值
	int avg_cr = 155;  //YCbCr顏色空間膚色cr的平均值
	int skinRange = 26;  //YCbCr顏色空間膚色的範圍
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

void fourier::boundary(){
	IplImage *tmp, *YCbCr_img;

	tmp = cvCloneImage(&(IplImage)img);
	YCbCr_img = cvCreateImage(cvGetSize(tmp), tmp->depth, tmp->nChannels);

	//========RGB2YCrCb==========
	cvCopy(tmp, YCbCr_img, NULL);
	RGBtoYCbCr(YCbCr_img);
	//==========================

	//==========skin color detection=========
	Skin_Color_Detection(YCbCr_img);

	img = Mat(YCbCr_img,0);

	//===========YCbCr轉灰階=============
	cvtColor(img,img_gray,CV_RGB2GRAY);

	//cvCvtColor( YCbCr_img, gray_img, CV_RGB2GRAY );
	//===========Find Contours & Draw=============
	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7));
	cv::morphologyEx(img_gray, img_gray, cv::MORPH_DILATE, element);

    vector<vector<cv::Point> > contours;
    vector<Vec4i> hierarchy;

    // Detect edges using canny
    //Canny( skinImage, canny_output, thresh, thresh*2, 3 );
    /// Find contours
    findContours( img_gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );
	
	/// Draw contours
	int maxIdx = 0;
	for (size_t i = 0; i < contours.size(); ++i) {
		if (contours[maxIdx].size() < contours[i].size()) {
			maxIdx = i;
		}
	}
	
    /// Draw contours
    /*Mat drawing = Mat::zeros( img.size(), CV_8UC3 );
	Scalar color = Scalar( 255,0,0);
	drawContours( drawing, contours, maxIdx, color, 2, 8, hierarchy, 0, cv::Point() );
	imshow("fourier_contour", drawing);
	waitKey(0);*/

	contours_one.push_back(contours[maxIdx]);

	cvReleaseImage(&YCbCr_img);
	cvReleaseImage(&tmp);
	//cvClearSeq(contour);
	//cvClearSeq(cont);
	//cvClearSeq(mcont);

	return;
}

void fourier::fourier_descriptor()
{
	vector<complex<float>> z (contours_one[0].size());
	//point to complex
	for(unsigned int i = 0 ; i < contours_one[0].size() ; i++)
	{
		z[i]=complex<float>((float)contours_one[0][i].x,(float)contours_one[0][i].y);
	}
	contours_one_p.push_back(z);

	//dft
	vector<complex<float>> z_dft (contours_one[0].size());
	dft(z,z_dft);
	
	//cut 128 
	float Re = z_dft[0].real(), Im = z_dft[0].imag();
	float magF0 = sqrt(Re*Re + Im*Im), magF;
	for(int i = 0 ; i < 64 ; i ++)
	{
		Re = z_dft[i].real();	Im = z_dft[i].imag();
		if (fabs(sqrt(Re*Re + Im*Im) / magF0) < numeric_limits<float>::epsilon())
			magF = 0.0;
		else
			magF = sqrt(Re*Re + Im*Im) / magF0;
		FD.push_back(magF);
	}
	for(unsigned int i = z_dft.size()-64 ; i < z_dft.size() ; i ++)
	{
		Re = z_dft[i].real();	Im = z_dft[i].imag();
		if (fabs(sqrt(Re*Re + Im*Im) / magF0) < numeric_limits<float>::epsilon())
			magF = 0.0;
		else
			magF = sqrt(Re*Re + Im*Im) / magF0;
		FD.push_back(magF);
	}

	cv::normalize(FD, FD, 0, 1, CV_MINMAX);

	/*
	dft(z,z,CV_DXT_INV_SCALE);
	//complex to point
	for(int i = 0 ; i < contours_one[0].size() ; i++)
	{
		p[i].x=z[i].real();
		p[i].y=z[i].imag();
	}
	vector<vector<Point> > contours_one_new;
	contours_one_new.push_back(p);

	//draw
	RNG rng(12345);
	Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
		drawContours( img_gray, contours_one_new,0, color, 2, 8, hierarchy, 0, Point() );
	imshow("",img_gray);
	waitKey(0);


	system("pause");
	*/
	
}

vector<float> fourier::get_vector()
{
	return FD;
}

void fourier::clear_vector()
{
	FD.clear();
	contours.clear();
	contours_one_p.clear();
	contours_one.clear();
	hierarchy.clear();
}

void fourier::image_process (Mat in_image)
{
	img = in_image;
	
	clear_vector();
	cvtColor(img,img_gray,CV_RGB2GRAY);
	//boundary of binary image , find contour
	boundary();

	//fourier 
	fourier_descriptor();
}

