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
#include "EventVertexRandomizerTask.hpp"

ClassImp(EventVertexRandomizerTask);

EventVertexRandomizerTask::EventVertexRandomizerTask(EventVertexRandomizerConfiguration * configuration)
:
Task()
{
  configuration->useParticles     = true;
  configuration->useEventStream0  = true;
  setConfiguration(configuration);
}

void EventVertexRandomizerTask::execute()
{
  EventVertexRandomizerConfiguration & config = *(EventVertexRandomizerConfiguration*) getConfiguration();
  // vertex position and size supplied in micro-meters.
  // vertex time is nanosecond relative to nominal crossing time
  // convert to fm for internal use: 1  micro-meter = 1E9 fm
  double rConversion = config.rConversion;  // micrometer to femtometer
  double tConversion = config.tConversion;  // nanosecond to fm/c

  double eventX = taskRandomGenerator->Gaus(rConversion*config.xAvg, rConversion*config.xRms);
  double eventY = taskRandomGenerator->Gaus(rConversion*config.yAvg, rConversion*config.yRms);
  double eventZ = taskRandomGenerator->Gaus(rConversion*config.zAvg, rConversion*config.zRms);
  double eventT = taskRandomGenerator->Gaus(tConversion*config.tAvg, tConversion*config.tRms);
  Event * event = eventStreams[0];
  unsigned int nParticles = event->getNParticles();
  for (unsigned int iParticle = 0; iParticle < nParticles; iParticle++)
    {
    Particle * particle = event->getParticleAt(iParticle);
    particle->shift(eventX,eventY,eventZ,eventT);
    }
  incrementEventProcessed();
  incrementEventAccepted();
}

