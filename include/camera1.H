/******************************************************************************
 * This is class CAMERA1. It is a child of class CAMERA and implements a      *
 * specific image plane geometry.                                             *
 ******************************************************************************/

#ifndef _CAMERA1_H_
#define _CAMERA1_H_

#include "camera.H"
#include <mpi.h>

class CAMERA1: public CAMERA{
public:
  CAMERA1(double T, double P, int n, int m);
  ~CAMERA1();
  virtual IMAGE snap(MODEL &m);
  aVec direction(double i, double j, int c=1);
private:
  double dt,dp;
};

#endif
