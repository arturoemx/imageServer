/*!
\file RingBuffer.h
\brief  Archivo de encabezado en donde se define ela clase RingBuffer.
*/

/*
  RingBuffer.h  - header file of the implementation of a Ringbuffer.

    Copyright (C) 2010  Arturo Espinosa-Romero (arturoemx@gmail.com)
    Facultad de Matemáticas, Universidad Autónoma de Yucatán, México.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef __RINGBUFFER__
#define __RINGBUFFER__

#include <RingCounter.h>
#include <pthread.h>
#include <stdint.h>

/*!
\class template < typename X > class RingBuffer
\brief Esta clase define una cola finita a partir de un arreglo. Depende del objeto RingCounter para manejar indices circulares. La clase se construye como un plantilla, y se espera que los objetos quw consitituyan la cola tengan sobrecargado el operador de copia. La clase provee métodos para añadir un objeto al final de la cola, sacar del frente de la cola. La clase cuenta con pthread_mutexes para sincronizar acceso cuando se utiliza en un ambiente multihebras.
*/
template < typename X > class RingBuffer
{
 protected:
	 uint32_t RBF_Size;	///< El tamaño del Buffer
	 X *R;              ///< Apuntador al arreglo que consitituye el area de almacenamiento del buffer.
	 RingCounter H; ///< Indice circular que indica la cabeza de la cola. Se incrementa cuando se saca un objeto de la cola.
	 RingCounter T; ///< Indice circular que indica el final de la cola. Se incrementa cuando se inserta un objeto de la cola.
	 //Incrementa H cuando se saca un elemento de la cola
	 pthread_mutex_t RB_mutex; ///< pthread_mutex que permite controlar el acceso cundo múltiples hebras operan sobre el objeto.
 public:

    /*!
    \fn RingBuffer (uint32_t N)
    \brief Constructor de la clase
    \param int N Tamaño del arreglo que se utiliza para almacenar la cola; la cola así creada puede almacenar hasta N-1 elementos.
    */
	 RingBuffer (uint32_t N)
	 {
			pthread_mutex_init (&RB_mutex, NULL);	//initialises the mutex referenced by mutex with attributes specified by attr.
			pthread_mutex_lock (&RB_mutex);
			RBF_Size = N;
			if (RBF_Size > 1)
			{
				 R = new X[RBF_Size];		//Reserva memoria dinamica para la cola
			}
			else
				 R = 0;
			H.SetRingSize (RBF_Size);
			T.SetRingSize (RBF_Size);
			H = T = 0;
			pthread_mutex_unlock (&RB_mutex);
	 }

    /*!
    \fn ~RingBuffer ()
    \brief Destructor del objeto.
    */
	 ~RingBuffer ()
	 {
			pthread_mutex_lock (&RB_mutex);
			if (R)
				 delete[]R;
			pthread_mutex_unlock (&RB_mutex);
	 }

    /*!
    \fn virtual int Queue (const X & e)
    \brief Método virtual que inserta un elemento en la cola.
    \param cont X & e Elemento a ser insertado en la cola.
    \return El método regresa 0 en caso de éxito, y 1 en caso de que la cola se encuentre llena.
    */
	 virtual int Queue (const X & e)
	 {
			pthread_mutex_lock (&RB_mutex);
			if (T + 1 != H)						//Compara T.C con H.C
			{
				 R[(uint32_t) T] = e;
				 T++;										//T indica cuantos elementos hay en la cola
				 pthread_mutex_unlock (&RB_mutex);
				 return 0;
			}
			pthread_mutex_unlock (&RB_mutex);
			return -1;
	 }


    /*!
    \fn virtual int Dequeue (const X & e)
    \brief Método virtual que saca un elemento de la cabeza la cola.
    \param cont X & e Objeto en donde se regresa una copia del objeto que se encuentra en la cabeza de la cola.
    \return El método regresa 0 en caso de éxito, y 1 en caso de que la cola se encuentre vacia.
    */
	 virtual int Dequeue (X & e)
	 {
			pthread_mutex_lock (&RB_mutex);
			if (H != T)
			{
				 e = R[(uint32_t) H];
				 H++;
				 pthread_mutex_unlock (&RB_mutex);
				 return 0;
			}
			pthread_mutex_unlock (&RB_mutex);
			return -1;
	 }

    /*!
    \fn virtual size_t getT ()
    \brief Método que regresa el índice en el buffer donde se va a insertar un objeto cuando se invoque el método Queue. Esto es, el final de la cola.
    \return El índice que referencia al final de la cola.
    */
	 virtual size_t getT ()
	 {
			return T.getC ();
	 }


    /*!
    \fn virtual int getH ()
    \brief Método que regresa el índice en el buffer donde se extraerá un objeto de la cola cuando se invoque método Dequeu. Esto es, la cabeza de la cola. 
    \return El índice que referencía la cabeza de la cola.
    */
	 virtual int getH ()
	 {
			return H.getC ();
	 }


    /*!
    \fn virtual bool QueueIsEmpty ()
    \brief Metodo que se utiliza para determinar si la cola esta vacia.
    \return El método regresa verdadero en caso de que la cosa este vacia, y falso en caso contrario.
    */
	 virtual bool QueueIsEmpty ()
	 {
			if (T == H)
				 return true;
			else
				 return false;
	 }


    /*!
    \fn virtual void Init_Elements (const X & e)
    \brief Inicializa el buffer con el objeto que se pasa por referencia.
    \param const X & e Objeto que se va a copiar a todos los elementos del buffer.
    */
	 virtual void Init_Elements (const X & e)
	 {
			uint32_t i;
			pthread_mutex_lock (&RB_mutex);
			H = T = 0;								//Asigna la C y RingSize de T a H
			for (i = 0; i < RBF_Size; ++i)
				 R[i] = e;
			pthread_mutex_unlock (&RB_mutex);
	 }
};

#endif
