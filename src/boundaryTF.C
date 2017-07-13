/******************************************************************************
 * boundaryTF. Is a model of a boundary which consists of points organized by *
 * theta and phi. A text file, for each value of theta radius at a number of  *
 * values of phi.                                                             *
 ******************************************************************************/

#include "../include/boundaryTF.H"

/*=============================================================================
  BOUNDARYTF(std::string file, int format=0) - constructor

  If file begins with shm:<name> then <name> is a named shared memory
  area to use for the data and ingore format. Otherwise load data from
  file using format.

  BOUNDARYTF(std::string file, std::string name, int format) - constructor
  
  If file is non-empty load data from it. If name is also non-empty
  put data in shared memory name.
  If file is empty connect to shared memory name. 
  
  File format is as follows:

  format 0:
  file format is text:
  int nt,np - dimension of theta and phi direction
  float theta(nt) - theta values
  float phi(nt) - phi values
  float x(nt,np) - x value of points on the MP
  float y(nt,np) - y value of points on the MP
  float z(nt,np) - z value of points on the MP
  float r(nt,np) - radius of point on the MP.
  for the last four arrays nt is the inner loop, np is the outer loop
  format 1
  like format 0 except that there are no x, y, z arrays
  ============================================================================*/

BOUNDARYTF::BOUNDARYTF(std::string source, int format){
  if(strncmp(source.c_str(),"shm:",4)==0)
    initialize(std::string(""),std::string(source.c_str()+4),format);
  else 
    initialize(source,std::string(""),format);
}


BOUNDARYTF::BOUNDARYTF(std::string source, std::string name, int format){
  initialize(source,name,format);
}


/*=============================================================================
  ~BOUNDARYTF() - destructor
  ============================================================================*/
BOUNDARYTF::~BOUNDARYTF(){
  gsl_interp_accel_free(pacc);
  gsl_interp_accel_free(tacc);
  gsl_interp2d_free(interp);

  if(memory==NULL){
    free(t);
    free(p);
    free(radius[0]);
  }
  else{
    munmap(memory,2*sizeof(int)+(nt+np+nt*np)*sizeof(double));
  }
  free(radius);

  if(shmname.size()!=0){
    shm_unlink(shmname.c_str());
  }
}


/*=============================================================================
  r(aVec p) - returns the radial distance to the boundary, by
  computing theta and phi and calling the function which evaluates in
  terms of those quantities.
  ============================================================================*/
double BOUNDARYTF::r(aVec p) const{
  double theta=acos(p.X()/length(p));
  double phi=atan2(p.Z(),p.Y());
  return r(theta,phi);
}


/*=============================================================================
  r(double theta, double phi) - return the radial distance to the
  boundary by interpolating the stored grid.
  ============================================================================*/
double BOUNDARYTF::r(double theta, double phi) const{
  if(phi<0)
    phi+=2*M_PI;
  double rr;
  if(theta<t[0]||theta>t[nt-1]||phi<p[0]||phi>p[np-1])
    return 1e300;
  if(gsl_interp2d_eval_e(interp,p,t,radius[0],phi,theta,pacc,tacc,&rr)
     ==GSL_EDOM)
    return 1e300;
  return rr;
}


/*=============================================================================
  bool inside(aVec p) - this function returns true if the point is
  inside the boundary. It evaluates the radius in the direciton of the
  point. If the radius is larger than the radial distance to the point
  then return true. Otherwise return false.
  ============================================================================*/
bool BOUNDARYTF::inside(aVec p) const{
  if(r(p)>length(p))
    return true;
  return false;
}


void BOUNDARYTF::initialize(std::string file, std::string name, int format){

  if(file.size()>0){
    std::ifstream is;
    std::ios_base::openmode mode=std::ios_base::in;
    is.open(file.c_str(),mode);
    if(is.fail()){
      std::cout << "error: could not open file: " << file << std::endl;
      exit(1);
    }
    
    float fnt,fnp;
    is >> fnt >> fnp;
    nt=round(fnt); np=round(fnp);

    // To aid in interpolation we make the arrays 2 elements longer in
    // the phi direction. Read in starting at element 1 in phi
    // direction until element np in the phi direction. The duplicate
    // element 1 in np+1 and np in 0.
    
    if(name.size()>0){
      if((fd=shm_open(name.c_str(),O_RDWR|O_CREAT|O_TRUNC,S_IRWXU))==-1){
	std::cout << "BOUNDARYTF: error: shm_open returned -1" << std::endl;
	perror("shm_open");
	exit(1);
      }
      shmname=name;
      int sz=2*sizeof(int)+(nt+(np+2)+nt*(np+2))*sizeof(double);
      ftruncate(fd,sz);
      memory=(char *)mmap(NULL,sz,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
      memcpy(memory,&nt,sizeof(int));
      memcpy(memory+sizeof(int),&np,sizeof(int));
      t=(double *)(memory+2*sizeof(int));
      p=t+nt;
      radius=(double **)malloc(sizeof(double *)*nt);
      radius[0]=p+np+2;
    }
    else{
      memory=NULL;
      t=(double *)malloc(nt*sizeof(double));
      p=(double *)malloc((np+2)*sizeof(double));
      
      radius=(double **)malloc(sizeof(double *)*nt);
      radius[0]=(double *)malloc(sizeof(double)*nt*(np+2));
    }
    for(int i=1;i<nt;i++)
      radius[i]=radius[0]+(np+2)*i;
    
    int i; 
    float tmp;
    for(i=0;i<nt;i++){
      is >> tmp;
      if(is.eof())
	break;
      t[i]=tmp/180*M_PI;
    }
    if(i<nt){
      std::cout << "error: reading t only " << i << " of " << nt << " read."
		<< std::endl;
      exit(1);
    }
    
    for(i=0;i<np;i++){
      is >> tmp;
      if(is.eof())
	break;
      p[i+1]=tmp/180*M_PI;
    }
    if(i<np){
      std::cout << "error: reading p only " << i << " of " << np << " read."
		<< std::endl;
      exit(1);
    }
    p[0]=p[np]-2*M_PI;
    p[np+1]=p[1]+2*M_PI;
    
    int nc,j,it,ip;
    char var[5];
    if(format==0){
      nc=4;
      strcpy(var,"xyzr");
    }
    else{
      nc=1;
      strcpy(var,"r");
    }
    for(j=0;j<nc;j++){
      for(ip=0;ip<np;ip++)
	for(it=0;it<nt;it++){
	  is >> tmp;
	  if(is.eof())
	    break;
	  if(j==nc-1)
	    radius[it][ip+1]=tmp;
	}
    }
    if(it<nt||ip<np){
      std::cout << "error: reading " << var[j] << " only got to (it,ip)=(" << it
		<< "," << ip << " out of (nt,np)=(" << nt << "," << np << ")"
		<< std::endl;
      exit(1);
    }
    for(it=0;it<nt;it++){
      radius[it][0]=radius[it][np];
      radius[it][np+1]=radius[it][1];      
    }
    
    is.close();    
  }
  else if(name.size()>0){
    fd=shm_open(name.c_str(),O_RDONLY,S_IRUSR);
    if((memory=(char *)mmap(NULL,2*sizeof(int),PROT_READ,MAP_SHARED,fd,0))==MAP_FAILED){
      std::cout << "BOUNDARYTF: error: MAP_FAILED for first mapping." 
		<< std::endl;
      perror("BOUNDARYTF second mapping: mmap");
      exit(1);
    }
    shmname=name;
    nt=*(int *)memory;
    np=*((int *)memory+1);
    munmap(memory,2*sizeof(int));
    if((memory=(char *)mmap(NULL,2*sizeof(int)+(nt+(np+2)+(np+2)*nt)*sizeof(double),
			    PROT_READ,MAP_SHARED,fd,0))==MAP_FAILED){
      std::cout << "BOUNDARYTF: error: MAP_FAILED for second mapping."
		<< std::endl;
      perror("BOUNDARYTF second mapping: mmap");
      exit(1);
    }
    t=(double *)(memory+2*sizeof(int));
    p=t+nt;
    radius=(double **)malloc(sizeof(double *)*nt);
    radius[0]=(p+(np+2));
    for(int i=1;i<nt;i++)
      radius[i]=radius[0]+(np+2)*i;
  }
  else{
    std::cout << "BOUNDARYTF: error: both file and name are empty." 
	      << std::endl;
    exit(1);
  }
  // Now make np one larger
  np+=2;

  
  // Initialize the interpolation function
  pacc=gsl_interp_accel_alloc();
  tacc=gsl_interp_accel_alloc();
  const gsl_interp2d_type *T=gsl_interp2d_bicubic;
  interp=gsl_interp2d_alloc(T,np,nt);
  gsl_interp2d_init(interp,p,t,radius[0],np,nt);  
}


