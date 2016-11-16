#ifndef IMAGE_SERVER_H
#define IMAGE_SERVER_H

#include "SockIO.h"
#include "ConnServer.h"

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
	static const int MAX_CONNECTIONS = 1000;
	int port;
	char *inetAddress;

public: 
	ImageServer(int port, const char* inetAddress);
	ImageServer();

	void start();
	void shutdown();

private:
	void init();
	void sendCommand(int cmd);
	virtual void *connectionHandler(void* cd);

};

#endif