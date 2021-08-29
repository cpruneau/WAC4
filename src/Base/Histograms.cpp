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
  setClassName("Histograms");
  setInstanceName(name);
  setReportLevel(debugLevel);
}

//!
//! Overload this class to create histograms.
//!
void Histograms::createHistograms()
{
  setFunctionName("createHistograms()");
  if (reportWarning()) cout << "Implement derived class to create histograms." << endl;
}

//!
//! Overload this class to load histograms.
//!
void Histograms::loadHistograms(TFile * inputFile)
{
  inputFile = 0; // stop warnings;
  setFunctionName("loadHistograms(TFile * inputFile)");
  if (reportWarning()) cout << "Implement derived class to load histograms." << endl;
}

//!
//! Returns the configuration of this task instance.
//!
TaskConfiguration * Histograms::getConfiguration() const
{
  return configuration;
}

//!
//! Sets the configuration of this task instance.
//!
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

//!
//! Overload this class to comple the filling of some histograms.
//! This method is considered deprecated.
//!
void Histograms::completeFill()
{
  //setFunctionName("completeFill()");
  //if (reportWarning()) cout << "Implement derived class to completeFill." << endl;
}

//!
//! Overload this class to calculated derived histograms based on members of primary histogram of this class.
//!
void Histograms::calculateDerivedHistograms()
{
//  setFunctionName("calculateDerivedHistograms()");
//  if (reportWarning()) cout << "Implement derived class to calculate derived histograms." << endl;
}

//!
//! Overload this class to calculated derived histograms based on members of the give primary histogram class instance.
//!
void Histograms::calculateDerivedHistograms(const Histograms & histograms)
{
//  setFunctionName("calculateDerivedHistograms(const Histograms & histograms)");
//  if (reportWarning()) cout << "Implement derived class to calculate derived histograms from external source..." << endl;
}
