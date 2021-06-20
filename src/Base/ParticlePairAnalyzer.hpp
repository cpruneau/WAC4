// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/

#ifndef WAC_ParticlePairAnalyzer
#define WAC_ParticlePairAnalyzer
#include <TParameter.h>
#include "TFile.h"
#include "TList.h"
#include "Task.hpp"
#include "Event.hpp"
#include "EventFilter.hpp"
#include "ParticleFilter.hpp"
#include "ParticlePairFilter.hpp"
#include "ParticleHistos.hpp"
#include "ParticlePairHistos.hpp"
#include "ParticlePairDerivedHistos.hpp"
#include "ParticlePairCombinedHistos.hpp"
#include "ParticlePairAnalyzerConfiguration.hpp"

class ParticlePairAnalyzer : public Task
{
public:

  ParticlePairAnalyzer(const TString &                 _name,
                       ParticleAnalyzerConfiguration * _configuration,
                       vector<EventFilter*>            _eventFilters,
                       vector<ParticleFilter*>         _particleFilters,
                       LogLevel                        _selectedLevel);
  virtual ~ParticlePairAnalyzer() {}
  virtual void execute();
  virtual void createHistograms();
  virtual void loadHistograms(TFile * inputFile);
  virtual void calculateDerivedHistograms();
  virtual void scaleHistograms();

protected:

  const TString makeHistoName(const TString & baseName,
                              const TString & evtFilterName,
                              const TString & partFilterName1,
                              const TString & suffix);
  const TString makePairHistoName(const TString & baseName,
                                  const TString & evtFilterName,
                                  const TString & partFilterName1,
                                  const TString & partFilterName2,
                                  const TString & suffix);

  unsigned int nHistos;
  unsigned int nDerivedHistos;
  unsigned int nCombinedHistos;

   ClassDef(ParticlePairAnalyzer,0)
};


#endif /* WAC_ParticlePairAnalyzer */
