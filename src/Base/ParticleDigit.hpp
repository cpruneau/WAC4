// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/

#ifndef WAC_ParticleDigit
#define WAC_ParticleDigit
#include "TString.h"
#include "Factory.hpp"

using namespace std;

//!
//!Class ParticleDigit
//!
class ParticleDigit
{
public:
  
  ParticleDigit();
  ParticleDigit(unsigned int _iY,
                unsigned int _iEta,
                unsigned int _iPhi,
                unsigned int _iPt,
                double       _pt,
                double       _e);
  virtual ~ParticleDigit();
  ParticleDigit(const ParticleDigit& other);
  ParticleDigit & operator=(const ParticleDigit & other);
  void printProperties(ostream & output);
  void reset();

  unsigned int iY;
  unsigned int iEta;
  unsigned int iPhi;
  unsigned int iPt;
  double pt;
  double e;

  static int factorySize;
  static Factory<ParticleDigit> * factory;
  static Factory<ParticleDigit> * getFactory();


  ClassDef(ParticleDigit,0)
};


#endif /* WAC_ParticleDigit */
