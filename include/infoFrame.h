/*!
\file infoFrame.h
\brief En este archivo se encuentra la defincion de la estructura infoFrame, que se utiliza para almacenar una imagen junto con el tiempo en que fue capturada.
*/

#ifndef __INFOFRAME__
#define __INFOFRAME__

#if __OCV_VER__ >= 3
#include <opencv2/opencv.hpp>
#else
#include <cv.h>
#endif
#include <sys/time.h>


/*!
\struct infoFrame
\brief Objetos instanciados a partir de esta estructura almacenan una imagen utilizando un objeto cv::Mat y el tiempo en que fue capturada (un 'timestamp').
*/
struct infoFrame
{
	 cv::Mat frame;///< Matriz en donde se almacenará la imagen capturada.
	 struct timeval t;///< estructura tipo timeval en donde se registra el instante en que la imagen fue capturada.

    /*!
        \fn infoFrame()
        \brief constructor por defecto.
    */
    infoFrame () { }

    /*!
        \fn infoFrame()
        \brief Constructor de copia.
        \param const infoFrame &I Objeto que se va a copiar durante la inicialiación.
        \return
    */
    infoFrame (const infoFrame & I)
    {
         I.frame.copyTo (this->frame);
         this->t = I.t;
    }

    /*!
        \fn infoFrame & operator = (const infoFrame & I)
        \brief Método que sobrecarga el operador de asignación
        \param const infoFrame &I Objeto que se va a copiar.
        \return
    */
    infoFrame & operator = (const infoFrame & I)
    {
         I.frame.copyTo (this->frame);
         this->t = I.t;
         return *this;
    }

    /*!
    \fn void setTime (struct timeval &_t)
    \brief Define el timestamp del objeto con el valor _t
    \param struct timeval &_t Objeto que se va a utilizar en la incializacion.
    */
    void setTime (struct timeval &_t)
    {
         t = _t;
    }

    /*!
    \fn void setTime ()
    \brief Método que asigna el timestamp del objeto con la fecha actual.
    */
    void setTime ()
    {
         gettimeofday (&t, NULL);
    }
};

#endif
