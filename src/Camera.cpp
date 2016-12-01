#include "Camera.h"

Camera::Camera() {
	Camera(0);
}

Camera::Camera(int captureDevice) {
	// Set device ID
	deviceID = captureDevice;

	// Init buffer
	imageBuffer = new ImageBuffer();
	imageBuffer->setBufferSize(5);

	// Open capture
	initDevice();

	// Start capture
	initCapture();
}

bool Camera::initDevice() {
	// init capture device
	cap.open(0);
    if(!cap.isOpened()) {
        perror("Failed to initialize video capture!");
        return false;
    }
    return true;
}

bool Camera::initCapture() {
	// init threads
	pthread_mutex_init(&captureMutex, NULL);
    pthread_create(&captureThread, NULL, Camera::readFramesThread, this);
    pthread_detach(captureThread);
}

void *Camera::readFramesThread(void *camera_ptr) {
	Camera *ptr = (Camera*) camera_ptr;
    Mat frame;
    do {
    	pthread_mutex_lock(&(ptr->captureMutex));
        ptr->cap >> frame; // get a new frame from camera
        pthread_mutex_unlock(&(ptr->captureMutex));

        ptr->imageBuffer->Queue(frame); // queue frame into buffer (CHECK IF LOCKING IS NEEDED)
    } while(true);
}


Mat Camera::lastFrame() {
	Mat frame;
	imageBuffer->getLast(frame);

	if(frame.empty())
		return Mat::zeros(1,1, frame.type());
	else
		return frame;
}