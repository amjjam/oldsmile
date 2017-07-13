/******************************************************************************
 * This is the base class of the model. It can return brightness at a point   *
 * and the integrated brightness along a ray path.                            *
 ******************************************************************************/

#include "../include/model.H"

/*=============================================================================
  MODEL() - constructor
  ============================================================================*/
MODEL::MODEL(){
  nWorkspace=1000;
  workspace=gsl_integration_workspace_alloc(nWorkspace);
  epsabs=0.1;
  epsrel=0.1;
}


/*=============================================================================
  ~MODEL() - destructor
  ============================================================================*/
MODEL::~MODEL(){
  gsl_integration_workspace_free(workspace);
}


/*=============================================================================
  double los(const aVec &s, const aVec &d) - do a line of sight integral of
  brightness starting at position s in direction d.
  
  aVec &s - starting position
  aVec &d - direction vector
  ============================================================================*/
double MODEL::los(const aVec &s, const aVec &dd) const{
  
  gsl_function F;
  struct MODEL_gsl_params gsl_p;
  gsl_p.start=s;
  gsl_p.dir=unit(dd);
  gsl_p.pt_MODEL=this;
  F.function=&MODEL_gsl_brightness;
  F.params=&gsl_p;
  double start=losStart(gsl_p.start,gsl_p.dir);
  double stop=losStop(gsl_p.start,gsl_p.dir);
  double sum,abserr;
  int error=0;
  if((error=gsl_integration_qag(&F,start,stop,epsabs,epsrel,nWorkspace,
				5,workspace,&sum,&abserr))>0)
    std::cout << "gsl_integration returned " << error << std::cout;
  // aVec dirVec=unit(dd);
  // aVec startVec=s;
  // double p;
  // double step=losStep(startVec,dirVec);
  
  // If using simple integration
  // aVec pos=s-d*0.5;
  // for(sum=0,p=start;p<stop;p+=step){
  //   pos+=d*step;
  //   sum+=step*brightness(pos);
  // }
  // return sum;

  return sum;
}


/*============================================================================
  size_t getLosNWorkspce() const - get the size of the workspace
  ===========================================================================*/
size_t MODEL::getLosNWorkspace() const{
  return nWorkspace;
}


/*============================================================================
  void getLosEps(double &epsabs, double &epsabs) - get the absolute
  and relative maximum error for the LOS integration
  ===========================================================================*/
void MODEL::getLosEps(double &epsabs, double &epsrel) const{
  epsabs=MODEL::epsabs;
  epsrel=MODEL::epsrel;
}


/*============================================================================
  void setLosNWorkspace(size_t nWorkspace) - change the LOS
  integration workspace size
  ===========================================================================*/
void MODEL::setLosNWorkspace(size_t nWorkspace){
  gsl_integration_workspace_free(workspace);
  workspace=gsl_integration_workspace_alloc(nWorkspace);
}


/*===========================================================================
  void MODEL::setLosEps(double epsabs, double epsrel) - set the
  absolute and relative LOS integration errors
  ===========================================================================*/
void MODEL::setLosEps(double epsabs, double epsrel){
  MODEL::epsabs=epsabs;
  MODEL::epsrel=epsrel;
}


/*============================================================================
  double MODEL_gsl_brightness(double x, void *pp) - interface function
  for GSL to the model brightness
  
  double x - position to evaluate the function at
  void *pp - pointer to parameters
  ===========================================================================*/
double MODEL_gsl_brightness(double x, void *pp){
  MODEL_gsl_params *p=(MODEL_gsl_params *) pp;
  return p->pt_MODEL->brightness(p->start+p->dir*x);
}
