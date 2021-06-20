// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/

#ifndef WAC_DetectorFastSimulatorTask
#define WAC_DetectorFastSimulatorTask
#include <TParameter.h>
#include "TFile.h"
#include "TList.h"
#include "Task.hpp"
#include "Event.hpp"
#include "EventFilter.hpp"
#include "ParticleFilter.hpp"
#include "ParticleSmearer.hpp"
//#include "DetectorFastSimulatorTaskConfiguration.hpp"

class DetectorFastSimulatorTask : public Task
{
public:
  
  DetectorFastSimulatorTask(const TString &                 _name,
                            vector<EventFilter*>            _eventFilters,
                            vector<ParticleFilter*>         _particleFilters,
                            vector<ParticleSmearer*>        _particleSmearers,
                            LogLevel                        _selectedLevel=Info);
  virtual ~DetectorFastSimulatorTask() {}
  virtual void execute();
 
protected:
  ClassDef(DetectorFastSimulatorTask,0)
};


#endif /* WAC_DetectorFastSimulatorTask */
