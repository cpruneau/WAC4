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

#include "RadialBoostTask.hpp"
#include "CollisionGeometryGradientHistograms.hpp"

ClassImp(RadialBoostTask);

RadialBoostTask::RadialBoostTask(const TString &            _name,
                                 RadialBoostConfiguration * _configuration,
                                 vector<EventFilter*>       _eventFilters,
                                 LogLevel                   _selectedLevel)
:
Task(),
param_b(0),
param_a(0),
betaMaximum(0)
{
  _configuration->useParticles      = true;
  _configuration->loadHistograms    = true;
  _configuration->createHistograms  = true;
  _configuration->saveHistograms    = true;
  setName(_name);
  setConfiguration(_configuration);
  setReportLevel(_selectedLevel);
  eventFilters  = _eventFilters;
 }

void RadialBoostTask::createHistograms()
{
  if (reportStart("RadialBoostTask",getName(),"createHistograms()"))
    ;
  RadialBoostConfiguration * ac = (RadialBoostConfiguration *) getConfiguration();
  LogLevel debugLevel = getReportLevel();
  param_b     = ac->param_b; // exponent of order 1
  param_a     = ac->param_a;
  betaMaximum = ac->betaMaximum;
  TString prefixName  = getName(); prefixName += "_";
  RadialBoostHistos * histos = new RadialBoostHistos(prefixName,ac,debugLevel);
  histos->createHistograms();
  histograms.push_back(histos);
  if (reportEnd("RadialBoostTask",getName(),"createHistograms()"))
    ;
}

void RadialBoostTask::loadHistograms(TFile * inputFile)
{
  if (reportStart("RadialBoostTask",getName(),"loadHistograms(TFile * inputFile)"))
    ;
  RadialBoostConfiguration * ac = (RadialBoostConfiguration *) getConfiguration();
  LogLevel debugLevel = getReportLevel();
  TString prefixName  = ac->getBaseName(); prefixName += "_";
  unsigned int nEventFilters = eventFilters.size();
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    TString evtFilterName = eventFilters[iEventFilter]->getName();
    TString histoName  = prefixName;
    histoName += evtFilterName;
    CollisionGeometryGradientHistograms * histos = new CollisionGeometryGradientHistograms(histoName,nullptr,getReportLevel());
    histos->loadHistograms(inputFile);
    inputHistograms.push_back(histos);
    }

  if (reportEnd("RadialBoostTask",getName(),"loadHistograms(TFile * inputFile)"))
    ;
}

void RadialBoostTask::execute()
{
  if (reportStart("RadialBoostTask",getName(),"execute()"))
    ;
  double beta, betax, betay;
  double rx, ry, r, gx,gy, phi;
  unsigned int nEventFilters    = eventFilters.size();
 // unsigned int nParticleFilters = particleFilters.size();
  Event & event = * eventStreams[0];

  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    if (!eventFilters[iEventFilter]->accept(event)) continue;
    incrementEventAccepted(iEventFilter);
    CollisionGeometryGradientHistograms * cggh = (CollisionGeometryGradientHistograms *) inputHistograms[iEventFilter];

    unsigned int nParticles = event.getNParticles();
    for (unsigned int iParticle=0; iParticle<nParticles; iParticle++)
      {

      Particle & particle = * event.getParticleAt(iParticle);
      if (particle.isLive() || particle.isInteraction() )
        {
        TLorentzVector & position = particle.getPosition();
        rx  = position.X(); // units are fm
        ry  = position.Y();
        cggh->getRadiusAndGradient(rx,ry, r,gx,gy);
//        phi = 0.0;
        phi = TMath::ATan2(gy,gx);
        if (phi<0) phi += TMath::TwoPi();
        beta = param_a * TMath::Power(r, param_b);
        if (beta > betaMaximum) beta = betaMaximum;
        //cout << " gx:" << gx << "  gy:" << gy << "  phi:" << phi*180.0/3.1415927 << endl;
        double g = sqrt(gx*gx+gy*gy);
        betax = beta * gx/g;
        betay = beta * gy/g;
        RadialBoostHistos * histos = (RadialBoostHistos *) histograms[0];
        histos->fill(rx,ry,r,phi,beta,1.0);
        particle.boost(betax,betay,0.0);
        }
      }
    }
  if (reportEnd("RadialBoostTask",getName(),"execute()"))
    ;

}
