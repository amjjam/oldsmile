/*********************************************************************
 * class MODEL3 is a child class of class MODEL which implements the *
 * 3D parabloic magnetopause.                                        *
 *********************************************************************/

#ifndef _MODEL3_H_
#define _MODEL3_H_

#include <math.h>
#include "model.H"

class MODEL3: public MODEL{
public:
  MODEL3();
  MODEL3(double dx, double dy, double dz);
  ~MODEL3();
  virtual double brightness(const aVec &p) const;
  virtual double losStart(const aVec &s, const aVec &d) const;
  virtual double losStop(const aVec &s, const aVec &d) const;
  virtual double losStep(const aVec &s, const aVec &d) const;  
private:
  double dx,dy,dz;
  double xmin,xmax,ymin,ymax,zmin,zmax;
  void initialize(double dx, double dy, double dz);
};

#endif
