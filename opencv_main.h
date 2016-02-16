#ifndef OPENCV_MAIN_H
#define OPENCV_MAIN_H

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <ctime>
#include <fstream>
#include <sstream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "cJSON.c"

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
