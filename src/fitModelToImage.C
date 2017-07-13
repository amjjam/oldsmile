/******************************************************************************
 * This is a set of functions to use to find the best fit parameters for a    *
 * model for it to fit a supplied image.                                      *
 ******************************************************************************/

#include "../include/fitModelToImage.H"

/*=============================================================================
  int fitModelToImage(IMAGE &image, CAMERA &camera, PMODEL &model, 
                      int difftype) -
  find the parameters for model which when observed through camera
  produces the best fit to image.

  The starting point for the search are the parameters of the model
  when it is passed, set with model.setP().
  ============================================================================*/
int fitModelToImage(IMAGE &image, CAMERA &camera, PMODEL &model, 
		    double (*diff)(IMAGE &I1, IMAGE &i2)){
  // Minimization function parameters
  fitModelToImage_params p;
  p.image=&image;
  p.camera=&camera;
  p.model=&model;
  p.diff=diff;

  // GSL function to minimize
  gsl_multimin_function f;
  std::vector<double> xInitial=model.getP();
  f.n=xInitial.size();
  f.f=&gsl_imageDifference;
  f.params=&p;
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  if(rank==0){
    std::cout << "Initial parameters: " << f.n << " parameters." << std::endl;
    for(unsigned int i=0;i<xInitial.size();i++)
      std::cout << xInitial[i] << " ";
    std::cout << std::endl;
  }
  
  gsl_vector *x=gsl_vector_alloc(f.n);
  for(unsigned int i=0;i<xInitial.size();i++)
    gsl_vector_set(x,i,xInitial[i]);
  gsl_vector *ss=gsl_vector_alloc(f.n);
  gsl_vector_set_all(ss,0.1);
  // Call minimization function
  //const gsl_multimin_fminimizer_type *T=gsl_multimin_fminimizer_nmsimplex2;
  gsl_multimin_fminimizer *s=gsl_multimin_fminimizer_alloc
    (gsl_multimin_fminimizer_nmsimplex2,f.n);
  gsl_multimin_fminimizer_set(s,&f,x,ss);
  
  // Iterate
  int iter=0;
  int status;
  double size;
  do{
    iter++;
    status=gsl_multimin_fminimizer_iterate(s);
    
    if(status)
      break;
    
    size=gsl_multimin_fminimizer_size(s);
    //status=gsl_multimin_test_size(size,1e-1);
    status=gsl_multimin_test_size(size,2e-3);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    if(rank==0)
      std::cout << "Iteration: " << iter << ", size: " << size << std::endl;
  }
  while(status==GSL_CONTINUE&&iter<1000);

  // Set the parameters of the model to be the best-fit parameters returned
  std::vector<double> xFinal=model.getP();
  for(unsigned int i=0;i<xFinal.size();i++)
    xFinal[i]=gsl_vector_get(s->x,i);
  model.setP(xFinal);

  // Clean
  gsl_vector_free(x);
  gsl_vector_free(ss);
  gsl_multimin_fminimizer_free(s);

  
  
  return status;
}


/*=============================================================================
  double gsl_imageDifference(const gsl_vector *v, void *params)
  ============================================================================*/
double gsl_imageDifference(const gsl_vector *v, void *params){
  fitModelToImage_params *p=(fitModelToImage_params *)params;
  
  // Transfer parameters from v to the model
  std::vector<double> x=p->model->getP();
  for(unsigned int i=0;i<x.size();i++) 
    x[i]=gsl_vector_get(v,i);
  p->model->setP(x);

  // Print diagnostic information
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  if(rank==0){
    for(unsigned int i=0;i<x.size();i++)
      std::cout << x[i] << " ";
    std::cout << std::endl;
  }

  // Snap the model image
  IMAGE image=p->camera->snap(*(p->model));
  
  // Return the difference between modeled and observed image
  double diff;
  diff=p->diff(image,*(p->image));

  return diff;
}


