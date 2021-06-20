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

 Class defining Two Particle Correlation Analyzer Task
 */
#include "NuDynTask.hpp"

ClassImp(NuDynTask);

NuDynTask::NuDynTask(const TString &         _name,
                     NuDynConfiguration *    _configuration,
                     vector<EventFilter*>    _eventFilters,
                     vector<ParticleFilter*> _particleFilters,
                     LogLevel                _selectedLevel)
:
Task()
{
  setName(_name);
  _configuration->useParticles = true;
  setConfiguration(_configuration);
  setReportLevel  (_selectedLevel);
  eventFilters    = _eventFilters;
  particleFilters = _particleFilters;
}



void NuDynTask::initialize()
{
  createIdentical();
  Task::initialize();
}


void NuDynTask::createHistograms()
{
  if (reportStart("NuDynTask",getName(),"createHistograms()"))
    ;
  unsigned int nEventFilters    = eventFilters.size();
  unsigned int nParticleFilters = particleFilters.size();
  NuDynConfiguration * ac = (NuDynConfiguration *) getConfiguration();
  TString prefixName = getName(); prefixName += "_";
  TString evtFilterName;
  TString histoName;
  TString partFilterName;
  nFilteredEventsAccepted.assign(nEventFilters,0.0);
  if (reportInfo("ParticleAnalyzer",getName(),"createHistograms()"))
    {
    cout << "Creating Histograms for..."  << endl;
    cout << "       nEventFilters: " << nEventFilters << endl;
    cout << "    nParticleFilters: " << nParticleFilters << endl;
    }
  partFilterName = particleFilters[0]->getName();
  for (unsigned int iParticleFilter=1; iParticleFilter<nParticleFilters; iParticleFilter++ )
    {
    partFilterName += particleFilters[iParticleFilter]->getName();
    }
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    evtFilterName  = eventFilters[iEventFilter]->getName();
    histoName      = prefixName;
    histoName      += evtFilterName;
    histoName      += "_";
    histoName      += partFilterName;
    NuDynHistos * nuDynHistos = new NuDynHistos(histoName,identical,ac,getReportLevel());
    nuDynHistos->createHistograms();
    histograms.push_back(nuDynHistos);
    if (ac->calculateDerivedHistograms)
      {
      NuDynDerivedHistos * nuDynDerivedHistos = new NuDynDerivedHistos(histoName,ac,getReportLevel());
      nuDynDerivedHistos->createHistograms();
      derivedHistograms.push_back(nuDynDerivedHistos);
      }
    }
  if (reportEnd("NuDynTask",getName(),"createHistograms()"))
    ;
}

// =========================================
// load histograms from given files
// =========================================
void NuDynTask::loadHistograms(TFile * inputFile)
{
  if (reportStart("NuDynTask",getName(),"loadHistograms(TFile * inputFile)"))
    ;
  unsigned int nEventFilters    = eventFilters.size();
  unsigned int nParticleFilters = particleFilters.size();
  NuDynConfiguration * ac = (NuDynConfiguration *) getConfiguration();
  TString prefixName = getName(); prefixName += "_";
  TString evtFilterName;
  TString histoName;
  TString partFilterName;
  nFilteredEventsAccepted.assign(nEventFilters,0.0);
  if (reportInfo("ParticleAnalyzer",getName(),"createHistograms()"))
    {
    cout << "Creating Histograms for..."  << endl;
    cout << "       nEventFilters: " << nEventFilters << endl;
    cout << "    nParticleFilters: " << nParticleFilters << endl;
    }
  partFilterName = particleFilters[0]->getName();
  for (unsigned int iParticleFilter=1; iParticleFilter<nParticleFilters; iParticleFilter++ )
    {
    partFilterName += particleFilters[iParticleFilter]->getName();
    }
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    evtFilterName  = eventFilters[iEventFilter]->getName();
    histoName      = prefixName;
    histoName      += evtFilterName;
    histoName      += "_";
    histoName      += partFilterName;
    NuDynHistos * nuDynHistos = new NuDynHistos(histoName,identical,ac,getReportLevel());
    nuDynHistos->loadHistograms(inputFile);
    histograms.push_back(nuDynHistos);
    if (ac->calculateDerivedHistograms)
      {
      NuDynDerivedHistos * nuDynDerivedHistos = new NuDynDerivedHistos(histoName,ac,getReportLevel());
      nuDynDerivedHistos->loadHistograms(inputFile);
      derivedHistograms.push_back(nuDynDerivedHistos);
      }
    }
  if (reportEnd("NuDynTask",getName(),"loadHistograms(TFile * inputFile)"))
    ;
}

void NuDynTask::execute()
{
  incrementEventProcessed();
  NuDynConfiguration & ac = *(NuDynConfiguration *) getConfiguration();

  unsigned int nEventFilters    = eventFilters.size();
  unsigned int nParticleFilters = particleFilters.size();
  Event * event = eventStreams[0];
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    if (!eventFilters[iEventFilter]->accept(*event)) continue;
    incrementEventAccepted(); // count eventStreams used to fill histograms and for scaling at the end...
    nFilteredEventsAccepted[iEventFilter]++;
    vector<double> nAccepted(nParticleFilters,0.0);
    for (unsigned long  iParticle=0; iParticle<event->getNParticles(); iParticle++)
      {
      Particle & particle = * event->getParticleAt(iParticle);
      for (unsigned int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++)
        {
        if (particleFilters[iParticleFilter]->accept(particle)) nAccepted[iParticleFilter]++;
        }
      }
    EventProperties ep = * event->getEventProperties();
    NuDynHistos * nuDynHistos = (NuDynHistos *) histograms[iEventFilter];
    switch ( ac.multiplicityType )
      {
        case NuDynConfiguration::Centrality:           nuDynHistos->fill(ep.centrality,   nAccepted,1.0); break;
        case NuDynConfiguration::TotalMultiplicity:    nuDynHistos->fill(ep.multiplicity, nAccepted,1.0); break;
        case NuDynConfiguration::AcceptedMultiplicity: nuDynHistos->fill(ep.multiplicity, nAccepted,1.0); break;
      }
    }
}

void NuDynTask::createIdentical()
{
  identical[0] = 1; //(particleFilters[0]==particleFilters[0]) ? 1 : 0;
  identical[1] = (particleFilters[0]==particleFilters[1]) ? 1 : 0;
  identical[2] = (particleFilters[0]==particleFilters[2]) ? 1 : 0;
  identical[3] = (particleFilters[0]==particleFilters[3]) ? 1 : 0;
  identical[4] = (particleFilters[1]==particleFilters[0]) ? 1 : 0;
  identical[5] = 1; //(particleFilters[1]==particleFilters[1]) ? 1 : 0;
  identical[6] = (particleFilters[1]==particleFilters[2]) ? 1 : 0;
  identical[7] = (particleFilters[1]==particleFilters[3]) ? 1 : 0;
  identical[8] = (particleFilters[2]==particleFilters[0]) ? 1 : 0;
  identical[9] = (particleFilters[2]==particleFilters[1]) ? 1 : 0;
  identical[10] = 1; //(particleFilters[2]==particleFilters[2]) ? 1 : 0;
  identical[11] = (particleFilters[2]==particleFilters[3]) ? 1 : 0;
  identical[12] = (particleFilters[3]==particleFilters[0]) ? 1 : 0;
  identical[13] = (particleFilters[3]==particleFilters[1]) ? 1 : 0;
  identical[14] = (particleFilters[3]==particleFilters[2]) ? 1 : 0;
  identical[15] = 1; //(particleFilters[3]==particleFilters[3]) ? 1 : 0;
}
