// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
#ifndef WAC_NucleonNucleonCollisionGeneratorConfiguration
#define WAC_NucleonNucleonCollisionGeneratorConfiguration
#include "TaskConfiguration.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Configuration of a given analysis
////////////////////////////////////////////////////////////////////////////////////////////////////////
class NucleonNucleonCollisionGeneratorConfiguration : public TaskConfiguration
{
public:
  
  NucleonNucleonCollisionGeneratorConfiguration();
  NucleonNucleonCollisionGeneratorConfiguration(const NucleonNucleonCollisionGeneratorConfiguration & source);
  virtual ~NucleonNucleonCollisionGeneratorConfiguration(){}
  NucleonNucleonCollisionGeneratorConfiguration & operator=(const NucleonNucleonCollisionGeneratorConfiguration & source);
  void printConfiguration(ostream & os);
  ClassDef(NucleonNucleonCollisionGeneratorConfiguration,0)
};

#endif /* WAC_NucleonNucleonCollisionGeneratorConfiguration */
