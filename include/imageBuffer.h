#ifndef __IMAGEBUFFER__
#define __IMAGEBUFFER__

#include "RingBuffer.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

class ImageBuffer: public RingBuffer<Mat>
{
    public:
    ImageBuffer(): RingBuffer<Mat>(0)
    {
    }
    void setBufferSize(int N)
    {
         pthread_mutex_lock(&this->RB_mutex);
         if (this->R)
            delete[] this->R;
         
         this->RBF_Size = N;
         if (this->RBF_Size > 1)
            this->R = new Mat[this->RBF_Size];//Reserva memoria dinamica para la cola
         else
            this->R = 0;
         this->H.SetRingSize (this->RBF_Size);
         this->T.SetRingSize (this->RBF_Size);
         this->H = this->T = 0;
         pthread_mutex_unlock(&this->RB_mutex);
      }
    int getSize()
    {
        return this->RBF_Size;
    }

    int Queue(const Mat &e)
    {
        return RingBuffer<Mat>::Queue(e);
    }

    int Dequeue(Mat &e) {
        return RingBuffer<Mat>::Dequeue(e);
    }

    int getLast(Mat &e)
    {
         pthread_mutex_lock(&this->RB_mutex);
         if (this->T != this->H)
         {
            e = this->R[(uint32_t)(this->T-1)];
            pthread_mutex_unlock(&(this->RB_mutex));
            return 0;
         }
         pthread_mutex_unlock(&this->RB_mutex);
         return -1;
      }
      int advHead()
		{
			pthread_mutex_lock(&this->RB_mutex);
			if (this->H != this->T)
			{
				this->H++;
				pthread_mutex_unlock(&this->RB_mutex);
				return 0;
			}
			pthread_mutex_unlock(&this->RB_mutex);
			return -1;
		}

};

#endif