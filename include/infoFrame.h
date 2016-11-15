#ifndef __INFOFRAME__
#define __INFOFRAME__

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <sys/time.h>
#include <iostream>

using namespace std;
using namespace cv;

class infoFrame
{
public:
  Mat frame;
  struct timeval t;
public:
  infoFrame();
  infoFrame (const infoFrame &I);
  infoFrame &operator = (const infoFrame &I);
  void copyToinfoFrame(Mat &result);
  void setTime();
  void setTime(struct timeval &_t);
};

void infoFrame::setTime(struct timeval &_t)
{
  t = _t;
}

void infoFrame::setTime()
{
  gettimeofday (&t, NULL);
}

infoFrame::infoFrame()
{
    cout<<"Constructor infoFrame"<<endl;
}

infoFrame::infoFrame (const infoFrame &I)
{
	I.frame.copyTo(this->frame);
	this->t = I.t;
}

infoFrame &infoFrame::operator = (const infoFrame &I)
{
	I.frame.copyTo(this->frame);
	this->t = I.t;
	return *this;
}

#endif
