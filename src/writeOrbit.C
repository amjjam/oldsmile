/******************************************************************************
 * This program writes an orbit from ORBIT3 to a file.                        *
 ******************************************************************************/

/*=============================================================================
  writeOrbit [-h] [-a <float>] [-p <float>] [-t0 yr mo dy hr mn] [-dt]
     [-Omega <float>] [-incl <float>] [-omega <float>] [-o <file>]

  -h - this help text
  -a - apogee radial distance in Re
  -p - perigee radial distance in Re
  -t0 - start time of orbit
  -dt - time in seconds between output points
  -Omega - Longitude of ascending node in degrees
  -incl - Inclination in degrees
  -omega - argument of periapsis in degrees
  -o - output file to write to
  ============================================================================*/

#include <stdlib.h>
#include <zlib.h>
#include <string>
#include <iostream>
#include <aTime.H>

#include "orbit3.H"

void parseArgs(int argc, char *argv[]);

double a=20,p=2;
aTime t0;
double dt=120,Omega=0,inclination=0,omega=0;
std::string oFile;

int main(int argc, char *argv[]){
  
  parseArgs(argc,argv);
  
  ORBIT3 o(a,p,t0,Omega,inclination,omega);

  // Open the output file
  gzFile fp=gzopen(oFile.c_str(),"w9");

  aTime tE=t0;
  tE+=o.period();

  aVec p;
  double tmp;
  int yr,mo,dy,hr,mn,se;
  for(aTime t=t0;t<=tE;t+=dt){
    t.get(yr,mo,dy,hr,mn,se);
    gzwrite(fp,&yr,sizeof(int));
    gzwrite(fp,&mo,sizeof(int));
    gzwrite(fp,&dy,sizeof(int));
    gzwrite(fp,&hr,sizeof(int));
    gzwrite(fp,&mn,sizeof(int));
    gzwrite(fp,&se,sizeof(int));
    p=o.position(t);
    tmp=p.X();
    gzwrite(fp,&tmp,sizeof(double));
    tmp=p.Y();
    gzwrite(fp,&tmp,sizeof(double));
    tmp=p.Z();
    gzwrite(fp,&tmp,sizeof(double));
  }
  gzclose(fp);

  return 0;
}


void parseArgs(int argc, char *argv[]){
  for(int i=1;i<argc;i++)
    if(strcmp(argv[i],"-h")==0){
      std::cout << "writeOrbit [-h] [-a <float>] [-p <float>] "
		<< "[-t0 yr mo dy hr mn]" << std::endl;
      std::cout << "   [-Omega <float>] [-incl <float>] [-omega <float>] "
		<< "[-o <file>]" << std::endl << std::endl;
      std::cout << "-h - this help text" << std::endl;
      std::cout << "-a - apogee radial distance in Re" << std::endl;
      std::cout << "-p - perigee radial distance in Re" << std::endl;
      std::cout << "-t0 - start time of orbit" << std::endl;
      std::cout << "-Omega - Longitude of ascending node in degrees" 
		<< std::endl;
      std::cout << "-incl - Inclination in degrees" << std::endl;
      std::cout << "-omega - argument of periapsis in degrees" << std::endl;
      std::cout << "-o - output file to write to" << std::endl;
      exit(0);
    }
    else if(strcmp(argv[i],"-a")==0){
      i++;
      a=atof(argv[i]);
    }
    else if(strcmp(argv[i],"-p")==0){
      i++;
      p=atof(argv[i]);
    }
    else if(strcmp(argv[i],"-t0")==0){
      int yr,mo,dy,hr,mn;
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
      t0.set(yr,mo,dy,hr,mn);
    }
    else if(strcmp(argv[i],"-Omega")==0){
      i++;
      Omega=atof(argv[i])/180*M_PI;
    }
    else if(strcmp(argv[i],"-incl")==0){
      i++;
      inclination=atof(argv[i])/180*M_PI;
    }
    else if(strcmp(argv[i],"-omega")==0){
      i++;
      omega=atof(argv[i])/180*M_PI;
    }
    else if(strcmp(argv[i],"-o")==0){
      i++;
      oFile=argv[i];
    }

  if(oFile.size()==0){
    std::cout << "error: no output time specified" << std::endl;
    exit(1);
  }
}

