/******************************************************************************
 * class MODELSHUE is a child class of class PMODEL. It implements a more     *
 * realistic model of the brightness distributio than MODEL2.                 *
 ******************************************************************************/

#ifndef _MODELSHUE_H_
#define _MODELSHUE_H_

#include <math.h>
#include "pModel.H"

class MODELSHUE: public PMODEL{
public:
  MODELSHUE();
  MODELSHUE(double fac, double x0, double alpha);
  ~MODELSHUE();
  virtual double brightness(const aVec &p) const;
  virtual double magnetopause(double x, double y, double z) const;
  virtual double magnetosheathBrightness(double x) const;
  virtual double losStart(const aVec &s, const aVec &d) const;
  virtual double losStop(const aVec &s, const aVec &d) const;
  virtual double losStep(const aVec &s, const aVec &d) const;  
  void setP(std::vector<double> p);
private:
  double fac;
  double r0;
  double alpha;
  double x1;
  double x2;
  double x3;

  double a1;
  double a2;
  double b2;

  mutable int imp,icusp,ibsh,ipos;

  void initialize(double fac, double r0, double alpha);
};

#endif
