#include "Client.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/highgui/highgui.hpp>

int main() {

	Client *client = new Client(8888, "127.0.0.1");
	client->connect();

	Mat frame;
	while(true) {
		client->getFrame(frame);
		imshow("rec", frame);
		if(waitKey(30) >= 0) break;
		// client->sendCommand("POLL");
		// sleep(1); // wait one second in between requests
	} 
		

	return 0;
}