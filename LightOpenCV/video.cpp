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
 *      - Document: video.cpp
 *      - Author: aleen42
 *      - Description: video class for all the video obj
 *      - Create Time: Dec 4th, 2015
 *      - Update Time: Mar 4th, 2016 
 *
 **********************************************************************/

#include "opencv_main.h"
#include "image.h"
#include "common.h"

class Video
{
protected:
	VideoCapture vdo;								// the container of the video
	const char* path;								// the local path of the video
	Mat bufferFrame;								// to store each buffer frame after capturing
public:
	/* constructors of the class */
	Video() {}
	Video(VideoCapture vdo) : vdo(vdo) {}
	
	/* read the video from the local path */
	void readVideo(const char* path) {
		/* set the path */
		this->path = path;
		/* read the video according to the path */
		
		this->vdo = VideoCapture(this->path);

		/* check existence */
		if (!this->vdo.isOpened()) {
			ostringstream os;
			os << "Cannot load video from" << path;
			Common::errorPrint(os.str().c_str());
			/* exit */
			exit(-1);
		}
	}

	VideoCapture getVideo() {
		return this->vdo;
	}

	Mat getBufferFrame() {
		return this->bufferFrame;
	}

	/* capture frame */
	/* frame: the frame number */
	Image capture(int frameNum, bool isBuffer = false){
		/* set the frame */
		/* this func will cause problems of losing precision */
		// this->vdo.set(CV_CAP_PROP_POS_FRAMES, frameNum);
		this->setFrames(frameNum);

		if (isBuffer) {
			this->vdo.read(this->bufferFrame);
			/* return the image */
			return Image(this->bufferFrame);	
		} else {
			Mat reserved;

			this->vdo.read(reserved);

			/* return the image */
			return Image(reserved);
		}
	}

	/* capture frames*/
	/* save the image when output is true */
	/* images will be saved in the current directory by default */
	std::vector<Image> capture(int startFrame, int endFrame, bool output = false, const char* path = "") {
		/* total number of frames */
		size_t total = endFrame - startFrame;
		/* dynamical array of images for storing all the frames */
		std::vector<Image> images(total + 1);
		
		this->setFrames(startFrame);
		size_t i = startFrame;

		for (; i <= endFrame; i++) {
			Mat reservedImg;
			this->vdo.read(reservedImg);
			/* save the image when output is [true] */
			if (output && i >= startFrame) {
				string num = Common::intToStr(i);
				string reserved(path);
				imwrite((reserved + num + ".jpg").c_str(), reservedImg);
				cout << "save image: " + num << endl;
			}
		}

		return images;
	}

	/* properties */
	/* get the total number of frames */
	int getTotalFramesNum() {
		return this->vdo.get(CV_CAP_PROP_FRAME_COUNT);
	}

	/* get current frame number */
	int getCurrentFrameNum() {
		return this->vdo.get(CV_CAP_PROP_POS_FRAMES);
	}

	/* rewrite set frame */
	void setFrames(int n) {
		/* this func will cause problems of losing precision */
		this->vdo.set(CV_CAP_PROP_POS_FRAMES, (double)0);
		// this->vdo.set(CV_CAP_PROP_POS_FRAMES, n - (double)1);
		// double frameRate = this->vdo.get(CV_CAP_PROP_FPS);
		// double frameTime = 1000.0 * n / frameRate;

		// this->vdo.set(CV_CAP_PROP_POS_MSEC, frameTime);
		int i = 0;
		
		Mat reserved;
		/* reserved Mat */
		while (i++ < n) {
			/* read the frame */
			this->vdo.read(reserved);

			// cout << "Frame Head: " << i << endl;
		}
	}
};
