/*!
\file ImageBuffer.h
\brief Este archivo define la clase ImageBuffer, que es una especializacion de la clase Ringbuffer para operar con objetos de tipo infoFrame.
*/
#ifndef __IMAGEBUFFER__
#define __IMAGEBUFFER__

#include <RingBuffer.h>
#include <infoFrame.h>

/*!
\class ImageBuffer
\brief Esta clase especializa la clase Ringbuffer para operar  on objetos de tipo infoFrame. Aparte añade dos métodos nuevos: getLast y advHead.
*/
class ImageBuffer:public RingBuffer < infoFrame >
{
 public:

    /*!
    \fn ImageBuffer ()
    \brief Constructor del objeto por defecto. Inicializa un una cola de objeto infoFrame con cero objetos
    */
	 ImageBuffer ():RingBuffer < infoFrame > (0)
	 {
	 }

    /*!
    \fn void setBufferSize (int N)
    \brief Define el tamaño del buffer a N elementos.
    \param int N El nuevo tamaño del buffer.
    */
	 void setBufferSize (int N)
	 {
			pthread_mutex_lock (&this->RB_mutex);
			if (this->R)
				 delete[]this->R;

			this->RBF_Size = N;
			if (this->RBF_Size > 1)
				 this->R = new infoFrame[this->RBF_Size];	//Reserva memoria dinamica para la cola
			else
				 this->R = 0;
			this->H.SetRingSize (this->RBF_Size);
			this->T.SetRingSize (this->RBF_Size);
			this->H = this->T = 0;
			pthread_mutex_unlock (&this->RB_mutex);
	 }

    /*!
    \fn int getSize ()
    \brief Método que regresa el tamaño del buffer.
    \return Regresa el tamaño del buffer.
    */
	 int getSize ()
	 {
			return this->RBF_Size;
	 }

    /*!
    \fn int Queue (const X & e)
    \brief Método que inserta un objeto de tipo infoFrame en la cola.
    \param cont infoFrame & e El objeto a ser insertado en la cola.
    \return El método regresa 0 en caso de éxito, y 1 en caso de que la cola se encuentre llena.
    */
	 int Queue (const infoFrame &e)
	 {
			return RingBuffer < infoFrame >::Queue (e);
	 }

    /*!
    \fn int Dequeue (const infoFrame &e)
    \brief Método que saca un elemento de la cola.
    \param cont infoFrame & e Objeto en donde se regresa una copia del objeto que se inserto en la cola.
    \return El método regresa 0 en caso de éxito, y 1 en caso de que la cola se encuentre vacia.
    */
	 int Dequeue (infoFrame &e)
	 {
			return RingBuffer < infoFrame >::Dequeue (e);
	 }

    /*!
	\fn int getLast (infoFrame &e)
    \brief Método que saca un elemento del final de la cola.
    \param cont infoFrame & e Objeto en donde se regresa una copia del objeto que se encuentra al final de la cola.
    \return El método regresa 0 en caso de éxito, y 1 en caso de que la cola se encuentre vacia.
    */
	 int getLast (infoFrame &e)
	 {
			pthread_mutex_lock (&this->RB_mutex);
			if (this->T != this->H)
			{
				 e = this->R[(uint32_t) (this->T - 1)];
				 pthread_mutex_unlock (&(this->RB_mutex));
				 return 0;
			}
			pthread_mutex_unlock (&this->RB_mutex);
			return -1;
	 }

    /*!
    \fn int advHead ()
    \brief Método que avanza la cabeza de la cola.
    \return Regresa 0 en caso de éxito y -1 en caso de la cola este vacia. 
    */
	 int advHead ()
	 {
			pthread_mutex_lock (&this->RB_mutex);
			if (this->H != this->T)
			{
				 this->H++;
				 pthread_mutex_unlock (&this->RB_mutex);
				 return 0;
			}
			pthread_mutex_unlock (&this->RB_mutex);
			return -1;
	 }
};

#endif
