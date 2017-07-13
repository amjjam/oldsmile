/******************************************************************************
 * This is the base class pModel. It is an extension of base class MODEL      *
 * because it accepts parameters and therefore can be used in fitting.        *
 ******************************************************************************/

/*=============================================================================
  In addition to defining and initializing as indicated in class
  MODEl, file model.C, the constructor of a child class of PMODEL must
  set a value for nP as well as an initial value for p.
  ============================================================================*/

#include "../include/pModel.H"

/*=============================================================================
  MODEL() - constructor
  ============================================================================*/
PMODEL::PMODEL(){

}


/*=============================================================================
  ~PMODEL() - destructor
  ============================================================================*/
PMODEL::~PMODEL(){

}


/*=============================================================================
  std::vector<double> getP() - return list of parameters.
  ============================================================================*/
std::vector<double> PMODEL::getP() const{
  return p;
}


/*=============================================================================
  void setP(std::vector<double> &p) - set parameters to provided list
  ============================================================================*/
void PMODEL::setP(std::vector<double> p){
  PMODEL::p=p;
}

