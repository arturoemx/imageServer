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

#define MAX_CONNECTIONS 1000

/*!
\class ImageServer
\brief Objetos instanciados de esta clase, capturan imágenes de una cámara y las almacena en un buffer, y permite la conexión via sockets de clientes a quienes provee de dichas imágenes.
*/
class ImageServer
{
	 int maxConnections; ///< El número máximo de conexiones permitidas.
	 static Camera *cam; ///< Apuntador a objeto de la lcase Camera.
	 static Mat *Hrv; ///< Apuntador a matriz de 3x3 que describe la transformación proyectiva que se aplica a las imágenes capturadas. 
	 static Mat *Hmv; ///< Apuntador a matriz de 3x3 que describe la transformación proyectiva que se aplica a la Máscara para ajustarla al tamáño de las imágenes capturadas. 
	 static Mat *Maze; ///< Apuntador a matriz que contiene la imagen de la máscara que se aplica a cada imagen capturada.

	 char *inetAddress; ///< Apuntador a arreglo de caracteres que contiene la dirección IP que se va a asociar al socket.
	 int port; ///< Puerto que se va a asociar al socket donde el servidor recibirá solicitudes.
	 int camId; ///< Numero que identifica la cámara que se va a utilizar.

	 ConnServer *serverConnection; ///< Apuntador al objeto del tipo ConnServer que administra el servicios.
	 static void *connectionHandler (void *cd); ///< Funcion que atiene a los clientes que se conectan al servicio.
 public:

     /*!
     \fn ImageServer (int cid, int port, const char *inetAddress, Mat *hrv, Mat *hmv, Mat *Mz, int mxConn = MAX_CONNECTIONS)
     \brief Constructor del objeto.
     \param int cid Identificador de la cámara que se va a usar.
     \param int port Puerto que se va a asociar al servidor.
     \param const char *inetAddress Apuntador a la cadena de caracteres que contiene la dirección IP a la que se va a asociar el servidor.
     \param Mat *hrv Apuntador a Matriz que describe transformación proyectiva entre la región rectangular en el piso a observar y la imagen.
     \param Mat *hmv Apuntador a la matriz que descrive la transformación proyectiva (usualmente una similitud), entre la imagen capturada, y la máscara.
     \param Mat *Mz Apuntador a matriz que contiene la imagen que contiene el laberinto/Mascara.
     \param int mxConn Número máximo de conexiones permitidas.
     */
	 ImageServer (int cid, int port, const char *inetAddress, Mat *hrv, Mat *hmv, Mat *Mz, int mxConn = MAX_CONNECTIONS);

     /*!
     \fn ImageServer ();
     \brief Constructor por defecto del objeto.
     */
	 ImageServer ();

     /*!
     \fn void start ();
     \brief Pone al servidor en un estado en donde puede aceptar conexiones de clientes.
     */
	 void start ();
};

/*!
\fn void paintMaze(Mat &I, Mat &Mask)
\brief Aplica una mascara a color a una imagen. En aquellos pixeles de la máscara son diferentes a RGB[255,255,255] (color blanco), el pixel correspondiente de la imagen se sustituye por el valor de la máscara. De caso contrario, el valor original del pixel, se preserva,
\param Mat &I Matriz que contiene la imagen a Enmascarar.
\param Mat &Mask Mascara que se debe a plicar a la imagen I.
*/
void paintMaze(Mat &I, Mat &Mask);


#endif
