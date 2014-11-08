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
	void concatenateFeature(vector< vector<double> > features);
	void trainSVM();

private:
	vector<double> featureVector;
};