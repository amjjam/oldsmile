
#include "../include/modelMHDSubSolar.H"

/*=============================================================================
  MODELMHDSUBSOLAR() - constructor
  ============================================================================*/
MODELMHDSUBSOLAR::MODELMHDSUBSOLAR(){


}


/*=============================================================================
  ~MODELMHDSUBSOLAR() - destructor
  ============================================================================*/
MODELMHDSUBSOLAR::~MODELMHDSUBSOLAR(){


}


/*=============================================================================
  double brightness(const aVec &p)
  ============================================================================*/
double MODELMHDSUBSOLAR::brightness(const aVec &p){
  double x=p.x();
  
  if(x<0)
    return 0;
  if(x<x1) 
    return x/x1;
  if(x<x2) 
    return 1;
  if(x<x3) 
    return exp((x2-x)/a1);
  return b2*exp((x3-x)/a2);
}


/*=============================================================================
  double losStart(aVec &s, aVec &d) const - return the start distance along
  the ray to begin integration.
  ============================================================================*/
double MODELMHDSUBSOLAR::losStart(const aVec &s, const aVec &d) const{
  return 0;
}


/*=============================================================================
  double losStop(aVec &s, aVec &d) const - return the stop distance along
  the ray to end integration.
  ============================================================================*/
double MODELMHDSUBSOLAR::losStop(const aVec &s, const aVec &d) const{
  return 50;
}

/*=============================================================================
  double losStep(aVec &s, aVec &d) - return the step size along the
  ray for integration.
  ============================================================================*/
double MODELMHDSUBSOLAR::losStep(const aVec &s, const aVec &d) const{
  return 0.01;
}


/*=============================================================================
  initialize() 
  ============================================================================*/
double MODELMHDSUBSOLAR::initialize(){

  // Parameters for the active Sun Tianran MHD run
  x1=0.35;
  x2=1.05;
  x3=2.25;
  
  a1=.9;
  a2=4.3;
  
  b2=exp((x2-x3)/a1);
}
