/******************************************************************************
 * This is class IMAGE. It contains and image and associated metadata.        *
 ******************************************************************************/

#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <aTime.H>
#include <aVec.H>
#include <vector>

#include <stdio.h>
#include <zlib.h>

#include <mpi.h>

#include <gsl/gsl_randist.h>

class IMAGE{
public:
  IMAGE();
  IMAGE(int n, int m);
  ~IMAGE();
  int getN();
  int getM();
  void setDir(double roll, double pitch, double yaw);
  void getDir(double &roll, double &pitch, double &yaw);
  void setPos(aVec p);
  aVec getPos();
  void setTime(aTime t);
  aTime getTime();
  void setP(int i, int j, double v);
  double getP(int i, int j);
  IMAGE &operator=(IMAGE img);
  IMAGE &operator*=(double f);
  int write(FILE *fp);
  int read(FILE *fp);
  int zwrite(gzFile fp);
  int zread(gzFile fp);
  // MPI-related functions:
  int maxNPixels();
  void gather();
  int nPixels();
  int nPixels(int rank);
  void seqToPixel(int seq, int &i, int &j);
  void seqToPixel(int rank, int seq, int &i, int &j);
  void setErrHandling(int errhandling);
  friend double absdiff(IMAGE &i1, IMAGE &i2);
  friend double rmsdiff(IMAGE &i1, IMAGE &i2);
  friend double poidiff(IMAGE &i1, IMAGE &i2);
  friend double average(IMAGE &i);
protected:
  int seqToIndex(int seq);
  int seqToIndex(int rank, int seq);
  int comm_size;
  int rank;
  int n,m;
  double roll,pitch,yaw;
  aVec pos;
  aTime time;
  int errhandling;
  std::vector<double> data;
};

double absdiff(IMAGE &i1, IMAGE &i2);
double rmsdiff(IMAGE &i1, IMAGE &i2);
double poidiff(IMAGE &i1, IMAGE &i2);
double average(IMAGE &i);

#endif
