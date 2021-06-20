//
//  EventCountHistos.cpp
//  MyMC
//
//  Created by Claude Pruneau on 9/23/16.
//  Copyright © 2016 Claude Pruneau. All rights reserved.
//

#include "TLorentzVector.h"
#include "EventCountHistos.hpp"
ClassImp(EventCountHistos);

EventCountHistos::EventCountHistos(const TString & name,
                                   LogLevel  debugLevel)
:
Histograms(name,nullptr,debugLevel),
h_eventCounts(nullptr)
{
  // no ops
}

// for now use the same boundaries for eta and y histogram
void EventCountHistos::createHistograms()
{
  TString bn = getHistoBaseName();
  h_eventCounts = createHistogram(bn+TString("eventCount"),100,  0.0,  100.0,  "FilterIndex","EventCount", false,true,false,false);
 }

//________________________________________________________________________
void EventCountHistos::loadHistograms(TFile * inputFile)
{
  if (!inputFile)
    {
    if (reportFatal()) cout << "Attempting to load EventCountHistos from an invalid file pointer" << endl;
    return;
    }
  TString bn = getHistoBaseName();
  TString histoName = bn+TString("eventCount");
  h_eventCounts = loadH1(inputFile,histoName);
  if (!h_eventCounts)
    {
    if (reportError()) cout << "Could not load histogram: " << histoName << endl;
    return;
    }
}

// This fills the event count histogram. It should be called only
// once per event or once per histo file save.
void EventCountHistos::fill(vector<double> eventCounts)
{
  double x = 0.5;
  for (unsigned int k=0; k<eventCounts.size(); k++)
    {
    h_eventCounts->Fill(x,eventCounts[k]);
    x += 1.0;
    }
}

