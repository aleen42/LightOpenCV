#include "image.cpp"

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
};
