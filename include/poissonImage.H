/*********************************************************************
 * Function for creating a random poisson image.                     *
 *********************************************************************/

#ifndef _POISSONIMAGE_H_
#define _POISSONIMAGE_H_

#include "image.H"
#include "gsl/gsl_rng.h"
#include "gsl/gsl_randist.h"
#include <mpi.h>

IMAGE poissonImage(IMAGE &image, unsigned long int s);

#endif

