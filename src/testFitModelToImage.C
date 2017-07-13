/******************************************************************************
 * This is testFitModelToImage - it creates a MHD image and then proceeds to  *
 * fit a Shue model image to it using fitModelToImage.                        *
 ******************************************************************************/

#include "../include/fitModelToImage.H"
#include "../include/orbitSMILE.H"
#include "../include/modelMHD.H"
#include "../include/modelShue.H"
#include "../include/camera1.H"
#include "../include/fitModelToImage.H"

#include "gsl/gsl_rng.h"
#include "gsl/gsl_randist.h"

void parseArgs(int argc, char *argv[]);

/*=============================================================================
  calling sequence:
  
  testFitModelToImage [-poisson <float>]

  -poisson <float> if supplied make a poisson random image with
   average count per pixel of <float>. Start random seed with default
   value
  ============================================================================*/

double poissonAverage=0;

int main(int argc, char *argv[]){
  parseArgs(argc,argv);

  MPI_Init(&argc,&argv);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);

  // Load the SMILE orbit
  ORBITSMILE orbit(std::string("../../dat/gsm.txt"));
  MODELMHD mhd(std::string("../../dat/S05D20V600B000005x_xems.dat"));
  
  CAMERA1 camera(25.0/180.0*M_PI,25.0/180.0*M_PI,251,251);
  IMAGE image;

  aTime t;

  // Time of truth image
  t.set(2021,10,1,17,0,0,0);

  // Position in the orbit at time t
  aVec p=orbit.position(t);

  // Set the camera position and look-direction
  aVec p0(7,0,0); // point camera here
  camera.setDir(p0-p);
  camera.setPos(p);

  // Open output file
  std::string oFile="../adat/test.images.gz";
  gzFile fp;
  if(rank==0)
    fp=gzopen(oFile.c_str(),"w9");

  // Snap a MHD model image
  image=camera.snap(mhd);
  image.setTime(t);

  // If a noisy image is desired
  if(poissonAverage>0){
    int n=image.getN();
    int m=image.getM();
    int i,j;
    double sum,average;
    for(i=0;i<n;i++)
      for(j=0;j<m;j++)
	sum+=image.getP(i,j);
    average=sum/n/m;
    for(i=0;i<n;i++)
      for(j=0;j<m;j++)
	image.setP(i,j,image.getP(i,j)/average*poissonAverage);
    gsl_rng *r=gsl_rng_alloc(gsl_rng_taus);
    for(i=0;i<n;i++)
      for(j=0;j<m;j++)
	image.setP(i,j,gsl_ran_poisson(r,image.getP(i,j)));
    gsl_rng_free(r);
  }

  // Write image to file
  if(rank==0)
    image.zwrite(fp);

  
  // Create the Shue model
  MODELSHUE shueModel=MODELSHUE(3,7,0.6);

  // Call the fitting function
  int status;
  aTime starttime;
  status=fitModelToImage(image,camera,shueModel);
  aTime stoptime;
 
  // Compute best-fit image and write it to file
  image=camera.snap(shueModel);
  image.setTime(t);
  if(rank==0){
    image.zwrite(fp);
    gzclose(fp);
  }

  // Print the results
  if(rank==0){
    // Print return status
    std::cout << "Status: " << status << std::endl;

    // Print best-fit model parameters
    std::vector<double> params=shueModel.getP();    
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
  for(int i=1;i<argc;i++){
    if(strcmp(argv[i],"-poisson")==0){
      i++;
      poissonAverage=atof(argv[i]);
    }
  }
}
