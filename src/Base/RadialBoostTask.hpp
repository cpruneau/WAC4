// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/

#ifndef WAC_RadialBoostTask
#define WAC_RadialBoostTask
#include <TParameter.h>
#include "TFile.h"
#include "Task.hpp"
#include "Event.hpp"
#include "CollisionGeometry.hpp"
#include "RadialBoostHistos.hpp"

class RadialBoostTask : public Task
{
public:

  RadialBoostTask(RadialBoostConfiguration * _configuration,
                  vector<EventFilter*>       _eventFilters,
                  LogLevel                   _selectedLevel=Info);
  virtual ~RadialBoostTask() {}
  virtual void execute();
  virtual void createHistograms();
  virtual void loadHistograms(TFile * inputFile);

protected:

  double param_b;
  double param_a;
  double betaMaximum;

  ClassDef(RadialBoostTask,0)
};


#endif /* WAC_RadialBoostTask */
