/******************************************************************************
 * This is base class ORBIT. It contains information about the satellite      *
 * orbit. For example, it can be used to obtain information about the         *
 * satellite position as a function of time.                                  *
 ******************************************************************************/

#include "../include/orbit.H"

/*=============================================================================
  ORBIT() - constructor
  ============================================================================*/
ORBIT::ORBIT(){

}


/*=============================================================================
  ~ORBIT() - destructor
  ============================================================================*/
ORBIT::~ORBIT(){

}


/*=============================================================================
  aTime first() - return the first time that an orbit position is
  available. The default is 1970/1/1 00:00. This can be modified by
  derived orbit classes that have a shorter time interval available.
  ============================================================================*/
aTime ORBIT::first(){
  aTime t;  
  t.set(1970,1,1);
  return t;
}


/*=============================================================================
  aTime last() - return the last time that an orbit position is
  available. The default is 2038/1/1 00:00. This can be modified by
  derived orbit classes that have a shorter time interval available.
  ============================================================================*/
aTime ORBIT::last(){
  aTime t;
  t.set(2038,1,1);
  return t;
}
