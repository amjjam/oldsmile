/******************************************************************************
 * class MODEL3 is a child class of class MODEL which implements a test box   *
 * model.                                                                     *
 ******************************************************************************/

#include "../include/model3.H"

/*=============================================================================
  This is a 3D model of a box 
  
  The box is centered on the origin and has sizes dx, dy, and dz and
  brightness 1 inside the box and 0 outside.
  ============================================================================*/

/*=============================================================================
  MODEL3() - constructor
  ============================================================================*/
MODEL3::MODEL3(){
  initialize(4,3,1);
}


/*=============================================================================
  MODEL3(double dx, double dy, double dz) - constructor
  ============================================================================*/
MODEL3::MODEL3(double dx, double dy, double dz){
  initialize(dx,dy,dz);
}


/*=============================================================================
  ~MODEL3() - destructor
  ============================================================================*/
MODEL3::~MODEL3(){

}


/*=============================================================================
  double brightness(aVec &p) const - get the brightness at the point p
  ============================================================================*/
double MODEL3::brightness(const aVec &p) const{
  double x=p.X(),y=p.Y(),z=p.Z();
  
  // If the points is inside the box then return 1
  if(xmin<x&&x<xmax&&ymin<y&&y<ymax&&zmin<z&&z<zmax)
    return 1;

  return 0;
}


/*=============================================================================
  double losStart(aVec &s, aVec &d) const - return the start distance along
  the ray to begin integration.
  ============================================================================*/
double MODEL3::losStart(const aVec &s, const aVec &d) const{
  return 0;
}


/*=============================================================================
  double losStop(aVec &s, aVec &d) const - return the stop distance along
  the ray to end integration.
  ============================================================================*/
double MODEL3::losStop(const aVec &s, const aVec &d) const{
  return 50;
}


/*=============================================================================
  double losStep(aVec &s, aVec &d) - return the step size along the ray for integration. 
  ============================================================================*/
double MODEL3::losStep(const aVec &s, const aVec &d) const{
  return 0.01;
}


/*=============================================================================
  void initialize(double dx, double dy, double dz) - initializer
  called from constructor
  ============================================================================*/
void MODEL3::initialize(double dx, double dy, double dz){

  MODEL3::dx=dx;
  MODEL3::dy=dy;
  MODEL3::dz=dz;
  xmin=-dx/2;
  xmax=dx/2;
  ymin=-dy/2;
  ymax=dy/2;
  zmin=-dz/2;
  zmax=dz/2;
}


