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