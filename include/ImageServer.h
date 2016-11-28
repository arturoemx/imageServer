#ifndef IMAGE_SERVER_H
#define IMAGE_SERVER_H

#include "ConnServer.h"
#include "RingBuffer.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/highgui/highgui.hpp>
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


enum CMD {
	SND,
	ACK,
	ERR,
	REQ,
	END
};

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
	RingBuffer<Mat> *imageBuffer;
	Mat testBuffer; // remove this 
	pthread_mutex_t bufferMutex;
	VideoCapture cap;
	pthread_t captureThread;

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
	void init();
	void sendCommand(const char* cmd);
	static void *connectionHandler(void* cd);
	static void *readFrames(void* data);
	void frameReader();
	

};

#endif