/*!
\file connServer.h
\brief Este archivo contiene la definicion de la clase ConnServer, que crea un "stream socket" y proporciona la infraestructura para atender a multiples clientes de manera concurrente. 
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>	
#include <pthread.h>
#include <iostream>
#include <cstdio>
#include <vector>


#define BACKLOG 5

using namespace std;

/*!
 \class class ConnServer
 \brief Esta clase crea un "stream socket" y lo asocia a una direccion IP y un puerto, y provee servicios básicos para atender de manera concurrente a multiples clientes.
*/
class ConnServer
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
	 int *connData; ///< Datos que se comparten a cada instancia que atiende al cliente. 
	 pthread_t *thrIds; ///< Apuntador a arreglo que contiene los identificadores de de cada hebra en ejecución.

 public:

	 /*!
	    \fn ConnServer (int pt, const char *ipa, void *(*connHndlr) (void *), int mxConn)
	    \brief Constructor de la clase. Crea un socket y lo asocia con la direccion y el puerto que se le pasa por parametro, e incializa la estructuras necesarias para atender hasta mxConn conexiones.
	    \param int pt Puerto que se va a usar para el socket.
	    \param const char *ipa Direccion IP  (IPV4) que se va a asociar al socket).
	    \param void *(*connHndlr) (void *) Función que se va a invocar para atender cada conexión.
	    \param int mxConn Número máximo de conexiones permitidas. 
	 */
	 ConnServer (int pt, const char *ipa, void *(*connHndlr) (void *), int mxConn);

	 /*!
	    \fn ~ConnServer()
	    \brief Destructor de la clase.
	 */
	 ~ConnServer();

	 /*!
	    \fn void acceptConnections ()
	    \brief Este método se invoca cuando el sistema está listo para recibir conexiones. Mientras accCon sea verdadero y el numero de conexiones actuales sea menor que maxConnections, se esperará solicitudes de conexion, y cuando ésto ocurra, se creará una hebra que ejecute connection_handler. 
	 */
	 void acceptConnections ();
};
