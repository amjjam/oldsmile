/******************************************************************************
 * class MODEL2 is a child class of class MODEL which implements the  3D      *
 * parabloic magnetopause.                                                    *
 ******************************************************************************/

#include "../include/model2.H"

/*=============================================================================
  This is a 3D model of the magnetopause.
  
  The magnetopause is modeled as two parabolas and it is bright
  between them. The sub-solar location of the magnetopause is X0, and
  the thickness at that point is T. The inner and outer edge of the
  magnetopause is modeled as two parabolas with apexes at X0+T/2 and
  X0-T/2. The width of the magnetopause at X=0 is Z0 such that
  A*W^2=X0. The brightness of the magnetoapause is B=X0 for X>0 and 0
  for X<=0.

  Then there are the cusps. They modeled as spheres located at X=X1
  and Y=0 and with radius R0 and with brightness B.

  The default parameters are 

  X0=10
  T=1
  Z0=10
  X1=X0/2
  R0=1
  B=X0
  ============================================================================*/

/*=============================================================================
  MODEL2() - constructor
  ============================================================================*/
MODEL2::MODEL2(){
  initialize(10,1,10,5,1,10);
}


/*=============================================================================
  MODEL2(double x0, double t, doble z0, double x1, double r0, double
  b) - constructor
  ============================================================================*/
MODEL2::MODEL2(double x0, double t, double z0, double x1, double r0, 
	       double b){
  initialize(x0,t,z0,x1,r0,b);
}


/*=============================================================================
  ~MODEL2() - destructor
  ============================================================================*/
MODEL2::~MODEL2(){

}


/*=============================================================================
  double brightness(aVec &p) const - get the brightness at the point p
  ============================================================================*/
double MODEL2::brightness(const aVec &p) const{
  double z,tx,ty,tz;
  
  // Check whether the point is inside the northern cusp disk
  z=sqrt((X0-X1)/A);
  tx=p.X()-X1;
  ty=p.Y();
  tz=p.Z()-z;
  if(sqrt(tx*tx+ty*ty+tz*tz)<R0)
    return B;

  // Check whether the point is inside the southern cusp disk
  z=-sqrt((X0-X1)/A);
  tx=p.X()-X1;
  ty=p.Y();
  tz=p.Z()-z;
  if(sqrt(tx*tx+ty*ty+tz*tz)<R0)
    return B;  

  double x;
  // Check whether the point is inside the magnetopause
  x=X0-A*(p.Z()*p.Z()+p.Y()*p.Y());  
  if(x<=0)
    return 0;
  if(fabs(p.X()-x)<T/2)
    return x;

  return 0;
}


/*=============================================================================
  double losStart(aVec &s, aVec &d) const - return the start distance along
  the ray to begin integration.
  ============================================================================*/
double MODEL2::losStart(const aVec &s, const aVec &d) const{
  return 0;
}


/*=============================================================================
  double losStop(aVec &s, aVec &d) const - return the stop distance along
  the ray to end integration.
  ============================================================================*/
double MODEL2::losStop(const aVec &s, const aVec &d) const{
  return 50;
}


/*=============================================================================
  double losStep(aVec &s, aVec &d) - return the step size along the ray for integration. 
  ============================================================================*/
double MODEL2::losStep(const aVec &s, const aVec &d) const{
  return 0.01;
}


/*=============================================================================
  void initialize(double x0, double t, double z0, double x1, double r0, 
  double b) - initializer called from constructor
  ============================================================================*/
void MODEL2::initialize(double x0, double t, double z0, double x1, double r0, 
			double b){
  X0=x0;
  T=t;
  Z0=z0;
  X1=x1;
  R0=r0;
  B=b;
  
  A=X0/Z0/Z0;
}


