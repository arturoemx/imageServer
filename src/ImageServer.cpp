#include <ImageServer.h>
#include <SockIO.h>
#include <cstring>

Camera *ImageServer::cam;
Mat *ImageServer::Hrv;
Mat *ImageServer::Hmv;
Mat *ImageServer::Maze;

ImageServer::ImageServer ()
{
     Hrv = new Mat;
     Hmv = new Mat;
     Maze = new Mat;
     *Hrv = Mat::eye(3,3,CV_32FC1);
     *Hmv = Mat::eye(3,3,CV_32FC1);
     *Maze = Mat::ones(480,640,CV_32FC1);
	 ImageServer (8888, "127.0.0.1", Hrv, Hmv, Maze);
}

ImageServer::ImageServer (int port, const char *inetAddress, Mat *hrv, Mat *hmv, Mat *Mz)
{
	 // config Server
	 this->port = port;

	 this->inetAddress = (char *) malloc (strlen (inetAddress));
	 strcpy (this->inetAddress, inetAddress);

	 // init camera and assign thread to it
	 cam = new Camera (0);
     Hrv = new Mat;
     Hmv = new Mat;
     Maze = new Mat;
	 hrv->copyTo(*Hrv);
	 hmv->copyTo(*Hmv);
	 Mz->copyTo(*Maze);
     warpPerspective(*Maze, *Maze, *Hmv, Size(Maze->cols, Maze->rows), INTER_LINEAR, BORDER_CONSTANT);

	 // init connection to server
	 serverConnection =
			new ConnServer < connectionData > (port, inetAddress, (ImageServer::connectionHandler), MAX_CONNECTIONS);
}

void ImageServer::start ()
{
	 serverConnection->acceptConnections ();
}

void ImageServer::shutdown ()
{

}


void *ImageServer::connectionHandler (void *cd)
{
	 //Get the socket descriptor
	 connectionData *conD = (connectionData *) cd;
	 int sock = conD->client_socket;
	 unsigned char msg[MSG_LENGTH];

	 //Recibe mensaje del cliente
	 do
	 {
			// Leer comando
			memset (msg, 0, MSG_LENGTH);
			if (!Read (sock, MSG_LENGTH, msg))
				 break;

			cout << "Servidor recibio: " << msg << endl;

			// Check what to send
			if (strncmp ((char *) msg, "IMG", 3) == 0)
			{
				 // Send an [IMG]

				 // Get frame from camera
				 Mat img, mImg;
				 img = cam->getLastFrame ();
				 warpPerspective(img, mImg, *Hrv, Size(img.cols, img.rows), INTER_LINEAR, BORDER_CONSTANT);
				 mImg += *Maze;

				 // Get info
				 struct ImageInfo imgInfo;
				 imgInfo.rows = mImg.rows;
				 imgInfo.cols = mImg.cols;
				 imgInfo.type = mImg.type ();
				 imgInfo.size = mImg.total () * mImg.elemSize ();

				 // send info
				 if (!Write
						 (sock, sizeof (struct ImageInfo), (unsigned char *) &imgInfo))
						break;

				 // send data
				 uchar *data;
				 data = mImg.data;
				 Write (sock, imgInfo.size, data);

				 cout << "Servidor mando IMG[" << imgInfo.
						rows << " x " << imgInfo.cols << "]: " << imgInfo.
						type << " (" << imgInfo.size << ")" << endl;

			}
			else if (strncmp ((char *) msg, "POLL", 4) == 0)
			{
				 // Send ACK
				 memset (msg, 0, MSG_LENGTH);
				 strncpy ((char *) msg, "ACK", 3);
				 if (!Write (sock, MSG_LENGTH, msg))
						break;
			}
			else
			{
				 // Send ERR
				 memset (msg, 0, MSG_LENGTH);
				 strncpy ((char *) msg, "ERR", 3);
				 if (!Write (sock, MSG_LENGTH, msg))
						break;
			}
	 }
	 while (true);

	 return (void *) 0;
}
