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

#include "ParticleAnalyzerV2.hpp"

ClassImp(ParticleAnalyzerV2);

ParticleAnalyzerV2::ParticleAnalyzerV2(const TString &                 _name,
                                   ParticleAnalyzerConfiguration * _configuration,
                                   vector<EventFilter*>            _eventFilters,
                                   vector<ParticleFilter*>         _particleFilters,
                                   LogLevel                        _selectedLevel)
:
Task(),
filteredParticles(_particleFilters.size())
{
  setName(_name);
  _configuration->useParticles      = true;
  _configuration->createHistograms  = true;
  _configuration->saveHistograms    = true;
  appendClassName("ParticleAnalyzerV2");
  setConfiguration(_configuration);
  setReportLevel(_selectedLevel);
  eventFilters    = _eventFilters;
  particleFilters = _particleFilters;
}


void ParticleAnalyzerV2::createHistograms()
{
  if (reportStart("ParticleAnalyzerV2",getName(),"createHistograms()"))
    ;
  ParticleAnalyzerConfiguration * ac = (ParticleAnalyzerConfiguration *) getConfiguration();
  TString prefixName = getName(); prefixName += "_";
  unsigned int nEventFilters    = eventFilters.size();
  unsigned int nParticleFilters = particleFilters.size();
  
  if (reportInfo("ParticleAnalyzerV2",getName(),"createHistograms()"))
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
      ParticleHistosV2 * histos = new ParticleHistosV2(histoName,ac,getReportLevel());
      histos->createHistograms();
      histograms.push_back(histos);
      }
    }
  if (reportEnd("ParticleAnalyzerV2",getName(),"createHistograms()"))
    ;
}


void ParticleAnalyzerV2::loadHistograms(TFile * inputFile)
{
  if (reportStart("ParticleAnalyzerV2",getName(),"loadHistograms(TFile * inputFile)"))
    ;
  ParticleAnalyzerConfiguration * ac = (ParticleAnalyzerConfiguration *) getConfiguration();
  TString prefixName = getName(); prefixName += "_";
  unsigned int nEventFilters    = eventFilters.size();
  unsigned int nParticleFilters = particleFilters.size();
  if (reportInfo("ParticleAnalyzerV2",getName(),"createHistograms()"))
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
      ParticleHistosV2 * histos = new ParticleHistosV2(histoName,ac,getReportLevel());
      histos->loadHistograms(inputFile);
      histograms.push_back(histos);
      }
    }
  if (reportEnd("ParticleAnalyzerV2",getName(),"loadHistograms(TFile * inputFile)"))
    ;
}

//!
//! Execute a single particle analysis  based on event and particle fileter registered with this task instance.
//! Two options are implemented and chosen automatically by the code.
//! If more than one particle filter is registered with this task, it is assumed that each filter substantially reduced the number of
//! particles to be used to fill histograms. Reduced lists of particles are thus assembled separately (and sequentially) and the filling of the
//! histograms is deledated to the ParticleHistos class used by this task. If instead, a single particle filter is registered with this task
//! instance than there is little justification to produced a sublist and the fill method of the ParticleHistos class is called for each particle.
//! Note that in the case that many reduced lists are used, they are constituted (currently) of vectors or ParticleDigit objects which
//! are essentially structs containing predigitized information about the momentum, the azimuth, the rapidity, and the pseudorapidity.
//!
void ParticleAnalyzerV2::execute()
{
  incrementEventProcessed();
  Event & event = *eventStreams[0];


  //Is this event accepted by this task's event filters?
  bool analyzeThisEvent = false;
  unsigned int nEventFilters = eventFilters.size();
  vector<unsigned int> eventFilterPassed;
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    if (!eventFilters[iEventFilter]->accept(event)) continue;
    incrementEventAccepted(iEventFilter);
    eventFilterPassed.push_back(iEventFilter);
    analyzeThisEvent = true;
    }
  if (!analyzeThisEvent) return;

  // The event is accepted at least one event filter
  // but it may not have  pairs. If so skip out.
  // Doing the checks in this order guarantees the accepted
  // event count is correct for normalization purposes.
  unsigned int nParticles = event.getNParticles();
  if (nParticles<1) return;

  unsigned int nParticleFilters = particleFilters.size();
  vector<double> nAccepted(nParticleFilters,0.0);
  vector<double> totalEnergy(nParticleFilters,0.0);

  if (eventFilterPassed.size()>1 || nParticleFilters>1)
    {
    // multiple events filters or particle filters
    // let us first reduce the number of particles to look at
    // and generate sublists for each particle filter
    // and use them to do the histo filling.

    Factory<ParticleDigit> * factory = ParticleDigit::getFactory();
    factory->reset();
    int index;
    int iEventFilter = 0;
    float pt,e;
    int iPt, iPhi, iEta, iY;
    ParticleHistosV2 * histos = (ParticleHistosV2 *) histograms[0];
    for (unsigned int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ ) filteredParticles[iParticleFilter].clear();

    for (unsigned int iParticle=0; iParticle<nParticles; iParticle++)
      {
      Particle & particle = * event.getParticleAt(iParticle);
      float pt,e;
      int iPt, iPhi, iEta, iY;
      ParticleDigit * pd;
      bool digitized = false;
      for (unsigned int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
        {
        if (particleFilters[iParticleFilter]->accept(particle))
          {
          if (!digitized)
            {
            TLorentzVector & momentum = particle.getMomentum();
            pt     = momentum.Pt();
            e      = momentum.E();
            iPt    = histos->getPtBinFor(pt);
            iPhi   = histos->getPhiBinFor(momentum.Phi());
            iEta   = histos->getEtaBinFor(momentum.Eta());
            iY     = histos->getYBinFor(momentum.Rapidity());
            pd     = factory->getNextObject();
            pd->iY   = iY;
            pd->iEta = iEta;
            pd->iPt  = iPt;
            pd->iPhi = iPhi;
            pd->pt   = pt;
            pd->e    = e;
            digitized = true; // so no need to digitize this particle again...
            }
          if (digitized && iPt>0 && iPhi>0 && iEta>=0 && iY>0)
            {
            filteredParticles[iParticleFilter].push_back(pd);
            }
          } // particle accepted by filter
        } // filter loop
      } //particle loop

    // use the filter particles to fill the histos for the accepted event filters
    for (unsigned int jEventFilter=0; jEventFilter<eventFilterPassed.size(); jEventFilter++ )
      {
      iEventFilter = eventFilterPassed[jEventFilter];
      for (unsigned int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
        {
        index = iParticleFilter+iEventFilter*nParticleFilters;
        ParticleHistosV2 * histos = (ParticleHistosV2 *) histograms[index];
        histos->fill(filteredParticles[iParticleFilter],1.0);
        } // iParticleFilter loop
      } // jEventFilter loop
    }
  else
    {
    // 1 event filter and 1 particle filter: just scan the particles...
    // no need for sublists.
    int iParticleFilter = 0;
    int iEventFilter = eventFilterPassed[0];
    int index = iParticleFilter+iEventFilter*nParticleFilters;
    ParticleHistosV2 * histos = (ParticleHistosV2 *) histograms[index];
    for (unsigned int iParticle=0; iParticle<nParticles; iParticle++)
      {
      Particle & particle = * event.getParticleAt(iParticle);
      TLorentzVector & momentum = particle.getMomentum();
      if (particleFilters[iParticleFilter]->accept(particle))
        {
        //cout << "ParticlePairAnalyzerV2::execute() accepted:" << endl;
        nAccepted[iParticleFilter]++;
        totalEnergy[iParticleFilter] += particle.getMomentum().E();
        histos->fill(particle,1.0);
        }
      }
    histos->fillMultiplicity(nAccepted[iParticleFilter],totalEnergy[iParticleFilter],1.0);
    }
// all done with this event...
}
