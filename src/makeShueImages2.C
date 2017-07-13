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

const int nr0=16;
double r0[nr0]={6,6.1,6.2,6.3,6.4,6.5,6.6,6.7,6.8,6.9,7,7.1,7.2,7.3,7.4,7.5};
const int nalpha=21;
double alpha[nalpha]={0.3,0.32,0.34,0.36,0.38,0.4,0.42,0.44,0.46,0.48,0.5,0.52,0.54,0.56,0.58,0.6,0.62,0.64,0.66,0.68,0.7};
std::string oFile;
aVec p0(7,0,0); // The point the camera points toward

int main(int argc, char *argv[]){

  // Load the SMILE orbit
  ORBITSMILE orbit(std::string("../../dat/gsm.txt"));
  MODELMHD mhd(std::string("../../dat/S05D20V600B000005x_xems.dat"));
 
  CAMERA1 camera(25.0/180.0*M_PI,25.0/180.0*M_PI,51,51);
  IMAGE image;

  aTime t;

  for(int i=0;i<3;i++){
    if(i==0){
      t.set(2021,10,1,17,0,0,0);
      oFile="../adat/MHDShue_2021100117_2.images.gz";
    }
    else if(i==1){
      t.set(2021,10,1,0,0,0,0);
      oFile="../adat/MHDShue_2021100100_2.images.gz";
    }
    else if(i==2){
      t.set(2021,10,2,11,0,0,0);
      oFile="../adat/MHDShue_2021100211_2.images.gz";
    }

    // Position in the orbit at time t
    aVec p=orbit.position(t);

    // Set the position and look-direction fo the camera
    camera.setDir(p0-p);
    camera.setPos(p);

    // Open output file
    gzFile fp=gzopen(oFile.c_str(),"w9");

    // Snap a MHD model image
    image=camera.snap(mhd);

    // Set the time of the image
    image.setTime(t);

    // Write the image to the file
    image.zwrite(fp);
    
    // Write multiple Shue model images
    for(int j=0;j<nr0;j++)
      for(int k=0;k<nalpha;k++){
	std::cout << i << " " << j << " " << k << std::endl;
	MODELSHUE shue(r0[j],alpha[k]);
	image=camera.snap(shue);
	image.zwrite(fp);
      }
    
    gzclose(fp);
  }
  
  return 0;
}
