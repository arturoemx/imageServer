#include <Camera.h>

Camera::Camera ()
{
	 Camera (0);
}

Camera::Camera (int captureDevice)
{
	 // Set device ID
	 deviceID = captureDevice;

	 // Init buffer
	 imageBuffer.setBufferSize (5);

	 // Open capture
	 initDevice ();

	 // Start capture
	 initCapture ();
}

Camera::~Camera ()
{
	 cap.release ();
}

bool Camera::initDevice ()
{
	 // init capture device
	 cap.open (deviceID);
	 if (!cap.isOpened ())
	 {
			perror ("Failed to initialize video capture!");
			return false;
	 }
	 return true;
}

void Camera::initCapture ()
{
	 // init threads
	 pthread_mutex_init (&captureMutex, NULL);
	 pthread_create (&captureThread, NULL, Camera::readFramesThread, this);
	 pthread_detach (captureThread);
}

void *Camera::readFramesThread (void *camera_ptr)
{
	 Camera *ptr = (Camera *) camera_ptr;
	 infoFrame frame;
	 do
	 {
			pthread_mutex_lock (&(ptr->captureMutex));
			ptr->cap >> frame.frame; // get a new frame from camera
			pthread_mutex_unlock (&(ptr->captureMutex));

			ptr->imageBuffer.advHead ();
			ptr->imageBuffer.Queue (frame);	// queue frame into buffer
	 }
	 while (true);
}


int Camera::getLastFrame (infoFrame &iF)
{
	 imageBuffer.getLast (iF);

	 if (iF.frame.empty ())
	    return -1;
	 else
	    return 0;
}

int Camera::getID ()
{
	 return deviceID;
}
