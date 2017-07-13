/******************************************************************************
 * This is class CAMERA1. It is a child of class CAMERA and implements a      *
 * specific image plane geometry.                                             *
 ******************************************************************************/

#include "camera1.H"

#include <iostream>

/*=============================================================================
  CAMERA1(double T, double P, int n, int m) - constructor
  ============================================================================*/
CAMERA1::CAMERA1(double T, double P, int n, int m):CAMERA(T,P,n,m){
  dt=T/n;
  dp=P/m;
}


/*=============================================================================
  ~CAMERA1() - destructor
  ============================================================================*/
CAMERA1::~CAMERA1(){

}


// /*=============================================================================
//   IMAGE snap(MODEL &m) - take an image of the model using the current
//   camera position and orientation.
//   ============================================================================*/
// IMAGE CAMERA1::snap(MODEL &model){
//   IMAGE d(n,m);

//   int i,j;

//   aVec dir;
//   double v;

//   for(i=0;i<n;i++)
//     for(j=0;j<m;j++){
//       dir=direction(i,j);
//       v=model.los(pos,dir);
//       d.setP(i,j,v);
//       if((i==25||i==0||i==50)&&(j==0||j==25||j==50)){
// 	//std::cout << "(" << i << "," << j << ") (" << pos.X() << "," << pos.Y() << "," << pos.Z() << ") ("
//       	//	<< dir.X() << "," << dir.Y() << "," << dir.Z() << ")" << " " << v << std::endl;
// 	//std::cout << "pitch: " << pitch << ", yaw: " << yaw << ", roll: " 
// 	//	 << roll << std::endl;
//       }
//     }

  
//   d.setPos(pos);
//   d.setDir(roll,pitch,yaw);

//   return d;
// }


IMAGE CAMERA1::snap(MODEL &model){
  IMAGE d(n,m);
  
  int nPixels=d.nPixels();
  int i,j;
  for(int k=0;k<nPixels;k++){
    d.seqToPixel(k,i,j);
    d.setP(i,j,model.los(pos,direction(i,j)));
  }  

  d.setPos(pos);
  d.setDir(roll,pitch,yaw);

  d.gather();

  return d;
}


// /*=============================================================================
//   THE FOLLOWING FUNCTION WILL BE MADE OBSOLETE WHEN I WRITE A FUNCTION
//   WHICH COMPUTES LOS FOR THE PIXELS OF THIS RANKS AND PUTS THOSE IN
//   THE CORRESPONDING PIXELS. GATHERING IMAGES WILL NOW BE AN EXPLICIT
//   OPERATION THAT NEEDS TO BE CALLED.
//   ============================================================================*/

// IMAGE CAMERA1::snap(MODEL &model){
//   // Every process compute pixels in stride comm_size and then sends
//   // those data to rank 0 process at the end which assembles them This
//   // way rank 0 returns the complete image while other ranks only
//   // return their portions of the image. 
//   IMAGE d(n,m);

//   int comm_size;
//   MPI_Comm_size(MPI_COMM_WORLD,&comm_size);
//   int maxNPixels=d.maxNPixels();
//   int rank;
//   MPI_Comm_rank(MPI_COMM_WORLD,&rank);
//   std::vector<double> v(d.maxNPixels());
//   int nPixels=d.nPixels();
//   int i,j;
//   for(int k=0;k<nPixels;k++){
//     d.seqToPixel(k,i,j);
//     v[k]=model.los(pos,direction(i,j));
//   }
//   if(rank>0)
//     MPI_Send(&v[0],maxNPixels,MPI_DOUBLE,0,0,MPI_COMM_WORLD);

//   // The rank=0 process receives all the computations and assembles
//   // them into an array
//   if(rank==0){
//     MPI_Status status;
//     int sourcerank=0;
//     for(int l=0;l<comm_size;l++){
//       if(l>0){
// 	MPI_Recv(&v[0],maxNPixels,MPI_DOUBLE,MPI_ANY_SOURCE,MPI_ANY_TAG,
// 		 MPI_COMM_WORLD,&status);
// 	sourcerank=status.MPI_SOURCE;
//       }
//       nPixels=d.nPixels(sourcerank);
//       for(int k=0;k<nPixels;k++){
// 	d.seqToPixel(sourcerank,k,i,j);
// 	d.setP(i,j,v[k]);
//       }
//     }
//   }
  
//   return d;
// }


/*=============================================================================
  aVec direction(double i, double j, int c=1) - get the look-direction
  vector for pixel (i,j)
  
  double i,j is the pixel number which is integer, but a real number can also
  be passed to return the look direction of a point within a pixel.

  int c is the coordinate system. The default is 1 which is the
  external coordinate system. 0 is the internal coordinate system of
  the camera.
  ============================================================================*/
aVec CAMERA1::direction(double i, double j, int c){
  double t,p;
  
  t=M_PI/2-T/2+dt/2+i*dt;
  p=-P/2+dp/2+j*dp;

  aVec v=unitVector(t,p);

  //if((i==25||i==0||i==50)&&(j==0||j==25||j==50)){
  //  std::cout << i << " " << j << std::endl;
  //  std::cout << t << " " << p << " " << v << std::endl;
  //}

  if(c==0)
    return v;

  return transform(v);
}
