#ifndef CLIENT_H
#define CLIENT_H

#if __OCV_VER__ >= 3
#include <opencv2/opencv.hpp>
#else
#include <cv.h>
#endif

/*!
\file Client.h
\brief En este archivo de encabezado se define la clase Client. Dicha clase crea al ser instanciada un objeto que facilita la parte cliente de un sistema de comunicación basado en sockets. 
*/

#include <SockIO.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include <string.h>
#include <structures.h>

#define MAXDATASIZE 6220800		// 1920*1080*3 = 6220800

using namespace std;
using namespace cv;

/*!
\class Client
\brief Esta clase crea un objeto que permite conectarse via un socket con un servidor, para transmitir información.
*/
class Client
{
	 char address[256]; ///< Cadena de caracteres que contiene la dirección a la cual conectarse.
	 int port; ///< El puerto al cual el cliente va a conectarse
	 int cfd; ///< El descriptor del conector.
	 struct sockaddr_in client; ///< Estructura en donde se almacena la direccion a conectarse.
	 unsigned char *msgData; ///< Apuntador al buffer de datos a enviar y/o recibir. 
	 long int msgDataSize; ///< Tamaño del bufer de datos.


    /*!
        \fn void configure ()
        \brief metodo que configura el socket. Se crea el socket y se inicializa la estructura que contiene la dirección a conectarse.
    */
	 void configure ();

 public:
    /*!
        \fn Client (const char *address, int port, int msgDS = MAXDATASIZE)
        \brief Constructor de la clase. 
        \param const char *address Cadena que contiene la dirección IP a la cual nos vamos a conectar.
        \param int port El puerto al cual nos vamos a conectar.
        \param int msgDS El tamaño máximo de mensaje.
    */
	   Client (int port, const char *address, int msgDS = MAXDATASIZE);

    /*!
        \fn ~Client ();
        \brief Destructor de la clase.
    */
	  ~Client ();

    /*!
        \fn int getFrame (Mat & out)
        \brief Método que recibe una imagen del servidor. 
        \return Regresa SEND_SUCCESS en caso de exito, y SEND_FAILURE en caso contrario. 
    */
	 int getFrame (Mat & out);

    /*!
        \fn void connectSocket ()
        \brief Realiza la conexion del objeto cliente con el servidor. En caso de falla, se invoca exit(1).
    */
	 void connectSocket ();
};

#endif
