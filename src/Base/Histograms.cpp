// Author: Claude Pruneau   09/25/2019

/*************************************************************************
 * Copyright (C) 2019, Claude Pruneau.                                   *
 * All rights reserved.                                                  *
 * Based on the ROOT package and environment                             *
 *                                                                       *
 * For the licensing terms see LICENSE.                                  *
 *************************************************************************/
/**
 \class Task
 \ingroup WAC

 Class defining Event Histograms
 */

#include "Histograms.hpp"
ClassImp(Histograms);

Histograms::Histograms(const TString & name,
                       TaskConfiguration * config,
                       LogLevel  debugLevel)
:
HistogramCollection(name,debugLevel),
configuration(config)
{
  setReportLevel(debugLevel);
}

// overload this class to create histograms...
void Histograms::createHistograms()
{
  if (reportWarning("Histograms",getName(),"createHistograms()"))
    cout << "Implement derived class to create histograms." << endl;
}

// ==============================================================
// load the cluster histograms from the given file and base name
// ==============================================================
void Histograms::loadHistograms(TFile * inputFile)
{
  inputFile = 0; // stop warnings;
  if (reportWarning("Histograms",getName(),"createHistograms()"))
    cout << "Implement derived class to load histograms." << endl;
}

TaskConfiguration * Histograms::getConfiguration() const
{
  return configuration;
}

void Histograms::setConfiguration(TaskConfiguration * config)
{
  configuration = config;
}

TString Histograms::getHistoBaseName() const
{
//  TaskConfiguration & ac = *(TaskConfiguration*) getConfiguration();
  TString bn; //ac.histoBaseName;
//  bn += "_";
  bn = getName();
  bn += "_";
  return bn;
}

void Histograms::completeFill()
{
  // noops in base class
}


void Histograms::calculateDerivedHistograms()
{
  calculateDerivedHistograms(*this);
}

void Histograms::calculateDerivedHistograms(const Histograms & histograms)
{
  // noops in base class
}
