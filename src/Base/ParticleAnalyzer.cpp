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

#include "ParticleAnalyzer.hpp"

ClassImp(ParticleAnalyzer);

ParticleAnalyzer::ParticleAnalyzer(const TString &                 _name,
                                   ParticleAnalyzerConfiguration * _configuration,
                                   vector<EventFilter*>            _eventFilters,
                                   vector<ParticleFilter*>         _particleFilters,
                                   LogLevel                        _selectedLevel)
:
Task()
{
  setName(_name);
  _configuration->useParticles      = true;
  _configuration->createHistograms  = true;
  _configuration->saveHistograms    = true;
  setConfiguration(_configuration);
  setReportLevel(_selectedLevel);
  eventFilters    = _eventFilters;
  particleFilters = _particleFilters;
}


void ParticleAnalyzer::createHistograms()
{
  if (reportStart("ParticleAnalyzer",getName(),"createHistograms()"))
    ;
  ParticleAnalyzerConfiguration * ac = (ParticleAnalyzerConfiguration *) getConfiguration();
  TString prefixName = getName(); prefixName += "_";
  unsigned int nEventFilters    = eventFilters.size();
  unsigned int nParticleFilters = particleFilters.size();
  nFilteredEventsAccepted.assign(nEventFilters,0.0);
  if (reportInfo("ParticleAnalyzer",getName(),"createHistograms()"))
    {
    cout << "Creating Histograms for..."  << endl;
    cout << "       nEventFilters: " << nEventFilters << endl;
    cout << "    nParticleFilters: " << nParticleFilters << endl;
    }
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    TString evtFilterName = eventFilters[iEventFilter]->getName();
    for (unsigned int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
      {
      TString partFilterName = particleFilters[iParticleFilter]->getName();
      TString histoName  = prefixName;
      histoName += evtFilterName;
      histoName += "_";
      histoName += partFilterName;
      ParticleHistos * histos = new ParticleHistos(histoName,ac,getReportLevel());
      histos->createHistograms();
      histograms.push_back(histos);
      }
    }
  if (reportEnd("ParticleAnalyzer",getName(),"createHistograms()"))
    ;
}


void ParticleAnalyzer::loadHistograms(TFile * inputFile)
{
  if (reportStart("ParticleAnalyzer",getName(),"loadHistograms(TFile * inputFile)"))
    ;
  ParticleAnalyzerConfiguration * ac = (ParticleAnalyzerConfiguration *) getConfiguration();
  TString prefixName = getName(); prefixName += "_";
  unsigned int nEventFilters    = eventFilters.size();
  unsigned int nParticleFilters = particleFilters.size();
  if (reportInfo("ParticleAnalyzer",getName(),"createHistograms()"))
    {
    cout << "Loading Histograms for..."  << endl;
    cout << "       nEventFilters: " << nEventFilters << endl;
    cout << "    nParticleFilters: " << nParticleFilters << endl;
    }
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    TString evtFilterName = * eventFilters[iEventFilter]->getName();
    for (unsigned int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
      {
      TString partFilterName = * particleFilters[iParticleFilter]->getName();
      TString histoName  = prefixName;
      histoName += evtFilterName;
      histoName += "_";
      histoName += partFilterName;
      ParticleHistos * histos = new ParticleHistos(histoName,ac,getReportLevel());
      histos->loadHistograms(inputFile);
      histograms.push_back(histos);
      }
    }
  if (reportEnd("ParticleAnalyzer",getName(),"loadHistograms(TFile * inputFile)"))
    ;
}


void ParticleAnalyzer::execute()
{
  incrementEventProcessed();
  bool accept;
  unsigned int nEventFilters    = eventFilters.size();
  unsigned int nParticleFilters = particleFilters.size();
  Event * event = eventStreams[0];
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    if (!eventFilters[iEventFilter]->accept(*event)) continue;
    incrementEventAccepted();
    nFilteredEventsAccepted[iEventFilter]++;
    vector<double> nAccepted(nParticleFilters,0.0);
    vector<double> totalEnergy(nParticleFilters,0.0);
    unsigned int nParticles = event->getNParticles();
    for (unsigned int iParticle=0; iParticle<nParticles; iParticle++)
      {
      Particle & particle = * event->getParticleAt(iParticle);
      for (unsigned int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
        {
        ParticleHistos * histos = (ParticleHistos *) histograms[iParticleFilter+iEventFilter*nParticleFilters];
        accept = particleFilters[iParticleFilter]->accept(particle);
        if (accept)
          {
          nAccepted[iParticleFilter]++;
          totalEnergy[iParticleFilter] += particle.getMomentum().E();
          histos->fill(particle,1.0);
          }
        }
      }
    for (unsigned int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
      {
      ParticleHistos * histos = (ParticleHistos *) histograms[iParticleFilter+iEventFilter*nParticleFilters];
      histos->fillMultiplicity(nAccepted[iParticleFilter],totalEnergy[iParticleFilter],1.0);
      }
    }
}
