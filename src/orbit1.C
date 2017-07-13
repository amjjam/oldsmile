/******************************************************************************
 * This is class ORBIT1. It implements the simplified noon-midnight orbit     *
 * with constant true anomaly rate which is a approximation to a real orbit   *
 * where the true anomaly rate varies with orbital position. But it           *
 * simplifies the calculation and does not compromise the basics of the       *
 * simulation.                                                                *
 ******************************************************************************/

#include "../include/orbit1.H"

/*=============================================================================
  ORBIT1() - default constructor

  Initialize with apogee 20 Re and perigee 2 Re and start time July 1,
  2021 at 0 UT
  ============================================================================*/
ORBIT1::ORBIT1(){
  aTime t0;
  t0.set(2021,7,1);
  initialize(20,2,t0);
}


/*=============================================================================
  ORBIT1(double a, double p, aTime t0) - constructor

  a - apogee geocentric distance
  p - perigee geocentric distance
  t0 - time at perigee
  ============================================================================*/
ORBIT1::ORBIT1(double a, double p, aTime t0){
  initialize(a,p,t0);
}


/*=============================================================================
  ~ORBIT1() - destructor
  ============================================================================*/
ORBIT1::~ORBIT1(){

}


/*=============================================================================
  aVec position(aTime t) - get the spacecraft position for a specific time
  ============================================================================*/
aVec ORBIT1::position(aTime t){
  aVec p;
  double dt=t-t0;

  double theta=dtheta*dt;
  double r=aa*(1-epsilon*epsilon)/(1+epsilon*cos(theta));
    
  p.setX(r*sin(theta));
  p.setZ(-r*cos(theta));

  return p;
}


/*=============================================================================
  double period() - orbital period in seconds
  ============================================================================*/
double ORBIT1::period(){
  
  return T;
}


/*=============================================================================
  initialize(double a, double p, aTime t0) - initialize object
  ============================================================================*/
void ORBIT1::initialize(double a, double p, aTime t0){
  ORBIT1::a=a;
  ORBIT1::p=p;
  ORBIT1::t0=t0;

  epsilon=(a-p)/(a+p);

  // Semi-major axis
  aa=(a+p)/2;

  // Compute the orbital period in seconds assuming apogee and
  // perigree distances in Earth radii
  T=2*M_PI*sqrt(aa*aa*aa/1.54e-6);

  // This is the true anomaly rate in radians per second
  dtheta=2*M_PI/T;
}


