/******************************************************************************
 * This is base class ORBIT. It contains information about the satellite      *
 * orbit. For example, it can be used to obtain information about the         *
 * satellite position as a function of time.                                  *
 ******************************************************************************/

#ifndef _ORBIT_H_
#define _ORBIT_H_

#include <aTime.H>
#include <aVec.H>

class ORBIT{
public:
  ORBIT();
  ~ORBIT();
  virtual aVec position(aTime t)=0;
  virtual double period()=0;
  virtual aTime first();
  virtual aTime last();
private:
  
};

#endif
