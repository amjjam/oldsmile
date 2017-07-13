/******************************************************************************
 * This program loads data into named shared memory spaces, or erases those   *
 * shared memory spaces.                                                      *
 ******************************************************************************/

/*=============================================================================
  loadData [-orbitSMILE <file> [<name>]] 
     [-modelMHD <file> [<format> [<name> [<file2>]]]] 
     [-boundaryTF <file> [<format> [<name>]]]
  
  -orbitSMILE <file> <name> This creates a named shared memory space called
   <name> (default ORBITSMILE) which contains the data for object of class 
   ORBITSMILE loaded from <file>.
  -modelMHD <file> <format> <name> <file2> This creates a named shared memory 
   space called <name> (default MODELMHD) which contains the data for a object 
   of class MODELMHD loaded from <file> in format <format> (default 0), see 
   modelMHD.H. <file2> is the name of a optional boundary file. The volume 
   inside the boundary is zeroed if it is supplied.  
  -boundaryTF <file> <format> <name> This creates a named shared memory space 
   called <name> (default BOUNDARYTF) which contains the data for a object of
   class BOUNDARYTF loaded from <file> in format <format> (default is 0), see
   boundaryTF.H 
  -delete <name> This deletes the shared memory space <name>. Shared memory 
   space can also be deleted by removing the corresponding file in the 
   filesystem.
  Each can be specified multiple times. If the shared memory area <name> already
  exists it will be overwritten with the new data. 
  ============================================================================*/

#include "../include/orbitSMILE.H"
#include "../include/modelMHD.H"
#include "../include/boundaryTF.H"

int main(int argc, char *argv[]){  
  int i;
  for(i=1;i<argc;i++){
    if(strcmp(argv[i],"-orbitSMILE")==0){
      std::string orbitFile;
      std::string orbitName="ORBITSMILE";
      i++;
      orbitFile=argv[i];
      if(argv[i+1][0]!='-'){
	i++;
	orbitName=argv[i];
      }
      std::cout << "Loading ORBITSMILE file " << orbitFile 
		<< " into shared memory " << orbitName << std::endl;
      ORBIT *o=new ORBITSMILE(orbitFile,orbitName);
      aTime first=o->first(),last=o->last();
      int fyr,fmo,fdy,lyr,lmo,ldy;
      first.get(fyr,fmo,fdy);
      last.get(lyr,lmo,ldy);
      std::cout << "ORBITSMILE first day: " << fyr << "/" << fmo << "/" << fdy 
		<< " ,last day: " << lyr << "/" << lmo << "/" << ldy
		<< std::endl;
    }
    else if(strcmp(argv[i],"-modelMHD")==0){
      std::string modelFile;
      std::string modelBoundaryFile;
      int modelFormat=0;
      std::string modelName="MODELMHD";
      i++;
      modelFile=argv[i];
      if(argc>i+1){
	if(argv[i+1][0]!='-'){
	  i++;
	  modelFormat=atoi(argv[i]);
	  if(argc>i+1){
	    if(argv[i+1][0]!='-'){
	      i++;
	      modelName=argv[i];
	      if(argc>i+1){
		if(argv[i+1][0]!='-'){
		  i++;
		  modelBoundaryFile=argv[i];
		}
	      }
	    }
	  }
	}
      }
      
      std::cout << "Loading MODELMHD file " << modelFile 
    		<< " with format " << modelFormat << " into shared memory "
    		<< modelName << std::endl;
      MODELMHD *m=new MODELMHD(modelFile,modelName,modelFormat);
      if(modelBoundaryFile.size()>0){
	std::cout << "Loading BOUNDARYTF file " << modelBoundaryFile
		  << " for zeroining MODELMHD model " << modelName << std::endl;
	BOUNDARYTF *b=new BOUNDARYTF(modelBoundaryFile);
	std::cout << "Zeroing using BOUNDARYTF file " << modelBoundaryFile 
		  << "." << std::endl;
	m->zeroinside(*b);
      }
    }
    else if(strcmp(argv[i],"-boundaryTF")==0){
      std::string boundaryFile;
      int boundaryFormat=0;
      std::string boundaryName="BOUNDARYTF";
      i++;
      boundaryFile=argv[i];
      if(argv[i+1][0]!='-'){
	i++;
	boundaryFormat=atoi(argv[i]);
	if(argv[i+1][0]!='-'){
	  i++;
	  boundaryName=argv[i];
	}
      }
      std::cout << "Loading BOUNDARYTF file " << boundaryFile 
		<< " with format " << boundaryFormat 
		<< " into shared memory " << boundaryName << std::endl;
      BOUNDARYTF *b=new BOUNDARYTF(boundaryFile,boundaryName,boundaryFormat);
      std::cout << "Subsolar point distance: " << b->r(aVec(1,0,0)) << " Re."
		<< std::endl;
    }
  }

  std::cout << "loadData: data ready." << std::endl;

  sleep(1000000);
  
  return 0;
}
