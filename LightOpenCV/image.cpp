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
 *      - Update Time: Apr 14th, 2016
 *
 **********************************************************************/

#include "opencv_main.h"
#include "common.h"

class Image
{
protected:
	Mat img;											// the container of the image
	const char* path;									// the local path of the image
	Point2f markedPoint;								// the marked point when cropping images, which is (0, 0) by default
	Point2f dropPoint;									// stroing (dx, dy) when cropping images, which is (0, 0) by default

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
		cout << "detected square" << endl;

		for (int i = 0; i < points.size(); i++) {
			printf("Squares %d\n", i);

			for (int j = 0; j < points[i].size(); j++) {
				printf("Point%d: (%d, %d)\n", j, points[i][j].x, points[i][j].y);
			}
		}

		for (int i = 0; i < points.size(); i++) {
			/* draw contour */
			drawContours(img, points, i, Scalar(0, 0, 255), 3, 8, vector<Vec4i>(), 0, Point());

			/* draw bounding rect */
			// Rect rect = boundingRect(Mat(points[i]));
			// rectangle(img, rect.tl(), rect.br(), Scalar(0, 0, 255), 3, 8, 0);

			// /* draw rotated rect */
			// RotatedRect minRect = minAreaRect(Mat(points[i]));
			// Point2f rect_points[4];
			// minRect.points(rect_points);
			// for (int j = 0; j < 4; j++) {
			// 	line(img, rect_points[j], rect_points[(j + 1) % 4], Scalar(0, 0, 255), 3, 8);
			// }
		}
	}

	/* calculate the size of your screen */
	double calcScreenWidth() {
		const char* widthCmd = "xrandr |grep '*' | sed 's/ *\\([^ ]*\\)x.*/\\1/'";
		FILE* fpipe = (FILE*)popen(widthCmd, "r");
		char line[256];
		double width;

		while (fgets( line, sizeof(line), fpipe)) {
			width = atof(line);
		}

		pclose(fpipe);

		return width;
	}

	double calcScreenHeight() {
		const char* heightCmd = "xrandr |grep '*' | sed 's/.*x\\([^ ]*\\).*/\\1/'";
		FILE* fpipe = (FILE*)popen(heightCmd, "r");
		char line[256];
		double height;

		while (fgets( line, sizeof(line), fpipe)) {
			height = atof(line);
		}

		pclose(fpipe);

		return height;
	}
public:
	/* constructors of the class */
	Image() {}
	Image(Mat img) : img(img) {
		this->markedPoint = Point2f(0.0, 0.0);
		this->dropPoint = Point2f(0.0, 0.0);
	}

	/* deconstructor of the class */
	~Image() {
		// cout << "release an image" << endl;
	}

	/* read the image from a local path */
	void readImage(const char* path, bool isTransparent = false) {
		/* set the path */
		this->path = path;
		/* read the image according to the path */
		if (isTransparent) {
			this->img = imread(path, IMREAD_UNCHANGED);
		} else {
			this->img = imread(path);
		}

		/* check existence */
		if (this->img.empty()) {
			ostringstream os;
			os << "Cannot load image from" << path;
			Common::errorPrint(os.str().c_str());
			/* exit */
			exit(-1);
		}
	}

	/* merge an image overlay this image */
	void mergeImage(const char* overlayPath, int x, int y) {
		Mat mask = imread(overlayPath, 0);
		Mat logo = imread(overlayPath);
		Mat imageROI;

		imageROI = this->img(Rect(x, y, logo.cols, logo.rows));
		logo.copyTo(imageROI, mask);
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

		int areaMinSize = 1000;

		/* reserved Points array */
		vector<vector<Point> > reserved;
		/* contours Points array*/
		vector<vector<Point> > contours;

		vector<Vec4i> hierarchy;

		findContours(gray0, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

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
					Canny(gray0, gray0, lowThreshold, ratio * lowThreshold, 3);

					/* Dilage helps to remove potential holes between edge segments */
					dilate(gray0, gray0, Mat(), Point(-1, -1));
				} else {
					gray0 = gray0 >= (l + 1) * 255 / threshold_level;
				}

				vector<Vec4i> hierarchy;
				/* find contours and store them in a list */
				findContours(gray0, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

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
					if (approx.size() == 4 && fabs(contourArea(Mat(approx))) > areaMinSize && isContourConvex(Mat(approx))) {
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
	std::vector<Point2f> detectCorner(const char* quantity, bool debug = false, const char* path = "data.json", bool outputJSON = true) {
		/* clock */
		time_t start = clock();

		vector<Point2f> Corners;

		/* parameters */
		const int maxCorners = atoi(quantity);
		const double qualityLevels = 0.01;
		const double minDistance = 10;
		const int blockSize = 10;
		const bool useHarrisDetector = false;
		const int r = 5;

		Mat gray;
		cvtColor(this->img, gray, COLOR_BGR2GRAY);

		goodFeaturesToTrack(gray, Corners, maxCorners, qualityLevels, minDistance, Mat(), blockSize, useHarrisDetector);

		cJSON* pointsArray = cJSON_CreateArray();

		double distance = 99999.0;
		Point2f shortestPoint;

		for (size_t i = 0; i < Corners.size(); i++) {
			/* recover the drop distance */
			Corners[i].x = Corners[i].x + this->dropPoint.x;
			Corners[i].y = Corners[i].y + this->dropPoint.y;

			double x = (double)Corners[i].x;
			double y = (double)Corners[i].y;

			if (debug) {
				circle(this->img, Corners[i], r, Scalar(255, 255, 255), 2, 8);
				ostringstream os;
				os << " (" << x << ", " << y << ")";
				putText(this->img, os.str(), Corners[i], FONT_HERSHEY_SCRIPT_SIMPLEX, 0.6, Scalar(0, 0, 255), 1, 8);
			}

			double newDistance = sqrt((x - this->markedPoint.x) * (x - this->markedPoint.x) + (y - this->markedPoint.y) * (y - this->markedPoint.y));

			if (distance > newDistance) {
				shortestPoint = Point2f(x, y);
				distance = newDistance;
			}

			/* create json */
			cJSON* item = cJSON_CreateObject();
			cJSON_AddNumberToObject(item, "xPrecent",  x / this->img.cols);
			cJSON_AddNumberToObject(item, "yPrecent", y / this->img.rows);
			cJSON_AddNumberToObject(item, "x", x);
			cJSON_AddNumberToObject(item, "y", y);

			cJSON_AddItemToArray(pointsArray, item);
		}

		time_t end = clock();
		cJSON* data = cJSON_CreateObject();
		const char* duration = (Common::doubleToStr(double(end - start) / CLOCKS_PER_SEC) + "s").c_str();
		cJSON_AddStringToObject(data, "time", duration);

		cJSON_AddNumberToObject(data, "width", this->img.cols);
		cJSON_AddNumberToObject(data, "height", this->img.rows);


		cJSON* item = cJSON_CreateObject();
		cJSON_AddNumberToObject(item, "x", shortestPoint.x);
		cJSON_AddNumberToObject(item, "y", shortestPoint.y);
		cJSON_AddItemToObject(data, "neartestPoint", item);

		item = cJSON_CreateObject();
		cJSON_AddNumberToObject(item, "x", dropPoint.x);
		cJSON_AddNumberToObject(item, "y", dropPoint.y);
		cJSON_AddItemToObject(data, "dropValue", item);

		/* write the data.json */
		ofstream file;
		file.open(path, ios::out);

		if (file.is_open()) {
			cJSON_AddItemToObject(data, "data", pointsArray);
			file << cJSON_Print(data);
			file.close();
		}

		if (outputJSON) {
			Common::successPrint(data);
		}

		std::vector<Point2f>(Corners).swap(Corners);

		return Corners;
	}

	Image cropImageWithCircle(Point2f center, double width, double height) {
		/* initiate marked point */
		this->markedPoint = Point2f(center.x, center.y);

		/* for calculate x and y to see whether the start point is outside the image you want to crop */
		double x = this->markedPoint.x - width;
		x = x < 0 ? 0.0 : x;
		double y = this->markedPoint.y - height;
		y = y < 0 ? 0.0 : y;

		/* if the start point is outside the image, the radius should be change relatively */
		double realWidth = x == 0.0 ? this->markedPoint.x + width : width * 2;
		double realHeight = y == 0.0 ? this->markedPoint.y + height : height * 2;

		/* initiate the drop value */
		this->dropPoint = Point2f(x, y);

		/* initiate the markedPoint value of the new Image */
		this->markedPoint = Point2f(this->markedPoint.x - x, this->markedPoint.y - y);

		this->img = Mat(this->img, Rect(x, y, realWidth, realHeight));
		return *this;
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

	/* pad an image without changing content size */
	void pad(int width, int height) {
		Mat img(width, height, CV_8UC3);

		Mat padded;
		padded.create(this->img.rows + height, this->img.cols + width, this->img.type());
		padded.setTo(Scalar::all(0));

		this->img.copyTo(padded(Rect(0, 0, this->img.cols, this->img.rows)));

		this->img = padded;
	}

	/* a mothod to show the image */
	void showImage(const char* windowName = "result") {
		/* scale to fit the screen */
		namedWindow(windowName, WINDOW_NORMAL);
		resizeWindow(windowName, (int)this->calcScreenWidth(), (int)this->calcScreenHeight());

		/* open the image */
		imshow(windowName, this->img);
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

	Point2f getDropPoint() {
		return this->dropPoint;
	}

	/* size */
	Size getSize() {
		return this->img.size();
	}

	/* pointer to the matrix */
	uchar* getPtr(int n) {
		return this->img.ptr<uchar>(n);
	}

	/* set markedPoints */
	Image setMarked(float x, float y) {
		this->markedPoint = Point2f(x, y);

		return *this;
	}

	/* convert file to Mat directly without creating an instance */
	static Mat read(const char* path, bool isTransparent = false) {
		/* read the image according to the path */
		Mat result;
		if (isTransparent) {
			result = imread(path, IMREAD_UNCHANGED);
		} else {
			result = imread(path);
		}

		/* check existence */
		if (result.empty()) {
			ostringstream os;
			os << "Cannot load image from " << path;
			Common::errorPrint(os.str().c_str());
			/* exit */
			exit(-1);
		}

		return result;
	}

	static Mat read(string path, bool isTransparent = false) {
		/* read the image according to the path */
		Mat result;
		if (isTransparent) {
			result = imread(path.c_str(), IMREAD_UNCHANGED);
		} else {
			result = imread(path.c_str());
		}

		/* check existence */
		if (result.empty()) {
			ostringstream os;
			os << "Cannot load image from" << path;
			Common::errorPrint(os.str().c_str());
			/* exit */
			exit(-1);
		}

		return result;
	}
};
