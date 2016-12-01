#ifndef _CAMERA_
#define _CAMERA_


#include "imageBuffer.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>  
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
#include <iostream>

using namespace std;
using namespace cv;

struct ImageInfo {
		int rows;
		int cols;
		int type;
		int size;
};

class Camera {
private:
	int deviceID;
	VideoCapture cap;
	pthread_t captureThread;
	pthread_mutex_t captureMutex;

	ImageBuffer *imageBuffer;

public:
	Mat getLastFrame();
	Camera(int captureDevice);
	Camera();
	~Camera();

	int getID();
	ImageInfo getInfo();

private:
	bool initDevice();
	bool initCapture();

	static void *readFramesThread(void *ptr);
	void readFrames(Camera *ptr);
};


#endif