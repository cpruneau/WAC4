// Author: Claude Pruneau   09/25/2019

/*************************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *************************************************************************/
#ifndef WAC_Histograms
#define WAC_Histograms
#include "HistogramCollection.hpp"
#include "TaskConfiguration.hpp"

class Histograms : public HistogramCollection
{
public:
  
  Histograms(const TString & name,
             TaskConfiguration * configuration,
             LogLevel  debugLevel);
  Histograms(TFile * inputFile,
             const TString & name,
             TaskConfiguration * configuration,
             LogLevel  debugLevel);
  ~Histograms() {}

  virtual void createHistograms();
  virtual void loadHistograms(TFile * inputFile);
  virtual void completeFill();
  virtual void calculateDerivedHistograms();
  virtual void calculateDerivedHistograms(const Histograms & histograms);

  TaskConfiguration * getConfiguration() const;
  void setConfiguration(TaskConfiguration * config);

  virtual TString getHistoBaseName() const;

protected:
  
  TaskConfiguration * configuration;

    ClassDef(Histograms,0)
};

#endif /* WAC_Histograms */
