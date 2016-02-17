/***********************************************************************
 *                                                                   _
 *       _____  _                           ____  _                 |_|
 *      |  _  |/ \   ____  ____ __ ___     / ___\/ \   __   _  ____  _
 *      | |_| || |  / __ \/ __ \\ '_  \ _ / /    | |___\ \ | |/ __ \| |
 *      |  _  || |__. ___/. ___/| | | ||_|\ \___ |  _  | |_| |. ___/| |
 *      |_/ \_|\___/\____|\____||_| |_|    \____/|_| |_|_____|\____||_| 
 *                                                                      
 *      ================================================================
 *                 More than a coder, More than a designer              
 *      ================================================================
 *
 *
 *      - Document: image.cpp
 *      - Author: aleen42
 *      - Description: image class for all the image obj
 *      - Create Time: Nov 29th, 2015
 *      - Update Time: Feb 17th, 2016 
 *
 **********************************************************************/

#include "opencv_main.h"
#include "common.h"

class Image
{
protected:
	Mat img;											// the container of the image
	const char* path;									// the local path of the image

	/* calculate the angle according to 3 points */
	double angle(Point pt1, Point pt2, Point pt0) {
		double dx1 = pt1.x - pt0.x;
		double dy1 = pt1.y - pt0.y;
		double dx2 = pt2.x - pt0.x;
		double dy2 = pt2.y - pt0.y;
		return (dx1 * dx2 + dy1 * dy2) / sqrt((dx1 * dx1 + dy1 * dy1) * (dx2 * dx2 + dy2 * dy2) + 1e-10);
	}

	/* show the detection of squares */
	void debugSquares(vector<vector<Point> > points) {

		for (int i = 0; i < points[0].size(); i++) {
			printf("Point%d: (%d, %d)\n", i, points[0][i].x, points[0][i].y);
		}

		drawContours(img, points, 0, Scalar(255, 0, 0), 1, 8, vector<Vec4i>(), 0, Point());
		// for (int i = 0; i < points.size(); i++) {
		// 	/* draw contour */
		// 	drawContours(img, points, i, Scalar(255, 0, 0), 1, 8, vector<Vec4i>(), 0, Point());

		// 	/* draw bounding rect */
		// 	Rect rect = boundingRect(Mat(points[i]));
		// 	rectangle(img, rect.tl(), rect.br(), cv::Scalar(255, 0, 0), 2, 8, 0);

		// 	/* draw rotated rect */
		// 	RotatedRect minRect = minAreaRect(Mat(points[i]));
		// 	Point2f rect_points[4];
		// 	minRect.points(rect_points);
		// 	for (int j = 0; j < 4; j++) {
		// 		line(img, rect_points[j], rect_points[(j + 1) % 4], Scalar(0, 0, 255), 1, 8);
		// 	}
		// }
	}
public:
	/* constructors of the class */
	Image() {}
	Image(Mat img) : img(img){}

	/* read the image from a local path */
	void readImage(const char* path) {
		/* set the path */
		this->path = path;
		/* read the image according to the path */
		this->img = imread(path);
		/* check existence */
		if (this->img.empty()) {
			ostringstream os;
			os << "Cannot load image from" << path;
			Common::errorPrint(os.str().c_str());
			/* exit */
			exit(-1);
		}
	}

	/* threshold algorithm */
	void thresholdImage(THRESHOLD_TYPE type) {
		Mat gray;
		const double threshold_value = 0.0;
		const double max_binary_value = 255.0;

		/* convert gray */
		cvtColor(this->img, gray, CV_BGR2GRAY);

		threshold(gray, this->img, threshold_value, max_binary_value, type);
	}

	/* detect squares algorithm by karlphillip */
	vector<vector<Point> > detectSquare(bool debug = false) {
		/* blur will enhance edge detection */
		Mat blurred(this->img);

		medianBlur(this->img, blurred, 9);

		Mat gray0(blurred.size(), CV_8U), gray;

		/* reserved Points array */
		vector<vector<Point> > reserved;
		/* contours Points array*/
		vector<vector<Point> > contours;

		/* find squares in every color plane of the image */
		for (int c = 0; c < 3; c++) {
			int ch[] = { c, 0 };
			
			/* canny ratio which is recommended as 3 by Canny */
			int ratio = 2;
			int lowThreshold = 10;
			
			mixChannels(&blurred, 1, &gray0, 1, ch, 1);

			/* try several threshold level */
			const int threshold_level = 2;
			for (int l = 0; l < threshold_level; l++) {
				/* Use Canny instead of zero threshold level */
				/* Canny helps to catch squares with gradient shading */
				if (l == 0) {
					Canny(gray0, gray, lowThreshold, ratio * lowThreshold, 3);

					/* Dilage helps to remove potential holes between edge segments */
					dilate(gray, gray, Mat(), Point(-1, -1));
				} else {
					gray = gray0 >= (l + 1) * 255 / threshold_level;
				}

				vector<Vec4i> hierarchy;
				/* find contours and store them in a list */
				findContours(gray, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
				
				/* draw contours to improve accuracy */
				for (size_t i = 0; i < contours.size(); i++) {
					drawContours(this->img, contours, i, Scalar(0, 0, 0), 1, 8, hierarchy, 0, Point());
				}

				/* test contours */
				vector<Point> approx;
				for (size_t i = 0; i < contours.size(); i++) {
					/* approximate contour with accuracy proportional to the contour perimeter */
					approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true) * 0.02, true);

					/*
					 * Note: absolute value of an area is used becuase
					 *       area may be positive or negative, in accordance
					 *		 with the contour orientation
					 */
					// cout << "size: " << approx.size() << endl;
					// cout << "area: " << fabs(contourArea(Mat(approx))) << endl;
					if (approx.size() == 4 && fabs(contourArea(Mat(approx))) > 1000 && isContourConvex(Mat(approx))) {
						double maxCosine = 0;

						for (int j = 2; j < 5; j++) {
							double cosine = fabs(angle(approx[j % 4], approx[j - 2], approx[j - 1]));
							maxCosine = MAX(maxCosine, cosine);
						}

						reserved.push_back(approx);

						//if (maxCosine < 0.3) {
						//	reserved.push_back(approx);
						//}
					}
				}
			}
		}

		/* check debug option */
 		if (debug && reserved.size() > 0) {
			debugSquares(reserved);
		}

		return reserved;
	}

	/* detect corner */
	vector<Point2f> detectCorner(bool debug = false, const char* path = "data.json") {
		/* clock */
		time_t start = clock();

		vector<Point2f> Corners;

		/* parameters */
		const int maxCorners = 200;
		const double qualityLevels = 0.001;
		const double minDistance = 10;
		const int blockSize = 10;
		const bool useHarrisDetector = false;
		const int r = 5;

		Mat gray;
		cvtColor(this->img, gray, COLOR_BGR2GRAY);

		goodFeaturesToTrack(gray, Corners, maxCorners, qualityLevels, minDistance, Mat(), blockSize, useHarrisDetector);
		
		cJSON* pointsArray = cJSON_CreateArray();

		for (size_t i = 0; i < Corners.size(); i++) {
			if (debug) {
				ostringstream os;
				os << " (" << Corners[i].x << ", " << Corners[i].y << ")";
				circle(this->img, Corners[i], r, Scalar(255, 255, 255), 2, 8);
				putText(this->img, os.str(), Corners[i], FONT_HERSHEY_SCRIPT_SIMPLEX, 0.6, Scalar(0, 0, 255), 1, 8);
			}

			/* create json */
			cJSON* item = cJSON_CreateObject();
			cJSON_AddNumberToObject(item, "x", x);
			cJSON_AddNumberToObject(item, "y", y);

			cJSON_AddItemToArray(pointsArray, item);
		}

		time_t end = clock();
		cJSON* data = cJSON_CreateObject();
		const char* duration = (Common::doubleToStr(double(end - start) / CLOCKS_PER_SEC) + "s").c_str();
		cJSON_AddStringToObject(data, "time", duration);
		
		/* write the data.json */
		ofstream file;
		file.open(path, ios::out);
		
		if (file.is_open()) {
			file << cJSON_Print(pointsArray);
			cJSON_AddItemToObject(data, "data", pointsArray);
			file.close();
		}

		Common::successPrint(data);

		return Corners;
	}

	void fastDetectCorner(bool debug = false) {
		vector<KeyPoint> keyPoints;

		FAST(this->img, keyPoints, 10, true, FastFeatureDetector::TYPE_7_12);

		if (debug) {
			drawKeypoints(this->img, keyPoints, this->img, Scalar(255, 255, 255), DrawMatchesFlags::DRAW_OVER_OUTIMG);
		}
	}

	/* write the image to the local path */
	void writeImage(const char* path) {
		/* save the image */
		imwrite(path, this->img);
	}


	/* a mothod to show the image */
	void showImage() {
		/* open the image */
		imshow("image", this->img);
		/* wait for the return */
		waitKey();
	}

	/* container */
	/* image */
	Mat getImage() {
		return this->img;
	}

	/* properties */
	/* width */
	int getWidth() {
		return this->img.cols;
	}

	/* height */
	int getHeight() {
		return this->img.rows;
	}

	/* size */
	Size getSize() {
		return this->img.size();
	}

	/* pointer to the matrix */
	uchar* getPtr(int n) {
		return this->img.ptr<uchar>(n);
	}
};
