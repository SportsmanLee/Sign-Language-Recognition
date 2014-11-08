#ifndef GABOR_H
#define GABOR_H

#include "opencv2/core/core.hpp"

using namespace cv;

class Gabor
{
	public:
		Gabor(double* angles, int n_Angles);
		void Process(Mat &src, Mat &dest, int angle_num);

	private:
		int m_n_Angles;
		double *m_angles;
		Mat *m_kernels;

		void initKernels();
};

#endif