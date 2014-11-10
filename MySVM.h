#pragma once

#include <vector>
#include "include\opencv2\core\core.hpp"
#include "include\opencv2\highgui\highgui.hpp"
#include "include\opencv2\ml\ml.hpp"

using namespace std;
using namespace cv;

class MySVM
{
public:
	void concatenateGt(vector< vector<float> > features);
	void concatenateOther(vector< vector<float> > features);
	void concatetest(vector< vector<float> > features);
	void clear_testVector();
	void trainSVM();
	float testSVM();

private:
	vector< vector<float> > gtVectors;
	vector< vector<float> > otherVectors;
	vector<float> testVector;
};