/*!
\file Client.h
\brief 
*/
#ifndef __IMAGEBUFFER__
#define __IMAGEBUFFER__

#include <RingBuffer.h>
#include <infoFrame.h>

class ImageBuffer:public RingBuffer < infoFrame >
{
 public:
	 ImageBuffer ():RingBuffer < infoFrame > (0)
	 {
	 }
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
	 int getSize ()
	 {
			return this->RBF_Size;
	 }

	 int Queue (const infoFrame &e)
	 {
			return RingBuffer < infoFrame >::Queue (e);
	 }

	 int Dequeue (infoFrame &e)
	 {
			return RingBuffer < infoFrame >::Dequeue (e);
	 }

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
