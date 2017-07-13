/******************************************************************************
 * This is class ORBIT2. It implements three consecutive circular orbits.     *
 * The first is in the XZ plane, the second in the YZ plane, and the final    *
 * in the XY plane. The radius of the orbits is 20 Re.                         *
 ******************************************************************************/

#ifndef _ORBIT2_H_
#define _ORBIT2_H_

#include "orbit.H"

class ORBIT2: public ORBIT{
public:
  ORBIT2();
  ORBIT2(double r, aTime t0);
  ~ORBIT2();
  virtual aVec position(aTime t);
  virtual double period();
private:
  void initialize(double r, aTime t0);
  
  double r; // Apogee distance in Re
  aTime t0; // Time of perigee
  double T; // Orbital period
  double T3; // Three times the orbital period
  double dtheta; // True anomaly rate in radians per second

};

#endif
