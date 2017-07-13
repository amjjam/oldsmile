/******************************************************************************
 * modelB - model of the brightness in three regions bounded by two           *
 * surfaces, the magnetopause and the bowshock.                               *
 ******************************************************************************/

#ifndef _MODELB_H_
#define _MODELB_H_

#include "pModel.H"
#include "boundary.H"

class MODELB: public PMODEL{
public:
  MODELB(BOUNDARY *mp, BOUNDARY *bs);
  ~MODELB();
  virtual std::vector<double> getP() const;
  virtual void setP(const std::vector<double> p);
  virtual double brightness(const aVec &p) const;
  virtual double losStart(const aVec &s, const aVec &d) const;
  virtual double losStop(const aVec &s, const aVec &d) const;
  virtual double losStep(const aVec &s, const aVec &d) const;
  int region(const aVec &p) const;
protected:
  BOUNDARY *mp;
  BOUNDARY *bs;
};

#endif
