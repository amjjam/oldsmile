/*********************************************************************
 * Write the SMILE orbit information to a binary file in the format  *
 * loaded by the loadOrbit.pro.                                      *
 *********************************************************************/

/*====================================================================
  writeSMILEOrbit <file> -o <file> [-start yr mo dy hr mn se] \
     [-end yr mo dy hr mn se] 

  <file> is the orbit file
  -o is the file to write to. 
  If start time is not specified then the first time in the data is
  assumed. If end time is not specified then the last time in the data
  is assumed. It is an error to specify either time outside the range
  of the data.
  ===================================================================*/

#include "../include/orbitSMILE.H"

void parseArgs(int argc, char *argv[]);

std::string file;
std::string oFile;
aTime tStart;
bool startProvided=false;
aTime tEnd;
bool endProvided=false;

int main(int argc, char *argv[]){

  parseArgs(argc,argv);
  
  ORBITSMILE o(file);

  aTime tFirst=o.first();
  aTime tLast=o.last();

  if(!startProvided)
    tStart=tFirst;

  if(!endProvided)
    tEnd=tLast;
  
  if(tStart<tFirst){
    std::cout << "Error: -start time is before start of data." 
	      << std::endl;
    exit(1);
  }
  
  if(tLast<tEnd){
    std::cout << "Error: -end time is after end of data."
	      << std::endl;
    exit(1);
  }
  
  if(tEnd<=tStart){
    std::cout << "Error: -end time is not after -start time."
	      << std::endl;
    exit(1);
  }
  
  o.write(oFile,tStart,tEnd);
  
  return 0;
}


void parseArgs(int argc, char *argv[]){
  bool fileProvided=false;
  bool oFileProvided=false;
  int yr,mo,dy,hr,mn,se;
  for(int i=1;i<argc;i++){
    if(strcmp(argv[i],"-start")==0){
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
      startProvided=true;
    }
    else if(strcmp(argv[i],"-end")==0){
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
      tEnd.set(yr,mo,dy,hr,mn,se);
      endProvided=true;
    }
    else if(strcmp(argv[i],"-o")==0){
      i++;
      oFile=std::string(argv[i]);
      oFileProvided=true;
    }
    else{
      if(fileProvided==true){
	std::cout << "Error: only specify one orbit file: "
		  << argv[i] << std::endl;
	exit(1);
      }
      file=std::string(argv[i]);
      fileProvided=true;
    }
  }
  
  if(fileProvided==false){
    std::cout << "Error: orbit file not provided." << std::endl;
    exit(1);
  }

  if(oFileProvided==false){
    std::cout << "Error: output file not provided." << std::endl;
    exit(1);
  }

}
