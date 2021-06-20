// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/

#ifndef WAC_ParticleDecayerTask
#define WAC_ParticleDecayerTask
#include "ParticleDecayer.hpp"
#include "Task.hpp"

class ParticleDecayerTask : public Task
{

protected:
  ParticleDecayer decayer;

public:

  ParticleDecayerTask(LogLevel _selectedLevel=Info);
  virtual ~ParticleDecayerTask() {}
  virtual void execute();
  ParticleDecayer & getParticleDecayer() { return decayer;}

  ClassDef(ParticleDecayerTask,0)
};


#endif /* WAC_ParticleDecayerTask */
