/******************************************************************************
 * This is class ORBIT2. It implements three consecutive circular orbits.     *
 * The first is in the XZ plane, the second in the YZ plane, and the final    *
 * in the XY plane. The radius of the orbits is 20 Re.                         *
 ******************************************************************************/

#include "../include/orbit2.H"

/*=============================================================================
  ORBIT2() - default constructor

  Initialize with 20 Re circular orbit and start time July 1, 2021 at
  0 UT
  ============================================================================*/
ORBIT2::ORBIT2(){
  aTime t0;
  t0.set(2021,7,1);
  initialize(20,t0);
}


/*=============================================================================
  ORBIT2(double r, aTime t0) - constructor

  r - radius of orbit in Re
  t0 - start time
  ============================================================================*/
ORBIT2::ORBIT2(double r, aTime t0){
  initialize(r,t0);
}


/*=============================================================================
  ~ORBIT2() - destructor
  ============================================================================*/
ORBIT2::~ORBIT2(){

}


/*=============================================================================
  aVec position(aTime t) - get the spacecraft position for a specific time
  ============================================================================*/
aVec ORBIT2::position(aTime t){
  aVec p;
  double dt=t-t0;
  int i=(int)(dt/T)%3;
  
  double theta=dtheta*dt;
  
  double x=r*cos(theta);
  double y=r*sin(theta);

  if(i==0){
    p.setX(x);
    p.setY(0);
    p.setZ(y);
  }
  else if(i==1){
    p.setX(0);
    p.setY(x);
    p.setZ(y);
  }
  else if(i==2){
    p.setX(x);
    p.setY(y);
    p.setZ(0);
  }

  return p;
}


/*=============================================================================
  double period() - return period of three orbits combined in seconds
  ============================================================================*/
double ORBIT2::period(){
  
  return T3;
}


/*=============================================================================
  initialize(double r, aTime t0) - initialize object
  ============================================================================*/
void ORBIT2::initialize(double r, aTime t0){
  ORBIT2::r=r;
  ORBIT2::t0=t0;
  
  // Compute the orbital period in seconds assuming apogee and
  // perigree distances in Earth radii
  T=2*M_PI*sqrt(r*r*r/1.54e-6);
  T3=3*T;
  
  // This is the true anomaly rate in radians per second
  dtheta=2*M_PI/T;
}


