/******************************************************************************
 * This program makes several series of Shue images. Each series is stored in *
 * a separate file, and each file has images for different values of r0 and   *
 * alpha.                                                                     *
 ******************************************************************************/

#include <iostream>
#include <string.h>
#include <stdio.h>

#include "../include/orbitSMILE.H"
#include "../include/modelMHD.H"
#include "../include/modelShue.H"
#include "../include/camera1.H"
#include "../include/stringutils.H"

const int nr0=13;
double r0[nr0]={5,5.25,5.5,5.75,6,6.25,6.5,6.75,7,7.25,7.5,7.75,8};
const int nalpha=21;
double alpha[nalpha]={0.3,0.32,0.34,0.36,0.38,0.4,0.42,0.44,0.46,0.48,0.5,0.52,0.54,0.56,0.58,0.6,0.62,0.64,0.66,0.68,0.7};
std::string oFile;
aVec p0(7,0,0); // The point the camera points toward

int main(int argc, char *argv[]){

  MPI_Init(&argc,&argv);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  
  // Load the SMILE orbit
  ORBITSMILE orbit(std::string("../../dat/gsm.txt"));
  MODELMHD mhd(std::string("../../dat/S05D20V600B000005x_xems.dat"));
 
  CAMERA1 camera(25.0/180.0*M_PI,25.0/180.0*M_PI,251,251);
  IMAGE image;

  aTime t;

  for(int i=0;i<3;i++){
    if(i==0){
      t.set(2021,10,1,17,0,0,0);
      oFile="../adat/MHDShue_2021100117.images.gz";
    }
    else if(i==1){
      t.set(2021,10,1,0,0,0,0);
      oFile="../adat/MHDShue_2021100100.images.gz";
    }
    else if(i==2){
      t.set(2021,10,2,11,0,0,0);
      oFile="../adat/MHDShue_2021100211.images.gz";
    }

    // Position in the orbit at time t
    aVec p=orbit.position(t);

    // Set the position and look-direction fo the camera
    camera.setDir(p0-p);
    camera.setPos(p);

    // Open output file
    gzFile fp;
    if(rank==0)
      fp=gzopen(oFile.c_str(),"w9");

    // Snap a MHD model image
    image=camera.snap(mhd);

    // Set the time of the image
    image.setTime(t);

    // Write the image to the file
    image.zwrite(fp);
    
    // Write multiple Shue model images
    for(int j=0;j<nr0;j++)
      for(int k=0;k<nalpha;k++){
	if(rank==0)
	  std::cout << i << " " << j << " " << k << std::endl;
	MODELSHUE shue(1,r0[j],alpha[k]);
	image=camera.snap(shue);
	if(rank==0)
	  image.zwrite(fp);
      }

    if(rank==0)
      gzclose(fp);
  }
  
  MPI_Finalize();

  return 0;
}
