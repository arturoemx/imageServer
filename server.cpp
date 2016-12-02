#include <ImageServer.h>
#include <cstring>

int main(int argc, char **argv)
{
	int port = 8888;
	char ipAddress[64] = "127.0.0.1";

	if (argc > 1)
	{
		strncpy (ipAddress, argv[1], 63);
		if (argc > 2)
			port = atoi(argv[2]);
	}
	
	ImageServer *imS = new ImageServer(port, ipAddress);
	imS->start();

	sleep(3); // wait for server to init

	do {

	} while(true);

	return 0;
}
