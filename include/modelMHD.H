/*****************************************************************************
 * This is class MODELMHD. It is a child of class MODEL. It loads xems file  *
 * and interpolates x-ray emission from that file for any point requested.   *
 * if the point is outside the simulation volume then zero is returned.      *
 *****************************************************************************/

#ifndef _MODELMHD_H_
#define _MODELMHD_H_

#include <string>
#include <vector>
#include <fstream>

#include "model.H"
#include "boundary.H"

#include <stdlib.h>
#include <string.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

class MODELMHD: public MODEL{
public:
  MODELMHD(std::string file, int binary=0);
  MODELMHD(std::string file, std::string name, int binary);
  ~MODELMHD();
  virtual double brightness(const aVec &p) const;
  virtual double losStart(const aVec &s, const aVec &d) const;
  virtual double losStop(const aVec &s, const aVec &d) const;
  virtual double losStep(const aVec &s, const aVec &d) const;
  void zeroinside(BOUNDARY &b);
private:
  std::string shmname;
  int fd;
  char *memory;
  unsigned int nx,ny,nz;
  double *x,*y,*z,*p;

  void initialize(std::string file, std::string name, int binary);
  int find(const double *v, const int n, const double vv) const;
  int midpoint(int imin, int imax) const;
  int index(int ix, int iy, int iz) const;
  double trilinear(int ix0, int iy0, int iz0, double xx, double yy, double zz) const;
};

#endif
