/******************************************************************************
 * This is class ORBIT3. It implements a orbit with arbitrary orientation     *
 * using the usual orbital elements. We use apogee radius and perigee radius  *
 * There are the three angles:                                                *
 * Omega - Longitude of ascending node                                        *
 * i - inclination                                                            *
 * omega - argument of periapsis                                              *
 *                                                                            *
 * The reference direction is the X-axis                                      *
 * The angles Omega, i, and omega are the Eurler angles to rate from the      *
 * XYZ coordinate system to a orbit oriented coordinate system xyz in which   *
 * x points to the periapsis, z is perpendicular to the orbital plane and the *
 * rotation is in the positive direction around the z-axis.                   *
 *                                                                            *
 *                                                                            *
 * Obtaining xyz from XYZ involves rotation by omega around the Z-axis, then  *
 * rotation i around the X-axis, and finally rotation Omega around the Z-axis *
 * We determine the xyz coordinates from the ellipse and then use the         *
 * transform to obtain the coordinates in the XYZ coordinate system.          *
 ******************************************************************************/

#ifndef _ORBIT3_H_
#define _ORBIT3_H_

#include "orbit.H"

class ORBIT3: public ORBIT{
public:
  ORBIT3();
  ORBIT3(double a, double p, aTime t0, double Omega, double inclination, 
	 double omega);
  ~ORBIT3();
  virtual aVec position(aTime t);
  virtual double period();
private:
  void initialize(double a, double p, aTime t0, double Omega, 
		  double inclination, double omega);
  
  double a; // Apogee distance in Re
  double p; // Perigee distance in Re
  aTime t0; // Time of perigee
  double T; // Orbital period
  double epsilon; // Eccentricity
  double aa; // semi-major axis in Re
  double dtheta; // True anomaly rate in radians per second

  double x1,x2,x3,y1,y2,y3,z1,z2,z3;
};

#endif
