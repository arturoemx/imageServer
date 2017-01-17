/*!
\file Client.h
\brief 
*/

#ifndef __INFOFRAME__
#define __INFOFRAME__

#if __OCV_VER__ >= 3
#include <opencv2/opencv.hpp>
#else
#include <cv.h>
#endif
#include <sys/time.h>


struct infoFrame
{
	 cv::Mat frame;
	 struct timeval t;

    infoFrame ()
    {
    }

    infoFrame (const infoFrame & I)
    {
         I.frame.copyTo (this->frame);
         this->t = I.t;
    }

    infoFrame & operator = (const infoFrame & I)
    {
         I.frame.copyTo (this->frame);
         this->t = I.t;
         return *this;
    }

    void setTime (struct timeval &_t)
    {
         t = _t;
    }

    void setTime ()
    {
         gettimeofday (&t, NULL);
    }
//void copyToinfoFrame (cv::Mat & result);
};

#endif
