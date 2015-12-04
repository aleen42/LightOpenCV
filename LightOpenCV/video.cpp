#include "opencv_main.h"
#include "image.h"

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
};