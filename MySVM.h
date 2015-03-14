#pragma once

#include <vector>
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\ml\ml.hpp"
#include "opencv2/contrib/contrib.hpp"
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

class MySVM
{
public:
	void concatenateGt(vector< vector<float> > features);
	void concatenateAll(vector< vector<float> > features);
	void concatenateAllclasses(int);
	void concatenateOther(vector< vector<float> > features);
	void concatenateTest(vector< vector<float> > features);
	void clear_testVector();
	vector<float> getTestVector();
	void trainSVM();
	void trainSVM_lda();
	void get_pca_lda();
	void k_means(int);
	void k_means_lda(Mat,int);
	float testSVM();
	float testSVM_lda();
	double distance(Mat , Mat , int);
	void setModel(string filename);
	void clearVectors();
	Mat project(const Mat &,const Mat&,const Mat&);
	int update_class();
private:
	vector< vector<float> > gtVectors;
	vector< vector<float> > Allvectors;
	vector< vector<float> > otherVectors;
	vector<int> Allclasses;
	vector<float> testVector;
	string modelFile;
	Mat transform;
	int now_class;
};