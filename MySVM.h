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
	void concatenateGt(vector< vector<double> > features);
	void concatenateOther(vector< vector<double> > features);
	void concatetest(vector< vector<double> > features);
	void clear_testVector();
	void trainSVM();
	int testSVM();

private:
	vector< vector<double> > gtVectors;
	vector< vector<double> > otherVectors;
	vector<double> testVector;
};