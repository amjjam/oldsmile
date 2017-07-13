/******************************************************************************
 * class MODEL4 is a child class of class MODEL. It implements a more         *
 * realistic model of the brightness distributio than MODEL2.                 *
 ******************************************************************************/

#include "../include/model4.H"

/*=============================================================================
  This is a 3D model of the brightness between the bow shock and the
  magnetopause as well as in the cusps.

  The magnetopause and bowshocks are each modeled as parabolas. The
  sub-solar distance to the magnetopause is MP0 (default 10 Re), and
  the sub-solar distance to the bow-shock is BS0 (default 15 Re). The 

  The radius of the magnetopause at X=0 is RM0 (default is 12 Re), and
  the radius of the bow-shock at X=0 is RS0 (default is 25 Re).

  The cusps are modeled as cones with the base located on the
  magnetopause and the axis pointing toward the center of the
  Earth. The X-location of the cusp base center is CX0 (default 4 Re),
  the base radius is CR0 (default 1 Re), and the length of the cusp
  cone is CL is 5 Re. 

  Between the magnetopause and the bow shock the brightness is as
  follows. It is X at the magnetopause and then scales as e^{-delta
  X/X0). X0 is 3 Re. This is from X=0 to the subsolar point, and for
  points where the magnetopause is located at X<0 the brightness is
  zero.

  Summary of model parameters
  
  MP0=10  Location of sub-solar point on magnetopause
  BS0=15  Location of sub-solar point on bow shock
  RM0=12  Radius of magnetosphere at X=0
  RS0=25  Radius of bow shock at X=0
  CX0=4   x-location of base of cusps
  CL=5    Length of the cusp
  CR=1   Radius of cusps at magnetopause
  X0=3    Width of magnetopause layer
  ============================================================================*/

/*=============================================================================
  MODEL4() - constructor
  ============================================================================*/
MODEL4::MODEL4(){
  initialize(10,15,12,25,4,5,1,3);
}


/*=============================================================================
  MODEL4(double mp0, double bs0, double rm0, double rs0, double cx0, 
         double cl, double cr, double x0) - constructor
  ============================================================================*/
MODEL4::MODEL4(double mp0, double bs0, double rm0, double rs0, 
	       double cx0, double cl, double cr, double x0){
  initialize(mp0,bs0,rm0,rs0,cx0,cl,cr,x0);

  imp=0;
  icusp=0;
  ibsh=0;
  ipos=0;
}


/*=============================================================================
  ~MODEL4() - destructor
  ============================================================================*/
MODEL4::~MODEL4(){

  std::cout << imp << " " << icusp << " " << ibsh << " " << ipos << std::endl;

}


/*=============================================================================
  double brightness(aVec &p) const - get the brightness at the point p
  ============================================================================*/
double MODEL4::brightness(const aVec &p) const{
  double x=p.X();
  double y=p.Y();
  double z=p.Z();
  
  double ryz2=y*y+z*z,xmp,zz;
  aVec pBase,pPoint,vBase,vP;

  // If inside magnetopause
  if(x<MP0-AMP*ryz2){
    imp++;
    zz=sqrt((MP0-CX0)/AMP);
    pBase.setX(CX0);
    // If inside the cone angle
    for(int i=-1;i<2;i+=2){
      pBase.setZ(zz*i);
      pPoint=pBase-unit(pBase)*CL;
      vBase=unit(pBase-pPoint);
      vP=unit(p-pPoint);
      if(vBase*vP>=COSCANGLE){
	icusp++;
	return MP0;
      }
    }
  }
  // If inside the bowshock
  else if(x<BS0-ABS*ryz2){
    ibsh++;
    xmp=MP0-AMP*ryz2;
    if(xmp>0){
      ipos++;
      return xmp*exp(-(x-xmp)/X0);
    }
  }

  return 0;
}


/*=============================================================================
  double losStart(aVec &s, aVec &d) const - return the start distance along
  the ray to begin integration.
  ============================================================================*/
double MODEL4::losStart(const aVec &s, const aVec &d) const{
  return 0;
}


/*=============================================================================
  double losStop(aVec &s, aVec &d) const - return the stop distance along
  the ray to end integration.
  ============================================================================*/
double MODEL4::losStop(const aVec &s, const aVec &d) const{
  return 50;
}


/*=============================================================================
  double losStep(aVec &s, aVec &d) - return the step size along the ray for integration. 
  ============================================================================*/
double MODEL4::losStep(const aVec &s, const aVec &d) const{
  return 0.01;
}


/*=============================================================================
  void initialize(double mp0, double bs0, double rm0, double rs0, double cx0,
  double cl, double cr, double x0) - initializer called from constructor
  ============================================================================*/
void MODEL4::initialize(double mp0, double bs0, double rm0, double rs0, 
			double cx0, double cl, double cr, double x0){

  MP0=mp0;
  BS0=bs0;
  RM0=rm0;
  RS0=rs0;
  CX0=cx0;
  CL=cl;
  CR=cr;
  X0=x0;
  
  AMP=MP0/RM0/RM0;
  ABS=BS0/RS0/RS0;
  COSCANGLE=cos(atan(CR/CL));
}


