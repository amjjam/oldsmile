/******************************************************************************
 * boundaryTF. Is a model of a boundary which consists of points organized by *
 * theta and phi. A text file, for each value of theta radius at a number of  *
 * values of phi.                                                             *
 ******************************************************************************/

#ifndef _BOUNDARYTF_H_
#define _BOUNDARYTF_H_

#include <math.h>
#include <stdlib.h>
#include <fstream>

#include "boundary.H"
#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>
#include <gsl/gsl_interp.h>
#include <gsl/gsl_interp2d.h>
#include <gsl/gsl_math.h>
#include <string.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

class BOUNDARYTF: public BOUNDARY{
public:
  BOUNDARYTF(std::string file, int format=0);
  BOUNDARYTF(std::string file, std::string name, int format);
  virtual ~BOUNDARYTF();
  virtual double r(aVec p) const;
  virtual double r(double theta, double phi) const;
  virtual bool inside(aVec p) const;
private:
  //std::vector<double> t;
  //std::vector<double> p;
  std::string shmname;
  int fd;
  char *memory;
  int nt;
  int np;
  double *t;
  double *p;
  double **radius; // dimensions [nt][np] (nt is outer loop)
  gsl_interp_accel *pacc;
  gsl_interp_accel *tacc;
  gsl_interp2d *interp;

  void initialize(std::string file, std::string name, int format);
};

#endif
