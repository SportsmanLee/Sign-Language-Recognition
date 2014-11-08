#include "stdafx.h"
#include "Gabor.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <cmath>

using namespace cv;

#define KERNEL_SIZE 9
#define POS_VAR 20
#define POS_W 7
#define POS_PSI 90

Gabor::Gabor(double *angles, int n_Angles): m_n_Angles(n_Angles)
{
    m_angles = new double[m_n_Angles];
	
	m_kernels = new Mat[m_n_Angles];
	for(int i = 0; i < m_n_Angles; i++) {
		m_angles[i] = angles[i];
		m_kernels[i] = Mat(KERNEL_SIZE, KERNEL_SIZE, CV_32FC1);
	}

	initKernels();
}

void Gabor::initKernels()
{
    int hks = (KERNEL_SIZE - 1) / 2;
    double psi = POS_PSI * CV_PI / 180;
    double var = POS_VAR / 10;
	double w = POS_W / 10.0;

	for (int k = 0; k < m_n_Angles; k++) {
		double phase = (float) (m_angles[k] - 90) * CV_PI / 180;
    
		for (int y = -hks; y <= hks; y++) {
			for (int x = -hks; x <= hks; x++) {
				m_kernels[k].at<float>(hks + y, hks + x) = 
					(float)exp(-(pow(x, 2) + pow(y, 2)) / (2 * var)) * cos(w * x * cos(phase) + w * y * sin(phase) + psi);
			}
		}
	}
}

void Gabor::Process(Mat &src, Mat &dest, int angle_num)
{
	Mat src_g, src_f;

    cvtColor(src, src_g, CV_BGR2GRAY);
    src_g.convertTo(src_f, CV_32F, 1.0 / 255, 0);
    
    filter2D(src_f, dest, CV_32F, m_kernels[angle_num]);

	pow(dest, 2, dest);

	src_g.~Mat(); src_f.~Mat();
}