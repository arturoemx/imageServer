/*!
\file RingCounter.h
\brief Este archivo contiene la definición de un contador módular.
*/
/*
  	RingCounter.h - A header file that defines a class of a modular arithmetic
	 counter.

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

#ifndef __RINGCOUNTER__
#define __RINGCOUNTER__

#include <cstdlib>

/*!
\class RingBuffer
\brief Esta clase implementa un contador modular. El objeto funciona como un entero, que aritmeticamente opera usando aritmética modular. 
*/
class RingCounter
{
	 size_t C;        ///< En esta variable se almacena el valor representado por el objeto
	 size_t RingSize; ///< El módulo; el valor máximo que puede tomar el objeto.
 public:

    /*!
    \fn RingCounter ()
    \brief Constructor del objeto.
    */
	 RingCounter ()
	 {
			RingSize = 0;
			C = 0;
	 }

    /*!
    \fn RingCounter (size_t rz)
    \brief Constructor del objeto.
    \param size_t rz El numero de valores diferentes permitidos en el objeto, i.e. el módulo.
    */
	 RingCounter (size_t rz)
	 {

			RingSize = rz;
			C = 0;
	 }

    /*!
    \fn RingCounter (const RingCounter & R)
    \brief Constructor de copia.
    \param const RingCounter & R Objeto de tipo RingCounter que se va a copiar.
    */
	 RingCounter (const RingCounter & R)
	 {
			C = R.C;
			RingSize = R.RingSize;
	 }

    /*!
    \fn void SetRingSize (size_t val)
    \brief Define el numero de valores diferentes permitidos en el objeto, i.e. el módulo.
    \param size_t val El nuevo valor a asignar
    */
	 void SetRingSize (size_t val)
	 {
			RingSize = val;
			if (C > val)
			    C = C % val;
	 }

	 
	 /*!
	 \fn operator size_t ()
	 \brief Método que sobrecarga el operador size_t.
	 */
	 operator size_t ()
	 {
			return C;
	 }

    /*!
    \fn RingCounter & operator = (const size_t & v)
    \brief Método que sobrecarga el operador de asignación para asignar un valor al objeto.
    \param const size_t & v El objeto que va a copiarse en el objeto que invoca
    \return Regresa el objeto que invoca.
    */
	 RingCounter & operator = (const size_t & v)
	 {
			C = v % RingSize;
			return *this;
	 }

    /*!
    \fn RingCounter & operator = (const RingCounter & R)
    \brief Método que sobrecarga el operador de asignación para asignar un objeto de tipo RingCounter al objeto que invoca.
    \param const RungCounter &R Objeto que va a copiarse al objeto que invoca. 
    \return Regresa el objeto que invoca.
    */
	 RingCounter & operator = (const RingCounter & R)
	 {
			C = R.C;
			RingSize = R.RingSize;
			return *this;
	 }

    /*!
    \fn template < typename X > RingCounter operator + (const X & n)
    \brief Método plantilla que sobrecarga el operador +.
    \param const X & n Objeto que se suma al objeto que invoca. 
    \return Regresa un nuevo objeto resultado de la suma del objeto que invoca y el parámetro recibido.
    */
	 template < typename X > RingCounter operator + (const X & n)
	 {
			RingCounter R (RingSize);

			R.C = (C + (size_t) n) % RingSize;
			return R;
	 }

    /*!
    \fn template < typename X > RingCounter operator - (const X & n)
    \brief Método plantilla que sobrecarga el operador -.
    \param const X & n Objeto que se resta al objeto que invoca. 
    \return Regresa un nuevo objeto resultado de la resta del parametro recibido al objeto que invoca.
    */
	 template < typename X > RingCounter operator - (const X & n)
	 {
			RingCounter R (RingSize);
			int diff = (int) C - (int) n;

			if (diff >= 0)
				 R.C = (C - n) % RingSize;
			else
				 R.C = RingSize - (-diff % RingSize);
			return R;
	 }

    /*!
    \fn template < typename X > RingCounter & operator += (const X & val)
    \brief Método plantilla que sobrecarga el operador +=.
    \param const X & val Objeto que se suma al objeto que invoca. 
    \return Regresa el objeto que invoca,
    */
	 template < typename X > RingCounter & operator += (X & val)
	 {
			C = (C + (size_t) val) % RingSize;
			return *this;
	 }

    /*!
    \fn template < typename X > RingCounter & operator -= (const X & n)
    \brief Método plantilla que sobrecarga el operador -=.
    \param const X & val Objeto que se resta al objeto que invoca. 
    \return Regresa el objeto que invoca,
    */
	 template < typename X > RingCounter & operator -= (const X & n)
	 {
			int diff = (int) C - (int) n;

			if (diff >= 0)
				 C = (C - n) % RingSize;
			else
				 C = RingSize - (-diff % RingSize);
			return *this;
	 }

    /*!
    \fn RingCounter & operator ++ ()
    \brief Método que sobrecarga el operador ++.
    \return Regresa el objeto que invoca.
    */
	 RingCounter & operator ++ ()
	 {
			C = (C + 1) % RingSize;
			return *this;
	 }

    /*!
    \fn RingCounter & operator -- ()
    \brief Método que sobrecarga el operador --.
    \return Regresa el objeto que invoca.
    */
	 RingCounter & operator -- ()
	 {
			C = !C ? RingSize - 1 : (C - 1) % RingSize;
			return *this;
	 }

    /*!
    \fn RingCounter & operator ++ (int)
    \brief Version postfix del método que sobrecarga el operador ++.
    \return Regresa el objeto que invoca.
    */
	 RingCounter & operator ++ (int)
	 {
			C = (C + 1) % RingSize;
			return *this;
	 }
    /*!
    \fn RingCounter & operator -- (int)
    \brief Version postfix del método que sobrecarga el operador --.
    \return Regresa el objeto que invoca.
    */
	 RingCounter & operator -- (int)
	 {
			C = !C ? RingSize - 1 : (C - 1) % RingSize;
			return *this;
	 }

    /*!
    \fn bool operator == (const RingCounter & R)
    \brief Método que sobrecarga el operador de comparacion (igualdad).
    \param const RingCounter &R El objeto que se va a comparar con el objeto que invoca.
    \return El método regresa verdadero si ambos objetos son iguales; i.e. si el módulo (RingSize) es el mismo, y si el valor almacenado (C) es igual. En caso contrario regresa falso.
    */
	 bool operator == (const RingCounter & R)
	 {
			if (RingSize == R.RingSize && C == R.C)
				 return true;
			return false;
	 }

    /*!
    \fn bool operator != (const RingCounter & R)
    \brief Método que sobrecarga el operador de comparacion (no igualdad).
    \param const RingCounter &R El objeto que se va a comparar con el objeto que invoca.
    \return El método regresa falso si ambos objetos son iguales; i.e. si el módulo (RingSize) es el mismo, y si el valor almacenado (C) es igual. En caso contrario regresa verdadero.
    */
	 bool operator != (const RingCounter & R)
	 {
			if (RingSize == R.RingSize && C != R.C)
				 return true;
			return false;
	 }

    /*!
    \fn bool operator > (const RingCounter & R)
    \brief Método que sobrecarga el operador 'mayor que' (>).
    \param const RingCounter &R El objeto que se va a comparar con el objeto que invoca.
    \return El método regresa verdadero si el objeto que invoca es mayor que el objeto que se pasa por parametro. En caso contrario regresa verdadero.
    */
	 bool operator > (const RingCounter & R)
	 {
			if (RingSize == R.RingSize && C > R.C)
				 return true;
			return false;
	 }

    /*!
    \fn bool operator >= (const RingCounter & R)
    \brief Método que sobrecarga el operador 'mayor o igual' (>=).
    \param const RingCounter &R El objeto que se va a comparar con el objeto que invoca.
    \return El método regresa verdadero si el objeto que invoca es mayor o igual que el objeto que se pasa por parametro. En caso contrario regresa verdadero.
    */
	 bool operator >= (const RingCounter & R)
	 {
			if (C > R.C && RingSize == R.RingSize)
				 return true;
			return false;
	 }

    /*!
    \fn size_t getC ()
    \brief Método que regresa el valor almacenado en el objeto.
    \return Regresa el valor almacenado en el objeto.
    */
	 size_t getC ()
	 {
			return C;
	 }
};


#endif
