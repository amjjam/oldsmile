/******************************************************************************
 * class MODELSHUE is a child class of class MODEL. It implements a more      *
 * realistic model of the brightness distributio than MODEL2.                 *
 ******************************************************************************/

#include "../include/modelShue.H"

/*=============================================================================
  This is a 3D model of the brightness distribution outside the
  magnetopause using the Shue model for the magnetopause shape and
  using a fit to Sun Tianran's MHD run at the sub-solar point as the
  brightness distribution relative to the magnetopause. There are no
  cusps in this model.

  The model parameters are those of the Shue model, 
  ============================================================================*/

/*=============================================================================
  MODELSHUE() - constructor
  ============================================================================*/
MODELSHUE::MODELSHUE(){
  initialize(1,10,0.6);
}


/*=============================================================================
  MODELSHUE(double fac, double x0, double alpha) - constructor
  ============================================================================*/
MODELSHUE::MODELSHUE(double fac, double x0, double alpha){
  initialize(fac,x0,alpha);
}


/*=============================================================================
  ~MODELSHUE() - destructor
  ============================================================================*/
MODELSHUE::~MODELSHUE(){

}


/*=============================================================================
  double brightness(aVec &p) const - get the brightness at the point p
  ============================================================================*/
double MODELSHUE::brightness(const aVec &p) const{
  double x=p.X();
  double y=p.Y();
  double z=p.Z();

  return magnetosheathBrightness(sqrt(x*x+y*y+z*z)-magnetopause(x,y,z));
  
  double rmp=magnetopause(x,y,z);

  double r2yz=y*y+z*z;
  double r2=x*x+r2yz;

  // If inside the magnetopause then return 0
  if(r2<rmp*rmp)
    return 0;
  
  // Find the X-coordinate of the magnetopause at the same (Y,Z)
  double xp=x;
  
  for(xp=x;rmp*rmp<r2&&xp>0;xp-=0.05){
    rmp=magnetopause(xp,y,z);
    r2=xp*xp+r2yz;
    //std::cout << rmp*rmp << " " << r2 << std::endl;
  }
  
  // double xpmin=-100;
  // double xpmax=100;
  
  // for(;dx>0.05;){
  //   rp=magnetopause(xp,y,z);
  //   newxp=xp*rp/sqrt(xp*xp+y*y+z*z);
  //   dx=xp-newxp;
  //   xp=newxp;
  // }
 
  return magnetosheathBrightness(x-xp);
}

/*=============================================================================
  double magnetopause(double x, double y, double z) - return radial
  distance to the magnetopause in the direction (x,y,z)
  ============================================================================*/
double MODELSHUE::magnetopause(double x, double y, double z) const{
  double r2=x*x+y*y+z*z;
  double costheta=x/sqrt(r2);
  return r0*pow(2/(1+costheta),alpha);
}

/*=============================================================================
  double magnetosheathBrightness(double x) - return brightness as a
  function of upstream distance from magnetopause.
  ============================================================================*/
double MODELSHUE::magnetosheathBrightness(double x) const{
  if(x<0)
    return 0;
  if(x<x1) 
    return fac*x/x1;
  if(x<x2) 
    return fac;
  if(x<x3) 
    return fac*exp((x2-x)/a1);
  return fac*b2*exp((x3-x)/a2);
}


/*=============================================================================
  double losStart(aVec &s, aVec &d) const - return the start distance along
  the ray to begin integration.
  ============================================================================*/
double MODELSHUE::losStart(const aVec &s, const aVec &d) const{
  return 0;
}


/*=============================================================================
  double losStop(aVec &s, aVec &d) const - return the stop distance along
  the ray to end integration.
  ============================================================================*/
double MODELSHUE::losStop(const aVec &s, const aVec &d) const{
  return 50;
}


/*=============================================================================
  double losStep(aVec &s, aVec &d) - return the step size along the ray for integration. 
  ============================================================================*/
double MODELSHUE::losStep(const aVec &s, const aVec &d) const{
  return 0.01;
}


/*=============================================================================
  void initialize(double fac, double r0, double alpha) - initializer
  called from constructor
  ============================================================================*/
void MODELSHUE::initialize(double fac, double r0, double alpha){

  // Model parameters for fitting
  std::vector<double> p;
  p.resize(3);
  p[0]=fac;
  p[1]=r0;
  p[2]=alpha;
  setP(p);
  
  // Model parameters for X-ray emissivity shape
  // Parameters for the quiet Sun Tianran MHD run
  // x1=0.4;
  // x2=3.0;
  // x3=4.3;
  
  // a1=1.05;
  // a2=5.5;
  
  // Parameters for the active Sun Tianran MHD run
  x1=0.35;
  x2=1.05;
  x3=2.25;
  
  a1=.9;
  a2=4.3;
  
  b2=exp((x2-x3)/a1);
}


/*=============================================================================
  void setP(std::vector<double> p) - set the parameters of the model
  ============================================================================*/
void MODELSHUE::setP(std::vector<double> p){
  MODELSHUE::p=p;
  fac=p[0];
  r0=p[1];
  alpha=p[2];
}
