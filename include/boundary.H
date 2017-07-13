/******************************************************************************
 * This is boundary.H defining BOUNDARY base class.                           *
 ******************************************************************************/

#ifndef _BOUNDARY_H_
#define _BOUNDARY_H_

#include <vector>
#include <aVec.H>

class BOUNDARY{
public:
  BOUNDARY();
  ~BOUNDARY();
  virtual void setP(std::vector<double> p);
  virtual std::vector<double> getP() const;
  virtual double r(aVec p) const =0;
  virtual bool inside(aVec p) const =0;
protected:
  std::vector<double> p;
};
  
#endif
