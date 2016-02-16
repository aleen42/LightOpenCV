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
 *      - Document: faceDetect.cpp
 *      - Author: aleen42
 *      - Description: to provide face detection algorithmn with opencv
 *      - Create Time: Feb 2nd, 2016
 *      - Update Time: Feb 2nd, 2016 
 *
 **********************************************************************/

#include "image.h"

/* face detector */
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>

/* face detector */

class FaceDetect : public Image
{
private:
	/* the path of the data of shape predictor 68 face landmarks */
	const string modelPath;

	/* detector */
	frontal_face_detector detector;

	/* shape predector */
	shape_predictor sp;

public:
	/* constructor */
	FaceDetect() : modelPath("shape_predictor_68_face_landmarks.dat"), detector(get_frontal_face_detector()) {
		/* initiate sp */
		deserialize(this->modelPath) >> sp;
	}

	/* make the image larger */
	void larger() {
		pyramid_up(this->dlib_img);
	}

	/* detect function */
	void detect() {
		std::vector<dlib::rectangle> dets = this->detector(this->dlib_img);
		cout << dets.size() << endl;
	}
};
