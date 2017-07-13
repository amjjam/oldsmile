/******************************************************************************
 * This is class ORBITSMILE. It is a chile of class ORBIT. It implements the  *
 * simulated orbit of SMILE. A file is loaded into memory and the values are  *
 * interpolated from those.                                                   *
 ******************************************************************************/

#include "../include/orbitSMILE.H"

/*=============================================================================
  ORBITSMILE(std::string source) - constructore
  If source begins with shm:<name> then link to share memory name to
  get the data. Otherwise load data from a file.

  ORBITSMILE(std::string file, std::string name) - constructor. 
  If file is non-empty string then load data from file. If name is also 
  non-empty then put data from file into shared memory name.
  If file is empty link to shared memory name to get data. 
  ============================================================================*/
ORBITSMILE::ORBITSMILE(std::string source){
  if(strncmp(source.c_str(),"shm:",4)==0){
    initialize(std::string(""),std::string(source.c_str()+4));
  }
  else{
    initialize(source,std::string(""));
  }
}


ORBITSMILE::ORBITSMILE(std::string file, std::string name){
  initialize(file,name);  
}


/*=============================================================================
  ~ORBITSMILE() - destructor
  ============================================================================*/
ORBITSMILE::~ORBITSMILE(){

}


/*=============================================================================
  aTime first() - return the first time in the data set
  ============================================================================*/
aTime ORBITSMILE::first(){
  return t[0];
}


/*=============================================================================
  aTime last() - return the last time in the data set
  ============================================================================*/
aTime ORBITSMILE::last(){
  return t[n-1];
}

/*=============================================================================
  aVec position(aTime tt) - get the position at this time
  ============================================================================*/
aVec ORBITSMILE::position(aTime tt){
  if(tt<t[0]){
    std::cout << "error: requested time is before start of data." 
  	      << std::endl;
    exit(1);
  }
  if(tt>t[n-1]){
    std::cout << "error: requested time after end of data."
  	      << std::endl;
    exit(1);
  }

  int imin=0,imax=n-1,imid;
  while(imin+1<imax){
    imid=midpoint(imin,imax);
    if(t[imid]>=tt)
      imax=imid;
    if(t[imid]<tt)
      imin=imid;
  }
  
  double dt=tt-t[imin],T=t[imin+1]-t[imin];
  aVec r=p[imin]*((T-dt)/T)+p[imin+1]*(dt/T);

  return r;
}


/*=============================================================================
  double period() - find the orbital period of the satellite

  This is done by finding the minimum and maximum radial
  distance. Those are perigee and apogee distances. They can then be
  used to compute the orbital period in seconds
  ============================================================================*/
double ORBITSMILE::period(){
  double rmin=1e100,rmax=-1e100;
  
  unsigned int i;
  double tmp;

  for(i=0;i<n;i++){
    tmp=length(p[i]);
    if(tmp<rmin)
      rmin=tmp;
    else if(tmp>rmax)
      rmax=tmp;
  }

  double aa=(rmax+rmin)/2;
  
  double T=2*M_PI*sqrt(aa*aa*aa/1.54e-6);

  return T;
}

/*=============================================================================
  size_t write(std::string file) - write entire data set to
  file. Implemented by calling write(file,tStart,tEnd) with entire time range.

  return is number of bytes written
  ============================================================================*/
size_t ORBITSMILE::write(std::string file){
  return write(file,first(),last());
}


/*=============================================================================
  size_t write(std::string file, aTime tStart, aTime tEnd) - write data
  in the time interval to a file as 

  yr,mo,dy,hr,mn,se (int) x,y,z (double)
  ============================================================================*/
size_t ORBITSMILE::write(std::string file, aTime tStart, aTime tEnd){
  size_t n=0;
  FILE *fp;
  int yr,mo,dy,hr,mn,se;
  double x,y,z;
  fp=fopen(file.c_str(),"w");
  for(unsigned int i;i<n;i++)
    if(tStart<=t[i]&&t[i]<=tEnd){
      t[i].get(yr,mo,dy,hr,mn,se);
      n+=fwrite(&yr,sizeof(int),1,fp);
      n+=fwrite(&mo,sizeof(int),1,fp);
      n+=fwrite(&dy,sizeof(int),1,fp);
      n+=fwrite(&hr,sizeof(int),1,fp);
      n+=fwrite(&mn,sizeof(int),1,fp);
      n+=fwrite(&se,sizeof(int),1,fp);
      x=p[i].X();
      y=p[i].Y();
      z=p[i].Z();
      n+=fwrite(&x,sizeof(double),1,fp);
      n+=fwrite(&y,sizeof(double),1,fp);
      n+=fwrite(&z,sizeof(double),1,fp);
    }
  
  fclose(fp);

  return n;
}


/*=============================================================================
  void initialize(std::string file, std::string name) - load a orbit data file
  ============================================================================*/
void ORBITSMILE::initialize(std::string file, std::string name){
  RE=6371;

  if(file.size()>0){
    std::ifstream f;
    f.open(file.c_str());
    if(f.fail()){
      std::cout << "Could not open file " << file << std::endl;
      exit(1);
    }
    
    std::string line;
    std::vector<std::string> tokens,ttokens;
    
    // Discard the first two lines as header
    std::getline(f,line);
    std::getline(f,line);
    
    // Read one line at a time parsing time and position
    std::vector<std::string> months;
    months.push_back("Jan");
    months.push_back("Feb");
    months.push_back("Mar");
    months.push_back("Apr");
    months.push_back("May");
    months.push_back("Jun");
    months.push_back("Jul");
    months.push_back("Aug");
    months.push_back("Sep");
    months.push_back("Oct");
    months.push_back("Nov");
    months.push_back("Dec");
    
    unsigned int i,j=0;
    n=256;
    std::vector<aTime> t(n);
    std::vector<aVec> p(n);
    //t.resize(n);
    //p.resize(n);
    int yr,mo,dy,hr,mn,se;
    aTime tt;
    aVec pp;
    std::getline(f,line);
    while(!f.eof()){
      tokens=stringsep(stringcompress(stringtrim(line)));
      if(tokens.size()!=7){
	std::cout << "error: there should be 7 tokens but only " << tokens.size()
		  << " were found." << std::endl;
	exit(1);
      }
      dy=atoi(tokens[0].c_str());
      for(mo=0,i=0;i<months.size();i++)
	if(months[i]==tokens[1]){
	  mo=i+1;
	  break;
	}
      if(i==months.size()){
	std::cout << "error: unknown month name: " << tokens[1] << std::endl;
	exit(1);
      }
      yr=atoi(tokens[2].c_str());
      ttokens=stringsep(tokens[3],':');
      if(ttokens.size()!=3){
	std::cout << "error: wrong number of time components: "
		  << ttokens.size() << ". Should be 3." << std::endl;
	exit(1);
      }
      hr=atoi(ttokens[0].c_str());
      mn=atoi(ttokens[1].c_str());
      se=(int)atof(ttokens[2].c_str());
      tt.set(yr,mo,dy,hr,mn,se);
      pp.setX(atof(tokens[4].c_str())/RE);
      pp.setY(atof(tokens[5].c_str())/RE);
      pp.setZ(atof(tokens[6].c_str())/RE);
      if(j==n){
	n*=2;
	t.resize(n);
	p.resize(n);
      }
      t[j]=tt;
      p[j]=pp;
      j++;
      std::getline(f,line);
    }
    
    t.resize(j);
    p.resize(j);
    n=t.size();

    if(name.size()>0){
      if((fd=shm_open(name.c_str(),O_RDWR|O_CREAT|O_TRUNC,S_IRWXU))==-1){
	std::cout << "ORBITSMILE: error: shm_open return -1" << std::endl;
	perror("shm_open");
	exit(1);
      }
      shmname=name;
      ftruncate(fd,sizeof(int)+t.size()*(sizeof(aTime)+sizeof(aVec)));
      memory=(char *)mmap(NULL,sizeof(int)+
			  t.size()*(sizeof(aTime)+sizeof(aVec)),
			  PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
      n=t.size();
      memcpy(memory,&n,sizeof(int));
      memcpy(memory+sizeof(int),&t[0],n*sizeof(aTime));
      memcpy(memory+sizeof(int)+n*sizeof(aTime),&p[0],n*sizeof(aVec));
      ORBITSMILE::t=(aTime *)(memory+sizeof(int));
      ORBITSMILE::p=(aVec *)(memory+sizeof(int)+n*sizeof(aTime));
    }
    else{
      ORBITSMILE::t=(aTime *)malloc(n*sizeof(aTime));
      ORBITSMILE::p=(aVec *)malloc(n*sizeof(aVec));
      memcpy(ORBITSMILE::t,&t[0],n*sizeof(aTime));
      memcpy(ORBITSMILE::p,&p[0],n*sizeof(aVec));
    }
  }
  else if(name.size()>0){
    fd=shm_open(name.c_str(),O_RDONLY,S_IRUSR);
    if((memory=(char *)mmap(NULL,sizeof(int),PROT_READ,MAP_SHARED,fd,0))==MAP_FAILED){
      std::cout << "ORBITSMILE: error: MAP_FAILED for first mapping."
		<< std::endl;
      perror("ORBITSMILE: mmap");
      exit(1);
    }
    shmname=name;
    n=(*(int *)memory);
    munmap(memory,sizeof(int));
    if((memory=(char *)mmap(NULL,sizeof(int)+n*(sizeof(aTime)+sizeof(aVec)),
			    PROT_READ,MAP_SHARED,fd,0))
       ==MAP_FAILED){
      std::cout << "ORBITSMILE: error: MAP_FAILED for second mapping."
		<< std::endl;
      exit(1);
    }
    n=*((unsigned int *)memory);
    t=(aTime *)(memory+sizeof(int));
    p=(aVec *)(memory+sizeof(int)+n*sizeof(aTime));
  }
  else{
    std::cout << "ORBITSMILE: error: both file and name are empty." 
	      << std::endl;
    exit(1);
  }
}


/*=============================================================================
  int midpoint(int imin, int imax) - returns the integer midpoint
  between imin and imax, guaranteeing that it is always < imax as long
  as imin < imax.
  ============================================================================*/
int ORBITSMILE::midpoint(int imin, int imax){
  return (imin+imax)/2;
}
