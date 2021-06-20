// Author: Claude Pruneau   09/25/2019

/*************************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *************************************************************************/
/**
 \class Property
 \ingroup WAC

 Utility class used to defineNucleonNucleonCollisionGeneratorConfiguration
 */

#include "NucleonNucleonCollisionGeneratorConfiguration.hpp"
#include "TMath.h"

ClassImp(NucleonNucleonCollisionGeneratorConfiguration);


NucleonNucleonCollisionGeneratorConfiguration::NucleonNucleonCollisionGeneratorConfiguration()
:
TaskConfiguration()
{
  // no  ops
}

NucleonNucleonCollisionGeneratorConfiguration::NucleonNucleonCollisionGeneratorConfiguration(const NucleonNucleonCollisionGeneratorConfiguration & source)
:
TaskConfiguration( source )
{
  // no  ops
}

NucleonNucleonCollisionGeneratorConfiguration & NucleonNucleonCollisionGeneratorConfiguration::operator=(const NucleonNucleonCollisionGeneratorConfiguration & source)
{
  if (this!=&source)
    {
    TaskConfiguration::operator=( source );
    }
  return *this;
}

void NucleonNucleonCollisionGeneratorConfiguration::printConfiguration(ostream & os)
{
  TaskConfiguration::printConfiguration(os);
  os
  << "               Parameters: " << endl
  << " ------------------------------------------------------------------------------------------" << endl;
}
