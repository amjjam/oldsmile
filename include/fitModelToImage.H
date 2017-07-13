#ifndef _FITMODELTOIMAGE_PARAMS_
#define _FITMODELTOIMAGE_PARAMS_

#include <image.H>
#include <camera.H>
#include <pModel.H>

#include <gsl/gsl_multimin.h>

struct fitModelToImage_params{
  IMAGE *image; // The image to be fitted
  CAMERA *camera; // The camera to use to record the image
  PMODEL *model; // The model to fit 
  double (*diff)(IMAGE &image1, IMAGE &image2);
};

int fitModelToImage(IMAGE &image, CAMERA &camera, PMODEL &model, 
		    double (*diff)(IMAGE &i1, IMAGE &i2));
double gsl_imageDifference(const gsl_vector *v, void *params);
//double gsl_imageAbsDifference(const gsl_vector *v, void *params);
//double imageAbsDifference(IMAGE &image1, IMAGE &image2);

#endif
