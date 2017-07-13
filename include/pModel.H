/******************************************************************************
 * This is the base class of the model. It can return brightness at a point   *
 * and the integrated brightness along a ray path.                            *
 ******************************************************************************/

#ifndef _PMODEL_H_
#define _PMODEL_H_

#include <vector>
#include "model.H"

class PMODEL: public MODEL{
public:
  PMODEL();
  ~PMODEL();
  virtual std::vector<double> getP() const;
  virtual void setP(std::vector<double> p);
protected:
  std::vector<double> p;  
};

#endif
