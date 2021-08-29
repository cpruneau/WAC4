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

#include <TLorentzVector.h>
#include "GlobalAnalyzer.hpp"

ClassImp(GlobalAnalyzer);

GlobalAnalyzer::GlobalAnalyzer(const TString &               _name,
                               GlobalAnalyzerConfiguration * _configuration,
                               vector<EventFilter*> &        _eventFilters,
                               vector<ParticleFilter*> &     _particleFilters,
                               LogLevel                      _requiredLevel)
:
Task(),
n(),
e(),
q(),
s(),
b()
{
  setName(_name);
  _configuration->useParticles    = true;
  setConfiguration(_configuration);
  eventFilters     = _eventFilters;
  particleFilters  = _particleFilters;
  setReportLevel(_requiredLevel);
}

void GlobalAnalyzer::initialize()
{
  int nParticleFilters = particleFilters.size();
  n.assign(nParticleFilters,0.0);
  e.assign(nParticleFilters,0.0);
  q.assign(nParticleFilters,0.0);
  s.assign(nParticleFilters,0.0);
  b.assign(nParticleFilters,0.0);
  Task::initialize();
}

void GlobalAnalyzer::createHistograms()
{
  if (reportStart("GlobalAnalyzer",getName(),"createHistograms()"))
    ;
  GlobalAnalyzerConfiguration * ac = (GlobalAnalyzerConfiguration *) getConfiguration();
  unsigned int nEventFilters    = 1;
  unsigned int nParticleFilters = particleFilters.size();
  if (reportInfo("GlobalAnalyzer",getName(),"createHistograms()"))
    {
    cout << "Creating Histograms for..."  << endl;
    cout << "       nEventFilters: " << nEventFilters << endl;
    cout << "    nParticleFilters: " << nParticleFilters << endl;
    }
  TString prefixName    = getName(); 
  TString evtFilterName = eventFilters[0]->getName();
  TString histoName;
  histoName =  prefixName;
  histoName += "_";
  histoName += evtFilterName;
  GlobalHistos * globalHistos = new GlobalHistos(histoName,ac,particleFilters,getReportLevel());
  globalHistos->createHistograms();
  histograms.push_back(globalHistos);
  if (reportEnd("GlobalAnalyzer",getName(),"createHistograms()"))
    ;
}

void GlobalAnalyzer::loadHistograms(TFile * inputFile)
{
  if (reportStart("GlobalAnalyzer",getName(),"loadHistograms(TFile * inputFile)"))
    ;
  GlobalAnalyzerConfiguration * ac = (GlobalAnalyzerConfiguration *) getConfiguration();
  unsigned int nEventFilters    = 1;
  TString prefixName    = getName();
  TString evtFilterName = eventFilters[0]->getName();
  TString histoName;
  histoName =  prefixName;
  histoName += "_";
  histoName += evtFilterName;
  GlobalHistos * globalHistos = new GlobalHistos(histoName,ac,particleFilters,getReportLevel());
  globalHistos-> loadHistograms(inputFile);
  histograms.push_back(globalHistos);
  if (reportEnd("GlobalAnalyzer",getName(),"loadHistograms(TFile * inputFile)"))
    ;
}

void GlobalAnalyzer::execute()
{
  incrementEventProcessed();
  Event * event = eventStreams[0];
  if (!eventFilters[0]->accept(*event)) return;
  incrementEventAccepted(0); // count eventStreams used to fill histograms and for scaling at the end...
  resetParticleCounters();
  GlobalAnalyzerConfiguration * ac = (GlobalAnalyzerConfiguration *) getConfiguration();
  EventProperties & ep = * event->getEventProperties();


  unsigned int nParticleFilters = particleFilters.size();

  if (reportDebug("GlobalAnalyzer",getName(),"execute()"))
    {
    cout << " Event has Nparticles: " << event->getNParticles() << endl;
    }
  if (ac->countParticles)
    {
    n.assign(nParticleFilters,0.0);
    e.assign(nParticleFilters,0.0);
    q.assign(nParticleFilters,0.0);
    s.assign(nParticleFilters,0.0);
    b.assign(nParticleFilters,0.0);    
    for (unsigned int iParticle=0; iParticle<event->getNParticles(); iParticle++)
      {
      incrementParticlesCounted(); // photons are NOT included in this tally

      Particle & particle = * event->getParticleAt(iParticle);
      for (unsigned int iFilter=0; iFilter<nParticleFilters; iFilter++ )
        {
        //cout << iFilter << "  " << particle.getType().getName() << endl;
        if (particleFilters[iFilter]->accept(particle))
          {
          incrementParticlesAccepted();

          ParticleType & type = particle.getType();
          TLorentzVector & momentum = particle.getMomentum();
          n[iFilter]++;
          e[iFilter] += momentum.E();
          q[iFilter] += type.getCharge();
          s[iFilter] += type.getStrange();
          b[iFilter] += type.getBaryon();
          //cout << "$$$$$$$$$$$ Accepted" << endl;
          }
//        else
//          {
//          cout << "XXXXXXXXXXX Rejected" << endl;
//          ParticleType & type = particle.getType();
//          type.printProperties(cout);
//          cout << "         Baryon:" << type.isBaryon() << endl;
//          cout << "          Meson:" << type.isMeson() << endl;
//          cout << "           STAT:" << type.getStatistics()  << endl;
//          cout << "STATUS Accepted:" << particleFilters[iFilter]->acceptStatus(particle.isLive()) << endl;
//          cout << "Charge Accepted:" << particleFilters[iFilter]->acceptCharge(type.getCharge()) << endl;
//          cout << "  Type Accepted:" << particleFilters[iFilter]->acceptType(particle.getType()) << endl;
//          TLorentzVector & momentum = particle.getMomentum();
//          cout<< "              Pt:" << momentum.Pt() << endl;
//          cout<< "             Eta:" << momentum.Eta() << endl;
//          cout<< "               Y:" << momentum.Rapidity() << endl;
//          cout << "    Pt Accepted:" << particleFilters[iFilter]->acceptPt(momentum.Pt())<< endl;
//          cout << "   Eta Accepted:" << particleFilters[iFilter]->acceptEta(momentum.Eta())<< endl;
//          //cout << "Y Accepted:" << particleFilters[iFilter]->acceptY(momentum.Rapidity())<< endl;
//          }
        }
      }
    GlobalHistos * globalHistos = (GlobalHistos * ) histograms[0];
    globalHistos->fill(n,e,q,s,b,1.0);
    if (ac->setEvent)  ep.fill(n,e,q,s,b);
    }
  else
    {
    GlobalHistos * globalHistos = (GlobalHistos * ) histograms[0];
    globalHistos->fill(ep.nFiltered,
                       ep.eFiltered,
                       ep.qFiltered,
                       ep.sFiltered,
                       ep.bFiltered,1.0);
    }
  if (reportDebug("GlobalAnalyzer",getName(),"execute()"))
    {
    event->printProperties(cout);
    }
}


void GlobalAnalyzer::scaleHistograms()
{
  double scalingFactor;
  unsigned int nEventFilters    = eventFilters.size();
  unsigned int nParticleFilters = particleFilters.size();
  if (nFilteredEventsAccepted[0]>0)
    {
    scalingFactor = 1.0/double(nFilteredEventsAccepted[0]);
    if (reportInfo("GlobalAnalyzer",getName(),"scaleHistograms()"))
      {
      cout << endl;
      cout << "              Accepted number of events: " <<  nEventAccepted << endl;
      cout << "                          nEventFilters: " <<  nEventFilters << endl;
      cout << "                       nParticleFilters: " <<  nParticleFilters << endl;
      cout << "                      histograms.size(): " <<  histograms.size() << endl;
      cout << "               derivedHistograms.size(): " <<  derivedHistograms.size() << endl;
      cout << "              combinedHistograms.size(): " <<  combinedHistograms.size() << endl;
      cout << "             nFilteredEventsAccepted[0]: " <<  nFilteredEventsAccepted[0]<< endl;
      cout << "                          scalingFactor: " <<  scalingFactor << endl;
      }
    histograms[0]->scale(scalingFactor);
    if (reportInfo("GlobalAnalyzer",getName(),"scaleHistograms()")) cout << "Completed successfully." << endl;
    }
  else
    {
      if (reportWarning("GlobalAnalyzer",getName(),"scaleHistograms()")) cout << "nFilteredEventsAccepted[0] == 0. No scaling performed." << endl;
    }

}

