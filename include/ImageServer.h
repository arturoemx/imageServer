#ifndef IMAGE_SERVER_H
#define IMAGE_SERVER_H

#include <ConnServer.h>
#include <imageBuffer.h>
#include <structures.h>
#include <Camera.h>

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

#include <vector>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>


#define WIDTH 512
#define HEIGHT 512

using namespace std;
using namespace cv;


struct connectionData
{
	 int client_socket;
	   connectionData ()
	 {
			client_socket = -1;
	 }
	 connectionData (int cs)
	 {
			client_socket = cs;
	 }
};

class ImageServer
{
 private:
	 static const int MAX_CONNECTIONS = 1000;
	 static Camera *cam;
	 static Mat *Hrv, *Hmv, *Maze;
	 Vec3b wall;

	 int port, camId;
	 char *inetAddress;

	 ConnServer < connectionData > *serverConnection;
 public:
	   ImageServer (int cid, int port, const char *inetAddress, Mat *hrv, Mat *hmv, Mat *Mz);
	   ImageServer ();

	 void start ();
	 void shutdown ();
 private:
	 static void *connectionHandler (void *cd);
};

#endif
