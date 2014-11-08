#pragma once
#include <string>
#include <opencv2/opencv.hpp> 
#include <complex>

using namespace std;
using namespace cv;
class fourier
{
	public: 
		fourier();
		vector<double> get_vector();
		void image_process(Mat);
	private:
		void readimg(string);
		void boundary();
		void fourier_descriptor();
		void inverse_fourier_descriptor();
		void RGBtoYCbCr (IplImage *);
		void Skin_Color_Detection (IplImage *);
		string img_path;
		Mat img;
		Mat img_gray;
		vector<vector<cv::Point> > contours;
		vector<vector<complex<double>>>contours_one_p;
		vector<vector<cv::Point> > contours_one;
		vector<vector<cv::Point> > inverse_contours;
		vector<double> FD;
		vector<Vec4i> hierarchy;

};
