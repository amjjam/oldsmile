#include "../include/modelBX.H"


/*=============================================================================
  MODELBX(BOUNDARY *mp, BOUNDARY *bs) - constructor
  ============================================================================*/
MODELBX::MODELBX(BOUNDARY *mp, BOUNDARY *bs):MODELB(mp,bs){
  
}


/*=============================================================================
  ~MODELBX() - destructor
  ============================================================================*/
MODELBX::~MODELBX(){

}


/*=============================================================================
  void pConfig(std::vector<int> bi, std::vector<int> mpi, std::vector<int> bsi)

  std::vector<int> bi, mpi, bsi - are indexes into the parameters
  vector p passed with setP().
  
  bi is for the brightness function of MODELB, mpi is for the
  magnetopause function BOUNDARY, and bsi is for the bow shock
  function BOUNDARY.

  A position in bi, mpi, or bsi is the corresponding position in the
  parameters vector to the brightness function, the magnetopause
  function, or the bow shock function.
  
  e.g. bi[0] is a number which indicates which element of p to MODELB
  should be used as the 0th element in the brightness function. A
  value in p can be referenced in several different locations. If a
  element should not be modified set the corresponding value in bi,
  mpi, or bsi to -1. bi, mpi, and bsi can be shorter than the
  parameters list for their functions but cannot be longer. 

  By definition the number of parmaeters is one more than that largest
  value appearing in any of bi, mpi, and bsi.
  ============================================================================*/
void MODELBX::pConfig(std::vector<int> bi, std::vector<int> mpi, 
		      std::vector<int> bsi){
  
  MODELBX::bi=bi;
  MODELBX::mpi=mpi;
  MODELBX::bsi=bsi;
}


/*=============================================================================
  void setP(std::vector<double> p) - set parameters
  ============================================================================*/
void MODELBX::setP(std::vector<double> pp){
  unsigned int i;

  for(i=0;i<bi.size();i++)
    if(bi[i]>=0)
      p[i]=pp[bi[i]];

  std::vector<double> tmp=mp->getP();
  for(i=0;i<mpi.size();i++)
    if(mpi[i]>=0)
      tmp[i]=pp[mpi[i]];
  mp->setP(tmp);

  tmp=bs->getP();
  for(i=0;i<bsi.size();i++)
    if(bsi[i]>=0)
      tmp[i]=pp[bsi[i]];
  bs->setP(tmp);
}


/*=============================================================================
  std::vector<double> getP() - 
  ============================================================================*/
std::vector<double> MODELBX::getP() const{
  int i;
  int n=bi.size();
  int imax=0;
  // Find the size of the output vector
  for(i=0;i<n;i++)
    if(bi[i]>imax)
      imax=bi[i];
  n=mpi.size();
  for(i=0;i<n;i++)
    if(mpi[i]>imax)
      imax=mpi[i];
  n=bsi.size();
  for(i=0;i<n;i++)
    if(bsi[i]>imax)
      imax=bsi[i];
  std::vector<double> pp(imax+1);

  // Fill the output vector
  n=bi.size();
  for(i=0;i<n;i++)
    pp[bi[i]]=p[i];
  std::vector<double> v=mp->getP();
  n=mpi.size();
  for(i=0;i<n;i++)
    pp[mpi[i]]=v[i];
  v=bs->getP();
  n=bsi.size();
  for(i=0;i<n;i++)
    pp[bsi[i]]=v[i];

  // Return the output vector
  return pp;
}
