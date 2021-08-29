// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/

#ifndef WAC_ParticleAnalyzerV2
#define WAC_ParticleAnalyzerV2
#include <TParameter.h>
#include "TFile.h"
#include "TList.h"
#include "Task.hpp"
#include "Event.hpp"
#include "EventFilter.hpp"
#include "ParticleDigit.hpp"
#include "ParticleFilter.hpp"
#include "ParticleHistosV2.hpp"
#include "ParticleAnalyzerConfiguration.hpp"

class ParticleAnalyzerV2 : public Task
{
public:
  
  ParticleAnalyzerV2(const TString &                 _name,
                   ParticleAnalyzerConfiguration * _configuration,
                   vector<EventFilter*>            _eventFilters,
                   vector<ParticleFilter*>         _particleFilters,
                   LogLevel                        _selectedLevel=Info);
  virtual ~ParticleAnalyzerV2() {}
  virtual void execute();
  virtual void createHistograms();
  virtual void loadHistograms(TFile * inputFile);

protected:

  vector< vector<ParticleDigit*> > filteredParticles;

  ClassDef(ParticleAnalyzerV2,0)
};


#endif /* WAC_ParticleAnalyzerV2 */
