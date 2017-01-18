/*!
\file ImageServer.h
\brief Este archivo contiene las definición de la clase ImageServer. Objetos instanciados de esta clase capturan imágenes de una cámara, y los ofrecen a traves de un socket a clientes que se conectan a el. Ademas provee capacidades, para transformar la imagenes capturadas (homografias), y mezclar éstas con imagenes predefinidas.
*/

#ifndef IMAGE_SERVER_H
#define IMAGE_SERVER_H

#include <ConnServer.h>
#include <imageBuffer.h>
#include <structures.h>
#include <Camera.h>

#include <vector>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>

using namespace std;
using namespace cv;

class ImageServer
{
	 static const int MAX_CONNECTIONS = 10000;
	 static Camera *cam;
	 static Mat *Hrv, *Hmv, *Maze;

	 int port, camId;
	 char *inetAddress;

	 ConnServer *serverConnection;
	 static void *connectionHandler (void *cd);
 public:
	 ImageServer (int cid, int port, const char *inetAddress, Mat *hrv, Mat *hmv, Mat *Mz);
	 ImageServer ();

	 void start ();
	 void shutdown ();
};

#endif
