/******************************************************************************
 * This is class IMAGE. It contains and image and associated metadata.        *
 ******************************************************************************/

#include "../include/image.H"

#include <iostream>

/*=============================================================================
  IMAGE() - default constructor. Creates a 0 by 0 pixel image
  ============================================================================*/
IMAGE::IMAGE(){
  n=0;
  m=0;
  errhandling=0;
  MPI_Comm_size(MPI_COMM_WORLD,&comm_size);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
}

/*=============================================================================
  IMAGE(int n, int m) - constructor

  int n - the number of pixels in the vertical, theta direction
  int m - the number of pixels in the azimutha, phi direction
  ============================================================================*/
IMAGE::IMAGE(int n, int m){
  IMAGE::n=n;
  IMAGE::m=m;
  errhandling=0;
  MPI_Comm_size(MPI_COMM_WORLD,&comm_size);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  
  data.resize(n*m);
}


/*=============================================================================
  ~IMAGE() - destructor
  ============================================================================*/
IMAGE::~IMAGE(){

}


/*=============================================================================
  int getN() - get the number of pixels in the vertical/theta direction
  ============================================================================*/
int IMAGE::getN(){
  return n;
}


/*=============================================================================
  int getM() - get the number of pixels in the azimuthal/phi direction
  ============================================================================*/
int IMAGE::getM(){
  return m;
}


/*=============================================================================
  void setDir(double roll, double pitch, double yaw) - set the look
  direction angles for the image.
  ============================================================================*/
void IMAGE::setDir(double r, double p, double y){
  roll=r;
  pitch=p;
  yaw=y;
}


/*=============================================================================
  void getDir(double roll, double pitch, double yaw) - get the look
  direction angle for the image
  ============================================================================*/
void IMAGE::getDir(double &r, double &p, double &y){
  r=roll;
  p=pitch;
  y=yaw;
}


/*=============================================================================
  void setPos(aVec p) - set the position of the image
  ============================================================================*/
void IMAGE::setPos(aVec p){
  pos=p;
}


/*=============================================================================
  aVec getPos() - get the position of the image
  ============================================================================*/
aVec IMAGE::getPos(){
  return pos;
}


/*=============================================================================
  void setTime(aTime t) - set the time of the image
  ============================================================================*/
void IMAGE::setTime(aTime t){
  time=t;
}


/*=============================================================================
  aTime getTime() - get the time of the image
  ============================================================================*/
aTime IMAGE::getTime(){
  return time;
}


/*=============================================================================
  void setP(int i, int j, double v) - set the value of a pixel
  ============================================================================*/
void IMAGE::setP(int i, int j, double v){
  data[i*m+j]=v;
}


/*=============================================================================
  double getP(int i, int j) - get the value of a pixel
  ============================================================================*/
double IMAGE::getP(int i, int j){
  return data[i*m+j];
}


/*=============================================================================
  IMAGE &operator=(IMAGE img) - assignment operator for pass by value. 

  This will copy the entire data area at each rank.
  ============================================================================*/
IMAGE &IMAGE::operator=(IMAGE img){
  comm_size=img.comm_size;
  rank=img.rank;
  n=img.n;
  m=img.m;
  roll=img.roll;
  pitch=img.pitch;
  yaw=img.yaw;
  pos=img.pos;
  time=img.time;
  data=img.data;
    
  return *this;
}


/*=============================================================================
  IMAGE &operator*=(double f) - multiply an image by a constant

  Multiply only pixels assigned to the local rank. 
  ============================================================================*/
IMAGE &IMAGE::operator*=(double f){
  int nn=nPixels();
  int j;
  for(int i=0;i<nn;i++){
    j=seqToIndex(i);
    data[j]*=f;
  }
  
  return *this;
}


/*=============================================================================
  int write(FILE *fp) - write the image to a open file pointer

  rank=0 writes what is stored at rank=0 to the open file pointer.
  rank=0 returns number of bytes writen, other ranks return 0.
  ============================================================================*/
int IMAGE::write(FILE *fp){
  if(rank==0){
    int nn=0;
    nn+=fwrite(&n,sizeof(int),1,fp);
    nn+=fwrite(&m,sizeof(int),1,fp);
    nn+=fwrite(&roll,sizeof(double),1,fp);
    nn+=fwrite(&pitch,sizeof(double),1,fp);
    nn+=fwrite(&yaw,sizeof(double),1,fp);
    double x=pos.X(),y=pos.Y(),z=pos.Z();
    nn+=fwrite(&x,sizeof(double),1,fp);
    nn+=fwrite(&y,sizeof(double),1,fp);
    nn+=fwrite(&z,sizeof(double),1,fp);
    int yr,mo,dy,hr,mn,se;
    long ns;
    time.get(yr,mo,dy,hr,mn,se,ns);
    nn+=fwrite(&yr,sizeof(int),1,fp);
    nn+=fwrite(&mo,sizeof(int),1,fp);
    nn+=fwrite(&dy,sizeof(int),1,fp);
    nn+=fwrite(&hr,sizeof(int),1,fp);
    nn+=fwrite(&mn,sizeof(int),1,fp);
    nn+=fwrite(&se,sizeof(int),1,fp);
    nn+=fwrite(&ns,sizeof(long),1,fp);
    nn+=fwrite(&data[0],sizeof(double),n*m,fp);
    
    return nn;
  }

  return 0;
}


/*=============================================================================
  int read(FILE *fp) - write the image to a open file pointer
  ============================================================================*/
int IMAGE::read(FILE *fp){
  int nn=0;
  nn+=fread(&n,sizeof(int),1,fp);
  nn+=fread(&m,sizeof(int),1,fp);
  nn+=fread(&roll,sizeof(double),1,fp);
  nn+=fread(&pitch,sizeof(double),1,fp);
  nn+=fread(&yaw,sizeof(double),1,fp);
  double x,y,z;
  nn+=fread(&x,sizeof(double),1,fp);
  nn+=fread(&y,sizeof(double),1,fp);
  nn+=fread(&z,sizeof(double),1,fp);
  pos=aVec(x,y,z);
  int yr,mo,dy,hr,mn,se;
  long ns;
  nn+=fread(&yr,sizeof(int),1,fp);
  nn+=fread(&mo,sizeof(int),1,fp);
  nn+=fread(&dy,sizeof(int),1,fp);
  nn+=fread(&hr,sizeof(int),1,fp);
  nn+=fread(&mn,sizeof(int),1,fp);
  nn+=fread(&se,sizeof(int),1,fp);
  nn+=fread(&ns,sizeof(long),1,fp);
  time.set(yr,mo,dy,hr,mn,se,ns);
  data.resize(n*m);
  nn+=fread(&data[0],sizeof(double),n*m,fp);

  return nn;
}


/*=============================================================================
  int zwrite(gzFile fp) - write the image to a open file pointer

  rank=0 writes the image stored at rank=0 to a file.
  rank=0 returns number of bytes written, other ranks return 0.
  ============================================================================*/
int IMAGE::zwrite(gzFile fp){
  if(rank==0){
    int nn=0;
    nn+=gzwrite(fp,&n,sizeof(int));
    nn+=gzwrite(fp,&m,sizeof(int));
    nn+=gzwrite(fp,&roll,sizeof(double));
    nn+=gzwrite(fp,&pitch,sizeof(double));
    nn+=gzwrite(fp,&yaw,sizeof(double));
    double x=pos.X(),y=pos.Y(),z=pos.Z();
    nn+=gzwrite(fp,&x,sizeof(double));
    nn+=gzwrite(fp,&y,sizeof(double));
    nn+=gzwrite(fp,&z,sizeof(double));
    int yr,mo,dy,hr,mn,se;
    long ns;
    time.get(yr,mo,dy,hr,mn,se,ns);
    nn+=gzwrite(fp,&yr,sizeof(int));
    nn+=gzwrite(fp,&mo,sizeof(int));
    nn+=gzwrite(fp,&dy,sizeof(int));
    nn+=gzwrite(fp,&hr,sizeof(int));
    nn+=gzwrite(fp,&mn,sizeof(int));
    nn+=gzwrite(fp,&se,sizeof(int));
    nn+=gzwrite(fp,&ns,sizeof(long));
    nn+=gzwrite(fp,&data[0],sizeof(double)*n*m);
    //std::cout << sizeof(int) << " " << sizeof(long) << std::endl;
    return nn;
  }

  return 0;
}


/*=============================================================================
  int zread(gzFile fp) - write the image to a open file pointer

  returns number of bytes read if errandling is 0. If errhandling is 1
  then returns 0 if image was read without problems, and >1 if an
  error occurred. If return value is 1 it was a EOF condition which
  means an iamge was not read because previous image read reached the
  end of the file. If return value is >1 too few bytes were read which
  indicates a more serious problem like file corruption.
  ============================================================================*/
int IMAGE::zread(gzFile fp){
  int nn=0,mm=0;
  nn+=gzread(fp,&n,sizeof(int));
  mm+=sizeof(int);
  if(errhandling==1){
    if(gzeof(fp)==1)
      return 1;
  }
  nn+=gzread(fp,&m,sizeof(int));
  mm+=sizeof(int);
  nn+=gzread(fp,&roll,sizeof(double));
  mm+=sizeof(double);
  nn+=gzread(fp,&pitch,sizeof(double));
  mm+=sizeof(double);
  nn+=gzread(fp,&yaw,sizeof(double));
  mm+=sizeof(double);
  double x,y,z;
  nn+=gzread(fp,&x,sizeof(double));
  mm+=sizeof(double);
  nn+=gzread(fp,&y,sizeof(double));
  mm+=sizeof(double);
  nn+=gzread(fp,&z,sizeof(double));
  mm+=sizeof(double);
  pos=aVec(x,y,z);
  int yr,mo,dy,hr,mn,se;
  long ns;
  nn+=gzread(fp,&yr,sizeof(int));
  mm+=sizeof(int);
  nn+=gzread(fp,&mo,sizeof(int));
  mm+=sizeof(int);
  nn+=gzread(fp,&dy,sizeof(int));
  mm+=sizeof(int);
  nn+=gzread(fp,&hr,sizeof(int));
  mm+=sizeof(int);
  nn+=gzread(fp,&mn,sizeof(int));
  mm+=sizeof(int);
  nn+=gzread(fp,&se,sizeof(int));
  mm+=sizeof(int);
  nn+=gzread(fp,&ns,sizeof(long));
  mm+=sizeof(long);
  if(errhandling==1){
    if(nn!=mm)
      return 2;
  }
  time.set(yr,mo,dy,hr,mn,se,ns);
  data.resize(n*m);
  nn+=gzread(fp,&data[0],sizeof(double)*n*m);
  mm+=sizeof(double)*n*m;
  if(errhandling==1){
    if(nn!=mm)
      return 3;
    else
      return 0;
  }
  return nn;
}


/*=============================================================================
  int maxNPixels() - return the largest number of pixels that any rank
  wille handle. This is the size of the communiated array of pixels.
  ============================================================================*/
int IMAGE::maxNPixels(){
  return n*m/comm_size+1;
}


/*=============================================================================
  int nPixels() - return the number of pixels that this rank will store
  ============================================================================*/
int IMAGE::nPixels(){
  return nPixels(rank);
}


/*=============================================================================
  int nPixels(int rank) - return the number of pixels that a specified
  rank will store
  ============================================================================*/
int IMAGE::nPixels(int rank){
  return (n*m+comm_size-1-rank)/comm_size;
}


/*=============================================================================
  int seqToIndex(int seq) - maps the process rank and pixel sequence
  number to pixel index in the data area
  ============================================================================*/
int IMAGE::seqToIndex(int seq){
  return seqToIndex(rank,seq);
}


/*=============================================================================
  int seqToIndex(rank, int seq) - maps the process rank and pixel
  sequence number to pixel index in the data area
  ============================================================================*/
int IMAGE::seqToIndex(int rank, int seq){
  return rank+seq*comm_size;  
}


/*=============================================================================
  void seqToPixel(int seq, int &i, int &j) - maps from process
  rank and pixel sequence number to pixel coordinates in image. 
  ============================================================================*/
void IMAGE::seqToPixel(int seq, int &i, int &j){
  return seqToPixel(rank,seq,i,j);
}


/*=============================================================================
  void seqToPixel(int rank, int seq, int &i, int &j) - maps from process
  rank and pixel sequence number to pixel coordinates in image. 
  ============================================================================*/
void IMAGE::seqToPixel(int rank, int seq, int &i, int &j){
  int k=seqToIndex(rank,seq);
  i=k/m;
  j=k%m;
}


/*====================================================================
  void gather() - collect all the image pixels at rank 0
  ===================================================================*/
void IMAGE::gather(){
  int nMax=maxNPixels();
  std::vector<double> v(nMax);
  int i,j;
  int n;
  if(rank>0){
    n=nPixels();
    for(i=0;i<n;i++){
      j=seqToIndex(i);
      v[i]=data[j];
    }
    //std::cout << "Rank " << rank << " sending gather()" << std::endl;
    MPI_Send(&v[0],nMax,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
    //std::cout << "Rank " << rank << " sent gather()" << std::endl;
  }
  else{
    int source_rank,k;
    for(i=1;i<comm_size;i++){
      MPI_Status status;
      MPI_Recv(&v[0],nMax,MPI_DOUBLE,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,
	       &status);
      source_rank=status.MPI_SOURCE;
      n=nPixels(source_rank);
      for(j=0;j<n;j++){
	k=seqToIndex(source_rank,j);
	data[k]=v[j];
      }
    }
  }
}


/*=============================================================================
  void setErrHandling(int e) - sets the error handling type. For now
  it only affects the function of zread. If set to 0 then zread
  returns number of bytes read. If set to 1 then zread return 0 for no
  error and >0 if an error occurs.
  ============================================================================*/
void IMAGE::setErrHandling(int errhandling){
  if(errhandling<0||errhandling>1){
    std::cout << "setErrHandling: error: illegal value of errhandling: "
	      << errhandling << ". 0 or 1 permitted." << std::endl;
    exit(1);
  }
  
  IMAGE::errhandling=errhandling;
}


/*=============================================================================
  bcast() - broadcast the image data and meta data from rank 0 to all
  other ranks
  ============================================================================*/


/*=============================================================================
  bcastmeta() - broadcast the meta data only from rank 0 to all other ranks
  ============================================================================*/


/*====================================================================
  double absdiff(IMAGE &i1, IMAGE &i2) - compute the average absolute
  difference between two images.

  The computation is distributed with each rank computing its part of
  the image and then combined. The same end result is returned by all
  ranks.
  ===================================================================*/
double absdiff(IMAGE &i1, IMAGE &i2){
  int n,i,j;
  n=i1.nPixels();
  double sum=0,allsum;
  for(int iSeq=0;iSeq<n;iSeq++){
    i1.seqToPixel(iSeq,i,j);
    sum+=fabs(i1.getP(i,j)-i2.getP(i,j));
  }

  MPI_Allreduce(&sum,&allsum,1,MPI_DOUBLE,MPI_SUM,MPI_COMM_WORLD);

  allsum/=i1.getN()*i1.getM();

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  if(rank==0)
    std::cout << allsum << std::endl;

  return allsum;
}


/*====================================================================
  double rmsdiff(IMAGE &i1, IMAGE &i2) - compute the RMS difference
  between two images.

  The computation is distributed with each rank computing its part of
  the image and then combined. The same end result is returned by all
  ranks.
  ===================================================================*/
double rmsdiff(IMAGE &i1, IMAGE &i2){
  int n,i,j;
  n=i1.nPixels();
  double diff,sum=0,allsum;
  for(int iSeq=0;iSeq<n;iSeq++){
    i1.seqToPixel(iSeq,i,j);
    diff=i1.getP(i,j)-i2.getP(i,j);
    sum+=diff*diff;
  }
  
  MPI_Allreduce(&sum,&allsum,1,MPI_DOUBLE,MPI_SUM,MPI_COMM_WORLD);

  allsum=sqrt(allsum/i1.getN()/i1.getM());

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  if(rank==0)
    std::cout << allsum << std::endl;

  return allsum;
}


/*====================================================================
  double poidiff(IMAGE &i1, IMAGE &i2) - compute the Poisson
  difference between two images. Use image 1 as a mean counts image
  and from it compute the probability of the other image. This
  function returns the negative of the log of the probability. This
  will be a positive and usully large number.

  The computation is distributed with each rank computing its part of
  the image and then combined. The same end result is returned by all
  ranks.
  ===================================================================*/
double poidiff(IMAGE &i1, IMAGE &i2){
  int n,i,j;
  n=i1.nPixels();
  double sum=0,allsum;
  for(int iSeq=0;iSeq<n;iSeq++){
    i1.seqToPixel(iSeq,i,j);

    sum+=log(gsl_ran_poisson_pdf((int)round(i2.getP(i,j)),
				  i1.getP(i,j)));
  }
  
  MPI_Allreduce(&sum,&allsum,1,MPI_DOUBLE,MPI_SUM,MPI_COMM_WORLD);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  if(rank==0)
    std::cout << allsum << std::endl;

  return -allsum;
}


/*=============================================================================
  double average(IMAGE &i) - compute the average value per pixel of an image

  The sum is computed at each rank, summed in a reduceall, and average
  computed and returned at all ranks.
  ============================================================================*/
double average(IMAGE &image){
  int n=image.nPixels();
  
  double sum=0,allsum;
  int j;
  for(int i=0;i<n;i++){
    j=image.seqToIndex(i);
    sum+=image.data[j];
  }
  
  MPI_Allreduce(&sum,&allsum,1,MPI_DOUBLE,MPI_SUM,MPI_COMM_WORLD);
  
  return allsum/(image.n*image.m);
}
