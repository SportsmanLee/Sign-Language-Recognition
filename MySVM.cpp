#include "stdafx.h"
#include "MySVM.h"
#include <fstream>
#include <iterator>

using namespace cv;

void MySVM::concatenateGt(vector< vector<float> > features)
{
	vector<float> insertVector;
	for (unsigned int i = 0; i < features.size(); ++i) {
		for (unsigned int j = 0; j < features[i].size(); ++j) {
			insertVector.push_back(features[i][j]);
		}
	}

	gtVectors.push_back(insertVector);
}

void MySVM::concatenateOther(vector< vector<float> > features)
{
	vector<float> insertVector;
	for (unsigned int i = 0; i < features.size(); ++i) {
		for (unsigned int j = 0; j < features[i].size(); ++j) {
			insertVector.push_back(features[i][j]);
		}
	}

	otherVectors.push_back(insertVector);
}

void MySVM::concatenateTest(vector< vector<float> > features)
{
	for (unsigned int i = 0; i < features.size(); ++i) {
		for (unsigned int j = 0; j < features[i].size(); ++j) {
			testVector.push_back(features[i][j]);
		}
	}
}

vector<float> MySVM::getTestVector()
{
	return testVector;
}

void MySVM::clearVectors()
{
	gtVectors.clear();
	otherVectors.clear();
}

void MySVM::trainSVM()
{
	// Set up training data
	Mat labels(gtVectors.size() + otherVectors.size(), 1, CV_32FC1, Scalar(0));
	for (int i = 0; i < labels.rows; ++i) {
		labels.at<float>(i, 0) = (i < (int)gtVectors.size()) ? 1.0 : -1.0;
	}

	Mat trainingData(gtVectors.size() + otherVectors.size(), gtVectors[0].size(), CV_32FC1);
	for (int i = 0; i < trainingData.rows; ++i) {
		for (int j = 0; j < trainingData.cols; ++j) {
			trainingData.at<float>(i, j) = (i < (int)gtVectors.size()) ? gtVectors[i][j] : otherVectors[i - gtVectors.size()][j];
		}
	}

    // Set up SVM's parameters
    CvSVMParams params;
    params.svm_type    = CvSVM::C_SVC;
	params.kernel_type = CvSVM::LINEAR;
    //params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);

    // Train the SVM
    CvSVM SVM;
    if (SVM.train_auto(trainingData, labels, Mat(), Mat(), params)) {
		SVM.save("svm_data_auto.xml");
	}
}

float MySVM::testSVM()
{
	Mat testImage(1, testVector.size(), CV_32FC1);
	for (unsigned int i = 0; i < testVector.size(); ++i) {
		testImage.at<float>(0, i) = testVector[i];
	}

	CvSVM SVM;
	SVM.load(modelFile.c_str());

	return SVM.predict(testImage, true); // test result 
}

void MySVM::setModel(string filename)
{
	modelFile = filename;
}

void MySVM::clear_testVector()
{
	testVector.clear();
}