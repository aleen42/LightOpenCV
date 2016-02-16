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
 *      - Document: oldStyleFilter.cpp
 *      - Author: aleen42
 *      - Description: Old Style Filter algorithmn
 *      - Create Time: Nov 29th, 2015
 *      - Update Time: Feb 2nd, 2016 
 *
 **********************************************************************/

#include "filter.h"

class oldStyleFilter : public Filter
{
private:
	/* Override the method filter() */
	Mat filter(Image img) {
		/* create a reserved image */
		Mat reservedImage(img.getSize(), CV_8UC3);
		for (int y = 0; y < img.getHeight(); y++)
		{
			uchar* P0 = img.getPtr(y);
			uchar* P1 = reservedImage.ptr<uchar>(y);
			for (int x = 0; x < img.getWidth(); x++)
			{
				/* get the RGB of the old image */
				double B = P0[3 * x];
				double G = P0[3 * x + 1];
				double R = P0[3 * x + 2];

				/* calculate the new RGB of the new image */
				double newB = 0.272 * R + 0.534 * G + 0.131 * B;
				double newG = 0.349 * R + 0.686 * G + 0.168 * B;
				double newR = 0.393 * R + 0.769 * G + 0.189 * B;

				/* check legality of RGB */
				this->checkRGB(newR, newG, newB);

				/* set the RGB of the new image */
				P1[3 * x] = (uchar)newB;
				P1[3 * x + 1] = (uchar)newG;
				P1[3 * x + 2] = (uchar)newR;
			}
		}
		return reservedImage;
	}
public:
	/* Constructor of the class oldStyleFilter */
	oldStyleFilter(Image img) {
		this->img = this->filter(img);
	}
};
