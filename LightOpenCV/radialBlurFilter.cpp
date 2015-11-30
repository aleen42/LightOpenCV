#include "filter.h"

class radialBlurFilter : public Filter
{
private:
	/* Override the method filter() */
	Mat filter(Image img, radialBlurType type, int num) {
		const int arr_size = 3;
		Mat src1u[arr_size];

		split(img.getImage(), src1u);

		/* create a reserved image */
		Mat reservedImage(img.getSize(), CV_8UC3);

		/* get the central point of the image */
		Point center(img.getWidth() / 2, img.getHeight() / 2);


		for (int y = 0; y < img.getHeight(); y++) {
			uchar* P = reservedImage.ptr<uchar>(y);
			for (int x = 0; x < img.getWidth(); x++) {
				// int tmp[arr_size] = {0};
				int tmp[arr_size];
				memset(tmp, 0, sizeof(int) * arr_size);

				int R = (int)norm(Point(x, y) - center);
				double angle = atan2((double)(y - center.y), (double)(x - center.x));

				/* num is the mean value */
				for (int i = 0; i < num; i++) {
					int newX, newY;

					/* filter of differente types: Rotate or Scale */
					switch (type) {
					case ROTATE:
						/* step lengh */
						angle += 0.01;

						newX = R * cos(angle) + center.x;
						newY = R * sin(angle) + center.y;
						break;
					case SCALE:
						/* i is the range of changment */
						int tmpR = (R - i) > 0 ? (R - i) : 0;

						newX = tmpR * cos(angle) + center.x;
						newY = tmpR * sin(angle) + center.y;
						break;
					}

					/* check the legality of width and height */
					this->checkWH(newX, newY, img.getWidth(), img.getHeight());

					for (int j = 0; j < arr_size; j++) {
						tmp[j] += src1u[j].at<uchar>(newY, newX);
					}
				
				}
				
				for (int j = 0; j < arr_size; j++) {
					P[3 * x + j] = (uchar)(tmp[j] / num);
				}
			}
		}
		return reservedImage;
	}
public:
	/* Constructor of the class oldStyleFilter */
	radialBlurFilter(Image img, radialBlurType type, int num) {
		this->img = this->filter(img, type, num);
	}
};