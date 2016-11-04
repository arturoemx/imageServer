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

using namespace std;

typedef union
{
    int val;
    unsigned char cmd[4];
} Content;

int main(int argc, char **argv)
{
    int puerto, cfd;
    char direccion[256];
    struct sockaddr_in client;
    struct hostent *hp;
    Content msg;

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


    int cont = 1;
    while (true)
    {
        if (!Read(cfd, 4, msg.cmd))
            break;
        if (!strncmp((char *)msg.cmd, "SND",3))
        {
            
            msg.val = cont;
            if (!Write (cfd, 4, msg.cmd))
                break;
            if (!Read(cfd, 4, msg.cmd))
                break;
            cout << "Clnt envió " << cont << " y recibió " << msg.val << endl;
            cont++;
        }
        sleep(1);
    }
}

