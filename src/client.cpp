#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ctype.h>
#include <unistd.h>
#include <iostream>
#include <SockIO.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define WIDTH 512
#define HEIGHT 512

using namespace std;
using namespace cv;


typedef union
{   
    unsigned char image[WIDTH*HEIGHT];
    unsigned char cmd[4];
} Content;

int main(int argc, char **argv)
{
    int puerto, cfd;
    char direccion[256];
    struct sockaddr_in client;
    struct hostent *hp;
    Content msg;

    //-------
    Mat src;
    src = imread("lena.jpg");

    cout << "cols: " << src.cols << endl;
    cout << "rows: " << src.rows << endl;
    cout << "type: " << src.type() << endl;

    unsigned char t[WIDTH*HEIGHT];
    memcpy(t, src.data, WIDTH*HEIGHT);

    bool equal = true;
    for(int i = 0; i < WIDTH*HEIGHT; i++) {
        if(*(t+i) != *(src.data+i)) {
            equal = false;
            break;
        }
    }  

    cout << "equal: " << equal << endl;

    //--------


    //---------- CREAR SOCKET ---------
    if (argc>1)
    {
        strncpy (direccion, argv[1], 255);
        if (argc>2)
            puerto = atoi (argv[2]);
        else
            puerto = 8888;
    }
    else
    {
        snprintf(direccion, 255, "127.0.0.1");
        puerto = 8888;
    }
    if ((cfd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
	{
		cerr << "Falla en el socket cliente" <<  errno << endl;
		perror ("conectar ");
		exit(1);
	}
	client.sin_port = htons (puerto);
	client.sin_family = AF_INET;

    //-------------------------------

    //---------------- GET HOST ---------

	fprintf (stderr, "conectando: %s:%d\n", direccion,
				ntohs (client.sin_port));

	hp = gethostbyname (direccion);
	if (hp == NULL)
	{
		printf ("fallo gethostbyname %d\n", errno);
		perror ("Socket cliente");
		close (cfd);
		return 0;
	}
    // ------------------------
	/*else
	{
		while (*hp->h_aliases)
			*hp->h_aliases++;
	}*/

	memcpy (&client.sin_addr.s_addr, hp->h_addr_list[0], hp->h_length);

	/* Ahora si conectar con el servidor.  */
	// warning: be careful with this cast
	if ((connect (cfd, (struct sockaddr *) & client, sizeof (client))) < 0)
	{
		printf ("Falla del cliente %d\n", errno);
		perror ("cliente");
		close (cfd);
        exit(1);	
	}


    while (true)
    {   
        // Read message from server
        if (!Read(cfd, 4, msg.cmd))
            break;

        // If server responds with SND then start transmission
        if (!strncmp((char *)msg.cmd, "SND",3))
        {   
            // Copy image to message and send
            memcpy(msg.image, src.data, WIDTH*HEIGHT);
            if (!Write (cfd, WIDTH*HEIGHT, msg.image))
                break;

            cout << "Cliente mando imagen" << endl;


            memset(msg.cmd, 0, 4);
            if (!Read(cfd, 4, msg.cmd))
                break;
            cout << "Cliente recibio: " << msg.cmd << endl;
        }
        sleep(1);

    }
}

