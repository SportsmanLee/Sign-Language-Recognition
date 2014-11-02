#pragma once

#include "include\opencv2\core\core.hpp"
#include "include\opencv2\highgui\highgui.hpp"

using namespace cv;

class MyCV
{
public:
	System::Drawing::Bitmap^ getBitmap();
	void readImage(std::string fileName);
	Mat getImage();

private:
	Mat cvImage;
	
};