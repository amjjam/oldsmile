#include "../include/modelB.H"

/*=============================================================================
  MODELB() - constructor
  ============================================================================*/
MODELB::MODELB(BOUNDARY *mp, BOUNDARY *bs){  
  MODELB::mp=mp;
  MODELB::bs=bs;

  p.resize(5);
  p[0]=3.2285e-5;
  p[1]=-1.79847e-5;
  p[2]=2.49080;
  p[3]=-1.64578;
  p[4]=1.35885e-5;
}


/*=============================================================================
  ~MODELB() - destructor
  ============================================================================*/
MODELB::~MODELB(){
  
  
}


/*=============================================================================
  void setP(std::vector<double> p) - set parameters of the model
  ============================================================================*/
void MODELB::setP(std::vector<double> pp){
  if(pp.size()!=p.size()){
    std::cout << "MODELB: setP(): wrong number of parameters: " 
	      << pp.size() << ", expected " << p.size() << std::endl;
    exit(1);
  }
  p=pp;
}


/*=============================================================================
  std::vector<double> getP() const - get parameters
  ============================================================================*/
std::vector<double> MODELB::getP() const{
  return p;
}


/*=============================================================================
  double brightness(const aVec &p) const - get brightness at a point

  order of parameters: [a1,beta,gamma,delta,epsilon,a2]
  
  Model:
  Inside magnetopause:
  0
  Between magnetopause and bow shock:
  a1*(r/rref)^(-(beta+gamma*cos(theta)+delta*sin(theta)))*(1+epsilon*cos(phi))
  Outside bow shock:
  a2*(r/rref)^(-3)

  rref is fixed at 10 Re
  ============================================================================*/
double MODELB::brightness(const aVec &pp) const{
  if(mp->inside(pp))
    return 0;
  double r=length(pp);
  static double rref=10;
  if(!bs->inside(pp))
    return p[4]/pow(r/rref,3);
  double costheta=pp.X()/r;
  double sintheta=sin(acos(costheta));
  double sintheta2=sintheta*sintheta;
  //double sintheta8=sintheta2*sintheta2*sintheta2*sintheta2;
  return (p[0]+p[1]*pow(sintheta2,4))*pow(r/rref,-(p[2]+p[3]*sintheta2));
  //return p[0]*
  //  pow(r/rref,-(p[1]+p[2]*costheta+p[3]*cos2phi))*(1.0+p[4]*costheta);
}


/*=============================================================================
  double losStart(const aVec &p, const aVec &d) const
  ============================================================================*/
double MODELB::losStart(const aVec &p, const aVec &d) const{
  return 0;
}


/*=============================================================================
  double losStop(const aVec &p, const aVec &d) const
  ============================================================================*/
double MODELB::losStop(const aVec &p, const aVec &d) const{
  return 50;
}


/*=============================================================================
  double losStep(const aVec &p, const aVec &d) const
  ============================================================================*/
double MODELB::losStep(const aVec &p, const aVec &d) const{
  return 0.01;
}


/*=============================================================================
  int region(const aVec &p) const - return the region the point is in
  0 for inside MP, 1 for between MP and BS, 2 for outside BS
  ============================================================================*/
int MODELB::region(const aVec &p) const{
  if(mp->inside(p))
    return 0;
  if(bs->inside(p))
    return 1;
  return 2;
}
