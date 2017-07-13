/******************************************************************************
 * boundaryShue2. Models a Shue-like boundary by modeling two Shue models,    *
 * one in the XY plane and another in the XZ plane and then combining them in *
 * the same way that the major and minor axis radii of a ellipse are combined *
 * This is not completely correct to do but we assume that then when the two  *
 * axis dimensions are not too different it is good enough.                   *
 ******************************************************************************/

#ifndef _BOUNDARYSHUE2_H_
#define _BOUNDARYSHUE2_H_

#include <math.h>
#include <stdlib.h>

#include "boundary.H"

class BOUNDARYSHUE2: public BOUNDARY{
public:
  BOUNDARYSHUE2(double r0, double alphay, double alphaz);
  ~BOUNDARYSHUE2();
  virtual void setP(std::vector<double> p);
  virtual std::vector<double> getP() const;
  virtual double r(aVec p) const;
  virtual bool inside(aVec p) const;
  double r(double theta, double phi) const;
  void set(double r0, double alphay, double alphaz);
  void get(double &r0, double &alphay, double &alphaz) const;
private:
  double r0;
  double alphay;
  double alphaz;
};

#endif
