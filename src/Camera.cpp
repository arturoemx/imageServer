#include <Camera.h>

Camera::Camera ()
{
	 Camera (0);
}

Camera::Camera (int captureDevice, unsigned long capPer, bool autoCapt)
{
	 // Set device ID
	 deviceID = captureDevice;

	 // Init buffer
	 buffer.setBufferSize (5);

	 capture = autoCapt;

	 capturePeriod = capPer;

	 // init capture device
	 cap.open (deviceID);
	 if (!cap.isOpened ())
	 {
			perror ("Camera::Failed to initialize video capture!");
			return;
	 }
	 
	 //Launch capture thread.
	 if (pthread_create (&captureThread, NULL, Camera::readFramesThread, this))
	 {
	    perror ("Camera::Couldn't start capture thread.");
	    return;
	 }

	 // Start capture
	 capture = autoCapt;

}

Camera::~Camera ()
{
     void *ret;

     for (int cont = 0; pthread_cancel(captureThread)!=0 && cont < 10; cont++)
     {
        perror ("Camera::~Camera: Couldn't finish capture thread on exit");
        usleep(100000);
     }
     if (pthread_join(captureThread, &ret) != 0)
        perror ("Camera::~Camera: Couldn't joint capture thread");
	 cap.release ();
}

void *Camera::readFramesThread (void *camera_ptr)
{
	 Camera *ptr = (Camera *) camera_ptr;
	 infoFrame frame;
	 do
	 {
	    if (ptr->capture)
		{
			ptr->cap >> frame.frame; // get a new frame from camera

			ptr->buffer.advHead ();
			ptr->buffer.Queue (frame);	// queue frame into buffer
	    }
	    usleep (ptr->capturePeriod);
	 }
	 while (true);
}


int Camera::getLastFrame (infoFrame &iF)
{
	 buffer.getLast (iF);

	 if (iF.frame.empty ())
	    return -1;
	 else
	    return 0;
}

int Camera::getID ()
{
	 return deviceID;
}
