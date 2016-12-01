#include "ImageServer.h"
int main() {

	ImageServer *imS = new ImageServer(8888, "127.0.0.1");
	imS->start();

	sleep(3); // wait for server to init

	do {

	} while(true);

	return 0;
}
