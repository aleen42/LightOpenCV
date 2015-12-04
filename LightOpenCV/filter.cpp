#include "image.h"

class Filter : public Image
{
protected:
	/* check the legality of RGB */
	void checkRGB(double& r, double& g, double& b) {
		/* ensure the value of R is between 0 and 255 */
		r = (r < 0) ? 0 : r;
		r = (r > 255) ? 255 : r;
		/* ensure the value of G is between 0 and 255 */
		g = (g < 0) ? 0 : g;
		g = (g > 255) ? 255 : g;
		/* ensure the value of B is between 0 and 255 */
		b = (b < 0) ? 0 : b;
		b = (b > 255) ? 255 : b;
	}
	/* check the legality of width and height */
	void checkWH(int& width, int& height, int origin_width, int origin_height) {
		/* ensure that the width is between 0 and the origin one */
		width = (width < 0) ? 0 : width;
		width = (width > origin_width - 1) ? origin_width - 1 : width;
		/* ensure that the height is between 0 and the origin one */
		height = (height < 0) ? 0 : height;
		height = (height > origin_height - 1) ? origin_height - 1 : height;
	}
	/* filter to run */
	virtual Mat filter() { return Mat(); }
};
