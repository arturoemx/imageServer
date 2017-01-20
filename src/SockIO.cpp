/*!
\file Client.h
\brief 
*/

#include <SockIO.h>

int Read (int sock, u_long tam, unsigned char *buffer)
{
	 u_long leeidos = 0;
	 int cnt, leer;

	 //double tiempo;

	 //  tiempo = clock()*1.0/CLOCKS_PER_SEC;
	 do
	 {
			// max chuncks of MAXCHUNK bytes
			if (tam - leeidos > MAXCHUNK)
				 leer = MAXCHUNK;
			else
				 leer = tam - leeidos;

			if ((cnt = read (sock, buffer + leeidos, leer)) < 0)
			{
				 fprintf (stderr, "ERROR FATAL. falla en read %d, EA=%d\n", errno,
									EAGAIN);
				 fprintf (stderr, "EI=%d EIO=%d\n", EINTR, EIO);
				 // close(sock);
				 /* no necesariamente hay que cerrar */
				 printf ("Error, Didn`t read all the bytes");
				 return -1;							//  exit(5);
			}
			else
			{
				 leeidos += cnt;
			}
	 }
	 while (leeidos < tam);
	 return 0;
}

int Write (int sock, u_long tam, unsigned char *buffer)
{
	 u_long escritos = 0;
	 int cnt, escribe;

	 do
	 {
			// max chuncks of MAXCHUNK bytes
			if (tam - escritos > MAXCHUNK)
				 escribe = MAXCHUNK;
			else
				 escribe = tam - escritos;

			if ((cnt = write (sock, buffer + escritos, escribe)) < 0)
			{
				 fprintf (stderr, "ERROR FATAL. falla en write %d\n", errno);
				 // close(sock);
				 /* no necesariamente hay que cerrar */
				 printf ("Error, Didn`t write all the bytes");
				 return -1;							//  exit(5);
			}
			else
			{
				 escritos += cnt;
			}
	 }
	 while (escritos < tam);
	 return 0;
}
