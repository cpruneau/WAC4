// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/

#ifndef WAC_ParticleAnalyzer
#define WAC_ParticleAnalyzer
#include <TParameter.h>
#include "TFile.h"
#include "TList.h"
#include "Task.hpp"
#include "Event.hpp"
#include "EventFilter.hpp"
#include "ParticleFilter.hpp"
#include "ParticleHistos.hpp"
#include "ParticleAnalyzerConfiguration.hpp"

class ParticleAnalyzer : public Task
{
public:
  
  ParticleAnalyzer(const TString &                 _name,
                   ParticleAnalyzerConfiguration * _configuration,
                   vector<EventFilter*>            _eventFilters,
                   vector<ParticleFilter*>         _particleFilters,
                   LogLevel                        _selectedLevel=Info);
  virtual ~ParticleAnalyzer() {}
  virtual void execute();
  virtual void createHistograms();
  virtual void loadHistograms(TFile * inputFile);

protected:
  ClassDef(ParticleAnalyzer,0)
};


#endif /* WAC_ParticleAnalyzer */
