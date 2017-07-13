/*****************************************************************************
 * This is class MODELMHD. It is a child of class MODEL. It loads xems file  *
 * and interpolates x-ray emission from that file for any point requested.   *
 * if the point is outside the simulation volume then zero is returned.      *
 *****************************************************************************/

#include "../include/modelMHD.H"

/*=============================================================================
  MODELMHD(std::string source, int binary=0) - constructor which loads
  emission brightness from a file. 

  If source begins with shm:<name> then link to shared memory area
  <name>

  MODELMHD(std::string file, std::string name, binary=0) - constructor
  which loads emissions from a file or links to shared memory area

  If file is not empty load from file. If name is also not empty put
  data in shared memory name

  If file is empty then link to shared memory area name
  ============================================================================*/
MODELMHD::MODELMHD(std::string file, int binary){
  if(strncmp(file.c_str(),"shm:",4)==0){
    initialize(std::string(""),std::string(file.c_str()+4),binary);
  }
  else{
    initialize(file,std::string(""),binary);
  }
}


MODELMHD::MODELMHD(std::string file, std::string name, int binary){
  initialize(file,name,binary);
}


/*=============================================================================
  ~MODELMHD() - destructor
  ============================================================================*/
MODELMHD::~MODELMHD(){
  if(memory==NULL){
    free(x);
    free(y);
    free(z);
    free(p);
  }
  else{
    munmap(memory,3*sizeof(int)+(nx+ny+nz+nx*ny*nz)*sizeof(double));
  }

  if(shmname.size()>0){
    shm_unlink(shmname.c_str());
  }
}


/*=============================================================================
  double brightness(const aVec &p) - return brightness at this point
  ============================================================================*/
double MODELMHD::brightness(const aVec &p) const{
  // Get the coordinates
  double xx=p.X(),yy=p.Y(),zz=p.Z();

  // If outside the model volume return zero
  if(xx<x[0]||x[nx-1]<xx)
    return 0;
  if(yy<y[0]||y[ny-1]<yy)
    return 0;
  if(zz<z[0]||z[nz-1]<zz)
    return 0;

  // Find elements
  int ix,iy,iz;
  ix=find(x,nx,xx);
  iy=find(y,ny,yy);
  iz=find(z,nz,zz);

  // Do tri-linear interpolation
  return trilinear(ix,iy,iz,xx,yy,zz);
}


/*=============================================================================
  double losStart(const aVec &s, const aVec &d) - return line of sight
  integration start

  s is the starting position
  d is the look direction
  ============================================================================*/
double MODELMHD::losStart(const aVec &s, const aVec &d) const{
  return 0;
}


/*=============================================================================
  double losStop(const aVec &s, const aVec &d) - return line of sight
  integration stop
  
  s is the starting position
  d is the look direction
  ============================================================================*/
double MODELMHD::losStop(const aVec &s, const aVec &d) const{
  return 100;
}


/*=============================================================================
  double losStep(const aVec &s, const aVec &d) const - get step size
  for line-of-sight integration
  ============================================================================*/
double MODELMHD::losStep(const aVec &s, const aVec &d) const{
  return 0.01;
}


/*=============================================================================
  void zeroinside(BOUNDARY &b) - zero all points which are located
  inside this boundary
  ============================================================================*/
void MODELMHD::zeroinside(BOUNDARY &b){
  aVec v;

  for(unsigned int ix=0;ix<nx;ix++)
    for(unsigned int iy=0;iy<ny;iy++)
      for(unsigned int iz=0;iz<nz;iz++){
	v.setX(x[ix]);
	v.setY(y[iy]);
	v.setZ(z[iz]);
	if(b.inside(v))
	  p[index(ix,iy,iz)]=0;
      }
}


void MODELMHD::initialize(std::string file, std::string name, int binary){
  if(file.size()>0){
    std::ifstream is;
    std::ios_base::openmode mode=std::ios_base::in;
    if(binary)
      mode=mode|std::ios_base::binary;
    is.open(file.c_str(),mode);
    if(is.fail()){
      std::cout << "error: could not open file: " << file << std::endl;
      exit(1);
    }
    
    if(binary){
      is.read((char*)&nx,sizeof(int));
      is.read((char*)&ny,sizeof(int));
      is.read((char*)&nz,sizeof(int));
    }
    else{
      is >> nx;
      is >> ny;
      is >> nz;
    }

    unsigned int n=nx*ny*nz;
    
    if(name.size()>0){
      if((fd=shm_open(name.c_str(),O_RDWR|O_CREAT|O_TRUNC,S_IRWXU))==-1){
	std::cout << "MODELMHD: error: shm_open returnd -1" << std::endl;
	perror("shm_open");
	exit(1);
      }
      shmname=name;
      ftruncate(fd,3*sizeof(int)+(nx+ny+nz+nx*ny*nz)*sizeof(double));
      memory=(char *)mmap(NULL,3*sizeof(int)+(nx+ny+nz+nx*ny*nz)*sizeof(double),
			  PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
      memcpy(memory,&nx,sizeof(int));
      memcpy(memory+sizeof(int),&ny,sizeof(int));
      memcpy(memory+2*sizeof(int),&nz,sizeof(int));
      x=(double *)(memory+3*sizeof(int));
      y=x+nx;
      z=y+nz;
      p=z+nz;
    }
    else{
      memory=NULL;
      x=(double *)malloc(nx*sizeof(double));
      y=(double *)malloc(ny*sizeof(double));
      z=(double *)malloc(nz*sizeof(double));
      p=(double *)malloc(n*sizeof(double));
    }
    
    float tmp;
    unsigned int i;
    
    for(i=0;i<nx;i++){
      if(binary)
	is.read((char*)&tmp,sizeof(float));
      else
	is>>tmp;
      if(is.eof())
	break;
      x[i]=tmp;
    }
    if(i<nx){
      std::cout << "error: reading x only " << i << " of " << nx << " read."
		<< std::endl;
      exit(1);
    }
    
    for(i=0;i<ny;i++){
      if(binary)
	is.read((char*)&tmp,sizeof(float));
      else
	is>>tmp;
      if(is.eof())
	break;
      y[i]=tmp;
    }
    if(i<ny){
      std::cout << "error: reading y only " << i << " of " << ny << " read."
		<< std::endl;
      exit(1);
    }
    
    
    for(i=0;i<nz;i++){
      if(binary)
	is.read((char*)&tmp,sizeof(float));
      else
	is>>tmp;
      if(is.eof())
	break;
      z[i]=tmp;
    }
    if(i<nz){
      std::cout << "error: reading z only " << i << " of " << nz << " read."
		<< std::endl;
      exit(1);
    }
    
    for(i=0;i<n;i++){
      if(binary)
	is.read((char*)&tmp,sizeof(float));
      else
	is>>tmp;
      if(is.eof())
	break;
      p[i]=tmp;
    }
    if(i<n){
      std::cout << "error: reading p only " << i << " of " << n << " read."
		<< std::endl;
      exit(1);
    }
  }
  else if(name.size()>0){
    fd=shm_open(name.c_str(),O_RDONLY,S_IRUSR);
    if((memory=(char *)mmap(NULL,3*sizeof(int),PROT_READ,MAP_SHARED,fd,0))==MAP_FAILED){
      std::cout << "MODELMHD: error: MAP_FAILED for first mapping." 
		<< std::endl;
      perror("MODELMHD: mmap");
      exit(1);
    }
    shmname=name;
    nx=*(int *)memory;
    ny=*((int *)memory+1);
    nz=*((int *)memory+2);
    munmap(memory,3*sizeof(int));
    if((memory=(char *)mmap(NULL,3*sizeof(int)+(nx+ny+nz+nx*ny*nz)
			    *sizeof(double),PROT_READ,MAP_SHARED,fd,0))==MAP_FAILED){
      std::cout << "MODELMHD: error: MAP_FAILED for second mapping."
		<< std::endl;
      exit(1);
    }
    x=(double *)(memory+3*sizeof(int));
    y=x+nx;
    z=y+ny;
    p=z+nz;
  }
  else{
    std::cout << "MODELMHD: error: both file and name are empty." << std::endl;
    exit(1);
  }
}


/*=============================================================================
  int find(const double *v, const int n, const double vv) - find the largest 
    index in vector v (which is in monotonic increasing order) with value 
    less than vv. Only use when v[0]<vv.
  ============================================================================*/
int MODELMHD::find(const double *v, const int n, const double vv) const{
  int imin=0,imax=n-1,imid;
  
  while(imin+1<imax){
    imid=midpoint(imin,imax);
    if(v[imid]>=vv)
      imax=imid;
    if(v[imid]<vv)
      imin=imid;
  }  
  
  return imin;
}


/*=============================================================================
  int midpoint(int imin, int imax) - returns the integer midpoint
  between imin and imax, guaranteeing that it is always < imax as long
  as imin < imax.
  ============================================================================*/
int MODELMHD::midpoint(int imin, int imax) const{
  return (imin+imax)/2;
}


/*=============================================================================
  int index(int ix, int iy, int iz) - find the indices in the linear
  array which corresponds to this coordinate.

  X coordinate varies mostly quickly and Z coordinate most slowly as
  we traverse the array p
  ============================================================================*/
int MODELMHD::index(int ix, int iy, int iz) const{
  return (iz*ny+iy)*nx+ix;
}


/*=============================================================================
  double trilinear(int ix0, int iy0, int iz0, double xx, double yy,
  double zz) - perform trilinear interpolation to get value of p

  int ix0, iy0, iz0 - the grid cells ([0;nx-1] etc) of the points with
  coordinate value just below the values in p.

  Use the algorithme in the wikipedia trilinear interpolation page

  https://en.wikipedia.org/wiki/Trilinear_interpolation
  ============================================================================*/
double MODELMHD::trilinear(int ix0, int iy0, int iz0, 
			   double xx, double yy, double zz) const{

  int ix1,iy1,iz1;
  ix1=ix0+1;
  iy1=iy0+1;
  iz1=iz0+1;

  double x0,x1,y0,y1,z0,z1;
  x0=x[ix0];
  x1=x[ix1];
  y0=y[iy0];
  y1=y[iy1];
  z0=z[iz0];
  z1=z[iz1];

  double xd,yd,zd;
  xd=(xx-x0)/(x1-x0);
  yd=(yy-y0)/(y1-y0);
  zd=(zz-z0)/(z1-z0);

  double mxd;
  double c00,c01,c10,c11;
  mxd=1-xd;
  c00=p[index(ix0,iy0,iz0)]*mxd+p[index(ix1,iy0,iz0)]*xd;
  c10=p[index(ix0,iy1,iz0)]*mxd+p[index(ix1,iy1,iz0)]*xd;
  c01=p[index(ix0,iy0,iz1)]*mxd+p[index(ix1,iy0,iz1)]*xd;
  c11=p[index(ix0,iy1,iz1)]*mxd+p[index(ix1,iy1,iz1)]*xd;

  double myd;
  double c0,c1;
  myd=1-yd;
  c0=c00*myd+c10*yd;
  c1=c01*myd+c11*yd;

  return c0*(1-zd)+c1*zd;
}
