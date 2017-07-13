/******************************************************************************
 * Make an MHD model image and fit MODELAX to it.                             *
 ******************************************************************************/

#include "../include/fitModelToImage.H"
#include "../include/orbitSMILE.H"
#include "../include/modelMHD.H"
#include "../include/modelBX.H"
#include "../include/camera1.H"
#include "../include/fitModelToImage.H"
#include "../include/boundaryShue2.H"

#include "../include/image.H"

#include "../include/poissonImage.H"

#include <string.h>
#include <string>

#include <mpi.h>

void parseArgs(int argc, char *argv[]);
void initialize(int initSelect);

/*=============================================================================
  calling sequence:
  
  testFitModelBToMHDImage [-poisson <float>]

  -poisson <float> if supplied make a poisson random image with
   average count per pixel of <float>. Start random seed with default
   value
  -o <string> output file. If not supplied default is ../adat/test.images.gz
  -c <int> configuration number. 1 for S05D22.5V400B000005 or 2 for 
     S05D20V600B000000. 1 is default. 
  -s <int> random seed for poisson images
  -costfunc <string> abs for absdiff, rms for rmsdiff, poi for poidiff. 
     Default is abs. 
  -loseps <double> <double> epsabs and epsrel for the LOS integration. The 
     largest limit applies, so to ignore one of them set it to zero. 
  -time yr mo dy hr mn se (all ints). Time, used to determine orbit position. 
  ============================================================================*/

std::string mhdFile;
std::string oFile="../adat/test.images.gz";
double poissonAverage=0;
int initSelect=1;
int randomSeed=1;
double epsrel=0.01,epsabs=1e-4;
double (*diff)(IMAGE &i1, IMAGE &i2)=NULL;
double a1,b,c,d,a2;
double mpr0,mpalphay,mpalphaz;
double bsr0,bsalphay,bsalphaz;
aTime t;

int main(int argc, char *argv[]){
  MPI_Init(&argc,&argv);

  // Default time of truth image
  t.set(2021,10,1,17,0,0,0);

  parseArgs(argc,argv);
  
  initialize(initSelect);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);

  // Create the initial MP and BS boundaries
  mpr0=8; mpalphay=0.5805; mpalphaz=0.4082;
  bsr0=10; bsalphay=0.7827; bsalphaz=0.7979;
  BOUNDARYSHUE2 mp(mpr0,mpalphay,mpalphaz);
  BOUNDARYSHUE2 bs(bsr0,bsalphay,bsalphaz);

  // Create the model and set default parameters of the brightness model
  MODELBX model(&mp,&bs);
  std::vector<int> bi,mpi,bsi;
  bi.resize(5); for(int i=0;i<5;i++) bi[i]=i;
  model.pConfig(bi,mpi,bsi);
  std::vector<double> pp=model.getP();
  pp[0]=3.2285e-5; pp[1]=-1.7985e-5; pp[2]=2.4908; pp[3]=-1.6458; pp[4]=1.3588e-5;  
  model.setP(pp);
  
  // Load the SMILE orbit
  ORBITSMILE orbit(std::string("shm:/orbit"));

  // Load the MHD model
  MODELMHD mhd("shm:/mhd",1);
  //mhd.zeroinside(mp);

  // Set the accuracy of the model
  model.setLosEps(epsabs,epsrel);

  // Set up the camera
  CAMERA1 camera(16.0/180.0*M_PI,27.0/180.0*M_PI,76,129);
  IMAGE image;

  // Position in the orbit at time t
  aVec p=orbit.position(t);

  // Set the camera position and look-direction
  aVec p0(9.5,0,0); // point camera here
  camera.setDir(p0-p);
  camera.setPos(p);

  // Open output file
  gzFile fp=NULL;
  if(rank==0)
    fp=gzopen(oFile.c_str(),"w9");

  // Snap a MHD model image
  image=camera.snap(mhd);
  image.setTime(t);
  
  // If a noisy image is desired
  if(poissonAverage>0){
    double avg=average(image);
    image*=poissonAverage/avg;
    image=poissonImage(image,randomSeed);
    
    // Scale the amplitude parameters of the model
    bi.resize(5);
    for(int i=0;i<5;i++) bi[i]=i;
    model.pConfig(bi,mpi,bsi);
    pp=model.getP();
    pp[0]*=poissonAverage/avg;
    pp[4]*=poissonAverage/avg;
    model.setP(pp);
  }
  
  // Write image to file
  image.gather();
  image.zwrite(fp);
  
  // Configure for 6 parameters going to MP model first and BS model second
  bi.clear();
  mpi.resize(3); mpi[0]=0; mpi[1]=1; mpi[2]=2;
  bsi.resize(3); bsi[0]=3; bsi[1]=4; bsi[2]=5;
  model.pConfig(bi,mpi,bsi);
  // pp.resize(6);
  // pp[0]=7.6153; pp[1]=0.50809; pp[2]=0.24529; pp[3]=9.6389; pp[4]=0.7986; pp[5]=0.7108;
  // //pp[0]=6.708; pp[1]=0.5805; pp[2]=0.4082; pp[3]=8.827; pp[4]=0.7827; pp[5]=0.7979;
  // model.setP(pp);
  
  // Call the fitting function
  int status;
  aTime starttime;
  status=fitModelToImage(image,camera,model,diff);
  aTime stoptime;

  // Compute best-fit image and write it to file
  image=camera.snap(model);
  image.setTime(t);
  image.gather();
  image.zwrite(fp);
  if(rank==0)
    gzclose(fp);
  
  // Print the results
  if(rank==0){
    // Print return status
    std::cout << "Status: " << status << std::endl;

    // Print best-fit model parameters
    std::vector<double> params=model.getP();    
    std::cout << "fitted model parameters: ";
    for(unsigned int i=0;i<params.size();i++){
      std::cout << params[i];
      if(i<params.size()-1)
	std::cout << ",";
    }
    std::cout << std::endl;

    // Print fitting time
    std::cout << "Fitting time (s):" << stoptime-starttime 
	      << std::endl;
  }
  
  MPI_Finalize();
  
  return 0;
}
    


void parseArgs(int argc, char *argv[]){
  std::string costFuncString;
  for(int i=1;i<argc;i++){
    if(strcmp(argv[i],"-poisson")==0){
      i++;
      poissonAverage=atof(argv[i]);
    }
    else if(strcmp(argv[i],"-o")==0){
      i++;
      oFile=std::string(argv[i]);
    }
    else if(strcmp(argv[i],"-c")==0){
      i++;
      initSelect=atoi(argv[i]);
    }
    else if(strcmp(argv[i],"-s")==0){
      i++;
      randomSeed=atoi(argv[i]);
    }
    else if(strcmp(argv[i],"-costfunc")==0){
      i++;
      if(strcmp(argv[i],"abs")==0)
	diff=&absdiff;
      else if(strcmp(argv[i],"rms")==0)
	diff=&rmsdiff;
      else if(strcmp(argv[i],"poi")==0)
	diff=&poidiff;
      else{
	std::cout << "unknown -costfunc: " << argv[i] << std::endl;
	exit(1);
      }
      costFuncString=std::string(argv[i]);
    }
    else if(strcmp(argv[i],"-loseps")==0){
      i++;
      epsabs=atof(argv[i]);
      i++;
      epsrel=atof(argv[i]);
    }
    else if(strcmp(argv[i],"-time")==0){
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
      t.set(yr,mo,dy,hr,mn,se,0);
    }
    else{
      std::cout << "parseArgs: unknown option: " << argv[i] << std::endl;
      exit(1);
    }
  }

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  if(rank==0){
    std::cout << "Poisson: " << poissonAverage << std::endl;
    std::cout << "oFile: " << oFile << std::endl;
    std::cout << "initSelect: " << initSelect << std::endl;
    std::cout << "randomSeed: " << randomSeed << std::endl;
    std::cout << "constFunc: " << costFuncString << std::endl;
    std::cout << "losEps: " << epsabs << " " << epsrel << std::endl;
  }
}



void initialize(int i){
  
  if(i==1){ // S05D22.5V400B000005
    mhdFile="../../mhd/adat/S05D22.5V400B000005x";
    a1=0.0028152;
    b=1.9449;
    c=-0.12922;
    d=1.3143;
    a2=0.013249;
    mpr0=7.6153;
    mpalphay=0.50809;
    mpalphaz=0.24529;
    bsr0=9.6389;
    bsalphay=0.7986;
    bsalphaz=0.7108;
  }
  else if(i==2){ // S05D20V600B000000
    mhdFile="../../mhd/adat/S05D20V600B000000x";
    a1=0.011699;
    b=2.4642;
    c=-0.21006;
    d=0.97015;
    a2=0.018039;
    mpr0=6.708;
    mpalphay=0.5805;
    mpalphaz=0.4082;
    bsr0=8.827;
    bsalphay=0.7827;
    bsalphaz=0.7979;
  }
  else{
    std::cout << "initialize(): invalid value of initSelect: " 
	      << i << std::endl;
    exit(1);
  }


}
