/*********************************************************************
 * This will sample a x-ray emissions model starting at the          *
 * magnetopause, as defined by a model, and outward along radial     *
 * lines. The result is saved in a file.                             *
 *********************************************************************/

/*====================================================================
  
  -if <string> name of file to load MHD data from
  -b set if the input file is binary

  -of <string> name of the output file to write samples to
  
  -mp <string> - name of the magnetopause file to load
  -bs <string> - name of the bowshock file to load

  -t0 <double> theta value for starting (near Sun) default 0.1
  -nt <int> number of theta values default 16
  -dt <double> step between sampling theta values efault 0.1
  -np <int> number of phi values to sample at default 8
  -nr <int> number of radial locations to sample starting at MP 32
  -dr <double> spacing between radial points default 0.25
  
  Results are written to file as 
  long ntheta,nphi,nr
  then ntheta value of theta
  then nphi values of phi
  then values of r, v (double) and q (long)
  r is inner loop, then phi, then theta
  q is the region 0 for inside MP, 1 for magnetosheath, 2 for solar wind.
  ===================================================================*/

#include <math.h>
#include <aVec.H>
#include "../include/modelMHD.H"
#include "../include/boundaryTF.H"
#include <string.h>
#include <string>

std::string mhdfile;
int binary=0;
std::string ofile;
std::string mpfile;
std::string bsfile;

double theta0=0.1;
int ntheta=16;
double dtheta=0.1;
int nphi=8;
double dr=0.25;
int nr=32;

void parseArgs(int argc, char *argv[]){
  for(int i=1;i<argc;i++){
    if(strcmp(argv[i],"-if")==0){
      i++;
      mhdfile=std::string(argv[i]);
    }
    else if(strcmp(argv[i],"-b")==0){
      binary=1;
    }
    else if(strcmp(argv[i],"-of")==0){
      i++;
      ofile=std::string(argv[i]);
    }
    else if(strcmp(argv[i],"-mp")==0){
      i++;
      mpfile=std::string(argv[i]);
    }
    else if(strcmp(argv[i],"-bs")==0){
      i++;
      bsfile=std::string(argv[i]);
    }
    else if(strcmp(argv[i],"-t0")==0){
      i++;
      theta0=atof(argv[i]);
    }
    else if(strcmp(argv[i],"-nt")==0){
      i++;
      ntheta=atoi(argv[i]);
    }
    else if(strcmp(argv[i],"-dt")==0){
      i++;
      dtheta=atof(argv[i]);
    }
    else if(strcmp(argv[i],"-np")==0){
      i++;
      nphi=atoi(argv[i]);
    }
    else if(strcmp(argv[i],"-nr")==0){
      i++;
      nr=atoi(argv[i]);
    }
    else if(strcmp(argv[i],"-dr")==0){
      i++;
      dr=atof(argv[i]);
    }
    else{
      std::cout << "unrecognized option: " << argv[i] << std::endl;
      exit(1);
    }
  }
}

int main(int argc, char *argv[]){
  parseArgs(argc,argv);

  std::cout << "Loading MHD file..." << std::endl;
  MODELMHD mhd(mhdfile,binary);
  std::cout << "Loading MP file..." << std::endl;
  BOUNDARYTF mp(mpfile);
  std::cout << "Loading BS file..." << std::endl;
  BOUNDARYTF bs(bsfile,1);
  std::cout << "Done loading files" << std::endl;

  std::cout << "Zeroing inside MP" << std::endl;
  mhd.zeroinside(mp);
  std::cout << "Done zeroing" << std::endl;

  std::ofstream os(ofile.c_str(),std::ios_base::binary);

  std::cout << ntheta << " " << nphi << " " << nr << std::endl;

  os.write((char*)&ntheta,sizeof(int));
  os.write((char*)&nphi,sizeof(int));
  os.write((char*)&nr,sizeof(int));

  std::cout << ntheta << " " << nphi << " " << nr << std::endl;

  std::vector<double> theta(ntheta);
  for(int itheta=0;itheta<ntheta;itheta++){
    theta[itheta]=theta0+itheta*dtheta;
    os.write((char*)&theta[itheta],sizeof(double));
  }
  std::vector<double> phi(nphi);
  double dphi=2*M_PI/nphi;
  for(int iphi=0;iphi<nphi;iphi++){
    phi[iphi]=dphi*iphi;
    os.write((char*)&phi[iphi],sizeof(double));
  }
  double r,v;
  int q;
  
  for(int itheta=0;itheta<ntheta;itheta++){
    for(int iphi=0;iphi<nphi;iphi++){
      r=mp.r(theta[itheta],phi[iphi]);
      aVec p(r*cos(theta[itheta]),
	     r*sin(theta[itheta])*cos(phi[iphi]),
	     r*sin(theta[itheta])*sin(phi[iphi]));
      aVec dp=unit(p)*dr;
      for(int ir=0;ir<nr;ir++,p+=dp){
	v=mhd.brightness(p);
	//std::cout << v << " " << p << std::endl;
	//theta=acos(p.X()/length(p));
	//phi=atan2(p.Z(),p.Y());
	r=length(p);
	if(mp.inside(p))
	  q=0;
	else if(bs.inside(p))
	  q=1;
	else 
	  q=2;
	os.write((char*)&r,sizeof(double));
	os.write((char*)&v,sizeof(double));
	os.write((char*)&q,sizeof(int));
      }
    }
  }
  
  os.close();

  return 0;
}
