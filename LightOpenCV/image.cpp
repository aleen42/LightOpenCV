#include "opencv_main.h"

class Image
{
protected:
	Mat img;											// the container of the image
	const char* path;									// the local path of the image
public:
	/* constructors of the class */
	Image() {}
	Image(Mat img): img(img){}
	/* read the image from a local path */
	void readImage(const char* path) {
		/* set the path */
		this->path = path;
		/* read the image according to the path */
		this->img = imread(path);
		/* check existence */
		if (this->img.empty()) {
			cout << "Can not load image from " << path << endl;
			/* exit */
			exit(-1);
		}
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