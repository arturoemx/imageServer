#include "session.h"

Session::Session(int port, char *address) {
	this->port = port;
	strncpy(this->address, address, 255);
}

void Session::configure() {
	createSocket();
	setHost();
	connectToSocket();
}

void Session::createSocket() {
	if ((cfd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
	{
		cerr << "Falla en el socket cliente" <<  errno << endl;
		perror ("conectar ");
		exit(1);
	}

	client.sin_port = htons (port);
	client.sin_family = AF_INET;
}

void Session::setHost() {
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

void Session::connectToSocket() {
	if ((connect (cfd, (struct sockaddr *) & client, sizeof (client))) < 0)
	{
		printf ("Falla del cliente %d\n", errno);
		perror ("cliente");
		close (cfd);
        exit(1);	
	}
}

// void Session::connectionHandler() {
// 	Content message;
// 	while (true)
//     {   
//         // Read message from server
//         if (!Read(cfd, 4, msg.cmd))
//             break;

//         // If server responds with SND then start transmission
//         if (!strncmp((char *)msg.cmd, "SND",3))
//         {   
//             // Copy image to message and send
//             memcpy(msg.image, src.data, WIDTH*HEIGHT);
//             if (!Write (cfd, WIDTH*HEIGHT, msg.image))
//                 break;

//             cout << "Cliente mando imagen" << endl;

//             memset(msg.cmd, 0, 4);
//             if (!Read(cfd, 4, msg.cmd))
//                 break;
//             cout << "Cliente recibio: " << msg.cmd << endl;
//         }
//         sleep(1);
//     }
// }

int Session::sendCommand(char *cmd_str) {

	unsigned char cmd[4];
	strncpy((char*)cmd, cmd_str, 3);

	// Read server response
	if(!Read(cfd, 4, cmd))
		return READ_ERR;

	cout << "Cliente recibio: " << cmd << endl;

	// Check if we can send
	if(!strncmp((char*)cmd, "SND", 3)) 
	{
		if(!Write(cfd, 4, cmd))
			return WRITE_ERR;

		cout << "Cliente mando: " << cmd_str << endl;

		memset(cmd, 0, 4);
		if(!Read(cfd, 4, cmd))
			return READ_ERR;
		
		cout << "Cliente recibio: " << cmd << endl; 
	}
}
