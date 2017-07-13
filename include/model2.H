/*********************************************************************
 * class MODEL1 is a child class of class MODEL which implements the *
 * 3D parabloic magnetopause.                                        *
 *********************************************************************/

#ifndef _MODEL2_H_
#define _MODEL2_H_

#include <math.h>
#include "model.H"

class MODEL2: public MODEL{
public:
  MODEL2();
  MODEL2(double x0, double t, double z0, double x1, double r0, 
	 double b);
  ~MODEL2();
  virtual double brightness(const aVec &p) const;
  virtual double losStart(const aVec &s, const aVec &d) const;
  virtual double losStop(const aVec &s, const aVec &d) const;
  virtual double losStep(const aVec &s, const aVec &d) const;  
private:
  double X0;
  double T;
  double Z0;
  double X1;
  double R0;
  double B;
  double A;
  void initialize(double x0, double t, double z0, double x1, 
		  double r0, double b);
};

#endif
