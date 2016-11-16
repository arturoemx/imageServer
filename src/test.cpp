#include "session.h"
#include <iostream>

using namespace std;

int main() {
	char address[256];
	strncpy(address, "127.0.0.1", 255);

	Session client_session(8888, address);
	client_session.configure();
	client_session.sendCommand("ACK");

}