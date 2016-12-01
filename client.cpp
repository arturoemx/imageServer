#include <Client.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/highgui/highgui.hpp>

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
	
	Client *client = new Client(port, ipAddress);
	client->connect();

	Mat frame;
	while(true)
	{
		client->getFrame(frame);
		imshow("rec", frame);
		if(waitKey(30) >= 0) break;
		// client->sendCommand("POLL");
		// sleep(1); // wait one second in between requests
	} 
		

	return 0;
}
