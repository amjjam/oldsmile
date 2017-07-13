/******************************************************************************
 * class MODEL4 is a child class of class MODEL. It implements a more         *
 * realistic model of the brightness distributio than MODEL2.                 *
 ******************************************************************************/

#ifndef _MODEL4_H_
#define _MODEL4_H_

#include <math.h>
#include "model.H"

class MODEL4: public MODEL{
public:
  MODEL4();
  MODEL4(double mp0, double bs0, double rm0, double rs0, 
	 double cx0, double cl, double cr, double x0);
  ~MODEL4();
  virtual double brightness(const aVec &p) const;
  virtual double losStart(const aVec &s, const aVec &d) const;
  virtual double losStop(const aVec &s, const aVec &d) const;
  virtual double losStep(const aVec &s, const aVec &d) const;  
private:
  double MP0;
  double BS0;
  double RM0;;
  double RS0;
  double CX0;
  double CL;
  double CR;
  double X0;
  double AMP;
  double ABS;
  double COSCANGLE;

  mutable int imp,icusp,ibsh,ipos;

  void initialize(double mp0, double bs0, double rm0, double rs0, 
		  double cx0, double cl, double cr, double x0);
};

#endif
