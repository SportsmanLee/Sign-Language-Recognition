#include "stdafx.h"
#include "MySVM.h"

using namespace cv;

void MySVM::concatenateFeature(vector< vector<double> > features)
{
	for (unsigned int i = 0; i < features.size(); ++i) {
		for (unsigned int j = 0; j < features[i].size(); ++j) {
			featureVector.push_back(features[i][j]);
		}
	}
}