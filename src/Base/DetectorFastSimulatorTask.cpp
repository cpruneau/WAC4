// Author: Claude Pruneau   09/25/2019

/*************************************************************************
 * Copyright (C) 2019, Claude Pruneau.                                   *
 * All rights reserved.                                                  *
 * Based on the ROOT package and environment                             *
 *                                                                       *
 * For the licensing terms see LICENSE.                                  *
 *************************************************************************/
/**
 \class Task
 \ingroup WAC

 Class defining Two Particle Correlation Analyzer Task
 */

#include "DetectorFastSimulatorTask.hpp"

ClassImp(DetectorFastSimulatorTask);

DetectorFastSimulatorTask::DetectorFastSimulatorTask(const TString &                 _name,
                                                     vector<EventFilter*>            _eventFilters,
                                                     vector<ParticleFilter*>         _particleFilters,
                                                     vector<ParticleSmearer*>        _particleSmearers,
                                                     LogLevel                        _selectedLevel)
:
Task()
{
  setName(_name);
  TaskConfiguration & configuration = *getConfiguration();
  configuration.useParticles     = true;
  configuration.useEventStream0  = true;
  configuration.useEventStream1  = true;
  setReportLevel(_selectedLevel);
  eventFilters     = _eventFilters;
  particleFilters  = _particleFilters;
  particleSmearers = _particleSmearers;
  // smearers added here....
}


void DetectorFastSimulatorTask::execute()
{
  incrementEventProcessed();
  ParticleFilter & particleFilterPrimary   = * particleFilters[0];
  ParticleFilter & particleFilterSecondary = * particleFilters[1];
  Event & event    = *eventStreams[0]; // source -- unsmeared
  Event & eventDet = *eventStreams[1]; // target -- smeared.

  unsigned int nParticles = event.getNParticles();
  for (unsigned int iParticle=0; iParticle<nParticles; iParticle++)
    {
    Particle & particle = * event.getParticleAt(iParticle);
    if (particle.isLive())
      {
      if (particle.isPrimary())
        {
        if (particleFilterPrimary.accept(particle))
          {
          Particle & particleDet = *particleFactory->getNextObject();
          particleDet = particle;
          particleDet.setTruth(&particle);
          particleSmearers[0]->smear(particle.getMomentum(),particleDet.getMomentum());
          eventDet.add(&particleDet);
          }
        else if (particleFilterSecondary.accept(particle))
          {
          Particle & particleDet = *particleFactory->getNextObject();
          particleDet = particle;
          particleDet.setTruth(&particle);
          particleSmearers[1]->smear(particle.getMomentum(),particleDet.getMomentum());
          eventDet.add(&particleDet);
          }
        }
      }
    }
}

