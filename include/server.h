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

enum CMD {
	SND,
	ACK,
	ERR,
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

class Server {
private:
	connServer<connectionData> *serverConnection;

public: 
	Server(int port, char* inetAddress);
	Server();

	void connect();
	void shutdown();

private:
	void init();
	void sendCommand(int cmd);
	void *connectionHandler(void* cd);


};