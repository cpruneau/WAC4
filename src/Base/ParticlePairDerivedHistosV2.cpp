//
//  DerivedParticleHistos.cpp
//  MyMC
//
//  Created by Claude Pruneau on 9/23/16.
//  Copyright © 2016 Claude Pruneau. All rights reserved.
//

#include "ParticlePairDerivedHistosV2.hpp"
ClassImp(ParticlePairDerivedHistosV2);

ParticlePairDerivedHistosV2::ParticlePairDerivedHistosV2(const TString & name,
                                                         ParticlePairAnalyzerConfiguration * configuration,
                                                         LogLevel  debugLevel)
:
Histograms(name,configuration,debugLevel),
fillEta(configuration->fillEta),
fillY(configuration->fillY),
fillP2(configuration->fillP2),
nBins_pt(configuration->nBins_pt),
min_pt(configuration->min_pt),
max_pt(configuration->max_pt),
nBins_phi(configuration->nBins_phi),
min_phi(configuration->min_phi),
max_phi(configuration->max_phi),
nBins_eta(configuration->nBins_eta),
min_eta(configuration->min_eta),
max_eta(configuration->max_eta),
nBins_y(configuration->nBins_y),
min_y(configuration->min_y),
max_y(configuration->max_y),
nBins_Dphi(configuration->nBins_Dphi),
min_Dphi(configuration->min_Dphi),
max_Dphi(configuration->max_Dphi),
nBins_Deta(configuration->nBins_Deta),
min_Deta(configuration->min_Deta),
max_Deta(configuration->max_Deta),
nBins_Dy(configuration->nBins_Dy),
min_Dy(configuration->min_Dy),
max_Dy(configuration->max_Dy),
h_n1n1_ptpt(nullptr),
h_R2_ptpt(nullptr),

h_n1n1_phiPhi(nullptr),
h_R2_phiPhi(nullptr),

h_pt1pt1_phiPhi(nullptr),
h_DptDpt_phiPhi(nullptr),
h_P2_phiPhi(nullptr),
h_G2_phiPhi(nullptr),

h_n1n1_etaEta(nullptr),
h_R2_etaEta(nullptr),

h_pt1pt1_etaEta(nullptr),
h_DptDpt_etaEta(nullptr),
h_G2_etaEta(nullptr),
h_P2_etaEta(nullptr),

h_n1n1_DetaDphi(nullptr),
h_R2_DetaDphi(nullptr),
h_R2_DetaDphi_shft(nullptr),

h_pt1pt1_DetaDphi(nullptr),
h_DptDpt_DetaDphi(nullptr),
h_P2_DetaDphi(nullptr),
h_G2_DetaDphi(nullptr),

h_DptDpt_DetaDphi_shft(nullptr),
h_P2_DetaDphi_shft(nullptr),
h_G2_DetaDphi_shft(nullptr),

h_n1n1_yY(nullptr),
h_R2_yY(nullptr),

h_pt1pt1_yY(nullptr),
h_DptDpt_yY(nullptr),
h_G2_yY(nullptr),
h_P2_yY(nullptr),

h_n1n1_DyDphi(nullptr),
h_R2_DyDphi(nullptr),
h_R2_DyDphi_shft(nullptr),

h_pt1pt1_DyDphi(nullptr),
h_DptDpt_DyDphi(nullptr),
h_P2_DyDphi(nullptr),
h_G2_DyDphi(nullptr),

h_DptDpt_DyDphi_shft(nullptr),
h_P2_DyDphi_shft(nullptr),
h_G2_DyDphi_shft(nullptr)
{
  appendClassName("ParticlePairDerivedHistosV2");
  setInstanceName(name);
}


ParticlePairDerivedHistosV2::~ParticlePairDerivedHistosV2()
{
  // no ops
}


void ParticlePairDerivedHistosV2::createHistograms()
{
  if (reportInfo("ParticlePairDerivedHistosV2",getName(),"createHistograms()")) cout << endl << "Started." << endl;
  TString bn = getHistoBaseName();
  float width_Dphi   = TMath::TwoPi()/nBins_Dphi;
  nBins_Dphi_shft    = int(double(nBins_Dphi)/4.0);
  min_Dphi_shft      = min_Dphi - width_Dphi*nBins_Dphi_shft;
  max_Dphi_shft      = max_Dphi - width_Dphi*nBins_Dphi_shft;

  h_n1n1_phiPhi          = createHistogram(bn+TString("n1n1_phiPhi"),         nBins_phi,   min_phi,   max_phi,   nBins_phi,   min_phi,  max_phi,  "#varphi_{1}", "#varphi_{2}",  "<n_{1}><n_{2}>", 0,1,0,0);
  h_R2_phiPhi            = createHistogram(bn+TString("R2_phiPhi"),           nBins_phi,   min_phi,   max_phi,   nBins_phi,   min_phi,  max_phi,   "#varphi_{1}", "#varphi_{2}", "R_{2}", 0,1,0,0);

  h_n1n1_ptpt            = createHistogram(bn+TString("n1n1_ptpt"),           nBins_pt,    min_pt,    max_pt,    nBins_pt,    min_pt,   max_pt,   "p_{T,1}", "p_{T,2}", "<n_{1}><n_{2}>", 0,1,0,0);
  h_R2_ptpt              = createHistogram(bn+TString("R2_ptpt"),             nBins_pt,    min_pt,    max_pt,    nBins_pt,    min_pt,   max_pt,   "p_{T,1}", "p_{T,2}", "R_{2}", 0,1,0,0);

  if (fillP2)
    {
    h_pt1pt1_phiPhi      = createHistogram(bn+TString("pt1pt1_phiPhi"),       nBins_phi,   min_phi,   max_phi,   nBins_phi,   min_phi,  max_phi,  "#varphi_{1}","#varphi_{2}",   "pt1pt1", 0,1,0,0);
    h_DptDpt_phiPhi      = createHistogram(bn+TString("DptDpt_phiPhi"),       nBins_phi,   min_phi,   max_phi,   nBins_phi,   min_phi,  max_phi,  "#varphi_{1}", "#varphi_{2}",   "<#Delta p_{T} #Delta p_{T}>", 0,1,0,0);
    h_P2_phiPhi          = createHistogram(bn+TString("P2_phiPhi"),           nBins_phi,   min_phi,   max_phi,   nBins_phi,   min_phi,  max_phi,  "#varphi_{1}", "#varphi_{2}",   "P_{2}", 0,1,0,0);
    h_G2_phiPhi          = createHistogram(bn+TString("G2_phiPhi"),           nBins_phi,   min_phi,   max_phi,   nBins_phi,   min_phi,  max_phi,  "#varphi_{1}", "#varphi_{2}",   "G_{2}", 0,1,0,0);
    }

  if (fillEta)
    {
    h_n1n1_etaEta        = createHistogram(bn+TString("n1n1_etaEta"),         nBins_eta,   min_eta,   max_eta,   nBins_eta,   min_eta,  max_eta,  "#eta_{1}", "#eta_{2}", "<n_{1}><n_{2}>", 0,1,0,0);
    h_R2_etaEta          = createHistogram(bn+TString("R2_etaEta"),           nBins_eta,   min_eta,   max_eta,   nBins_eta,   min_eta,  max_eta,  "#eta_{1}", "#eta_{2}", "R_{2}", 0,1,0,0);

    h_n1n1_DetaDphi      = createHistogram(bn+TString("n1n1_DetaDphi"),       nBins_Deta,  min_Deta,  max_Deta,  nBins_Dphi,  min_Dphi,      max_Dphi,      "#Delta#eta","#Delta#varphi", "<n_{1}><n_{1}>", 0,1,0,0);
    h_R2_DetaDphi        = createHistogram(bn+TString("R2_DetaDphi"),         nBins_Deta,  min_Deta,  max_Deta,  nBins_Dphi,  min_Dphi,      max_Dphi,      "#Delta#eta","#Delta#varphi", "R_{2}", 0,1,0,0);
    h_R2_DetaDphi_shft   = createHistogram(bn+TString("R2_DetaDphi_shft"),    nBins_Deta,  min_Deta,  max_Deta,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta#eta","#Delta#varphi", "R_{2}", 0,1,0,0);

    if (fillP2)
      {
      h_pt1pt1_etaEta        = createHistogram(bn+TString("pt1pt1_etaEta"),       nBins_eta,   min_eta,   max_eta,   nBins_eta,   min_eta,  max_eta,            "#eta_{1}", "#eta_{2}",      "pt1pt1", 0,1,0,0);
      h_DptDpt_etaEta        = createHistogram(bn+TString("DptDpt_etaEta"),       nBins_eta,   min_eta,   max_eta,   nBins_eta,   min_eta,  max_eta,            "#eta_{1}", "#eta_{2}",      "<#Delta p_{T} #Delta p_{T}>", 0,1,0,0);
      h_P2_etaEta            = createHistogram(bn+TString("P2_etaEta"),           nBins_eta,   min_eta,   max_eta,   nBins_eta,   min_eta,  max_eta,            "#eta_{1}", "#eta_{2}",      "P_{2}", 0,1,0,0);
      h_G2_etaEta            = createHistogram(bn+TString("G2_etaEta"),           nBins_eta,   min_eta,   max_eta,   nBins_eta,   min_eta,  max_eta,            "#eta_{1}", "#eta_{2}",         "G_{2}", 0,1,0,0);

      h_pt1pt1_DetaDphi      = createHistogram(bn+TString("pt1pt1_DetaDphi"),     nBins_Deta,  min_Deta,  max_Deta,  nBins_Dphi,  min_Dphi,      max_Dphi,      "#Delta#eta", "#Delta#varphi", "pt1pt1", 0,1,0,0);
      h_DptDpt_DetaDphi      = createHistogram(bn+TString("DptDpt_DetaDphi"),     nBins_Deta,  min_Deta,  max_Deta,  nBins_Dphi,  min_Dphi,      max_Dphi,      "#Delta#eta",  "#Delta#varphi", "<#Delta p_{T}#Delta p_{T}>", 0,1,0,0);
      h_P2_DetaDphi          = createHistogram(bn+TString("P2_DetaDphi"),         nBins_Deta,  min_Deta,  max_Deta,  nBins_Dphi,  min_Dphi,      max_Dphi,      "#Delta#eta",  "#Delta#varphi", "P_{2}", 0,1,0,0);
      h_G2_DetaDphi          = createHistogram(bn+TString("G2_DetaDphi"),         nBins_Deta,  min_Deta,  max_Deta,  nBins_Dphi,  min_Dphi,      max_Dphi,      "#Delta#eta","#Delta#varphi",   "G_{2}", 0,1,0,0);
      h_DptDpt_DetaDphi_shft = createHistogram(bn+TString("DptDpt_DetaDphi_shft"),nBins_Deta,  min_Deta,  max_Deta,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta#eta",  "#Delta#varphi", "<#Delta p_{T} #Delta p_{T}>", 0,1,0,0);
      h_P2_DetaDphi_shft     = createHistogram(bn+TString("P2_DetaDphi_shft"),    nBins_Deta,  min_Deta,  max_Deta,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta#eta",  "#Delta#varphi", "P_{2}", 0,1,0,0);
      h_G2_DetaDphi_shft     = createHistogram(bn+TString("G2_DetaDphi_shft"),    nBins_Deta,  min_Deta,  max_Deta,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta#eta", "#Delta#varphi",  "G_{2}", 0,1,0,0);
      }
    }

  if (fillY)
    {
    h_n1n1_yY           = createHistogram(bn+TString("n1n1_yY"),           nBins_y,   min_y,   max_y,   nBins_y,   min_y,  max_y,                 "y_{1}",     "y_{2}", "<n_{1}><n_{1}>", 0,1,0,0);
    h_R2_yY             = createHistogram(bn+TString("R2_yY"),             nBins_y,   min_y,   max_y,   nBins_y,   min_y,  max_y,                  "y_{1}",    "y_{2}", "R_{2}", 0,1,0,0);

    h_n1n1_DyDphi       = createHistogram(bn+TString("n1n1_DyDphi"),       nBins_Dy,  min_Dy,  max_Dy,  nBins_Dphi,  min_Dphi,      max_Dphi,     "#Delta y",  "#Delta#varphi", "<n_{1}><n_{1}>", 0,1,0,0);
    h_R2_DyDphi         = createHistogram(bn+TString("R2_DyDphi"),         nBins_Dy,  min_Dy,  max_Dy,  nBins_Dphi,  min_Dphi,      max_Dphi,      "#Delta y", "#Delta#varphi", "R_{2}", 0,1,0,0);
    h_R2_DyDphi_shft    = createHistogram(bn+TString("R2_DyDphi_shft"),    nBins_Dy,  min_Dy,  max_Dy,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta y", "#Delta#varphi", "R_{2}", 0,1,0,0);

    if (fillP2)
      {
      h_pt1pt1_yY         = createHistogram(bn+TString("pt1pt1_yY"),         nBins_y,   min_y,   max_y,   nBins_y,   min_y,  max_y,                  "y_{1}",    "y_{2}", "pt1pt1", 0,1,0,0);
      h_DptDpt_yY         = createHistogram(bn+TString("DptDpt_yY"),         nBins_y,   min_y,   max_y,   nBins_y,   min_y,  max_y,                  "y_{1}",    "y_{2}", "<#Delta p_{T} #Delta p_{T}>", 0,1,0,0);
      h_P2_yY             = createHistogram(bn+TString("P2_yY"),             nBins_y,   min_y,   max_y,   nBins_y,     min_y,         max_y,         "y_{1}",    "y_{2}",         "P_{2}", 0,1,0,0);
      h_G2_yY             = createHistogram(bn+TString("G2_yY"),             nBins_y,   min_y,   max_y,   nBins_y,   min_y,  max_y,                  "y_{1}",    "y_{2}", "G_{2}", 0,1,0,0);

      h_pt1pt1_DyDphi     = createHistogram(bn+TString("pt1pt1_DyDphi"),     nBins_Dy,  min_Dy,  max_Dy,  nBins_Dphi,  min_Dphi,      max_Dphi,      "#Delta y", "#Delta#varphi", "pt1pt1", 0,1,0,0);
      h_DptDpt_DyDphi     = createHistogram(bn+TString("DptDpt_DyDphi"),     nBins_Dy,  min_Dy,  max_Dy,  nBins_Dphi,  min_Dphi,      max_Dphi,      "#Delta y", "#Delta#varphi", "<#Delta p_{T}#Delta p_{T}>", 0,1,0,0);
      h_P2_DyDphi         = createHistogram(bn+TString("P2_DyDphi"),         nBins_Dy,  min_Dy,  max_Dy,  nBins_Dphi,  min_Dphi,      max_Dphi,      "#Delta y", "#Delta#varphi", "P_{2}", 0,1,0,0);
      h_G2_DyDphi         = createHistogram(bn+TString("G2_DyDphi"),         nBins_Dy,  min_Dy,  max_Dy,  nBins_Dphi,  min_Dphi,      max_Dphi,      "#Delta y", "#Delta#varphi", "G_{2}", 0,1,0,0);

      h_DptDpt_DyDphi_shft= createHistogram(bn+TString("DptDpt_DyDphi_shft"),nBins_Dy,  min_Dy,  max_Dy,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta y", "#Delta#varphi", "<#Delta p_{T}#Delta p_{T}>", 0,1,0,0);
      h_P2_DyDphi_shft    = createHistogram(bn+TString("P2_DyDphi_shft"),    nBins_Dy,  min_Dy,  max_Dy,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta y", "#Delta#varphi", "P_{2}", 0,1,0,0);
      h_G2_DyDphi_shft    = createHistogram(bn+TString("G2_DyDphi_shft"),    nBins_Dy,  min_Dy,  max_Dy,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta y", "#Delta#varphi", "G_{2}", 0,1,0,0);
      }
    }

  if (reportInfo("ParticlePairDerivedHistosV2",getName(),"createHistograms()")) cout << endl << "Completed." << endl;
}

void ParticlePairDerivedHistosV2::loadHistograms(TFile * inputFile)
{
  TString fct = "loadHistograms(TFile * inputFile)";
  if (reportDebug(fct))
    ;
  if (ptrFileExist(fct,inputFile)) return;
}

// Histograms from ParticlePairDerivedHistosV2 must be normalized "per event" before calling this function
void ParticlePairDerivedHistosV2::calculatePairDerivedHistograms(ParticleHistosV2     & part1Histos,
                                                                 ParticleHistosV2     & part2Histos,
                                                                 ParticlePairHistosV2 & pairHistos,
                                                                 double bincorrection)
{
  TString fct = "calculatePairDerivedHistograms(...)";
  if (reportStart(fct))
    ;
  MessageLogger::LogLevel store = getReportLevel();
  //setReportLevel(MessageLogger::Debug);


  double avgPt1Phi;
  double avgPt2Phi;
  double scalingFactorPhi;
  double avgPt1Eta;
  double avgPt2Eta;
  double scalingFactorEta;
  int ijNormalization = 0;

  if (reportDebug(fct))  cout << " Calculate R2 vs ptpt" << endl;
  calculateN1N1_H1H1H2( part1Histos.h_n1_pt,   part2Histos.h_n1_pt,  h_n1n1_ptpt,1.0, 1.0);
  calculateR2_H2H2H2(   pairHistos.h_n2_ptpt,  h_n1n1_ptpt,          h_R2_ptpt,0, 1.0, 1.0);

  if (reportDebug(fct))  cout << " Calculate R2 vs phiPhi" << endl;
  calculateN1N1_H1H1H2( part1Histos.h_n1_phi,  part2Histos.h_n1_phi, h_n1n1_phiPhi,1.0, 1.0);
  calculateR2_H2H2H2(   pairHistos.h_n2_phiPhi,h_n1n1_phiPhi,        h_R2_phiPhi,0, 1.0, 1.0);


  if (fillP2)
    {
    if (reportDebug(fct))  cout << " Calculate P2 vs phiPhi" << endl;
    avgPt1Phi = avgValue(part1Histos.h_pt_phi);
    avgPt2Phi = avgValue(part2Histos.h_pt_phi);
    scalingFactorPhi = 1.0/avgPt1Phi/avgPt2Phi;
    h_P2_phiPhi->Add(h_DptDpt_phiPhi,h_DptDpt_phiPhi,1.0,0.0);
    h_P2_phiPhi->Scale(scalingFactorPhi);
    }


  if (fillEta)
    {
    if (reportDebug(fct))  cout <<  " calculateN1N1_H1H1H2(part1Histos.h_n1_eta,part2Histos.h_n1_eta,h_n1n1_etaEta,1.0, 1.0)" << endl;

    calculateN1N1_H1H1H2(part1Histos.h_n1_eta,part2Histos.h_n1_eta,h_n1n1_etaEta,1.0, 1.0);
    calculateR2_H2H2H2(pairHistos.h_n2_etaEta,h_n1n1_etaEta,h_R2_etaEta,0,1.0,1.0);
    reduce_n1EtaPhiN1EtaPhiOntoN1N1DetaDphi(part1Histos.h_n1_phiEta,part2Histos.h_n1_phiEta,h_n1n1_DetaDphi,nBins_Deta,nBins_Dphi);
    if (reportDebug(fct))  cout <<  " calculateR2_H2H2H2(pairHistos.h_n2_DetaDphi,h_n1n1_DetaDphi,h_R2_DetaDphi,0,1.0,1.0)" << endl;
    calculateR2_H2H2H2(pairHistos.h_n2_DetaDphi,h_n1n1_DetaDphi,h_R2_DetaDphi,0,1.0,1.0);
    if (reportDebug(fct))  cout <<  " shiftY(*h_R2_DetaDphi,*h_R2_DetaDphi_shft,nBins_Dphi_shft)" << endl;
    shiftY(*h_R2_DetaDphi,*h_R2_DetaDphi_shft,nBins_Dphi_shft);

    if (fillP2)
      {
      avgPt1Eta = avgValue(part1Histos.h_pt_eta);
      avgPt2Eta = avgValue(part2Histos.h_pt_eta);
      scalingFactorEta = 1.0/avgPt1Eta/avgPt2Eta;
      h_P2_etaEta->Add(h_DptDpt_etaEta,h_DptDpt_etaEta,1.0,0.0);
      h_P2_etaEta->Scale(scalingFactorEta);

      if (reportDebug(fct))  cout <<  " shiftY(*h_R2_DetaDphi,*h_R2_DetaDphi_shft,nBins_Dphi_shft)" << endl;

      shiftY(*h_DptDpt_DetaDphi, *h_DptDpt_DetaDphi_shft,nBins_Dphi_shft);

      /// calculate average pt base on h_pt_eta histograms
      h_P2_DetaDphi->Add(h_DptDpt_DetaDphi,h_DptDpt_DetaDphi,1.0,0.0);
      h_P2_DetaDphi->Scale(scalingFactorEta);
      //symmetrizeDeltaEtaDeltaPhi(h_P2_DetaDphi,ijNormalization);
      shiftY(*h_P2_DetaDphi,*h_P2_DetaDphi_shft, nBins_Dphi_shft);

      calculateG2_H2H2H2H2( pairHistos.h_DptDpt_etaEta,   h_n1n1_etaEta,   h_pt1pt1_etaEta,   h_G2_etaEta,   ijNormalization, bincorrection, 1.0);
      calculateG2_H2H2H2H2( pairHistos.h_DptDpt_phiPhi,   h_n1n1_phiPhi,   h_pt1pt1_phiPhi,   h_G2_phiPhi,   ijNormalization, bincorrection, 1.0);
      calculateG2_H2H2H2H2( pairHistos.h_DptDpt_DetaDphi, h_n1n1_DetaDphi, h_pt1pt1_DetaDphi, h_G2_DetaDphi, ijNormalization, bincorrection, 1.0);
      shiftY(*h_G2_DetaDphi,*h_G2_DetaDphi_shft,nBins_Dphi_shft);
      }
    }

  if (reportEnd())
    ;
  setReportLevel(store);
}
