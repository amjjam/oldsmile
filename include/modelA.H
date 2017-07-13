/******************************************************************************
 * modelA - model of the brightness in three regions bounded by two           *
 * surfaces, the magnetopause and the bowshock.                               *
 ******************************************************************************/

#ifndef _MODELA_H_
#define _MODELA_H_

#include "pModel.H"
#include "boundary.H"

class MODELA: public PMODEL{
public:
  MODELA(BOUNDARY *mp, BOUNDARY *bs);
  ~MODELA();
  virtual std::vector<double> getP() const;
  virtual void setP(const std::vector<double> p);
  virtual double brightness(const aVec &p) const;
  virtual double losStart(const aVec &s, const aVec &d) const;
  virtual double losStop(const aVec &s, const aVec &d) const;
  virtual double losStep(const aVec &s, const aVec &d) const;
protected:
  BOUNDARY *mp;
  BOUNDARY *bs;
};

#endif
