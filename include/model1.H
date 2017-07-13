/*********************************************************************
 * class MODEL1 is a child class of class MODEL which implements the *
 * 2D parabloic magnetopause.                                        *
 *********************************************************************/

#ifndef _MODEL1_H_
#define _MODEL1_H_

#include <math.h>
#include "model.H"

class MODEL1: public MODEL{
public:
  MODEL1();
  MODEL1(double x0, double t, double z0, double x1, double r0, 
	 double b);
  ~MODEL1();
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
