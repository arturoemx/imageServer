#include <Client.h>

Client::Client (int port, const char *address, int mxImSize)
{
	 this->port = port;
	 strncpy (this->address, address, 255);
	 imageData = new unsigned char[mxImSize];
	 if (!imageData)
	 {
			std::cerr << "Error Client Constructor: Couln not allocate enough"
				 << " memory for image buffer." << std::endl;
			return;
	 }

	 configure ();
}

Client::~Client ()
{
	 if (imageData)
			delete[]imageData;
}

void Client::configure ()
{
	 createSocket ();
	 setHost ();
}

void Client::connect ()
{
	 connectToSocket ();
}

void Client::createSocket ()
{
	 if ((cfd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
	 {
			cerr << "Falla en el socket cliente" << errno << endl;
			perror ("conectar ");
			exit (1);
	 }

	 client.sin_port = htons (port);
	 client.sin_family = AF_INET;
}

void Client::setHost ()
{
	 fprintf (stderr, "conectando: %s:%d\n", address, ntohs (client.sin_port));

	 hp = gethostbyname (address);
	 if (hp == NULL)
	 {
			printf ("fallo gethostbyname %d\n", errno);
			perror ("Socket cliente");
			close (cfd);
			exit (1);
	 }

	 memcpy (&client.sin_addr.s_addr, hp->h_addr_list[0], hp->h_length);
}

void Client::connectToSocket ()
{
	 if ((::connect (cfd, (struct sockaddr *) &client, sizeof (client))) < 0)
	 {
			printf ("Falla del cliente %d\n", errno);
			perror ("cliente");
			close (cfd);
			exit (1);
	 }
}


int Client::getFrame (Mat & out)
{
	 unsigned char msg[MSG_LENGTH];
	 struct ImageInfo imgInfo;

	 strncpy ((char *) msg, "IMG", MSG_LENGTH);
	 if (!Write (cfd, MSG_LENGTH, msg))
			return SEND_FAILURE;



	 Read (cfd, sizeof (struct ImageInfo), (unsigned char *) &imgInfo);

	 if (imgInfo.size > MAXIMAGESIZE)
	 {
			cerr << "error in Client::getFrame" << endl << endl
				 << "Image sent size exceeds the buffer size" << endl;
			return SEND_FAILURE;
	 }
	 // Read image data
	 if (!Read (cfd, imgInfo.size, imageData))
			return SEND_FAILURE;

	 Mat img (imgInfo.rows, imgInfo.cols, imgInfo.type, (void *) imageData);
	 out = img.clone ();


	 return SEND_SUCCESS;

}

int Client::sendCommand (const char *cmd_str)
{

	 unsigned char msg[MSG_LENGTH];
	 memset (msg, 0, MSG_LENGTH);

	 if (strlen (cmd_str) > 10)
			return FORMAT_ERROR;

	 // Read server response
	 if (!Read (cfd, MSG_LENGTH, msg))
			return SEND_FAILURE;


	 // Check if we can send
	 if (!strncmp ((char *) msg, "SND", 3))
	 {
			strncpy ((char *) msg, cmd_str, MSG_LENGTH);
			if (!Write (cfd, MSG_LENGTH, msg))
				 return SEND_FAILURE;


			memset (msg, 0, MSG_LENGTH);
			if (!Read (cfd, MSG_LENGTH, msg))
				 return SEND_FAILURE;

	 }

	 return SEND_SUCCESS;
}
