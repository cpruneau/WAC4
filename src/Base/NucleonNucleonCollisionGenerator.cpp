#include "NucleonNucleonCollisionGenerator.hpp"
#include "TDatabasePDG.h"

ClassImp(NucleonNucleonCollisionGenerator);

NucleonNucleonCollisionGenerator::NucleonNucleonCollisionGenerator()
:
Task()
{
  // no ops
}

void NucleonNucleonCollisionGenerator::generate(Particle * parent)
{
  incrementEventProcessed();
  if (parent!=nullptr)  incrementEventAccepted();
}

NucleonNucleonCollisionGenerator * NucleonNucleonCollisionGenerator::defaultNNCollisionGenerator = nullptr;

NucleonNucleonCollisionGenerator * NucleonNucleonCollisionGenerator::getDefaultNNCollisionGenerator()
{
  if (!defaultNNCollisionGenerator)
    {
    defaultNNCollisionGenerator = new NucleonNucleonCollisionGenerator();
    }
  return defaultNNCollisionGenerator;
}
