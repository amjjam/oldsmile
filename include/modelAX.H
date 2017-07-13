/*********************************************************************
 * This is MODELAX - an extension of MODELA for more flexible        *
 * setting and gettig of the parameters.                             *
 *********************************************************************/

#ifndef _MODELAX_H_
#define _MODELAX_H_

#include "modelA.H"

class MODELAX: public MODELA{
public:
  MODELAX(BOUNDARY *mp, BOUNDARY *bs);
  ~MODELAX();
  void pConfig(std::vector<int> bi, std::vector<int> mpi,
	       std::vector<int> bsi);
  virtual void setP(const std::vector<double> pp);
  virtual std::vector<double> getP() const;
private:
  std::vector<int> bi,mpi,bsi;
};

#endif
