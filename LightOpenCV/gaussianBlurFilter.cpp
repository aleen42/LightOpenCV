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
 *      - Document: gaussianBlurFilter.cpp
 *      - Author: aleen42
 *      - Description: Gaussian Blur Algorithm
 *      - Create Time: Dec 1st, 2015
 *      - Update Time: Feb 2nd, 2016 
 *
 **********************************************************************/

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
