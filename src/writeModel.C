/******************************************************************************
 * Sample the model and write an image to a file.                             *
 ******************************************************************************/

/*=============================================================================
  writeModel [-m <int>] [-o <file>] [-x xmin xmax nx] [-y ymin ymax ny]
     [-z zmin zmax nz]
  -m <int> - model number. If model=5 then a file name must also be specified
   like this -m 5 <file> which contains the name of the model file. If model=6
   then two parameters must also be specified like this -m 6 r0 alpha. Good 
   values are r0=10 and alpha=0.6. See Shue magnetopause paper for more. 
  -o <file> - name of output file to write to.
  -x,-y,-z min max n - specify x, y, and z-ranges. Default is -15 15 300
  ============================================================================*/

#include <iostream>
#include <string>
#include <vector>
#include <zlib.h>
#include <stdlib.h>
#include <string.h>

#include <aVec.H>

#include "../include/model.H"
#include "../include/model1.H"
#include "../include/model2.H"
#include "../include/model3.H"
#include "../include/model4.H"
#include "../include/modelMHD.H"
#include "../include/boundaryE.H"
#include "../include/boundaryTF.H"
#include "../include/modelShue.H"

std::string oFile;
int modelNumber=1;
double xmin=-15,xmax=15,ymin=-15,ymax=15,zmin=-15,zmax=15;
int nx=300,ny=300,nz=300;
std::string modelFile;
std::string boundaryFile;
double model6R0=10;
double model6Alpha=0.6;

void parseArgs(int argc, char *argv[]);

int main(int argc, char *argv[]){
  
  parseArgs(argc,argv);
  
  double dx=(xmax-xmin)/nx,dy=(ymax-ymin)/ny,dz=(zmax-zmin)/nz;

  // Create the model object
  MODEL1 m1;
  MODEL2 m2;
  MODEL3 m3;
  MODEL4 m4;
  MODEL *m;

  if(modelNumber==1)
    m=new MODEL1();
  else if(modelNumber==2)
    m=new MODEL2();
  else if(modelNumber==3)
    m=new MODEL3();
  else if(modelNumber==4)
    m=new MODEL4();
  else if(modelNumber==5||modelNumber==7||modelNumber==8){
    MODELMHD *mm=new MODELMHD(modelFile,1);
    if(modelNumber==7){
      BOUNDARYE *bb=new BOUNDARYE(boundaryFile);
      mm->zeroinside((*bb));
    }
    else if(modelNumber==8){
      BOUNDARYTF *bb=new BOUNDARYTF(boundaryFile);
      mm->zeroinside((*bb));
      delete bb;
    }
    m=mm;
  }
  else if(modelNumber==6)
    m=new MODELSHUE(1,model6R0,model6Alpha);
  else{
    std::cout << "Unknown model number: " << modelNumber << std::endl;
    exit(1);
  }

  gzFile fp=gzopen(oFile.c_str(),"w9");
  gzwrite(fp,&xmin,sizeof(double));
  gzwrite(fp,&xmax,sizeof(double));
  gzwrite(fp,&nx,sizeof(int));
  gzwrite(fp,&ymin,sizeof(double));
  gzwrite(fp,&ymax,sizeof(double));
  gzwrite(fp,&ny,sizeof(int));
  gzwrite(fp,&zmin,sizeof(double));
  gzwrite(fp,&zmax,sizeof(double));
  gzwrite(fp,&nz,sizeof(int));
  
  double r;
  aVec p(-5,0,-15);
  for(int i=0;i<nx;i++){
    p.setX(xmin+((double)i+0.5)*dx);
    for(int j=0;j<ny;j++){
      p.setY(ymin+((double)j+0.5)*dy);
      for(int k=0;k<nz;k++){
	p.setZ(zmin+((double)k+0.5)*dz);
	r=(*m).brightness(p);
	
	gzwrite(fp,&r,sizeof(double));
      }
    }
  }
  
  gzclose(fp);
  
  return 0;
}


void parseArgs(int argc, char *argv[]){
  
  for(int i=1;i<argc;i++)
    if(strcmp(argv[i],"-h")==0||strcmp(argv[i],"-help")==0||
       strcmp(argv[i],"--help")==0){
      std::cout << "writeModel [-m <int>] [-o <file>] [-x xmin xmax nx] " 
		<< "[-y ymin ymax ny]" << std::endl;
      std::cout << "   [-z zmin zmax nz]" << std::endl;
      std::cout << "-m <int> - model number. If model=5 then a file name "
		<< "must also be specified" << std::endl;
      std::cout << " like this -m 5 <file> which contains the name of the " 
		<< "model file. If model=6" << std::endl;
      std::cout << " then two parameters must also be specified like this "
	"-m 6 r0 alpha. Good" << std::endl; 
      std::cout << " values are r0=10 and alpha=0.6. See Shue magnetopause "
	"paper for more. " << std::endl;
      std::cout << " If model=7 two additional parameters. First" << std::endl;
      std::cout << " a model file same as model 5, next a boundary file "
	"for a BOUNDARYE class" << std::endl;
      std::cout << " to set zero inside it." << std::endl; 
      std::cout << "-o <file> - name of output file to write to." << std::endl;
      std::cout << "-x,-y,-z min max n - specify x, y, and z-ranges. "
		<< "Default is -15 15 300" << std::endl;
      exit(0);
    }  
  
  for(int i=1;i<argc;i++){
    if(strcmp(argv[i],"-o")==0){
      i++;
      oFile=argv[i];
    }
    else if(strcmp(argv[i],"-m")==0){
      i++;
      modelNumber=atoi(argv[i]);
      if(modelNumber==5||modelNumber==7||modelNumber==8){
	i++;
	modelFile=argv[i];
	if(modelNumber==7||modelNumber==8){
	  i++;
	  boundaryFile=argv[i];
	}
      }
      else if(modelNumber==6){
	i++;
	model6R0=atof(argv[i]);
	i++;
	model6Alpha=atof(argv[i]);
      }
    }
    else if(strcmp(argv[i],"-x")==0){
      i++;
      xmin=atof(argv[i]);
      i++;
      xmax=atof(argv[i]);
      i++;
      nx=atoi(argv[i]);
    }
    else if(strcmp(argv[i],"-y")==0){
      i++;
      ymin=atof(argv[i]);
      i++;
      ymax=atof(argv[i]);
      i++;
      ny=atoi(argv[i]);
    }
    else if(strcmp(argv[i],"-z")==0){
      i++;
      zmin=atof(argv[i]);
      i++;
      zmax=atof(argv[i]);
      i++;
      nz=atoi(argv[i]);
    }
    else if(argv[i][0]=='-'){
      std::cout << "Unknown option " << argv[i] << std::endl;
      exit(1);
    }
    else{
      std::cout << "Unknown parameter " << argv[i] << std::endl;
      exit(1);
    }
  }
}
