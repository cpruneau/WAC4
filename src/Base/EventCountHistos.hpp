// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
#ifndef WAC_EventCountHistos
#define WAC_EventCountHistos
#include "Histograms.hpp"
#include "Particle.hpp"
#include "ParticleAnalyzerConfiguration.hpp"
#include "TLorentzVector.h"


class EventCountHistos : public Histograms
{
public:

  EventCountHistos(const TString & collectionName,
                   LogLevel  debugLevel);
  virtual ~EventCountHistos(){}
  virtual void createHistograms();
  virtual void loadHistograms(TFile * inputFile);
  virtual void fill(vector<double> eventCounts);

  // //////////////////////////////////////////////////////////////////////////
  // Data Members - Histograms
  // //////////////////////////////////////////////////////////////////////////
  TH1 * h_eventCounts;

    ClassDef(EventCountHistos,0)

};

#endif /* WAC_EventCountHistos  */



