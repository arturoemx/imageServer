#ifndef _CAMERA_
#define _CAMERA_


#include <imageBuffer.h>

#if __OCV_VER__ >= 3
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#else
#include <cv.h>
#include <highgui.h>
#endif

#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
#include <iostream>

using namespace std;
using namespace cv;


class Camera
{
	 int deviceID;
	 VideoCapture cap;
	 pthread_t captureThread;
	 pthread_mutex_t captureMutex;

	 ImageBuffer imageBuffer;

 public:
	   Camera (int captureDevice);
	   Camera ();
	  ~Camera ();
       int getLastFrame (infoFrame &iF);

	 int getID ();

 private:
	 bool initDevice ();
	 void initCapture ();

	 static void *readFramesThread (void *ptr);
	 void readFrames (Camera * ptr);
};


#endif
