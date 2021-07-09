// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
/**
 \class Task
 \ingroup WAC

 A simple task to random change the reaction plane of eventStreams by an arbitrary angle phi.
 */
#include "EventPlaneRandomizerTask.hpp"

ClassImp(EventPlaneRandomizerTask);

EventPlaneRandomizerTask::EventPlaneRandomizerTask()
:
Task()
{
  TaskConfiguration & configuration = *getConfiguration();
  configuration.useParticles     = true;
  configuration.useEventStream0  = true;
}

void EventPlaneRandomizerTask::execute()
{
  double eventAngle= TMath::TwoPi() * gRandom->Rndm();
  Event * event = eventStreams[0];
  unsigned int nParticles = event->getNParticles();
  for (unsigned int iParticle = 0; iParticle < nParticles; iParticle++)
    {
    Particle * particle = event->getParticleAt(iParticle);
    particle->getPosition().RotateZ(eventAngle);
    particle->getMomentum().RotateZ(eventAngle);
    }
  incrementEventProcessed();
}

