NucleonNucleonType#include "AACollisionGenerator.hpp"
#include "TDatabasePDG.h"

ClassImp(AACollisionGenerator);

AACollisionGenerator::AACollisionGenerator()
:
Task(),
nnCollisionGenerator(NucleonNucleonCollisionGenerator::getDefaultNNCollisionGenerator() )
{
  // no ops
}

void AACollisionGenerator::initialize()
{
  TaskConfiguration * configuration = getConfiguration();
  configuration.useParticles     = true;
  configuration.useEventStream0  = true;
  Task::initialize();
  nnCollisionGenerator->initialize();
}

void AACollisionGenerator::execute()
{
  unsigned int nParticles = eventStreams[0]->getNParticles()
  for (unsigned int iParticle = 0; iParticle < nParticles; iParticle++)
  {
  Particle * particle = event->getParticleAt(iParticle)
  if (particle->isInteraction() && particle->isLive() )
    {
    nnCollisionGenerator->generate(particle);
    particle->setLive(false); // the interaction has been generated
    }
  incrementEventProcessed();
  incrementEventAccepted();
  }
}

void AACollisionGenerator::finalize()
{
  Task::finalize();
  nnCollisionGenerator->printStatistics();
}

void AACollisionGenerator::setNucleonNucleonCollisionGenerator(NucleonNucleonCollisionGenerator  * _nnCollisionGenerator)
{
  if (!_nnCollisionGenerator)
    {
    if (reportFatal("AACollisionGenerator",getName(),"initialize()"))
      {
      cout << "No nnCollisionGenerator available (nnCollisionGenerator==nullptr)" <<endl;
      }
    postTaskFatal();
    return;
    }
  nnCollisionGenerator = _nnCollisionGenerator;
}
