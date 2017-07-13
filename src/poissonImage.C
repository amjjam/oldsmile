#include "../include/poissonImage.H"

/*=============================================================================
  IMAGE poissonImage(IMAGE &i, unsigned long int s) - create a poisson
  image from the input rate image. 

  Only compute poisson image for pixels in the local rank
  ============================================================================*/
IMAGE poissonImage(IMAGE &image, unsigned long int s){
  IMAGE oimage=image;

  int comm_size,rank;
  MPI_Comm_size(MPI_COMM_WORLD,&comm_size);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);

  gsl_rng *r=gsl_rng_alloc(gsl_rng_taus);
  gsl_rng_set(r,s*comm_size+rank);
  int i,j;
  int nPixels=image.nPixels();
  for(int k=0;k<nPixels;k++){
    image.seqToPixel(k,i,j);
    oimage.setP(i,j,gsl_ran_poisson(r,image.getP(i,j)));
  }
  gsl_rng_free(r);
  
  return oimage;
}
