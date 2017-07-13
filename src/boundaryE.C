/******************************************************************************
 * boundaryE. Is a model of a boundary which consists of a series of Elipses  *
 * in YZ plane starting at positive X and going negative.                     *
 * The radius function is not implemented, will exit with error message if it *
 * is called.                                                                 *
 ******************************************************************************/

#include "../include/boundaryE.H"

/*=============================================================================
  BOUNDARYE(std::string file) - constructor which load boundary
  parameters from a file
  file format is binary:
  int n - number of points following
  float x,ry,yz - elipse parameters. 
  points are ordered in decreasing value of x
  ============================================================================*/
BOUNDARYE::BOUNDARYE(std::string file){
  std::ifstream is;
  std::ios_base::openmode mode=std::ios_base::in|std::ios_base::binary;
  is.open(file.c_str(),mode);
  if(is.fail()){
    std::cout << "error: could not open file: " << file << std::endl;
    exit(1);
  }

  int n;
  is.read((char*)&n,sizeof(int));
  if(is.eof()){
    std::cout << "error: end-of-file reading n." << std::endl;
    exit(1);
  }

  x.resize(n);
  ry.resize(n);
  rz.resize(n);

  float tmp;
  int i;

  for(i=n-1;i>=0;i--){
    is.read((char*)&tmp,sizeof(float));
    if(is.eof())
      break;
    x[i]=tmp;
    is.read((char*)&tmp,sizeof(float));
    if(is.eof())
      break;
    ry[i]=tmp;
    is.read((char*)&tmp,sizeof(float));
    if(is.eof())
      break;
    rz[i]=tmp;
  }
  
  if(i>=0){
    std::cout << "error: end-of-file reading data. Only" << (n-1)-i << " of " 
	      << n << " read." << std::endl;
    exit(1);
  }

  is.close();

  // Initialize the interpolation function
  acc=gsl_interp_accel_alloc();
  ty=gsl_spline_alloc(gsl_interp_cspline,x.size());
  tz=gsl_spline_alloc(gsl_interp_cspline,x.size());
  gsl_spline_init(ty,&x[0],&ry[0],x.size());
  gsl_spline_init(tz,&x[0],&rz[0],x.size());
}


/*=============================================================================
  ~BOUNDARYE() - destructor
  ============================================================================*/
BOUNDARYE::~BOUNDARYE(){
  gsl_interp_accel_free(acc);
  gsl_spline_free(ty);
  gsl_spline_free(tz);
}


/*=============================================================================
  r(aVec p) - this function is not currently implemented. If called it
  will bomb.
  ============================================================================*/
double BOUNDARYE::r(aVec p) const{
  
  std::cout << "error: this function is not implemented. Exiting." << std::endl;
  exit(1);
  
  return 0;
}


/*=============================================================================
  bool inside(aVec p) - this function returns true if the point is
  inside the boundary. Specifically it will return true if the nearest
  distance from the x-axis to the point is less than the radius of the
  elipse at that x-value in the that direction. If the supplied point
  has x-value greater than the first x-point or smaller than the last
  x-point then return false.
  ============================================================================*/
bool BOUNDARYE::inside(aVec p) const{
  if(p.X()>=x[x.size()-1])
    return false;
  if(p.X()<=x[0])
    return false;

  // Interpolate the values of ry and rz
  double rry,rrz;
  if(gsl_spline_eval_e(ty,p.X(),acc,&rry)==GSL_EDOM||
     gsl_spline_eval_e(tz,p.X(),acc,&rrz)==GSL_EDOM)
    return false;
  
  double phi=atan2(p.Z(),p.Y());
  double tmp1=rry*cos(phi);
  double tmp2=rrz*sin(phi);
  
  double rr=rry*rrz/sqrt(tmp1*tmp1+tmp2*tmp2);
  
  if(rr*rr>p.Y()*p.Y()+p.Z()*p.Z())
    return true;
  
  return false;
}
