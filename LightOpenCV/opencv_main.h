#ifndef OPENCV_MAIN_H
#define OPENCV_MAIN_H

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

enum THRESHOLD_TYPE {
	BINARY,
	BINARY_INVERTED,
	THRESHODLD_TRUNCATED,
	THRESHOLD_TO_ZERO,
	THRESHOLD_TO_ZERO_INVERTED
};

#endif // !OPENCV_MAIN_H
