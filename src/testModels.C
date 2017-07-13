/*********************************************************************
 * This program loads the 0.1 RE MHD model and the boundaryTF model  *
 * twice. Once it is loaded from file. The second is loaded from     *
 * shared memory. Then it pauses to allow the user to enter          *
 * debugger and call functions of the MHD objects.                   *
 *********************************************************************/

#include <time.h>

#include "../include/modelMHD.H"
#include "../include/boundaryTF.H"

int main(int argc, char *argv[]){
  MODELMHD *modelMHD;
  BOUNDARYTF *zeroBoundaryTF,*boundaryTF;

  for(int i=0;i<argc;i++){
    if(strcmp(argv[i],"-modelMHD")==0){
      i++;
      std::cout << "Loading MODELMHD from file " << argv[i] << "..." 
		<< std::endl;
      modelMHD=new MODELMHD(std::string(argv[i]),1);
      if(i+1<argc){
	if(argv[i][0]!='-'){
	  i++;
	  std::cout << "Loading zero BOUNDARYTF from file " << argv[i] << "..."
		    << std::endl;
	  zeroBoundaryTF=new BOUNDARYTF(std::string(argv[i]));
	  std::cout << "Zeroing MODELMHD with BOUNDARYTF..." << std::endl;
	  modelMHD->zeroinside(*zeroBoundaryTF);
	}
      }
    }
    else if(strcmp(argv[i],"-boundaryTF")==0){
      i++;
      std::cout << "Loading BOUNDARYTF from file " << argv[i] << "..."
		<< std::endl;
      boundaryTF=new BOUNDARYTF(std::string(argv[i]));
    }
  }

  std:: cout << "Data read, pausing.... " << std::endl;
  
  sleep(1000000);
  
  return 0;
}

