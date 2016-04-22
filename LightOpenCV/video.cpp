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
 *      - Update Time: Apr 14th, 2016 
 *
 **********************************************************************/

#include "opencv_main.h"
#include "image.h"
#include "common.h"

// #define FFMPEG

class Video
{
protected:
	camDetectionType detectType;					// the detection type
	VideoCapture vdo;								// the container of the video
	const char* path;								// the local path of the video
	Mat bufferFrame;								// to store each buffer frame after capturing
public:
	/* constructors of the class */
	Video() {}
	Video(VideoCapture vdo) : vdo(vdo) {
		this->detectType = DEFAULT;
	}
	/* deconstructor of the class */
	~Video() {
		// cout << "release a video" << endl;
	}
	
	/* read the video from the local path */
	void readVideo(const char* path) {
		/* set the path */
		this->path = path;
		/* read the video according to the path */
		this->vdo = (strcmp(path, "camera") == 0) ? VideoCapture(0) : VideoCapture(this->path);

		/* check existence */
		if (!this->vdo.isOpened()) {
			ostringstream os;
			os << "Cannot load video from" << path;
			Common::errorPrint(os.str().c_str());
			/* exit */
			exit(-1);
		}
	}
	
	/* detect video */
	Video detectVideo(camDetectionType type) {
		this->detectType = type;
		return *this;
	}

	/* show video directly */
	void showVideo() {
		for (;;) {
			Mat frame;
			if (this->vdo.read(frame)) {

				if (this->detectType != DEFAULT) {
					Image image(frame);
					switch (this->detectType)
					{
					case DETECT_CAM_SQUARE:
						image.detectSquare(true);
						break;
					default:
						break;
					}
				}
				imshow("video", frame);
			}
			else {
				break;
			}

			/* wait for key to return */
			if (waitKey(30) >= 0) {
				break;
			}
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
		this->setFrames(frameNum, false);

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
	std::vector<string> capture(int startFrame, int endFrame, bool output = false, const char* path = "", string filename = "") {
		/* total number of frames */
		size_t total = endFrame - startFrame;
		/* dynamical array of images for storing all the frames */
		/* store string rather than Image objects because those objects will eat all the memory */
		std::vector<string> images;
		
		this->setFrames(startFrame, false);

#ifdef FFMPEG

		/* use ffmpeg to improve speed */
		string videoPath(this->path);
		string framePath(path);
		string cmd = "ffmpeg -r \"" + Common::doubleToStr(this->getVideo().get(CV_CAP_PROP_FPS)) + "\" -i " + videoPath + " -vframes \"" + Common::intToStr(total + 1) + "\" \"" + framePath + "f" + filename + "_%1d.png\"";
		
		/* excute the command */
		FILE* captureProcess = popen(cmd.c_str(), "w");

		if (captureProcess == NULL) {
			ostringstream os;
			os << "Failed to capture frames from the video";
			Common::errorPrint(os.str().c_str());
			/* exit */
			exit(-1);
		}

		pclose(captureProcess);

		size_t i = 1;
		for (; i <= total + 1; i++) {
			/* save the image when output is [true] */
			if (output && i >= startFrame) {
				string num = Common::intToStr(i);
				string reserved(path);
				images.push_back(reserved + "f" + filename + "_" + num + ".png");
			}
		}

#endif

#ifndef FFMPEG
	
		/* this loop will cost too much time */
		size_t i = startFrame;
		for (; i <= endFrame; i++) {
			Mat reservedImg;
			this->vdo.read(reservedImg);
			/* save the image when output is [true] */
			if (output && i >= startFrame) {
				string num = Common::intToStr(i);
				string reserved(path);

				// imwrite((reserved + filename + num + ".png").c_str(), reservedImg);
				images.push_back(reserved + "f" + filename + "_" + num + ".png");
				cout << "save image: " + num << endl;
			}
		}
		
#endif

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
	void setFrames(int n, bool isPrecise = true) {
		/* this func will cause problems of losing precision */
		if (isPrecise) {
			this->vdo.set(CV_CAP_PROP_POS_FRAMES, (double)0);
			int i = 0;
			
			Mat reserved;
			/* reserved Mat */
			while (i++ < n) {
				/* read the frame */
				this->vdo.read(reserved);

				// cout << "Frame Head: " << i << endl;
			}
		} else {
			this->vdo.set(CV_CAP_PROP_POS_FRAMES, (double)n);
		}
	}
};
