#ifndef IMAGE_SERVER_H
#define IMAGE_SERVER_H

#include "ConnServer.h"
#include "ImageBuffer.h"
#include "Camera.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>  
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
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


// void *connectionHandler(void* cd);

struct connectionData
{
    int client_socket;
    connectionData()
    {
        client_socket = -1;
    }
    connectionData(int cs)
    {
        client_socket = cs;
    }
};

typedef union
{
    unsigned char image[WIDTH*HEIGHT];
    unsigned char cmd[4];
} Content;


class ImageServer {
private:
	static const int MAX_CONNECTIONS = 1000;
	static const int CMD_LENGTH = 4;
	static Camera *cam;

	int port;
	char *inetAddress;

	ConnServer<connectionData> *serverConnection;
	
	

public: 
	ImageServer(int port, const char* inetAddress);
	ImageServer();

	void start();
	void shutdown();
	void getLastFrame(Mat &frame);
private:
	void sendCommand(const char* cmd);
	static void *connectionHandler(void* cd);
	void readCamera();

};

#endif