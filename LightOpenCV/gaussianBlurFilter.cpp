#include "filter.h"

class gaussianBlurFilter : public Filter
{
private:
	/* Override the method filter() */
	Mat filter(Image img) {
		/* create a reserved image */
		Mat reservedImage(img.getSize(), IPL_DEPTH_8U, 3);
		GaussianBlur(img.getImage(), reservedImage, Size(5, 5), 0, 0);
		return reservedImage;
	}
public:
	/* Constructor of the class oldStyleFilter */
	gaussianBlurFilter(Image img) {
		this->img = this->filter(img);
	}
};