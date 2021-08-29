//
//  ParticleHistosV2.cpp
//  MyMC
//
//  Created by Claude Pruneau on 9/23/16.
//  Copyright © 2016 Claude Pruneau. All rights reserved.
//

#include "TLorentzVector.h"
#include "ParticleHistosV2.hpp"
ClassImp(ParticleHistosV2);

ParticleHistosV2::ParticleHistosV2(const TString & name,
                                   ParticleAnalyzerConfiguration * configuration,
                                   LogLevel  debugLevel)
:
Histograms(name,configuration,debugLevel),
fillEta(configuration->fillEta),
fillY(configuration->fillY),
fillP2(configuration->fillP2),
nBins_n1(configuration->nBins_n1),
min_n1(configuration->min_n1),
max_n1(configuration->max_n1),
nBins_pt(configuration->nBins_pt),
min_pt(configuration->min_pt),
max_pt(configuration->max_pt),
range_pt(configuration->max_pt-configuration->min_pt),
nBins_phi(configuration->nBins_phi),
min_phi(configuration->min_phi),
max_phi(configuration->max_phi),
range_phi(configuration->max_phi-configuration->min_phi),
nBins_eta(configuration->nBins_eta),
min_eta(configuration->min_eta),
max_eta(configuration->max_eta),
range_eta(configuration->max_eta-configuration->min_eta),
nBins_y(configuration->nBins_y),
min_y(configuration->min_y),
max_y(configuration->max_y),
range_y(configuration->max_y-configuration->min_y),
h_n1(nullptr),
h_n1_eTotal(nullptr),
h_n1_pt(nullptr),
h_n1_ptXS(nullptr),
h_n1_phi(nullptr),
h_spt_phi(nullptr),
h_n1_eta(nullptr),
h_n1_phiEta(nullptr),
h_spt_eta(nullptr),
h_spt_phiEta(nullptr),
h_n1_y(nullptr),
h_n1_phiY(nullptr),
h_spt_y(nullptr),
h_spt_phiY(nullptr),
h_pt_phi(nullptr),
h_pt_eta(nullptr),
h_pt_phiEta(nullptr),
h_pt_y(nullptr),
h_pt_phiY(nullptr)
{
  appendClassName("ParticleHistosV2");
  setInstanceName(name);
}

ParticleHistosV2::~ParticleHistosV2()
{
  //deleteHistograms();
}

// for now use the same boundaries for eta and y histogram
void ParticleHistosV2::createHistograms()
{
  TString bn = getHistoBaseName();
  h_n1         = createHistogram(bn+TString("n1"),           nBins_n1,  min_n1,  max_n1,  "n_1","N", true,true,true,false);
  h_n1_eTotal  = createHistogram(bn+TString("n1_eTotal"),    nBins_n1,  min_n1,  10.0*max_n1,  "n1_eTotal","N", true,true,true,false);
  h_n1_pt      = createHistogram(bn+TString("n1_pt"),        nBins_pt,  min_pt,  max_pt,  "p_{T}","N", true,true,true,false);
  h_n1_ptXS    = createHistogram(bn+TString("n1_ptXS"),      nBins_pt,  min_pt,  max_pt,  "p_{T}","1/p_{T} dN/p_{T}", true,true,true,false);

  if (fillEta)
    {
    h_n1_phiEta  = createHistogram(bn+TString("n1_phiEta"),  nBins_eta, min_eta, max_eta, nBins_phi, min_phi, max_phi, "#eta", "#varphi","N", true,true,true,false);
    h_n1_phi     = createHistogram(bn+TString("n1_phi"),     nBins_phi, min_phi, max_phi, "#varphi","N", 0,1,0,0);
    h_n1_eta     = createHistogram(bn+TString("n1_eta"),     nBins_eta, min_eta, max_eta, "#eta","N", 0,1,0,0);
    if (fillP2)
      {
      h_spt_phiEta  = createHistogram(bn+TString("spt_phiEta"),  nBins_eta, min_eta, max_eta, nBins_phi, min_phi, max_phi, "#eta", "#varphi","N", true,true,true,false);
      h_spt_phi     = createHistogram(bn+TString("sumpt1_phi"),   nBins_phi, min_phi, max_phi, "#varphi","#sum p_{T}",0,1,0,0);
      h_pt_phi      = createHistogram(bn+TString("pt_phi"), nBins_phi, min_phi, max_phi,  "#varphi",  "<p_{T}>",0,1,0,0);
      h_spt_eta     = createHistogram(bn+TString("sumpt1_eta"), nBins_eta, min_eta, max_eta, "#eta", "#sum p_{T}", 0,1,0,0);
      h_pt_eta      = createHistogram(bn+TString("pt_eta"), nBins_eta, min_eta, max_eta,  "#eta",  "<p_{T}>",0,1,0,0);
      }
    }
  if (fillY)
    {
    h_n1_phiY  = createHistogram(bn+TString("n1_phiY"),     nBins_y, min_y, max_y, nBins_phi, min_phi, max_phi, "#eta", "#varphi","N", true,true,true,false);
    h_n1_phi   = createHistogram(bn+TString("n1_phi"),      nBins_phi, min_phi, max_phi, "#varphi","N", 0,1,0,0);
    h_n1_y     = createHistogram(bn+TString("n1_y"),            nBins_eta, min_eta, max_eta, "y","N", 0,1,0,0);


    if (fillP2)
      {
      h_spt_phiY  = createHistogram(bn+TString("spt_phiY"),  nBins_y, min_y, max_y, nBins_phi, min_phi, max_phi, "y", "#varphi","N", true,true,true,false);
      h_spt_phi   = createHistogram(bn+TString("sumpt1_phi"),nBins_phi, min_phi, max_phi, "#varphi","#sum p_{T}", 0,1,0,0);
      h_spt_y     = createHistogram(bn+TString("sumpt1_y"),  nBins_y, min_y, max_y, "y", "#sum p_{T}",0,1,0,0);
      h_pt_y      = createHistogram(bn+TString("pt_y"),      nBins_y,  min_y,  max_y,  "y",  "<p_{T}>",0,1,0,0);
      h_pt_phi    = createHistogram(bn+TString("pt_phi"),    nBins_phi,  min_phi,  max_phi,  "#varphi",  "<p_{T}>",0,1,0,0);
      }
    }
}

//________________________________________________________________________
void ParticleHistosV2::loadHistograms(TFile * inputFile)
{
  TString fct = "loadHistograms(TFile * inputFile)";
  if (reportStart(fct))
    ;
  if (!ptrFileExist(fct,inputFile)) return;
}

//!
//! Fiil  single particle histograms of this class with the particles contained in the given list.
//!
void ParticleHistosV2::fill(vector<ParticleDigit*> & particles, double weight)
{
  double nSingles    = 0;
  double totalEnergy = 0;

  for (unsigned int iPart=0; iPart<particles.size(); iPart++)
    {
    float        e    = particles[iPart]->e;
    float        pt   = particles[iPart]->pt;
    unsigned int iPt  = particles[iPart]->iPt;
    unsigned int iPhi = particles[iPart]->iPhi;
    unsigned int iEta = particles[iPart]->iEta;
    unsigned int iY   = particles[iPart]->iY;

    nSingles++;
    totalEnergy += e;

    int iG = h_n1_pt->GetBin(iPt);
    h_n1_pt  ->AddBinContent(iG,weight);
    h_n1_ptXS->AddBinContent(iG,weight/pt);

    if (fillEta)
      {
      iG = h_n1_phiEta->GetBin(iEta,iPhi);
      h_n1_phiEta->AddBinContent(iG,weight);
      if (fillP2) h_spt_phiEta->AddBinContent(iG,weight*pt);
      }

    if (fillY)
      {
      iG = h_n1_phiY->GetBin(iY,iPhi);
      h_n1_phiY->AddBinContent(iG,weight);
      if (fillP2) h_spt_phiY->AddBinContent(iG,weight*pt);
      }
    }
  h_n1_pt->SetEntries(h_n1_pt->GetEntries()+nSingles);
  h_n1_ptXS->SetEntries(h_n1_ptXS->GetEntries()+nSingles);
  if (fillEta)
    {
    h_n1_phiEta->SetEntries(h_n1_phiEta->GetEntries()+nSingles);
    if (fillP2) h_spt_phiEta->SetEntries(h_spt_phiEta->GetEntries()+nSingles);
    }
  if (fillY)
    {
    h_n1_phiY->SetEntries(h_n1_phiY->GetEntries()+nSingles);
    if (fillP2) h_spt_phiY->SetEntries(h_spt_phiY->GetEntries()+nSingles);
    }
  h_n1->Fill(nSingles, weight);
  h_n1_eTotal->Fill(totalEnergy, weight);
}

//!
//! Fiil  single particle histograms of this class with the particles contained in the given list.
//!
void ParticleHistosV2::fill(Particle & particle, double weight)
{
  TLorentzVector & momentum = particle.getMomentum();
  float pt   = momentum.Pt();
  float eta  = momentum.Eta();
  float phi  = momentum.Phi();
  if (phi<0) phi += TMath::TwoPi();
  h_n1_pt  ->Fill(pt,weight);
  h_n1_ptXS->Fill(pt,weight/pt);
  if (fillEta)
    {
    h_n1_phiEta->Fill(eta,phi,weight);
    if (fillP2) h_spt_phiEta->Fill(eta,phi,weight*pt);
    }
  if (fillY)
    {
    float rapidity = momentum.Rapidity();
    h_n1_phiY->Fill(rapidity,phi,weight);
    if (fillP2) h_spt_phiY->Fill(rapidity,phi,weight*pt);
    }
}

//!
//! Fiil the global histogram for the multiplicity and the total energy of an event.
//! Call this function only once per event.
//!
void ParticleHistosV2::fillMultiplicity(double nAccepted, double totalEnergy, double weight)
{
  h_n1->Fill(nAccepted, weight);
  h_n1_eTotal->Fill(totalEnergy, weight);
}

//!
//! Calculates derived histograms. This is currently limited to pt averages vs eta and phi.
//!
void ParticleHistosV2::calculateDerivedHistograms()
{
  //setHoldLogLevel(MessageLogger::Debug);

  TString fct = "calculateDerivedHistograms()";
  if (reportStart(fct))
    ;
  TString bn = getHistoBaseName();
  TH1* hTemp;

  if (h_n1_phiEta)
    {
    if (reportDebug(fct)) cout << "Compute h_n1_phi" << endl;
    hTemp = h_n1_phiEta->ProjectionY();
    h_n1_phi->Add(hTemp);
    delete hTemp;

    if (reportDebug(fct)) cout << "Compute h_n1_eta" << endl;
    hTemp = h_n1_phiEta->ProjectionX();
    h_n1_eta->Add(hTemp);
    delete hTemp;

    if (fillP2)
      {
      if (reportDebug(fct)) cout << "Compute sumpt1_phi" << endl;
      hTemp = h_spt_phiEta->ProjectionY();
      h_spt_phi->Add(hTemp);
      delete hTemp;

      if (reportDebug(fct)) cout << "Compute pt_phi" << endl;
      calculateAveragePt(h_spt_phi, h_n1_phi, h_pt_phi);

      if (reportDebug(fct)) cout << "Compute h_spt_eta" << endl;
      hTemp = h_spt_phiEta->ProjectionX();
      h_spt_eta->Add(hTemp);
      delete hTemp;

      if (reportDebug(fct)) cout << "Compute h_pt_eta" << endl;
      calculateAveragePt(h_spt_eta, h_n1_eta, h_pt_eta);
      }
    }

  if (h_n1_phiY)
    {
    if (!h_n1_phiEta)
      {
      if (reportDebug(fct)) cout << "Compute h_n1_phi" << endl;
      hTemp = h_n1_phiY->ProjectionY();
      h_n1_phi->Add(hTemp);
      delete hTemp;
      }

    if (reportDebug(fct)) cout << "Compute h_n1_y" << endl;
    hTemp = h_n1_phiY->ProjectionX();
    h_n1_y->Add(hTemp);
    delete hTemp;

    if (fillP2)
      {
      if (!h_n1_phiEta)
        {
        if (reportDebug(fct)) cout << "Compute h_spt_phi" << endl;
        hTemp = h_spt_phiY->ProjectionY();
        h_spt_phi->Add(hTemp);
        delete hTemp;

        if (reportDebug(fct)) cout << "Compute h_pt_phi" << endl;
        calculateAveragePt(h_spt_phi, h_n1_phi, h_pt_phi);
        }

      if (reportDebug(fct)) cout << "Compute h_spt_y" << endl;
      hTemp = h_spt_phiEta->ProjectionX();
      h_spt_y->Add(hTemp);
      delete hTemp;

      if (reportDebug(fct)) cout << "Compute h_pt_y" << endl;
      calculateAveragePt(h_spt_y, h_n1_y, h_pt_y);
      }
    }

  //restoreLogLevel();
}

