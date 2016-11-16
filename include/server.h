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

class Server {
	connServer<connectionData> *serverConnection;



};