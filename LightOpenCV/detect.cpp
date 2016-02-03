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
*      - Document: filter.cpp
*      - Author: aleen42
*      - Description: abstract Filter Class file to give abstract
*      			   definition for filtering image algorithms.
*      - Create Time: Dec 4th, 2015
*      - Update Time: Feb 2nd, 2016
*
**********************************************************************/

#include "image.h"

class Detect : public Image
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
};
