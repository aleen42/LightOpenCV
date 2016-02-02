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
 *      - Update Time: Feb 2nd, 2016 
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
			cout << "Can not load video from " << path << endl;
			/* exit */
			exit(-1);
		}
	}

	VideoCapture getVideo() {
		return this->vdo;
	}

	/* capture frame */
	/* frame: the frame number */
	Image capture(int frameNum){
		/* set the frame */
		this->vdo.set(CV_CAP_PROP_POS_FRAMES, frameNum);
		/* reserved Mat */
		Mat reserved;
		/* read the frame */
		this->vdo.read(reserved);
		/* return the image */
		return Image(reserved);
	}

	/* capture frames*/
	/* save the image when output is true */
	/* images will be saved in the current directory by default */
	std::vector<Image> capture(int startFrame, int endFrame, bool output = false, const char* path = ".") {
		/* total number of frames */
		int total = endFrame - startFrame;
		/* dynamical array of images for storing all the frames */
		std::vector<Image> images(total + 1);
		int i, j = 0;
		for (i = startFrame; i <= endFrame; i++) {
			images[j++] = capture(i);
			/* save the image when output is [true] */
			if (output) {
				string num = Common::intToStr(i);
				string reserved(path);
				images[j - 1].writeImage((reserved + num + ".jpg").c_str());
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
};
