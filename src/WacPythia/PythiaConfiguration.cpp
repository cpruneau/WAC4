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

 Utility class used to definePythiaConfiguration
 */
#include "PythiaConfiguration.hpp"

ClassImp(PythiaConfiguration);

PythiaConfiguration::PythiaConfiguration()
:
NucleonNucleonCollisionGeneratorConfiguration(),
clonesArraySize(10000),
removePhotons(true),
standaloneMode(true),
ppOnly(true),
beam(2212),
target(2212),
energy(2726.0),
options()
{
// no ops
}

void PythiaConfiguration::printConfiguration(ostream & os)
{
  NucleonNucleonCollisionGeneratorConfiguration::printConfiguration(os);
  os
  << "    PYTHIA   Parameters: " << endl
  << " ------------------------------------------------------------------------------------------" << endl;
  os << "  removePhotons: " << removePhotons << endl;
  os << "         ppOnly: " << ppOnly << endl;
  os << " standaloneMode: " << standaloneMode << endl;
  os << "           Beam: " << beam  << endl;
  os << "         Target: " << target  << endl;
  os << "         Energy: " << energy  << endl;
  for (unsigned int i=0; i<options.size(); i++)
  {
  os << *options[i] << endl;
  }
}
