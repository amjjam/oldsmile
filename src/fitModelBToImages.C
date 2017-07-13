/******************************************************************************
 * This program fits modelB to an image, or to an ensemble of noisy images    *
 * derived from a single noise-free image.                                    *
 ******************************************************************************/

/*=============================================================================
  fitModelBToImage -i <file> [-o <file>] [-c <float>...] [-n <integer>]
     [-s <integer>] [-f (y|n)...] [-p <float>...]
  
  -i <file> is the input file to read images from
  -o <file> is the output file to write to. Default is 
     test.fitModelBToImages.gz
  -c <float>.... one or more float values to indicate the average number of
     counts per pixel in the noisy truth image. 0 means a noise free image. 
     Default is 0
  -n <integer> Number of noisy images to generate for each pixel counts. 
     Default is 1.
  -s <integer> random seed to use.
  -f (y|n)... Up to 11 of either y or n, one for each parameter. y indicates 
     the parameter should be fit, n indicates that it should not be fit. 
     Default is n for all. 
  -p <float>... Up to 11 float values which are the initial values for the 
     fit. And also fixed values for those parameters that will not vary. If
     fewer than 11 values are supplied the list must be terminated with '--'
  -fov <float> <float> theta and phi field of view in degrees. 
  For -f and -v the order of parameters is A1 B alpha beta A2 mpr0 mpalphay
  mpalphaz bsr0 bsalphay bsalphaz

  The total number of images generated is the number of float values
  in -c multiplied by the value in -n. 
  ============================================================================*/

std::vector<double> p={3.2285e-5,-1.7985e-5,2.4908,-1.6458,1.3588e-5,
		       8,0.5805,04082,10,0.7827,0.7979};
std::vector<char> f={'n','n','n','n','n','n','n','n','n','n','n'};
double epsabs=0,epsrel=0.01;
double fovTheta=-1,fovPhi=-1;
double randomSeed=1;

CAMERA1 *setupCamera(IMAGE &image);

int main(int argc, char *argv[]){

  parseArgs(argc,argv);
  
  MPI_Init(&argc,&argv);
  
  IMAGE inputImage();
  
  // Open input file
  gzFile iFp=gzopen(iFile.c_str(),"r");
  
  // Open output file
  gzFile oFp=NULL;
  if(rank==0)
    oFp=gzopen(oFile.c_str(),"w9");
  
  // Loop over images in input file
  int r;
  while((r=inputImage.zread(iFp))==0){
    // Write noise-free truth image to file
    inputImage.gather();
    inputImage.zwrite(oFp);
    
    // Set up the camera
    CAMERA1 camera=setupCamera(inputImage,fovTheta,fovPhi);

    for(int iCounts=0;iCounts<avgCounts.size();iCounts++){
      for(int iEnsemble=0;iEnsemble<nEnsemble;iEnsemble++){
	
	// Create noisy truth image and scale model parameters
	double scaleFactor;
	IMAGE noisyImage=createNoisyImage(inputImage,avgCounts[iCounts],
					  randomSeed,scaleFactor);
	
	// Create model and set parameters scaled to noisy truth image
	BOUNDARYSHUE2 *mp, *bs;
	MODELBX model=createModel(scaleFactor,&mp,&bs);

	// Set up the fitting
	
	// Fit to noisy truth image
	
	// Write noisy truth image and fitted image to file
	
	// Write parameters of fit to parameters file
	
	
	delete mp;
	delete bs;
      }
    }
  }
  // End loop over number of images

  gzclose(iFp);
  gzclose(oFp);
  
  if(r>1){
    std::cout << "fitModelBToImage: error: IMAGE.zread() returned " 
	      << r << std::endl;
    exit(1);
  }
  
  // Close output file
  gzclose(fp);

  MPI_Finalize();
  
  return 0;
}


void parseArgs(int argc, char *argv[]){
  
  for(int i=1;i<argc;i++){
    if(strcmp(argv[i],"-i")==0){
      i++;
      iFile=argv[i];
    }
    else if(strcmp(argv[i],"-o")==0){
      i++;
      oFile=argv[i];
    }
    else if(strcmp(argv[i],"-c")==0){
      i++;
      for(;i<argc;i++){
	if(argv[i][0]=='-')
	  break;
	counts.push_back(atof(argv[i]));
      }
    }
    else if(strcmp(argv[i],"-n")==0){
      i++;
      nEnsemble=atoi(argv[i]);
    }
    else if(strcmp(argv[i],"-s")==0){
      i++;
      randomSeed=atoi(argv[i]);
    }
    else if(strcmp(argv[i],"-f")==0){
      i++;
      for(int j;j<11&&i<argc-1;j++,i++){
	if(argv[i][0]=='-')
	  break;
	if(argv[i][0]!='y'||argv[i][0]!='n'){
	  std::cout << "parseArgs: error: -f arguments must be either y or n: "
		    << argv[i] << std::endl;
	  exit(1);
	}
	f[j]=argv[i][0];
      }
    }
    else if(strcmp(argv[i],"-v")==0){
      i++;
      for(int j;j<11&&i<argc-1;j++,i++){
	if(strcmp(argv[i],"--")==0)
	  break;
	v[i]=atof(argv[i]);
      }
    }
    else if(strcmp(argv[i],"-fov")==0){
      i++;
      fovTheta=atof(argv[i]);
      i++;
      fovPhi=atof(argv[i]);
    }
  }
  if(fovTheta<0||fovPhi<0){
    std::cout << "parseArgs: error: either fovTheta and/or fovPhi were not "
	      << "specified, or at least one negative values was provided: "
	      << "fovTheta=" << fovTheta << " fovPhi=" << fovPhi << std::endl;
    exit(1);
  }
}


CAMERA1 setupCamera(IMAGE &image, double fovTheta, double fovPhi){
  CAMERA1 camera(fovTheta/180.0*M_PI,fovPhi/180.0*M_PI,
		 image.getN(),inputImage.getM());
  camera.setPos(inputImage.getPos());
  double roll,pitch,yaw;
  image.getDir(roll,pitch,yaw);
  camera.setDir(roll,pitch,yaw);
  
  return camera;
}


IMAGE createNoisyImage(IMAGE &image, double avgCounts, int randomSeed,
		       double &scaleFactor){
  IMAGE noisyImage;
  noisyImage=image;
  if(avgCounts>0){
    double avg=average(noisyImage);
    scaleFactor=avgCounts/avg;
    noisyImage*=scaleFactor;
    noisyImage=poissonImage(noisyImage,randomSeed);
  }
  else
    scaleFactor=1;
  
  return noisyImage;
}

MODEL createModel(double scaleFactor, BOUNDARYSHUE2 **mp, BOUNDARYSHUE2 **bs){
  std::vector<double> pp=p;
  pp[0]*=scaleFactor;
  pp[4]*=scaleFactor;
  (*mp)=new BOUNDARYSHUE2(pp[5],pp[6],pp[7]);
  (*bs)=new BOUNDARYSHUE2(pp[8],pp[9],pp[10]);  
  MODELBX model(mp,bs);
  model.setLosEps(epsabs,epsrel);
  std::vector<double> bi;
  bi.resize(5);
  for(int i=0;i<5;i++) bi[i]=i;

  ..... set parameters of bi here

  return MODELBX
}
