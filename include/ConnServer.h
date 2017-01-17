/*!
\file connServer.h
\brief Este archivo contiene la definicion de la clase ConnServerm que crea un socket y proporciona la infraestructura para atender a multiples clientes de manera concurrente. 
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>					//inet_addr
#include <unistd.h>							//write
#include <pthread.h>						//para los hilos , compilar con -lpthread
#include <iostream>
#include <cstdio>
#include <vector>


#define BACKLOG 5

using namespace std;

/*!
 \class template <typename X> class ConnServer
 \brief Esta clase crea un socket y lo asocia a una direccion IP y un puerto, y provee servicios básicos para atender de manera concurrente a multiples clientes.
*/
template < typename X > class ConnServer
{
 private:
	 int maxConnections; ///< Número máximo de conexiones permitidas.
	 int nConnections; ///< Número de conecciones realizadas
	 int socketDesc; ///< descriptor del socket.
	 int port; ///< Puerto que se va usar para la comunicación
	 struct sockaddr_in server; ///< Estructura en donde se almacena la informacion de conexión del servidor.
	 string ipAddress; ///< Cadena que contienen la direccion ip que se va a asociar al servidor.
	 bool accCon; ///< Bandera que se va a utilizar para controla cuando el servidor acepta conexiones.
	 void *(*connection_handler) (void *); ///< Funcion que se invoca para atender cada conexión.
	 X *connData; ///< Datos que se comparten a cada instancia que atiende al cliente. 
	 pthread_t *thrIds; ///< Apuntador a arreglo que contiene los identificadores de de cada hebra en ejecución.

 public:
	 ConnServer (int pt, const char *ipa, void *(*connHndlr) (void *),
							 int mxConn)
	 {
			accCon = false;
			connection_handler = connHndlr;
			socketDesc = socket (AF_INET, SOCK_STREAM, 0);
			maxConnections = mxConn;
			nConnections = 0;
			if (socketDesc == -1)
			{
				 perror ("ConnServer Constructor: Creating socket endpoint\n");
				 return;
			}
			port = pt;
			server.sin_family = AF_INET;
			if (ipa[0] == '\0')
			{
				 server.sin_addr.s_addr = INADDR_ANY;
				 ipAddress = string ("Anny");
			}
			else
			{
				 ipAddress = string (ipa);

				 if (!inet_aton (ipAddress.c_str (), &server.sin_addr))
				 {
						cerr << "conServer Constructor: invalid IP address: " << ipAddress
							 << endl;
						return;
				 }
			}
			server.sin_port = htons (port);

			if (bind (socketDesc, (struct sockaddr *) &server, sizeof (server)) <
					0)
			{
				 perror ("ConnServer Constructor: binding socket endpoint\n");
				 return;
			}

			if (listen (socketDesc, BACKLOG) == -1)
			{
				 perror ("ConnServer Constructor: error on listen.");
				 return;
			}
			accCon = true;
			connData = new X[maxConnections];
			thrIds = new pthread_t[maxConnections];
	 }

	 ~ConnServer()
	 {
	    if (connData)
	        delete[] connData;
	    if (thrIds)
	        delete[] thrIds;
	 }


	 void acceptConnections ()
	 {
			int clntSocket, sockAddInSize = sizeof (struct sockaddr_in);
	        struct sockaddr_in client;

			while (accCon)
			{
				 if (nConnections < maxConnections)
				 {
						clntSocket =
							 accept (socketDesc, (struct sockaddr *) &client,
											 (socklen_t *) & sockAddInSize);
						if (clntSocket == -1)
						{
							 perror ("ConnServer:acceptConnections");
							 accCon = false;
						}
						else
						{
							 connData[nConnections].client_socket = clntSocket;
							 if (pthread_create
									 (&(thrIds[nConnections]), NULL, connection_handler,
										(void *) &(connData[nConnections])))
							 {
									perror ("No se pudo crear el hilo");
									return;

							 }
							 else
							 {
									nConnections++;
									cout << "Se acaba de hacer una nueva conexión: " 
									     << nConnections << endl;
								    cout.flush();
						     }
						}
				 }
			}
	 }
};
