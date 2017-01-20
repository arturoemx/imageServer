/*!
\file SockIO.h
\brief En este archivo de encabezado se definen dos funciones que permiten enviar y recibir una cantidad arbitrariamente grande de datos a través de un socket.
*/

#ifndef __SOCKIO__
#define __SOCKIO__

#define MAXCHUNK 4096

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

/*!
\fn int Read (int sock, u_long tam, unsigned char *buffer);
\brief Esta funcion lee tam bytes del socket sock y lo almacena en el arreglo apuntado por buffer.
\param int sock Descriptor del socket de donde se va a leer.
\param u_long tam Número de bytes a leer.
\param unsigned char *buffer Apuntador al arreglo en donde se van a escribir los datos leidos
\return La funcion regresa -1 en caso de que haya habido un error en la lectura, y 0 en caso de exito.
*/
int Read (int sock, u_long tam, unsigned char *buffer);

/*!
\fn int Write (int sock, u_long tam, unsigned char *buffer);
\brief Esta función escribe tam bytes, que se encuentran almacenados en la cadena de caracteres referida por el apuntador buffer en el socket identificado por sock.
\param int sock Descriptor del socket de donde se va a escribir.
\param u_long tam Número de bytes a escribir.
\param unsigned char *buffer Apuntador al arreglo en donde se van a tomar los datos a enviar. 
\return La funcion regresa -1 en caso de que haya habido un error en la escritura, y 0 en caso de exito.
*/
int Write (int sock, u_long tam, unsigned char *buffer);

#endif
