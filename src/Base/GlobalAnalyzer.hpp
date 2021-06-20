// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/

#ifndef WAC_GlobalAnalyzer
#define WAC_GlobalAnalyzer
#include <TParameter.h>
#include "TFile.h"
#include "TList.h"
#include "Task.hpp"
#include "Event.hpp"
#include "EventFilter.hpp"
#include "ParticleFilter.hpp"
#include "GlobalHistos.hpp"
#include "GlobalAnalyzerConfiguration.hpp"

class GlobalAnalyzer : public Task
{
public:

  //GlobalAnalyzer();

  GlobalAnalyzer(const TString &               _name,
                 GlobalAnalyzerConfiguration * _configuration,
                 vector<EventFilter*> &        _eventFilters,
                 vector<ParticleFilter*> &     _particleFilters,
                 LogLevel                      _requiredLevel=Info);
  virtual ~GlobalAnalyzer() {} 
  virtual void initialize();
  virtual void execute();
  virtual void createHistograms();
  virtual void loadHistograms(TFile * inputFile);
  virtual void scaleHistograms();

  vector<double> n;
  vector<double> e;
  vector<double> q;
  vector<double> s;
  vector<double> b;

  ClassDef(GlobalAnalyzer,0)
};


#endif /* WAC_GlobalAnalyzer */
