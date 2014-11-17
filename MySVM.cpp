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

	return SVM.predict(testImage, false); // test result 
}

void MySVM::setModel(string filename)
{
	modelFile = filename;
}

void MySVM::clear_testVector()
{
	testVector.clear();
}

string MySVM::setTXTName(string filename)
{
	const char* chars = (const char*)(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(filename.c_str()[0] + ".txt")).ToPointer();
	txtname.assign(filename.c_str(), 1);
	//txtname.append(filename);
	txtname.append(".txt");
	return txtname;
}

void MySVM::VectorToFile()
{

	const char * filename = txtname.c_str() ;
	fstream fp;
	fp.open(filename, ios::out);//開啟檔案
	if(!fp){//如果開啟檔案失敗，fp為0；成功，fp為非0
		cout<<"Fail to open file: "<<filename<<endl;
	}

	int rows = gtVectors.size();
	int cols = gtVectors[0].size();
	string vector_string ;
	// Write to File
	for(int i = 0 ; i < rows ; i ++)
	{
		vector_string = "";
		for(int j = 0 ; j < cols ; j++)
		{
			std::ostringstream strs;
			strs << gtVectors[i][j];
			vector_string +=strs.str() + " ";
			
		}
		fp<< vector_string<<endl;//寫入字串
	}
	fp.close();//關閉檔案
}