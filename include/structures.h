/*!
\file structures.h
\brief En este archivo se define la estructura ImageInfo, que se utiliza en el servidor de imagenes, así como se definen varios valores por defecto.
*/

#ifndef STRUCTURES_H
#define STRUCTURES_H

#define SEND_FAILURE 0
#define SEND_SUCCESS 1

#define FORMAT_ERROR 3

#define MSG_LENGTH 10


/*!
\struct ImageInfo
\brief Esta clase define la estructura ImagenInfo, que define el encabezado que se utiliza para la transmisión de imagenes.
*/
struct ImageInfo
{
	 int rows; ///< Número de renglones en la imagen.
	 int cols; ///< Numero de columnas en la imagen.
	 int type; ///< Tipo de la imagen (de acuerdo a la representación usada en openCV).
	 int size; ///< El tamaño de la imagen.

	 /*!
	    \fn ImageInfo()
	    \brief Constructor por defecto.
	 */
	 ImageInfo()
	 {
	    rows = cols = type = size = 0;
	 }
};

#endif
