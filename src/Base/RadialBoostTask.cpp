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

ClassImp(RadialBoostTask);




RadialBoostTask::RadialBoostTask(RadialBoostConfiguration * _configuration,
                                 LogLevel                   _selectedLevel)
:
Task(),
param_b(0),
param_a(0),
betaMaximum(0)
{
  setName("RadialBoost");
  setConfiguration(_configuration);
  setReportLevel(_selectedLevel);
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
  TString prefixName  = getName(); prefixName += "_";
  RadialBoostHistos * histos = new RadialBoostHistos(prefixName,ac,debugLevel);
  histos->loadHistograms(inputFile);
  histograms.push_back(histos);
  if (reportEnd("RadialBoostTask",getName(),"loadHistograms(TFile * inputFile)"))
    ;
}

void RadialBoostTask::execute()
{
//  if (reportInfo("RadialBoostTask",getName(),"execute()"))
//    ;
  double beta, betax, betay;
  double rx, ry, phi, r;
  Event & event = * eventStreams[0];
  unsigned int nParticles = event.getNParticles();
  for (unsigned int iParticle=0; iParticle<nParticles; iParticle++)
  {

  Particle & particle = * event.getParticleAt(iParticle);
  if (particle.isLive() || particle.isInteraction())
    {
    TLorentzVector & position = particle.getPosition();
    rx  = position.X(); // units are fm
    ry  = position.Y();
    r   = sqrt(rx*rx+ry*ry);
    if (r>30.0) continue;
    phi = 0.0;
    if (r > 1E-6) phi = atan2(ry,rx);
    if (phi<0) phi += TMath::TwoPi();
    beta = param_a * TMath::Power(r, param_b);
    if (beta > betaMaximum) beta = betaMaximum;
    betax = beta * cos(phi);
    betay = beta * sin(phi);
    RadialBoostHistos * histos = (RadialBoostHistos *) histograms[0];
    histos->fill(r,phi,beta,1.0);
    particle.boost(betax,betay,0.0);
    }
  }
}
