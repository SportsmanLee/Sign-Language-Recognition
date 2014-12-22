#pragma once

#include <vector>
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\ml\ml.hpp"
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

class MySVM
{
public:
	void concatenateGt(vector< vector<float> > features);
	void concatenateOther(vector< vector<float> > features);
	void concatenateTest(vector< vector<float> > features);
	void clear_testVector();
	vector<float> getTestVector();
	void trainSVM();
	float testSVM();
	void setModel(string filename);
	void clearVectors();

private:
	vector< vector<float> > gtVectors;
	vector< vector<float> > otherVectors;
	vector<float> testVector;
	string modelFile;
};