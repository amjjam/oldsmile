/******************************************************************************
 * This is base class CAMERA. It is the base class for any camera class       *
 ******************************************************************************/


#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <aVec.H>
#include <aTime.H>
#include "model.H"
#include "image.H"

class CAMERA{
public:
  CAMERA(double T, double P, int n, int m);
  ~CAMERA();
  void setDir(double roll, double pitch, double yaw);
  void setDir(aVec dir, double roll=0);
  void setPos(aVec p);
  virtual IMAGE snap(MODEL &m)=0;
  virtual aVec direction(double i, double j, int c=1)=0;
  aVec unitVector(double t, double p);
protected:
  double T,P;
  int n,m;
  aVec pos;
  aVec dir;
  double roll,pitch,yaw;

  aVec transform(aVec &v);
};

#endif
