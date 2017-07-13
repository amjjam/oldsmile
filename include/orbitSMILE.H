/******************************************************************************
 * This is class ORBITSMILE. It is a chile of class ORBIT. It implements the  *
 * simulated orbit of SMILE. A file is loaded into memory and the values are  *
 * interpolated from those.                                                   *
 ******************************************************************************/

#ifndef _ORBITSMILE_H_
#define _ORBITSMILE_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <aTime.H>
#include <aVec.H>

#include "orbit.H"

#include "stringutils.H"

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

class ORBITSMILE: public ORBIT{
public:
  ORBITSMILE(std::string source);
  ORBITSMILE(std::string file, std::string name);
  ~ORBITSMILE();
  virtual aTime first();
  virtual aTime last();
  virtual aVec position(aTime t);
  virtual double period();
  size_t write(std::string file);
  size_t write(std::string, aTime tStart, aTime tEnd);
private:
  double RE;
  int fd;
  char *memory;
  std::string shmname;
  unsigned int n;
  aTime *t;
  aVec *p;
  //std::vector<aTime> t;
  //std::vector<aVec> p;
  
  void initialize(std::string file, std::string name);
  int midpoint(int imin, int imax); 
};

#endif
