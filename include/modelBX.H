/*********************************************************************
 * This is MODELBX - an extension of MODELB for more flexible        *
 * setting and getting of the parameters.                            *
 *********************************************************************/

#ifndef _MODELBX_H_
#define _MODELBX_H_

#include "modelB.H"

class MODELBX: public MODELB{
public:
  MODELBX(BOUNDARY *mp, BOUNDARY *bs);
  ~MODELBX();
  void pConfig(std::vector<int> bi, std::vector<int> mpi,
	       std::vector<int> bsi);
  virtual void setP(const std::vector<double> pp);
  virtual std::vector<double> getP() const;
private:
  std::vector<int> bi,mpi,bsi;
};

#endif
