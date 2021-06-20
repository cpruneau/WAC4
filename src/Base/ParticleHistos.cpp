//
//  ParticleHistos.cpp
//  MyMC
//
//  Created by Claude Pruneau on 9/23/16.
//  Copyright © 2016 Claude Pruneau. All rights reserved.
//

#include "TLorentzVector.h"
#include "ParticleHistos.hpp"
ClassImp(ParticleHistos);

ParticleHistos::ParticleHistos(const TString & name,
                               ParticleAnalyzerConfiguration * configuration,
                               LogLevel  debugLevel)
:
Histograms(name,configuration,debugLevel),
h_n1(nullptr),
h_n1_eTotal(nullptr),
h_n1_pt(nullptr),
h_n1_ptXS(nullptr),
h_n1_eta(nullptr),
h_n1_phi(nullptr),
h_n1_ptEta(nullptr),
h_n1_phiEta(nullptr),
h_n1_ptPhiEta(nullptr),
h_spt_phi(nullptr),
h_spt_eta(nullptr),
h_spt_phiEta(nullptr),
h_pt_phi(nullptr),
h_pt_eta(nullptr),
h_pt_phiEta(nullptr),

h_n1_y(nullptr),
h_n1_ptY(nullptr),
h_n1_phiY(nullptr),
h_n1_ptPhiY(nullptr),
h_spt_y(nullptr),
h_spt_phiY(nullptr),
h_pt_y(nullptr),
h_pt_phiY(nullptr)
{
  // no ops
}

ParticleHistos::~ParticleHistos()
{
  //deleteHistograms();
}

// for now use the same boundaries for eta and y histogram
void ParticleHistos::createHistograms()
{
  ParticleAnalyzerConfiguration & ac = *(ParticleAnalyzerConfiguration*)getConfiguration();
  TString bn = getHistoBaseName();
  h_n1         = createHistogram(bn+TString("n1"),            ac.nBins_n1,  ac.min_n1,  ac.max_n1,  "n_1","N", true,true,true,false);
  h_n1_eTotal  = createHistogram(bn+TString("n1_eTotal"),     ac.nBins_eTot,ac.min_eTot,ac.max_eTot,"n_1_eTotal","N", true,true,true,false);
  h_n1_pt      = createHistogram(bn+TString("n1_pt"),         ac.nBins_pt,  ac.min_pt,  ac.max_pt,  "p_{T}","N", true,true,true,false);
  h_n1_ptXS    = createHistogram(bn+TString("n1_ptXS"),       ac.nBins_pt,  ac.min_pt,  ac.max_pt,  "p_{T}","1/p_{T} dN/p_{T}", true,true,true,false);


  if (ac.fillEta)
    {
    h_n1_phi     = createHistogram(bn+TString("n1_phi"),        ac.nBins_phi, ac.min_phi, ac.max_phi, "#varphi","N", true,true,true,false);
    h_n1_eta     = createHistogram(bn+TString("n1_eta"),        ac.nBins_eta, ac.min_eta, ac.max_eta, "#eta","N", true,true,true,false);
    h_n1_ptEta   = createHistogram(bn+TString("n1_ptEta"),      ac.nBins_eta, ac.min_eta, ac.max_eta, ac.nBins_pt, ac.min_pt, ac.max_pt,"#eta","p_{T}","N", true,true,true,false);
    h_n1_phiEta  = createHistogram(bn+TString("n1_phiEta"),     ac.nBins_eta, ac.min_eta, ac.max_eta, ac.nBins_phi, ac.min_phi, ac.max_phi,"#eta","#varphi","N", true,true,true,false);

    h_spt_phi    = createHistogram(bn+TString("sumpt1_phi"),    ac.nBins_phi, ac.min_phi, ac.max_phi, "#varphi","#sum p_{T}", true,true,true,false);
    h_spt_eta    = createHistogram(bn+TString("sumpt1_eta"),    ac.nBins_eta, ac.min_eta, ac.max_eta, "#eta", "#sum p_{T}", true,true,true,false);
    h_spt_phiEta = createHistogram(bn+TString("sumpt1_phiEta"), ac.nBins_eta, ac.min_eta, ac.max_eta, ac.nBins_phi, ac.min_phi, ac.max_phi,"#eta","#varphi","#sum p_{T}", true,true,false,false);

    h_pt_phi     = createHistogram(bn+TString("pt1_phi"),       ac.nBins_phi, ac.min_phi, ac.max_phi, "#varphi","#LTp_{T}#GT", false,true,false,false);
    h_pt_eta     = createHistogram(bn+TString("pt1_eta"),       ac.nBins_eta, ac.min_eta, ac.max_eta, "#eta", "#LTp_{T}#GT", false,true,false,false);
    h_pt_phiEta  = createHistogram(bn+TString("pt1_phiEta"),    ac.nBins_eta, ac.min_eta, ac.max_eta, ac.nBins_phi, ac.min_phi, ac.max_phi,"#eta","#varphi","#LTp_{T}#GT", false,true,false,false);
    }
  if (ac.fillY)
    {
    h_n1_y     = createHistogram(bn+TString("n1_y"),        ac.nBins_y, ac.min_y, ac.max_y, "y","N", true,true,true,false);
    h_n1_ptY   = createHistogram(bn+TString("n1_ptY"),      ac.nBins_y, ac.min_y, ac.max_y, ac.nBins_pt, ac.min_pt, ac.max_pt,"#eta","p_{T}","N", true,true,false,false);
    h_n1_phiY  = createHistogram(bn+TString("n1_phiY"),     ac.nBins_y, ac.min_y, ac.max_y, ac.nBins_phi, ac.min_phi, ac.max_phi,"y","#varphi","N", true,true,false,false);

    h_spt_y    = createHistogram(bn+TString("sumpt1_y"),    ac.nBins_y, ac.min_y, ac.max_y, "y", "#sum p_{T}", true,true,false,false);
    h_spt_phiY = createHistogram(bn+TString("sumpt1_phiY"), ac.nBins_y, ac.min_y, ac.max_y, ac.nBins_phi, ac.min_phi, ac.max_phi,"y","#varphi","#sum p_{T}", true,true,false,false);

    h_pt_y     = createHistogram(bn+TString("pt1_y"),       ac.nBins_y, ac.min_y, ac.max_y, "y", "#LTp_{T}#GT", false,true,false,false);
    h_pt_phiY  = createHistogram(bn+TString("pt1_phiY"),    ac.nBins_y, ac.min_y, ac.max_y, ac.nBins_phi, ac.min_phi, ac.max_phi,"y","#varphi","#LTp_{T}#GT", false,true,false,false);
    }
}

//________________________________________________________________________
void ParticleHistos::loadHistograms(TFile * inputFile)
{
  if (!inputFile)
    {
    if (reportFatal()) cout << "Attempting to load ParticleHistos from an invalid file pointer" << endl;
    return;
    }
  ParticleAnalyzerConfiguration & ac = *(ParticleAnalyzerConfiguration*) getConfiguration();
  TString bn = getHistoBaseName();
  h_n1         = loadH1(inputFile,bn+TString("n1"));
  if (!h_n1)
    {
    if (reportError()) cout << "Could not load histogram: " << bn+TString("n1") << endl;
    return;
    }
  h_n1_eTotal  = loadH1(inputFile,bn+TString("n1_eTotal"));
  h_n1_pt      = loadH1(inputFile,bn+TString("n1_pt"));
  h_n1_ptXS    = loadH1(inputFile,bn+TString("n1_ptXS"));


  if (ac.fillEta)
    {
    h_n1_phi     = loadH1(inputFile,bn+TString("n1_phi"));
    h_n1_eta     = loadH1(inputFile,bn+TString("n1_eta"));
    h_n1_ptEta   = loadH2(inputFile,bn+TString("n1_ptEta"));
    h_n1_phiEta  = loadH2(inputFile,bn+TString("n1_phiEta"));

    h_spt_phi    = loadH1(inputFile,bn+TString("sumpt1_phi"));
    h_spt_phiEta = loadH2(inputFile,bn+TString("sumpt1_phiEta"));
    h_spt_eta    = loadH1(inputFile,bn+TString("sumpt1_eta"));

    h_pt_eta     = loadH1(inputFile,bn+TString("pt1_eta"));
    h_pt_phi     = loadH1(inputFile,bn+TString("pt1_phi"));
    h_pt_phiEta  = loadH2(inputFile,bn+TString("pt1_phiEta"));
    }
  if (ac.fillY)
    {
    h_n1_y     = loadH1(inputFile,bn+TString("n1_y"));
    h_n1_ptY   = loadH2(inputFile,bn+TString("n1_ptY"));
    h_n1_phiY  = loadH2(inputFile,bn+TString("n1_phiY"));
    h_spt_phiY = loadH2(inputFile,bn+TString("sumpt1_phiY"));
    h_spt_y    = loadH1(inputFile,bn+TString("sumpt1_y"));
    h_pt_phiY  = loadH2(inputFile,bn+TString("pt1_phiY"));
    h_pt_y     = loadH1(inputFile,bn+TString("pt1_y"));
    }
}


void ParticleHistos::fill(Particle & particle, double weight)
{
  fill(particle.getMomentum(),weight);
}


void ParticleHistos::fill(TLorentzVector & p, double weight)
{
  ParticleAnalyzerConfiguration & ac = *(ParticleAnalyzerConfiguration*)getConfiguration();

  double pt    = p.Pt();
  double phi   = p.Phi(); if (phi<0) phi += TMath::TwoPi();
  h_n1_pt      ->Fill(pt, weight);
  h_n1_ptXS    ->Fill(pt, weight/pt);
  if (ac.fillEta)
    {
    double eta   = p.Eta();
    h_n1_ptEta   ->Fill(eta, pt,  weight);
    h_n1_phiEta  ->Fill(eta, phi, weight);
    h_spt_phiEta ->Fill(eta, phi, pt*weight);
    }
  if (ac.fillY)
    {
    double y     = p.Rapidity();
    h_n1_ptY    ->Fill(y, pt,  weight);
    h_n1_phiY   ->Fill(y, phi, weight);
    h_spt_phiY  ->Fill(y, phi, pt*weight);
    }
}

void ParticleHistos::fillMultiplicity(double nAccepted, double totalEnergy, double weight)
{
  h_n1->Fill(nAccepted, weight);
  h_n1_eTotal->Fill(totalEnergy, weight);
}

// Complete filling the addicional histograms by projecting the
// higher dimensional ones
void ParticleHistos::completeFill()
{
  ParticleAnalyzerConfiguration & ac = *(ParticleAnalyzerConfiguration*)getConfiguration();


  if (ac.fillEta)
    {
    TH1* h0 = h_n1_phiEta->ProjectionY();
    h_n1_phi->Reset();
    h_n1_phi->Add(h0);
    delete h0;
    TH1 * h1 = h_n1_phiEta->ProjectionX();
    h_n1_eta->Reset();
    h_n1_eta->Add(h1);
    delete h1;
    TH1 *h2 = h_spt_phiEta->ProjectionY();
    h_spt_phi->Reset();
    h_spt_phi->Add(h2);
    delete h2;
    TH1 * h3 = h_spt_phiEta->ProjectionX();
    h_spt_eta->Reset();
    h_spt_eta->Add(h3);
    delete h3;
    }

  if (ac.fillY)
    {
    TH1* h0 = h_n1_phiY->ProjectionY();
    h_n1_phi->Reset();
    h_n1_phi->Add(h0);
    delete h0;
    TH1 * h1 = h_n1_phiY->ProjectionX();
    h_n1_y->Reset();
    h_n1_y->Add(h1);
    delete h1;
    TH1 *h2 = h_spt_phiY->ProjectionY();
    h_spt_phi->Reset();
    h_spt_phi->Add(h2);
    delete h2;
    TH1 * h3 = h_spt_phiY->ProjectionX();
    h_spt_y->Reset();
    h_spt_y->Add(h3);
    delete h3;
    }
}

void ParticleHistos::calculateAverages()
{
  ParticleAnalyzerConfiguration & ac = *(ParticleAnalyzerConfiguration*)getConfiguration();
  if (ac.fillEta)
    {
    if (h_spt_phi)    calculateAveragePt(  h_spt_phi,     h_n1_phi,    h_pt_phi);
    if (h_spt_eta)    calculateAveragePt(  h_spt_eta,     h_n1_eta,    h_pt_eta);
    if (h_spt_phiEta) calculateAveragePtH2(h_spt_phiEta,  h_n1_phiEta, h_pt_phiEta);
    }
  if (ac.fillY)
    {
    if (h_spt_y)      calculateAveragePt(  h_spt_y,       h_n1_y,      h_pt_y);
    if (h_spt_phiY)   calculateAveragePtH2(h_spt_phiY,    h_n1_phiY,   h_pt_phiY);
    }
}

