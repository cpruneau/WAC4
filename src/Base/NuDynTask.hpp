// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/

#ifndef WAC_NuDynTask
#define WAC_NuDynTask
#include <TParameter.h>
#include "TFile.h"
#include "TList.h"
#include "Task.hpp"
#include "Event.hpp"
#include "EventFilter.hpp"
#include "NuDynHistos.hpp"
#include "NuDynDerivedHistos.hpp"
#include "NuDynConfiguration.hpp"
#include "ParticleFilter.hpp"
#include "NuDynConfiguration.hpp"

class NuDynTask : public Task
{
public:

  NuDynTask(const TString &         name,
            NuDynConfiguration *    configuration,
            vector<EventFilter*>    eventFilters,
            vector<ParticleFilter*> particleFilters,
            LogLevel                selectedLevel);
  virtual ~NuDynTask() {}
  virtual void initialize();
  virtual void execute();
  virtual void createHistograms();
  virtual void loadHistograms(TFile * inputFile);
  virtual void createIdentical();

protected:
  vector<int> identical;

  ClassDef(NuDynTask,0)
};


#endif /* WAC_NuDynTask */
