#ifndef SESSION_H
#define SESSION_H


#include "SockIO.h"

#include <opencv2/core/core.hpp>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include "structures.h"

using namespace std;
using namespace cv;

class Session {

// Variable declarations
private:
	int port;
    int cfd;
    char address[256];
    struct sockaddr_in client;
    struct hostent *hp;


// Routine declarations
public:
	Session(int port, char* address);
	void configure();
	Mat getLastFrame();
    int sendCommand(char *cmd);

private:
    void createSocket();
    void setHost();
    void connectToSocket();
    

};


#endif