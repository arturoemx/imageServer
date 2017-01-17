/*!
\file Camera.h
\brief Archivo de encabezado donde se define la clase Camera. Esta Clase tiene
como función capturar imágenes una camara. Cada cuadro capturado se almacena en una cola, para que pueda ser procesados posteriormente. 

Al instanciar un objeto de esta clase, se genera para asegurar la ejecución continua del ciclo de captura. 
*/
#ifndef _CAMERA_
#define _CAMERA_

#include <imageBuffer.h>

#if __OCV_VER__ >= 3
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#else
#include <cv.h>
#include <highgui.h>
#endif

#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
#include <iostream>

using namespace std;
using namespace cv;

/*!
\class Camera
\brief Esta clase permite capturar imágenes de una cámara, utilizando los métodos provisto por la biblioteca highgui. Las imagenes capturadas son almacenadas en una cola. Se proveen mecanismos de sincronización para el acceso concurrente a dicha cola.
*/
class Camera
{
	 int deviceID; ///< Identificador del dispositivo de captura.  
	 VideoCapture cap; ///< Objeto asociado al dispositivo de captura.
	 pthread_t captureThread; ///< Identificador de la hebra de captura.

	 ImageBuffer buffer; ///< Cola de imagenes en donde se almacenarán los cuadros capturados.
     unsigned long capturePeriod; ///< Periodo de captura (en microsegundos).
     bool capture; ///< Bandera que controla cuando se capturan imágenes.

	 /*!
	    \func static void *readFramesThread (void *ptr)
	    \brief Esta es la función principal que es ejecutada como un hebra. Captura los cuadros, del dispositivo de captura y los inserta en un buffer.
	    \param De acuerdo al estandar definido por pthread_create, recibe un apuntador a void, pero este es interpretado por la funcíon como un apuntador a 
	 */
	 static void *readFramesThread (void *ptr);

 public:
       /*!
       \func Camera (int captureDevice, unsigned long capPer = 33333, bool autoCapt = true);
       \brief Constructor de la clase. 
       \param int captureDevice Número que indica la cámara que se va a usar para la captura.
       \param unsigned long capPer Periodo de captura de imagenes: por default el valoe es igual 33 ms (lo que equivale a una frecuencia de 30 fps).
       \param bool autoCapt Bandera que se utiliza para indicar si se deben inicial la captura de manera automñatica: true => si, false => no.
       */
       Camera (int captureDevice, unsigned long capPer = 33333, bool autoCapt = true);

       /*!
       \func Camera ()
       \brief Constructor de la clase. Abre la cámara 0 por defecto con un peridod de captura d 33 ms e inicial la captura automáticamente.
       */
	   Camera ();

       /*!
       \func ~Camera()
       \brief Destructor de la clase.
       */
	  ~Camera ();

       /*!
       \func int getLastFrame (infoFrame &iF);
       \brief Esta función regresa el ultimo cuadro que se haya insertado al buffer. Esto es, es el cuadro mas reciente, el que se encuentra al final de la cola, no al inicio. 
       \param infoFrame &if objeto de tipo infoFrame en donde se copiara el cuadro al final del buffer, junto con el tiempo de captura.
       \return la función regresa 0 en case de exito y -1 en caso de que el Frame este vacio. 
       */
       int getLastFrame (infoFrame &iF);

       /*!
       \func void startCapture();
       \brief Inicia la captura de imágenes y su almacenamiento en el buffer.
       */
       void startCapture() {capture = true;}

       /*!
       \func void stopCapture();
       \brief Detiene la captura de imágenes y su almacenamiento en el buffer.
       */
       void stopCapture() {capture = false;}
};


#endif
