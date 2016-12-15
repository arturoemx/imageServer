#ifndef CLIENT_H
#define CLIENT_H

#if __OCV_VER__ >= 3
#include <opencv2/opencv.hpp>
#else
#include <cv.h>
#endif


#include <SockIO.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include <string.h>
#include <structures.h>

#define MAXIMAGESIZE 6220800		// 1920*1080*3 = 6220800

using namespace std;
using namespace cv;

class Client
{

// Variable declarations
 private:
	 int port;
	 int cfd;
	 char address[256];
	 struct sockaddr_in client;
	 struct hostent *hp;
	 unsigned char *imageData;


// Private Routine declarations
	 void configure ();
	 void createSocket ();
	 void setHost ();
	 void connectToSocket ();

 public:
// Public Routine declarations
	   Client (int port, const char *address, int mxImSize = MAXIMAGESIZE);
	  ~Client ();

	 int getFrame (Mat & out);
	 int sendCommand (const char *cmd);
	 void connect ();

 private:


};


#endif
