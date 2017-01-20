/*!
\file testClient.h
\brief Este es un programa de ejemplo en donde se muestra como un programa puede recibir imágenes del servidor y mostrarlas en una ventana.
*/

#include <Client.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

int main (int argc, char **argv)
{
	 int port = 8888;
	 char ipAddress[64] = "127.0.0.1";

	 if (argc > 1)
	 {
			strncpy (ipAddress, argv[1], 63);
			if (argc > 2)
				 port = atoi (argv[2]);
	 }

	 Client *client = new Client (port, ipAddress);
	 client->connectSocket ();

	 Mat frame;
	 while (true)
	 {
			client->getFrame (frame);
			imshow ("rec", frame);
			if (waitKey (30) >= 0)
				 break;
	 }


	 return 0;
}
