#include "../include/modelA.H"

/*=============================================================================
  MODELA() - constructor
  ============================================================================*/
MODELA::MODELA(BOUNDARY *mp, BOUNDARY *bs){  
  MODELA::mp=mp;
  MODELA::bs=bs;

  p.resize(5);
  p[0]=0.01;
  p[1]=2;
  p[2]=0;
  p[3]=0;
  p[4]=0.01;
}


/*=============================================================================
  ~MODELA() - destructor
  ============================================================================*/
MODELA::~MODELA(){
  
  
}


/*=============================================================================
  void setP(std::vector<double> p) - set parameters of the model
  ============================================================================*/
void MODELA::setP(std::vector<double> pp){
  if(pp.size()!=p.size()){
    std::cout << "MODELA: setP(): wrong number of parameters: " 
	      << pp.size() << ", expected " << p.size() << std::endl;
    exit(1);
  }
  p=pp;
}


/*=============================================================================
  std::vector<double> getP() const - get parameters
  ============================================================================*/
std::vector<double> MODELA::getP() const{
  return p;
}


/*=============================================================================
  double brightness(const aVec &p) const - get brightness at a point

  order of parameters: [a1,b,c,d,a2]
  ============================================================================*/
double MODELA::brightness(const aVec &pp) const{
  if(mp->inside(pp))
    return 0;
  double r=length(pp);
  if(!bs->inside(pp))
    return p[4]/r/r/r;
  double costheta=pp.X()/r;
  double sintheta=sin(acos(costheta));
  return p[0]*pow(r,-(p[1]+p[2]*sintheta))*(1.0+p[3]*costheta)/(1+p[3]);
}


/*=============================================================================
  double losStart(const aVec &p, const aVec &d) const
  ============================================================================*/
double MODELA::losStart(const aVec &p, const aVec &d) const{
  return 0;
}


/*=============================================================================
  double losStop(const aVec &p, const aVec &d) const
  ============================================================================*/
double MODELA::losStop(const aVec &p, const aVec &d) const{
  return 50;
}


/*=============================================================================
  double losStep(const aVec &p, const aVec &d) const
  ============================================================================*/
double MODELA::losStep(const aVec &p, const aVec &d) const{
  return 0.01;
}

