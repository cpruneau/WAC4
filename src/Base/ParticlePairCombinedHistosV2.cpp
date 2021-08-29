//
//  ParticlePairCombinedHistosV2.cpp
//  MyMC
//
//  Created by Claude Pruneau on 12/18/16.
//  Copyright © 2016 Claude Pruneau. All rights reserved.
//
#include "ParticlePairCombinedHistosV2.hpp"
ClassImp(ParticlePairCombinedHistosV2)


ParticlePairCombinedHistosV2::ParticlePairCombinedHistosV2(const TString & name,
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
nBins_Dphi(configuration->max_Dphi),
min_Dphi(configuration->max_Dphi),
max_Dphi(configuration->max_Dphi),
nBins_Deta(configuration->nBins_Deta),
min_Deta(configuration->min_Deta),
max_Deta(configuration->max_Deta),
nBins_Dy(configuration->nBins_Dy),
min_Dy(configuration->min_Dy),
max_Dy(configuration->max_Dy)
{
  appendClassName("ParticlePairCombinedHistosV2");
  setInstanceName(name);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
// DTOR
//////////////////////////////////////////////////////////////////////////////////////////////////////
ParticlePairCombinedHistosV2::~ParticlePairCombinedHistosV2()
{
  if (reportDebug()) cout << "ParticlePairCombinedHistosV2::DTOR() No ops." << endl;
}

void ParticlePairCombinedHistosV2::createHistograms()
{
  if (reportDebug()) cout << "ParticlePairCombinedHistosV2::initialize() started" << endl;
  TString name;
  TString bn = getHistoBaseName();
  nBins_Dphi       = nBins_phi;
  float width_Dphi = TMath::TwoPi()/nBins_Dphi;
  nBins_Dphi_shft  = int(double(nBins_Dphi)/4.0);
  min_Dphi         = -width_Dphi/2.;
  max_Dphi         = TMath::TwoPi() - width_Dphi/2.;

  min_Dphi_shft    = min_Dphi - width_Dphi*double(nBins_Dphi/4);
  max_Dphi_shft    = max_Dphi - width_Dphi*double(nBins_Dphi/4);

  nBins_Deta       = 2*nBins_eta - 1;
  min_Deta         = min_eta - max_eta;
  max_Deta         = max_eta - min_eta;

  nBins_Dy         = 2*nBins_y - 1;
  min_Dy           = min_y - max_y;
  max_Dy           = max_y - min_y;

  h_R2_ptpt    = createHistogram(bn+TString("R2_ptpt"),              nBins_pt,    min_pt,    max_pt,    nBins_pt,    min_pt,        max_pt,        "p_{T,1}",     "p_{T,2}",       "R_{2}", true,true,true,true);

  if (fillEta)
    {
    h_R2_etaEta            = createHistogram(bn+TString("R2_etaEta"),            nBins_eta,   min_eta,   max_eta,   nBins_eta,   min_eta,       max_eta,       "#eta_{1}",    "#eta_{2}",      "R_{2}", true,true,true,true);
    h_R2_phiPhi            = createHistogram(bn+TString("R2_phiPhi"),            nBins_phi,   min_phi,   max_phi,   nBins_phi,   min_phi,       max_phi,       "#varphi_{1}", "#varphi_{2}",   "R_{2}", true,true,true,true);
    h_R2_DetaDphi_shft     = createHistogram(bn+TString("R2_DetaDphi_shft"),     nBins_Deta,  min_Deta,  max_Deta,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta#eta",  "#Delta#varphi", "R_{2}", true,true,true,true);

    if (fillP2)
      {
      h_DptDpt_etaEta        = createHistogram(bn+TString("DptDpt_etaEta"),        nBins_eta,   min_eta,   max_eta,   nBins_eta,   min_eta,       max_eta,       "#eta_{1}",    "#eta_{2}",      "<#Delta p_{T} #Delta p_{T}>", true,true,true,true);
      h_DptDpt_phiPhi        = createHistogram(bn+TString("DptDpt_phiPhi"),        nBins_phi,   min_phi,   max_phi,   nBins_phi,   min_phi,       max_phi,       "#varphi_{1}", "#varphi_{2}",   "<#Delta p_{T} #Delta p_{T}>", true,true,true,true);
      h_DptDpt_DetaDphi_shft = createHistogram(bn+TString("DptDpt_DetaDphi_shft"), nBins_Deta,  min_Deta,  max_Deta,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta#eta",  "#Delta#varphi", "<#Delta p_{T} #Delta p_{T}>", true,true,true,true);
      }

    if (fillP2)
      {
      h_P2_etaEta            = createHistogram(bn+TString("P2_etaEta"),            nBins_eta,   min_eta,   max_eta,   nBins_eta,   min_eta,       max_eta,       "#eta_{1}",    "#eta_{2}",      "P_{2}", true,true,true,true);
      h_P2_phiPhi            = createHistogram(bn+TString("P2_phiPhi"),            nBins_phi,   min_phi,   max_phi,   nBins_phi,   min_phi,       max_phi,       "#varphi_{1}", "#varphi_{2}",   "P_{2}", true,true,true,true);
      h_P2_DetaDphi_shft     = createHistogram(bn+TString("P2_DetaDphi_shft"),     nBins_Deta,  min_Deta,  max_Deta,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta#eta",  "#Delta#varphi", "P_{2}", true,true,true,true);
      }

    if (fillP2)
      {
      h_G2_etaEta            = createHistogram(bn+TString("G2_etaEta"),            nBins_eta,   min_eta,   max_eta,   nBins_eta,   min_eta,       max_eta,       "#eta_{1}",    "#eta_{2}",      "G_{2}", true,true,true,true);
      h_G2_phiPhi            = createHistogram(bn+TString("G2_phiPhi"),            nBins_phi,   min_phi,   max_phi,   nBins_phi,   min_phi,       max_phi,       "#varphi_{1}", "#varphi_{2}",   "G_{2}", true,true,true,true);
      h_G2_DetaDphi_shft     = createHistogram(bn+TString("G2_DetaDphi_shft"),     nBins_Deta,  min_Deta,  max_Deta,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta#eta",  "#Delta#varphi", "G_{2}", true,true,true,true);
      }

//    if (fillP2)
//      {
//      h_BF_etaEta            = createHistogram(bn+TString("BF_etaEta"),            nBins_eta,   min_eta,   max_eta,   nBins_eta,   min_eta,       max_eta,       "#eta_{1}",    "#eta_{2}",      "BF_{2}", true,true,true,true);
//      h_BF_phiPhi            = createHistogram(bn+TString("BF_phiPhi"),            nBins_phi,   min_phi,   max_phi,   nBins_phi,   min_phi,       max_phi,       "#varphi_{1}", "#varphi_{2}",   "BF_{2}", true,true,true,true);
//      h_BF_DetaDphi_shft     = createHistogram(bn+TString("BF_DetaDphi_shft"),     nBins_Deta,  min_Deta,  max_Deta,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta#eta",  "#Delta#varphi", "BF_{2}", true,true,true,true);
//      }
    }

  if (fillY)
    {
    h_R2_yY              = createHistogram(bn+TString("R2_yY"),                nBins_y,   min_y,   max_y,   nBins_y,     min_y,         max_y,         "y_{1}",      "y_{2}", "R_{2}", true,true,true,true);
    h_R2_DyDphi_shft     = createHistogram(bn+TString("R2_DyDphi_shft"),       nBins_Dy,  min_Dy,  max_Dy,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta y",   "#Delta#varphi", "R_{2}", true,true,true,true);

    if (fillP2)
      {
      h_DptDpt_yY          = createHistogram(bn+TString("DptDpt_yY"),            nBins_y,   min_y,   max_y,   nBins_y,     min_y,         max_y,         "y_{1}",      "y_{2}",         "<#Delta p_{T} #Delta p_{T}>", true,true,true,true);
      h_DptDpt_DyDphi_shft = createHistogram(bn+TString("DptDpt_DyDphi_shft"),   nBins_Dy,  min_Dy,  max_Dy,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta y",   "#Delta#varphi", "<#Delta p_{T} #Delta p_{T}>", true,true,true,true);
      }
    if (fillP2)
      {
      h_P2_yY              = createHistogram(bn+TString("P2_yY"),                nBins_y,   min_y,   max_y,   nBins_y,     min_y,         max_y,         "y_{1}",      "y_{2}",         "P_{2}", true,true,true,true);
      h_P2_DyDphi_shft     = createHistogram(bn+TString("P2_DyDphi_shft"),       nBins_Dy,  min_Dy,  max_Dy,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta y",   "#Delta#varphi", "P_{2}", true,true,true,true);
      }
    if (fillP2)
      {
      h_G2_yY              = createHistogram(bn+TString("G2_yY"),                nBins_y,   min_y,   max_y,   nBins_y,     min_y,         max_y,         "y_{1}",      "y_{2}",         "G_{2}", true,true,true,true);
      h_G2_DyDphi_shft     = createHistogram(bn+TString("G2_DyDphi_shft"),       nBins_Dy,  min_Dy,  max_Dy,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta y",   "#Delta#varphi", "G_{2}", true,true,true,true);
      }
//    if (fillP2)
//      {
//      h_BF_yY              = createHistogram(bn+TString("BF_yY"),                nBins_y,   min_y,   max_y,   nBins_y,     min_y,         max_y,         "y_{1}",      "y_{2}",         "BF_{2}", true,true,true,true);
//      h_BF_DyDphi_shft     = createHistogram(bn+TString("BF_DyDphi_shft"),       nBins_Dy,  min_Dy,  max_Dy,  nBins_Dphi,  min_Dphi_shft, max_Dphi_shft, "#Delta y",   "#Delta#varphi", "BF_{2}", true,true,true,true);
//      }
    }

  if (reportDebug()) cout << "ParticlePairCombinedHistosV2::initialize() completed" << endl;
}


void ParticlePairCombinedHistosV2::loadHistograms(TFile * inputFile)
{
  TString fct = "loadHistograms(TFile * inputFile)";
  if (reportStart(fct))
    ;
  if (!ptrFileExist(fct,inputFile))  return;
}

void ParticlePairCombinedHistosV2::calculate(ParticlePairDerivedHistosV2 * pp,
                                           ParticlePairDerivedHistosV2 * pm,
                                           ParticlePairDerivedHistosV2 * mp,
                                           ParticlePairDerivedHistosV2 * mm,
                                           double app, double apm, double amp, double amm)
{
  TString fct = "calculate(ParticlePairDerivedHistosV2 * pp, ...)";
  if (reportStart(fct))
    ;

  if (fillEta)
    {
    if (!sameDimensions(fct,pp->h_R2_etaEta, mm->h_R2_etaEta,pm->h_R2_etaEta, h_R2_etaEta)) return;
    h_R2_etaEta->Add(pp->h_R2_etaEta, mm->h_R2_etaEta, app, amm);
    h_R2_etaEta->Add(pm->h_R2_etaEta, apm);
    h_R2_etaEta->Add(mp->h_R2_etaEta, amp);

    if (!sameDimensions(fct,pp->h_R2_phiPhi, mm->h_R2_phiPhi,pm->h_R2_phiPhi, h_R2_phiPhi)) return;
    h_R2_phiPhi->Add(pp->h_R2_phiPhi, mm->h_R2_phiPhi, app, amm);
    h_R2_phiPhi->Add(pm->h_R2_phiPhi, apm);
    h_R2_phiPhi->Add(mp->h_R2_phiPhi, amp);

    h_R2_ptpt->Add(pp->h_R2_ptpt, mm->h_R2_ptpt, app, amm);
    h_R2_ptpt->Add(pm->h_R2_ptpt, apm);
    h_R2_ptpt->Add(mp->h_R2_ptpt, amp);

    if (!sameDimensions(fct,pp->h_R2_DetaDphi_shft, mm->h_R2_DetaDphi_shft,pm->h_R2_DetaDphi_shft, h_R2_DetaDphi_shft)) return;

    h_R2_DetaDphi_shft->Add(pp->h_R2_DetaDphi_shft, mm->h_R2_DetaDphi_shft, app, amm);
    h_R2_DetaDphi_shft->Add(pm->h_R2_DetaDphi_shft, apm);
    h_R2_DetaDphi_shft->Add(mp->h_R2_DetaDphi_shft, amp);

    if (fillP2)
      {
      if (!sameDimensions(fct,pp->h_DptDpt_etaEta,  mm->h_DptDpt_etaEta,pm->h_DptDpt_etaEta, h_DptDpt_etaEta)) return;
      h_DptDpt_etaEta->Add(pp->h_DptDpt_etaEta, mm->h_DptDpt_etaEta, app, amm);
      h_DptDpt_etaEta->Add(pm->h_DptDpt_etaEta, apm);
      h_DptDpt_etaEta->Add(mp->h_DptDpt_etaEta, amp);
      h_DptDpt_phiPhi->Add(pp->h_DptDpt_phiPhi, mm->h_DptDpt_phiPhi, app, amm);
      h_DptDpt_phiPhi->Add(pm->h_DptDpt_phiPhi, apm);
      h_DptDpt_phiPhi->Add(mp->h_DptDpt_phiPhi, amp);
      h_DptDpt_DetaDphi_shft->Add(pp->h_DptDpt_DetaDphi_shft, mm->h_DptDpt_DetaDphi_shft, app, amm);
      h_DptDpt_DetaDphi_shft->Add(pm->h_DptDpt_DetaDphi_shft, apm);
      h_DptDpt_DetaDphi_shft->Add(mp->h_DptDpt_DetaDphi_shft, amp);
      }

    if (fillP2)
      {
      h_P2_etaEta->Add(pp->h_P2_etaEta, mm->h_P2_etaEta, app, amm);
      h_P2_etaEta->Add(pm->h_P2_etaEta, apm);
      h_P2_etaEta->Add(mp->h_P2_etaEta, amp);
      h_P2_phiPhi->Add(pp->h_P2_phiPhi, mm->h_P2_phiPhi, app, amm);
      h_P2_phiPhi->Add(pm->h_P2_phiPhi, apm);
      h_P2_phiPhi->Add(mp->h_P2_phiPhi, amp);
      h_P2_DetaDphi_shft->Add(pp->h_P2_DetaDphi_shft, mm->h_P2_DetaDphi_shft, app, amm);
      h_P2_DetaDphi_shft->Add(pm->h_P2_DetaDphi_shft, apm);
      h_P2_DetaDphi_shft->Add(mp->h_P2_DetaDphi_shft, amp);
      }

    if (fillP2)
      {
      h_G2_etaEta->Add(pp->h_G2_etaEta, mm->h_G2_etaEta, app, amm);
      h_G2_etaEta->Add(pm->h_G2_etaEta, apm);
      h_G2_etaEta->Add(mp->h_G2_etaEta, amp);
      h_G2_phiPhi->Add(pp->h_G2_phiPhi, mm->h_G2_phiPhi, app, amm);
      h_G2_phiPhi->Add(pm->h_G2_phiPhi, apm);
      h_G2_phiPhi->Add(mp->h_G2_phiPhi, amp);
      h_G2_DetaDphi_shft->Add(pp->h_G2_DetaDphi_shft, mm->h_G2_DetaDphi_shft, app, amm);
      h_G2_DetaDphi_shft->Add(pm->h_G2_DetaDphi_shft, apm);
      h_G2_DetaDphi_shft->Add(mp->h_G2_DetaDphi_shft, amp);
      }

//    if (fillP2)
//      {
//      if (!sameDimensions(pp->h_bf12_DetaDphi_shft, mm->h_bf12_DetaDphi_shft,pm->h_bf12_DetaDphi_shft, pm->h_bf21_DetaDphi_shft, h_BF_DetaDphi_shft)) return;
//      h_BF_etaEta->Add(pp->h_bf12_etaEta, mm->h_bf12_etaEta, app, amm);
//      h_BF_etaEta->Add(pm->h_bf12_etaEta, apm);
//      h_BF_etaEta->Add(pm->h_bf21_etaEta, amp);
//      h_BF_phiPhi->Add(pp->h_bf12_phiPhi, mm->h_bf12_phiPhi, app, amm);
//      h_BF_phiPhi->Add(pm->h_bf12_phiPhi, apm);
//      h_BF_phiPhi->Add(pm->h_bf21_phiPhi, amp);
//      h_BF_DetaDphi_shft->Add(pp->h_bf12_DetaDphi_shft, mm->h_bf12_DetaDphi_shft, app, amm);
//      h_BF_DetaDphi_shft->Add(pm->h_bf12_DetaDphi_shft, apm);
//      h_BF_DetaDphi_shft->Add(pm->h_bf21_DetaDphi_shft, amp);
//      }
   }

  if (fillY)
    {
    h_R2_yY->Add(pp->h_R2_yY, mm->h_R2_yY, app, amm);
    h_R2_yY->Add(pm->h_R2_yY, apm);
    h_R2_yY->Add(mp->h_R2_yY, amp);
    h_R2_DyDphi_shft->Add(pp->h_R2_DyDphi_shft, mm->h_R2_DyDphi_shft, app, amm);
    h_R2_DyDphi_shft->Add(pm->h_R2_DyDphi_shft, apm);
    h_R2_DyDphi_shft->Add(mp->h_R2_DyDphi_shft, amp);

    if (fillP2)
      {
      h_DptDpt_yY->Add(pp->h_DptDpt_yY, mm->h_DptDpt_yY, app, amm);
      h_DptDpt_yY->Add(pm->h_DptDpt_yY, apm);
      h_DptDpt_yY->Add(mp->h_DptDpt_yY, amp);
      h_DptDpt_DyDphi_shft->Add(pp->h_DptDpt_DyDphi_shft, mm->h_DptDpt_DyDphi_shft, app, amm);
      h_DptDpt_DyDphi_shft->Add(pm->h_DptDpt_DyDphi_shft, apm);
      h_DptDpt_DyDphi_shft->Add(mp->h_DptDpt_DyDphi_shft, amp);
      }

    if (fillP2)
      {
      h_P2_yY->Add(pp->h_P2_yY, mm->h_P2_yY, app, amm);
      h_P2_yY->Add(pm->h_P2_yY, apm);
      h_P2_yY->Add(mp->h_P2_yY, amp);
      h_P2_DyDphi_shft->Add(pp->h_P2_DyDphi_shft, mm->h_P2_DyDphi_shft, app, amm);
      h_P2_DyDphi_shft->Add(pm->h_P2_DyDphi_shft, apm);
      h_P2_DyDphi_shft->Add(mp->h_P2_DyDphi_shft, amp);
      }

    if (fillP2)
      {
      h_G2_yY->Add(pp->h_G2_yY, mm->h_G2_yY, app, amm);
      h_G2_yY->Add(pm->h_G2_yY, apm);
      h_G2_yY->Add(mp->h_G2_yY, amp);
      h_G2_DyDphi_shft->Add(pp->h_G2_DyDphi_shft, mm->h_G2_DyDphi_shft, app, amm);
      h_G2_DyDphi_shft->Add(pm->h_G2_DyDphi_shft, apm);
      h_G2_DyDphi_shft->Add(mp->h_G2_DyDphi_shft, amp);
      }

//    if (fillP2)
//      {
//      h_BF_yY->Add(pp->h_bf12_yY, mm->h_bf12_yY, app, amm);
//      h_BF_yY->Add(pm->h_bf12_yY, apm);
//      h_BF_yY->Add(pm->h_bf21_yY, amp);
//      h_BF_DyDphi_shft->Add(pp->h_bf12_DyDphi_shft, mm->h_bf12_DyDphi_shft, app, amm);
//      h_BF_DyDphi_shft->Add(pm->h_bf12_DyDphi_shft, apm);
//      h_BF_DyDphi_shft->Add(pm->h_bf21_DyDphi_shft, amp);
//      }
    }
  if (reportEnd("ParticlePairCombinedHistosV2",getName(),"calculate(...)"))
    ;

}
