/******************************************************************************
 * boundaryShue2. Models a Shue like boundary by modeling two Shue models,    *
 * one in the XY plane and another in the XZ plane and then combining them in *
 * the same way that the major and minor axis radii of a ellipse are combined *
 * This is not completely correct to do but we assume that then when the two  *
 * axis dimensions are not too different it is good enough.                   *
 ******************************************************************************/

#include "../include/boundaryShue2.H"

/*=============================================================================
  BOUNDARYSHUE2(double r0, double alphay, double alphaz) - constructor
  
  double r0 - subsolar point radial distance
  double alphay - exponent in XY plane
  double alphaz - exponent in XZ plane
  ============================================================================*/
BOUNDARYSHUE2::BOUNDARYSHUE2(double r0, double alphay, double alphaz){
  BOUNDARYSHUE2::r0=r0;
  BOUNDARYSHUE2::alphay=alphay;
  BOUNDARYSHUE2::alphaz=alphaz;
  p.resize(3);
  p[0]=r0;
  p[1]=alphay;
  p[2]=alphaz;
}


/*=============================================================================
  ~BOUNDARYSHUE2() - destructor
  ============================================================================*/
BOUNDARYSHUE2::~BOUNDARYSHUE2(){

}


/*=============================================================================
  void setP(std::vector<double> p) - set the parameters of the model
  
  Three paramters in this case: r0, alphay, alphaz
  ============================================================================*/
void BOUNDARYSHUE2::setP(std::vector<double> p){
  if(p.size()!=3){
    std::cout << "BOUNDARYSHUE2: wrong number of parameters: " << p.size()
	      << ". Expected 3 parameters." << std::endl;
    exit(1);
  }
  
  BOUNDARYSHUE2::p=p;
  r0=p[0];
  alphay=p[1];
  alphaz=p[2];
}


/*=============================================================================
  std::vector<double> getP() - get the parameters of the model
  
  three parameters: r0, alphay, alphaz
  ============================================================================*/
std::vector<double> BOUNDARYSHUE2::getP() const{
  return p;
}


/*=============================================================================
  double r(aVec p) - get the radial distance of the boundary in the
  direction aVec from the origin.
  ============================================================================*/
double BOUNDARYSHUE2::r(aVec p) const{
  double theta=acos(p.X()/length(p));
  double phi=atan2(p.Z(),p.Y());
  return r(theta,phi);
}


/*=============================================================================
  bool inside(aVec p) - return true if the point is closer to origin
  than the boundary in that same direction. Otherwise return false. 
  ============================================================================*/
bool BOUNDARYSHUE2::inside(aVec p) const{
  if(length(p)<r(p))
    return true;

  return false;
}


/*=============================================================================
  double r(double theta, double phi) - compute the radius of the
  boundary in direction theta, phi

  double theta - the antle from the X-axis
  double phi - the rotation angle in the right-hand sense around the X-axis 
  starting at the positive Y-axis.

  Procedure: 

  1) Calculate two Shue model radii
  ry=r0*(2/(1+cos(theta)))^alphay
  rz=r0*(2/(1+cos(theta)))^alphaz

  2) Compute radius as radius of ellipse in direction phi
  r=ry*rz/sqrt((rz*cos(phi))^2+(ry*sin(phi))^2)
  ============================================================================*/
double BOUNDARYSHUE2::r(double theta, double phi) const{
  double ct=cos(theta);
  double ry=r0*pow(2/(1+ct),alphay);
  double rz=r0*pow(2/(1+ct),alphaz);
  
  double a=rz*cos(phi);
  double b=ry*sin(phi);

  return ry*rz/sqrt(a*a+b*b);
}


/*=============================================================================
  void set(double r0, double alphay, double alphaz) - set the parameters
  ============================================================================*/
void BOUNDARYSHUE2::set(double r0, double alphay, double alphaz){
  BOUNDARYSHUE2::r0=r0;
  BOUNDARYSHUE2::alphay=alphay;
  BOUNDARYSHUE2::alphaz=alphaz;
}


/*=============================================================================
  void get(double &r0, double &alphay, double &alphaz) - get the parameters
  ============================================================================*/
void BOUNDARYSHUE2::get(double &r0, double &alphay, double &alphaz) const{
  r0=BOUNDARYSHUE2::r0;
  alphay=BOUNDARYSHUE2::alphay;
  alphaz=BOUNDARYSHUE2::alphaz;
}
