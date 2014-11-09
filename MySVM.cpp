#include "stdafx.h"
#include "MySVM.h"

using namespace cv;

void MySVM::concatenateGt(vector< vector<double> > features)
{
	vector<double> insertVector;
	for (unsigned int i = 0; i < features.size(); ++i) {
		for (unsigned int j = 0; j < features[i].size(); ++j) {
			insertVector.push_back(features[i][j]);
		}
	}

	gtVectors.push_back(insertVector);
}

void MySVM::concatenateOther(vector< vector<double> > features)
{
	vector<double> insertVector;
	for (unsigned int i = 0; i < features.size(); ++i) {
		for (unsigned int j = 0; j < features[i].size(); ++j) {
			insertVector.push_back(features[i][j]);
		}
	}

	otherVectors.push_back(insertVector);
}

void MySVM::trainSVM()
{
	// Set up training data
	Mat labels(gtVectors.size() + otherVectors.size(), 1, CV_32F, Scalar(0));
	Mat textImage(100, 100, CV_8UC1, Scalar(0));
	for (unsigned int i = 0; i < labels.rows; ++i) {
		labels.at<float>(i, 0) = (i < gtVectors.size()) ? 1.0 : -1.0;
	}

	Mat trainingData(gtVectors.size() + otherVectors.size(), gtVectors[0].size(), CV_64F);
	for (unsigned int i = 0; i < trainingData.rows; ++i) {
		for (unsigned int j = 0; j < trainingData.cols; ++j) {
			trainingData.at<double>(i, j) = i < gtVectors.size() ? gtVectors[i][j] : otherVectors[i - gtVectors.size()][j];
		}
	}

    // Set up SVM's parameters
    CvSVMParams params;
    params.svm_type    = CvSVM::C_SVC;
    params.kernel_type = CvSVM::LINEAR;
    params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);

    // Train the SVM
    CvSVM SVM;
    SVM.train(trainingData, labels, Mat(), Mat(), params);
}