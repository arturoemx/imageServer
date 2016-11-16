#ifndef IMAGE_SERVER_H
#define IMAGE_SERVER_H

#include "SockIO.h"
#include "server_threads.cpp"

#include <opencv2/core/core.hpp>
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
	connServer<connectionData> *serverConnection;

public: 
	ImageServer(int port, char* inetAddress);
	ImageServer();

	void connect();
	void shutdown();

private:
	void init();
	void sendCommand(int cmd);
	void *connectionHandler(void* cd);


};

#endif