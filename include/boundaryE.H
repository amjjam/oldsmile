/******************************************************************************
 * boundaryE. Is a model of a boundary which consists of a series of Elipses  *
 * in YZ plane starting at positive X and going negative.                     *
 * The radius function is not implemented, will exit with error message if it *
 * is called.                                                                 *
 ******************************************************************************/

#ifndef _BOUNDARYE_H_
#define _BOUNDARYE_H_

#include <math.h>
#include <stdlib.h>
#include <fstream>

#include "boundary.H"
#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>
#include <gsl/gsl_interp.h>
#include <gsl/gsl_math.h>

class BOUNDARYE: public BOUNDARY{
public:
  BOUNDARYE(std::string file);
  virtual ~BOUNDARYE();
  virtual double r(aVec p) const;
  virtual bool inside(aVec p) const;
private:
  std::vector<double> x;
  std::vector<double> ry;
  std::vector<double> rz;
  gsl_interp_accel *acc;
  gsl_spline *ty;
  gsl_spline *tz;
};

#endif
