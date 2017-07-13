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

#include "../include/orbit3.H"

/*=============================================================================
  ORBIT3() - default constructor

  Initialize with apogee 20 Re and perigee 2 Re and start time July 1,
  2021 at 0 UT, Omega=315 degrees,inclination=70 degrees,omega=0 degrees
  ============================================================================*/
ORBIT3::ORBIT3(){
  aTime t0;
  t0.set(2021,7,1);
  initialize(20,2,t0,315./180.*M_PI,70./180.*M_PI,0);
}


/*=============================================================================
  ORBIT3(double a, double p, aTime t0, double Omega, double inclination, 
  double omega) - constructor

  a - apogee geocentric distance
  p - perigee geocentric distance
  t0 - time at perigee
  Omega,inclination, omega - orbital orientation parameters
  ============================================================================*/
ORBIT3::ORBIT3(double a, double p, aTime t0, double Omega, double inclination,
	       double omega){
  initialize(a,p,t0,Omega,inclination,omega);
}


/*=============================================================================
  ~ORBIT3() - destructor
  ============================================================================*/
ORBIT3::~ORBIT3(){

}


/*=============================================================================
  aVec position(aTime t) - get the spacecraft position for a specific time
  ============================================================================*/
aVec ORBIT3::position(aTime t){
  aVec p;
  double dt=t-t0;

  double theta=dtheta*dt;
  double r=aa*(1-epsilon*epsilon)/(1+epsilon*cos(theta));
  
  double x=r*cos(theta),y=r*sin(theta);
  
  p.setX(x*x1+y*y1);
  p.setY(x*x2+y*y2);
  p.setZ(x*x3+y*y3);
    
  return p;
}


/*=============================================================================
  double period() - orbital period in seconds
  ============================================================================*/
double ORBIT3::period(){
  
  return T;
}


/*=============================================================================
  initialize(double a, double p, aTime t0, double Omega, double inclination, 
  double omega) - initialize object
  ============================================================================*/
void ORBIT3::initialize(double a, double p, aTime t0, double Omega, 
			double inclination, double omega){
  ORBIT3::a=a;
  ORBIT3::p=p;
  ORBIT3::t0=t0;

  epsilon=(a-p)/(a+p);

  // Semi-major axis
  aa=(a+p)/2;

  // Compute the orbital period in seconds assuming apogee and
  // perigree distances in Earth radii
  T=2*M_PI*sqrt(aa*aa*aa/1.54e-6);

  // This is the true anomaly rate in radians per second
  dtheta=2*M_PI/T;

  // Transformation matrix
  x1=cos(Omega)*cos(omega)-sin(Omega)*cos(inclination)*sin(omega);
  x2=sin(Omega)*cos(omega)+cos(Omega)*cos(inclination)*sin(omega);
  x3=sin(inclination)*sin(omega);
  y1=-cos(Omega)*sin(omega)-sin(Omega)*cos(inclination)*cos(omega);
  y2=-sin(Omega)*sin(omega)+cos(Omega)*cos(inclination)*cos(omega);
  y3=sin(inclination)*cos(omega);
  z1=sin(inclination)*sin(Omega);
  z2=-sin(inclination)*cos(Omega);
  z3=cos(inclination);
}
