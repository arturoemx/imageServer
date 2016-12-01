#include "Client.h"

Client::Client(int port, char *address) {
	this->port = port;
	strncpy(this->address, address, 255);
}

void Client::configure() {
	createSocket();
	setHost();
	connectToSocket();
}

void Client::createSocket() {
	if ((cfd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
	{
		cerr << "Falla en el socket cliente" <<  errno << endl;
		perror ("conectar ");
		exit(1);
	}

	client.sin_port = htons (port);
	client.sin_family = AF_INET;
}

void Client::setHost() {
	fprintf (stderr, "conectando: %s:%d\n", address,
				ntohs (client.sin_port));

	hp = gethostbyname (address);
	if (hp == NULL)
	{
		printf ("fallo gethostbyname %d\n", errno);
		perror ("Socket cliente");
		close (cfd);
		exit(1);
	}

	memcpy (&client.sin_addr.s_addr, hp->h_addr_list[0], hp->h_length);
}

void Client::connectToSocket() {
	if ((connect (cfd, (struct sockaddr *) & client, sizeof (client))) < 0)
	{
		printf ("Falla del cliente %d\n", errno);
		perror ("cliente");
		close (cfd);
        exit(1);	
	}
}


int Client::getFrame(Mat &out) {

	unsigned char msg[4];

	// Read server response
	if(!Read(cfd, 4, msg))
		return READ_ERR;

	cout << "Cliente recibio: " << msg << endl;

	// Check if we can send
	if(!strncmp((char*)msg, "SND", 3)) 
	{
		memset(msg, 0, 4);
		memcpy(msg, "IMG", 3);
		if(!Write(cfd, 4, msg))
			return WRITE_ERR;

		cout << "Cliente mando: " << msg << endl;

		uchar *imageData;
		imageData = new uchar[786432];

		// Read image data
		Read(cfd, 786432, imageData);

		Mat img(512, 512, 16, (void*)imageData);
		imwrite("rec.png", img);
		out = img.clone();
		

		cout << "Cliente recibio: " << "[IMG]\n" << endl; 
	}


}

int Client::sendCommand(char *cmd_str) {

	unsigned char rcmd[4];
	unsigned char tcmd[4];

	memset(rcmd, 0, 4);
	memset(tcmd, 0, 4);
	
	strncpy((char*)tcmd, cmd_str, 3);

	// Read server response
	if(!Read(cfd, 4, rcmd))
		return READ_ERR;

	cout << "Cliente recibio: " << rcmd << endl;

	// Check if we can send
	if(!strncmp((char*)rcmd, "SND", 3)) 
	{
		if(!Write(cfd, 4, tcmd))
			return WRITE_ERR;

		cout << "Cliente mando: " << tcmd << endl;

		memset(rcmd, 0, 4);
		if(!Read(cfd, 4, rcmd))
			return READ_ERR;
		
		cout << "Cliente recibio: " << rcmd << endl; 
	}
}
