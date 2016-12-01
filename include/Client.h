#ifndef CLIENT_H
#define CLIENT_H


#include "SockIO.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/highgui/highgui.hpp>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include <string.h>
#include "structures.h"

using namespace std;
using namespace cv;

class Client {

// Variable declarations
private:
	int port;
    int cfd;
    char address[256];
    struct sockaddr_in client;
    struct hostent *hp;


// Routine declarations
public:
	Client(int port, const char* address);
	
	int getFrame(Mat &out);
    int sendCommand(const char *cmd);
    void connect();

private:
    void configure();
    void createSocket();
    void setHost();
    void connectToSocket();
    

};


#endif