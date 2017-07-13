/*********************************************************************
 * This is class ORBIT1. It implements the simplified noon-midnight  *
 * orbit with constant true anomaly rate which is a approximation to *
 * a real orbit where the true anomaly rate varies with orbital      *
 * position. But it simplifies the calculation and does not          *
 * compromise the basics of the simulation.                          *
 *********************************************************************/

#ifndef _ORBIT1_H_
#define _ORBIT1_H_

#include "orbit.H"

class ORBIT1: public ORBIT{
public:
  ORBIT1();
  ORBIT1(double a, double p, aTime t0);
  ~ORBIT1();
  virtual aVec position(aTime t);
  virtual double period();
private:
  void initialize(double a, double p, aTime t0);
  
  double a; // Apogee distance in Re
  double p; // Perigee distance in Re
  aTime t0; // Time of perigee
  double T; // Orbital period
  double epsilon; // Eccentricity
  double aa; // semi-major axis in Re
  double dtheta; // True anomaly rate in radians per second

};

#endif
