// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
/**
 \class Task
 \ingroup WAC

 Class defining Task
 */
#include "CollisionGeometryAnalyzer.hpp"
ClassImp(CollisionGeometryAnalyzer);

CollisionGeometryAnalyzer::CollisionGeometryAnalyzer(const TString &                  _name,
                                                     CollisionGeometryConfiguration * _configuration,
                                                     vector<EventFilter*>             _eventFilters,
                                                     LogLevel                         _requiredLevel)
:
Task(),
collisionGeometry(nullptr)
{
  setName(_name);
  _configuration->useParticles      = true;
  setConfiguration(_configuration);
  setReportLevel  (_requiredLevel);
  setEventFilters (_eventFilters);
}

void CollisionGeometryAnalyzer::createHistograms()
{
  if (reportStart("CollisionGeometryAnalyzer",getName(),"createHistograms()"))
    ;
  CollisionGeometryConfiguration * config = (CollisionGeometryConfiguration *) getConfiguration();
  TString prefixName = getName(); prefixName += "_";
  unsigned int nEventFilters    = eventFilters.size();

  if (reportInfo("CollisionGeometryAnalyzer",getName(),"createHistograms()"))
    {
    cout << "Creating Histograms for..."  << endl;
    cout << "       nEventFilters: " << nEventFilters << endl;
    }
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    cout << " iEventFilter:"  << iEventFilter << endl;
    TString evtFilterName = eventFilters[iEventFilter]->getName();
    cout << " evtFilterName:"  << evtFilterName << endl;
    TString histoName     = prefixName;
    histoName += evtFilterName;
    cout << " histoName:"  << histoName << endl;
    CollisionGeometryHistograms * histos = new CollisionGeometryHistograms(histoName, config, getReportLevel() );
    histos->createHistograms();
    histograms.push_back( histos);
    }
  if (reportEnd("CollisionGeometryAnalyzer",getName(),"createHistograms()"))
    ;
}

void CollisionGeometryAnalyzer::loadHistograms(TFile * inputFile)
{
  if (reportStart("CollisionGeometryAnalyzer",getName(),"loadHistograms(TFile * inputFile)"))
    ;
  CollisionGeometryConfiguration * config = (CollisionGeometryConfiguration *) getConfiguration();
  TString prefixName = getName(); prefixName += "_";
  unsigned int nEventFilters    = eventFilters.size();
  if (reportInfo("CollisionGeometryAnalyzer",getName(),"loadHistograms(TFile * inputFile)"))
    {
    cout << "Loading Histograms for..."  << endl;
    cout << "       nEventFilters: " << nEventFilters << endl;
    }
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    TString evtFilterName = eventFilters[iEventFilter]->getName();
    TString histoName     = prefixName;
    histoName += evtFilterName;
    CollisionGeometryHistograms * histos = new CollisionGeometryHistograms(histoName, config, getReportLevel() );
    histos->loadHistograms(inputFile);
    histograms.push_back( histos);
    }
  if (reportEnd("CollisionGeometryAnalyzer",getName(),"loadHistograms(TFile * inputFile)"))
    ;
}

void CollisionGeometryAnalyzer::execute()
{
  if (reportStart("CollisionGeometryAnalyzer",getName(),"execute()"))
    ;
  incrementEventProcessed();
  bool accept;
  unsigned int nEventFilters    = eventFilters.size();
  Event & event = *eventStreams[0];
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    if (eventFilters[iEventFilter]->accept(event))
      {
      incrementEventAccepted(iEventFilter); // count eventStreams used to fill histograms and for scaling at the end...
      CollisionGeometryHistograms * histos = (CollisionGeometryHistograms *) histograms[iEventFilter];
      histos->fill(event,1.0);
      }
    else
      {
      CollisionGeometryHistograms * histos = (CollisionGeometryHistograms *) histograms[iEventFilter];
      histos->noFill(event,1.0);
      }
    }
  if (reportEnd("CollisionGeometryAnalyzer",getName(),"execute()"))
    ;
}
