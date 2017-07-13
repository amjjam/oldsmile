/*********************************************************************
 * This is program makeImages. It computes a series of images of the *
 * model and writes those to a file.                                 *
 *********************************************************************/

/*=============================================================================
  makeImages [-o <file>] [-model <integer>] [-orbit <intgeger>] \
     [-tstart yr mo dy hr mn se] [-tstop yr mo dy hr mn se] \
     [-dt <float>] [-cycle <float> <float>] [-target <x> <y> <z>]

  -o <file> - output file to write images to
  -model <integer> - model number to use for X-ray emission. If model=5 then 
   a second parameter must be specified like this -model 5 <file> which is the
   name of a file to load the model from. If model=6 then two additional 
   parameters must be specified, r0 and alpha like this -m 6 r0 alpha. Good 
   values are r0=10 and alpha=0.6. If model=7 two additional parameters. First
   a model file same as model 5, next a boundary file for a BOUNDARYE class
   to set zero inside it. 
  -orbit <integer> - model number for satellite orbit
  -tstart yr mo dy hr mn se - start time for recording images. If not 
   specified it is 2021/10/1. If not specified and -cyclic is specified 
   then set it to start time for orbit data.
  -tstop yr mo dy hr mn se - stop time for recording images. If not specified 
   it is start time plus one period. If -cyclic is specified then the default
   is the end of the data set.
  -dt <float> - time between images in seconds
  -cyclic tCycle tRecord - record data in cycles of length tCycle in seconds, 
     and record only for the period tRecord. 
  -target x y z - where to point the look-direction of the camera. The default
   is (X,Y,Z)=(7,0,0)
  -fovTheta <float> Field of view in theta direction in degrees. Default 50.
  -nTheta <integer> Number of pixels in the theta direction. Default 251.
  -fovPhi <float> Field of view in phi direction in degrees. Default 50.
  -nPhi <integer> Number of pixels in the phi direction. Default is 251.
  Overall only record images if the satellite position Z-coordinate is
  greater than 10.
  ============================================================================*/

#include <iostream>
#include <string.h>
#include <stdio.h>

#include "../include/orbit1.H"
#include "../include/orbit2.H"
#include "../include/orbit3.H"
#include "../include/orbitSMILE.H"
#include "../include/model1.H"
#include "../include/model2.H"
#include "../include/model3.H"
#include "../include/model4.H"
#include "../include/modelB.H"
#include "../include/modelMHD.H"
#include "../include/boundaryShue2.H"
#include "../include/boundaryE.H"
#include "../include/boundaryTF.H"
#include "../include/modelShue.H"
#include "../include/camera1.H"
#include "../include/stringutils.H"

void parseArgs(int argc, char *argv[]);

std::string oFile;
int model=1;
int orbit=1;
aTime tStart;
aTime tStop;
double tCycle=1e100;
double tRecord=1e100;
double dt=600;
bool tStartSpecified=false;
bool tStopSpecified=false;
bool cyclicSpecified=false;
std::string modelFile;
std::string boundaryFile;
std::string orbitFile="../../dat/gsm.txt";
double model6R0,model6Alpha;
aVec p0(7,0,0); // The point the camera points toward

// For MODELB:
double a1,b,alpha,beta,a2;
double mpr0,mpalphay,mpalphaz,bsr0,bsalphay,bsalphaz;

// Camera parameters
double fovTheta=50;
double fovPhi=50;
int nTheta=251;
int nPhi=251;

int main(int argc, char *argv[]){

  parseArgs(argc,argv);

  MPI_Init(&argc,&argv);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  
  aTime t0;
  t0.set(2021,10,1);

  ORBIT *o;

  if(orbit==1)
    o=new ORBIT1();
  else if(orbit==2)
    o=new ORBIT2();
  else if(orbit==3)
    o=new ORBIT3(20,2,t0,300.0/180.0*M_PI,80.0/180.0*M_PI,270.0/180.0*M_PI);
  else if(orbit==4)
    o=new ORBITSMILE(orbitFile);
  else{
    std::cout << "Error: unknown orbit number: " 
	      << orbit << std::endl;
    exit(1);
  }

  MODEL *m;
  
  if(model==1)
    m=new MODEL1();
  else if(model==2)
    m=new MODEL2();
  else if(model==3){
    m=new MODEL3();
    p0.setX(0);
    p0.setY(0);
    p0.setZ(0);
  }
  else if(model==4){
    m=new MODEL4();
  }
  else if(model==5||model==7||model==8){
    MODELMHD *mm=new MODELMHD(modelFile,1);
    if(model==7){
      BOUNDARYE *bb=new BOUNDARYE(boundaryFile);
      mm->zeroinside((*bb));
      //delete bb;
    }
    else if(model==8){
      BOUNDARYTF *bb=new BOUNDARYTF(boundaryFile);
      mm->zeroinside((*bb));
      delete bb;
    }
    m=mm;
  }
  else if(model==6){
    m=new MODELSHUE(1,model6R0,model6Alpha);
  }
  else if(model==9){
    BOUNDARYSHUE2 *mp=new BOUNDARYSHUE2(mpr0,mpalphay,mpalphaz);
    BOUNDARYSHUE2 *bs=new BOUNDARYSHUE2(bsr0,bsalphay,bsalphaz);
    MODELB *mb=new MODELB(mp,bs);
    std::vector<double> p=mb->getP();
    p[0]=a1;
    p[1]=b;
    p[2]=alpha;
    p[3]=beta;
    p[4]=a2;
    mb->setP(p);
    m=mb;
  }
  else{
    std::cout << "Error: invalid model number: " << model << std::endl;
    exit(1);
  }
  
  // Set the integration accuracy of the model
  m->setLosNWorkspace(10000);
  m->setLosEps(0,0.01);

  // Set start and stop time
  if(!tStartSpecified){
    if(cyclicSpecified)
      tStart=o->first();
    else
      tStart.set(2021,10,1);
  } 
  if(!tStopSpecified){
    if(cyclicSpecified)
      tStop=o->last();
    else{
      tStop=tStart;
      tStop+=o->period();
    }
  }

  //CAMERA1 c(25.0/180.0*M_PI,25.0/180.0*M_PI,251,251);
  CAMERA1 c(fovTheta/180.0*M_PI,fovPhi/180.0*M_PI,nTheta,nPhi);
  IMAGE img;

  if(orbit==2)
    dt=3600;

  int yr,mo,dy,hr,mn;

  gzFile fp;
  if(rank==0)
    if(oFile.size()>0)
      fp=gzopen(oFile.c_str(),"w9");

  aTime tCycleStart=tStart;
  for(aTime t=tStart;t<tStop;t+=dt){
    if(t-tCycleStart>tRecord){
      tCycleStart+=tCycle;
      t=tCycleStart;
      t-=dt;
      continue;
    }

    aVec p=(*o).position(t);
    
    t.get(yr,mo,dy,hr,mn);

    if(rank==0)
      std::cout << yr << "/" << mo << "/" << dy << " " << hr << ":" 
		<< mn << " " << p << " " << p0-p << std::endl;
    
    if(orbit!=2)
      if(p.Z()<10)
	continue;
      
    // Set position and look-direction of the camera
    c.setDir(p0-p);
    c.setPos(p);
    
    // Recording an image from the camera
    img=c.snap(*m);
    
    // Set the time of the iamge
    img.setTime(t);
    
    // Write image to file (if a file is open)
    if(rank==0)
      if(oFile.size()>0)
	img.zwrite(fp);
  }

  if(rank==0)
    if(oFile.size()>0)
      gzclose(fp);

  MPI_Finalize();

  return 0;
}


void parseArgs(int argc, char *argv[]){

  for(int i=1;i<argc;i++)
    if(strcmp(argv[i],"-h")==0||strcmp(argv[i],"-help")==0||
       strcmp(argv[i],"--help")==0){
      std::cout << "makeImages [-o <file>] [-model <integer>] " 
		<< "[-orbit <intgeger>] \\" << std::endl;
      std::cout << "   [-tstart yr mo dy hr mn se] "
		<< "[-tstop yr mo dy hr mn se] \\"
		<< std::endl;
      std::cout << "   [-dt <float>] [-cycle <float> <float>]" << std::endl;
      std::cout << std::endl;
      std::cout << "-o <file> - output file to write images to" << std::endl;
      std::cout << "-model <integer> - model number to use for X-ray "
		<< "emission. If model=5 then" << std::endl;
      std::cout << " a second parameter must be specified like this "
		<< "-model 5 <file> which is the" << std::endl;
      std::cout << " name of a file to load the model from. If model=6 "
	"then two additional" << std::endl;
      std::cout << " parameters must be specified, r0 and alpha like this "
	"-m 6 r0 alpha. Good" << std::endl; 
      std::cout << " values are r0=10 and alpha=0.6. If model=7 two "
	"additional parameters. First" << std::endl;
      std::cout << " a model file same as model 5, next a boundary file "
	"for a BOUNDARYE class" << std::endl;
      std::cout << " to set zero inside it." << std::endl; 
      std::cout << "-orbit <integer> - model number for satellite orbit" 
		<< std::endl;
      std::cout << "-tstart yr mo dy hr mn se - start time for recording "
		<< "images. If not" << std::endl;
      std::cout << " specified it is 2021/10/1. If not specified and "
		<< "-cyclic is specified" << std::endl;
      std::cout << " then set it to start time for orbit data." << std::endl;
      std::cout << "-tstop yr mo dy hr mn se - stop time for recording "
		<< "images. If not specified" << std::endl; 
      std::cout << " it is start time plus one period. If -cyclic is specified "
		<< "then the default" << std::endl;
      std::cout << " is the end of the data set." << std::endl;
      std::cout << "-dt <float> - time between images in seconds" << std::endl;
      std::cout << "-cyclic tCycle tRecord - record data in cycles of length "
		<< "tCycle in seconds," << std::endl; 
      std::cout << " and record only for the period tRecord." << std::endl;
      std::cout << "-target x y z - where to point the look-direction of " 
		<< "the camera. The default" << std::endl;
      std::cout << " is (X,Y,Z)=(7,0,0)" << std::endl;
      std::cout << std::endl;
      std::cout << "Overall only record images if the satellite position " 
		<< "Z-coordinate is" << std::endl;
      std::cout << "greater than 10." << std::endl;
      exit(0);
    }
      
  for(int i=1;i<argc;i++){
    if(strcmp(argv[i],"-o")==0){
      i++;
      oFile=argv[i];
    }
    else if(strcmp(argv[i],"-model")==0){
      i++;
      model=atoi(argv[i]);
      if(model==5||model==7||model==8){
	i++;
	modelFile=argv[i];
	if(model==7||model==8){
	  i++;
	  boundaryFile=argv[i];
	}
      }
      else if(model==6){
	i++;
	model6R0=atof(argv[i]);
	i++;
	model6Alpha=atof(argv[i]);
      }
      else if(model==9){
	i++;
	a1=atof(argv[i]);
	i++;
	b=atof(argv[i]);
	i++;
	alpha=atof(argv[i]);
	i++;
	beta=atof(argv[i]);
	i++;
	a2=atof(argv[i]);
	i++;
	mpr0=atof(argv[i]);
	i++;
	mpalphay=atof(argv[i]);
	i++;
	mpalphaz=atof(argv[i]);
	i++;
	bsr0=atof(argv[i]);
	i++;
	bsalphay=atof(argv[i]);
	i++;
	bsalphaz=atof(argv[i]);
      }
    }
    else if(strcmp(argv[i],"-orbit")==0){
      i++;
      orbit=atoi(argv[i]);
      if(orbit==4){
	if(argv[i+1][0]!='-'){
	  i++;
	  orbitFile=argv[i];
	}
      }
    }
    else if(strcmp(argv[i],"-tstart")==0){
      int yr,mo,dy,hr,mn,se;
      i++;
      yr=atoi(argv[i]);
      i++;
      mo=atoi(argv[i]);
      i++;
      dy=atoi(argv[i]);
      i++;
      hr=atoi(argv[i]);
      i++;
      mn=atoi(argv[i]);
      i++;
      se=atoi(argv[i]);
      tStart.set(yr,mo,dy,hr,mn,se);
      tStartSpecified=true;
    }
    else if(strcmp(argv[i],"-tstop")==0){
      int yr,mo,dy,hr,mn,se;
      i++;
      yr=atoi(argv[i]);
      i++;
      mo=atoi(argv[i]);
      i++;
      dy=atoi(argv[i]);
      i++;
      hr=atoi(argv[i]);
      i++;
      mn=atoi(argv[i]);
      i++;
      se=atoi(argv[i]);
      tStop.set(yr,mo,dy,hr,mn,se);
      tStopSpecified=true;
    }
    else if(strcmp(argv[i],"-dt")==0){
      i++;
      dt=atof(argv[i]);
    }
    else if(strcmp(argv[i],"-cyclic")==0){
      i++;
      tCycle=atof(argv[i]);
      i++;
      tRecord=atof(argv[i]);
      cyclicSpecified=true;
    }
    else if(strcmp(argv[i],"-target")==0){
      i++;
      p0.setX(atof(argv[i]));
      i++;
      p0.setY(atof(argv[i]));
      i++;
      p0.setZ(atof(argv[i]));
    }
    else if(strcmp(argv[i],"-fovTheta")==0){
      i++;
      fovTheta=atof(argv[i]);
    }
    else if(strcmp(argv[i],"-fovPhi")==0){
      i++;
      fovPhi=atof(argv[i]);
    }
    else if(strcmp(argv[i],"-nTheta")==0){
      i++;
      nTheta=atoi(argv[i]);
    }
    else if(strcmp(argv[i],"-nPhi")==0){
      i++;
      nPhi=atoi(argv[i]);
    }
    else if(argv[i][0]=='-'){
      std::cout << "Unknown option: " << argv[i] << std::endl;
      exit(1);
    }
    else{
      std::cout << "Unknown parameter: " << argv[i] << std::endl;
      exit(1);
    }
  }
}
