//
//  ParticlePairCombinedHistos.cpp
//  MyMC
//
//  Created by Claude Pruneau on 12/18/16.
//  Copyright © 2016 Claude Pruneau. All rights reserved.
//
#include "ParticlePairCombinedHistos.hpp"
ClassImp(ParticlePairCombinedHistos)


ParticlePairCombinedHistos::ParticlePairCombinedHistos(const TString & name,
                                                       ParticlePairAnalyzerConfiguration * configuration,
                                                       LogLevel  debugLevel)
:
Histograms(name,configuration,debugLevel)
{
  appendClassName("ParticlePairCombinedHistos");
  setInstanceName(name);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
// DTOR
//////////////////////////////////////////////////////////////////////////////////////////////////////
ParticlePairCombinedHistos::~ParticlePairCombinedHistos()
{
  if (reportDebug()) cout << "ParticlePairCombinedHistos::DTOR() No ops." << endl;
}

void ParticlePairCombinedHistos::createHistograms()
{
  if (reportDebug()) cout << "ParticlePairCombinedHistos::initialize() started" << endl;
  TString name;

  ParticlePairAnalyzerConfiguration & ac = *(ParticlePairAnalyzerConfiguration*) getConfiguration();
  TString bn = getHistoBaseName();
  ac.nBins_phiEta     = ac.nBins_eta*ac.nBins_phi;
  ac.nBins_Dphi_shft  = ac.nBins_phi/4;
  ac.nBins_Dphi       = ac.nBins_phi;
  ac.width_Dphi       = TMath::TwoPi()/ac.nBins_Dphi;
  ac.min_Dphi         = -ac.width_Dphi/2.;
  ac.max_Dphi         = TMath::TwoPi() - ac.width_Dphi/2.;
  ac.min_Dphi_shft    = ac.min_Dphi - ac.width_Dphi*double(ac.nBins_Dphi/4);
  ac.max_Dphi_shft    = ac.max_Dphi - ac.width_Dphi*double(ac.nBins_Dphi/4);
  ac.nBins_Deta       = 2*ac.nBins_eta - 1;
  ac.width_Deta       = (ac.max_eta - ac.min_eta)/ac.nBins_Deta;
  ac.min_Deta         = ac.min_eta - ac.max_eta;
  ac.max_Deta         = ac.max_eta - ac.min_eta;

  ac.nBins_Dy         = 2*ac.nBins_y - 1;
  ac.width_Dy         = (ac.max_y - ac.min_y)/ac.nBins_Dy;
  ac.min_Dy           = ac.min_y - ac.max_y;
  ac.max_Dy           = ac.max_y - ac.min_y;

  h_R2_ptPt    = createHistogram(bn+TString("R2_ptPt"),              ac.nBins_pt,    ac.min_pt,    ac.max_pt,    ac.nBins_pt,    ac.min_pt,        ac.max_pt,        "p_{T,1}",     "p_{T,2}",       "R_{2}", true,true,true,true);

  if (ac.fillEta)
    {
    h_R2_etaEta            = createHistogram(bn+TString("R2_etaEta"),            ac.nBins_eta,   ac.min_eta,   ac.max_eta,   ac.nBins_eta,   ac.min_eta,       ac.max_eta,       "#eta_{1}",    "#eta_{2}",      "R_{2}", true,true,true,true);
    h_R2_phiPhi            = createHistogram(bn+TString("R2_phiPhi"),            ac.nBins_phi,   ac.min_phi,   ac.max_phi,   ac.nBins_phi,   ac.min_phi,       ac.max_phi,       "#varphi_{1}", "#varphi_{2}",   "R_{2}", true,true,true,true);
    h_R2_DetaDphi_shft     = createHistogram(bn+TString("R2_DetaDphi_shft"),     ac.nBins_Deta,  ac.min_Deta,  ac.max_Deta,  ac.nBins_Dphi,  ac.min_Dphi_shft, ac.max_Dphi_shft, "#Delta#eta",  "#Delta#varphi", "R_{2}", true,true,true,true);

    if (ac.fillP2)
      {
      h_DptDpt_etaEta        = createHistogram(bn+TString("DptDpt_etaEta"),        ac.nBins_eta,   ac.min_eta,   ac.max_eta,   ac.nBins_eta,   ac.min_eta,       ac.max_eta,       "#eta_{1}",    "#eta_{2}",      "<#Delta p_{T} #Delta p_{T}>", true,true,true,true);
      h_DptDpt_phiPhi        = createHistogram(bn+TString("DptDpt_phiPhi"),        ac.nBins_phi,   ac.min_phi,   ac.max_phi,   ac.nBins_phi,   ac.min_phi,       ac.max_phi,       "#varphi_{1}", "#varphi_{2}",   "<#Delta p_{T} #Delta p_{T}>", true,true,true,true);
      h_DptDpt_DetaDphi_shft = createHistogram(bn+TString("DptDpt_DetaDphi_shft"), ac.nBins_Deta,  ac.min_Deta,  ac.max_Deta,  ac.nBins_Dphi,  ac.min_Dphi_shft, ac.max_Dphi_shft, "#Delta#eta",  "#Delta#varphi", "<#Delta p_{T} #Delta p_{T}>", true,true,true,true);
      }

    if (ac.fillP2)
      {
      h_P2_etaEta            = createHistogram(bn+TString("P2_etaEta"),            ac.nBins_eta,   ac.min_eta,   ac.max_eta,   ac.nBins_eta,   ac.min_eta,       ac.max_eta,       "#eta_{1}",    "#eta_{2}",      "P_{2}", true,true,true,true);
      h_P2_phiPhi            = createHistogram(bn+TString("P2_phiPhi"),            ac.nBins_phi,   ac.min_phi,   ac.max_phi,   ac.nBins_phi,   ac.min_phi,       ac.max_phi,       "#varphi_{1}", "#varphi_{2}",   "P_{2}", true,true,true,true);
      h_P2_DetaDphi_shft     = createHistogram(bn+TString("P2_DetaDphi_shft"),     ac.nBins_Deta,  ac.min_Deta,  ac.max_Deta,  ac.nBins_Dphi,  ac.min_Dphi_shft, ac.max_Dphi_shft, "#Delta#eta",  "#Delta#varphi", "P_{2}", true,true,true,true);
      }

    if (ac.fillP2)
      {
      h_G2_etaEta            = createHistogram(bn+TString("G2_etaEta"),            ac.nBins_eta,   ac.min_eta,   ac.max_eta,   ac.nBins_eta,   ac.min_eta,       ac.max_eta,       "#eta_{1}",    "#eta_{2}",      "G_{2}", true,true,true,true);
      h_G2_phiPhi            = createHistogram(bn+TString("G2_phiPhi"),            ac.nBins_phi,   ac.min_phi,   ac.max_phi,   ac.nBins_phi,   ac.min_phi,       ac.max_phi,       "#varphi_{1}", "#varphi_{2}",   "G_{2}", true,true,true,true);
      h_G2_DetaDphi_shft     = createHistogram(bn+TString("G2_DetaDphi_shft"),     ac.nBins_Deta,  ac.min_Deta,  ac.max_Deta,  ac.nBins_Dphi,  ac.min_Dphi_shft, ac.max_Dphi_shft, "#Delta#eta",  "#Delta#varphi", "G_{2}", true,true,true,true);
      }

    if (ac.fillEta)
      {
      h_BF_etaEta            = createHistogram(bn+TString("BF_etaEta"),            ac.nBins_eta,   ac.min_eta,   ac.max_eta,   ac.nBins_eta,   ac.min_eta,       ac.max_eta,       "#eta_{1}",    "#eta_{2}",      "BF_{2}", true,true,true,true);
      h_BF_phiPhi            = createHistogram(bn+TString("BF_phiPhi"),            ac.nBins_phi,   ac.min_phi,   ac.max_phi,   ac.nBins_phi,   ac.min_phi,       ac.max_phi,       "#varphi_{1}", "#varphi_{2}",   "BF_{2}", true,true,true,true);
      h_BF_DetaDphi_shft     = createHistogram(bn+TString("BF_DetaDphi_shft"),     ac.nBins_Deta,  ac.min_Deta,  ac.max_Deta,  ac.nBins_Dphi,  ac.min_Dphi_shft, ac.max_Dphi_shft, "#Delta#eta",  "#Delta#varphi", "BF_{2}", true,true,true,true);
      }
    }

  if (ac.fillY)
    {
    h_R2_yY              = createHistogram(bn+TString("R2_yY"),                ac.nBins_y,   ac.min_y,   ac.max_y,   ac.nBins_y,     ac.min_y,         ac.max_y,         "y_{1}",      "y_{2}", "R_{2}", true,true,true,true);
    h_R2_DyDphi_shft     = createHistogram(bn+TString("R2_DyDphi_shft"),       ac.nBins_Dy,  ac.min_Dy,  ac.max_Dy,  ac.nBins_Dphi,  ac.min_Dphi_shft, ac.max_Dphi_shft, "#Delta y",   "#Delta#varphi", "R_{2}", true,true,true,true);

    if (ac.fillP2)
      {
      h_DptDpt_yY          = createHistogram(bn+TString("DptDpt_yY"),            ac.nBins_y,   ac.min_y,   ac.max_y,   ac.nBins_y,     ac.min_y,         ac.max_y,         "y_{1}",      "y_{2}",         "<#Delta p_{T} #Delta p_{T}>", true,true,true,true);
      h_DptDpt_DyDphi_shft = createHistogram(bn+TString("DptDpt_DyDphi_shft"),   ac.nBins_Dy,  ac.min_Dy,  ac.max_Dy,  ac.nBins_Dphi,  ac.min_Dphi_shft, ac.max_Dphi_shft, "#Delta y",   "#Delta#varphi", "<#Delta p_{T} #Delta p_{T}>", true,true,true,true);
      }
    if (ac.fillP2)
      {
      h_P2_yY              = createHistogram(bn+TString("P2_yY"),                ac.nBins_y,   ac.min_y,   ac.max_y,   ac.nBins_y,     ac.min_y,         ac.max_y,         "y_{1}",      "y_{2}",         "P_{2}", true,true,true,true);
      h_P2_DyDphi_shft     = createHistogram(bn+TString("P2_DyDphi_shft"),       ac.nBins_Dy,  ac.min_Dy,  ac.max_Dy,  ac.nBins_Dphi,  ac.min_Dphi_shft, ac.max_Dphi_shft, "#Delta y",   "#Delta#varphi", "P_{2}", true,true,true,true);
      }
    if (ac.fillP2)
      {
      h_G2_yY              = createHistogram(bn+TString("G2_yY"),                ac.nBins_y,   ac.min_y,   ac.max_y,   ac.nBins_y,     ac.min_y,         ac.max_y,         "y_{1}",      "y_{2}",         "G_{2}", true,true,true,true);
      h_G2_DyDphi_shft     = createHistogram(bn+TString("G2_DyDphi_shft"),       ac.nBins_Dy,  ac.min_Dy,  ac.max_Dy,  ac.nBins_Dphi,  ac.min_Dphi_shft, ac.max_Dphi_shft, "#Delta y",   "#Delta#varphi", "G_{2}", true,true,true,true);
      }
    if (ac.fillP2)
      {
      h_BF_yY              = createHistogram(bn+TString("BF_yY"),                ac.nBins_y,   ac.min_y,   ac.max_y,   ac.nBins_y,     ac.min_y,         ac.max_y,         "y_{1}",      "y_{2}",         "BF_{2}", true,true,true,true);
      h_BF_DyDphi_shft     = createHistogram(bn+TString("BF_DyDphi_shft"),       ac.nBins_Dy,  ac.min_Dy,  ac.max_Dy,  ac.nBins_Dphi,  ac.min_Dphi_shft, ac.max_Dphi_shft, "#Delta y",   "#Delta#varphi", "BF_{2}", true,true,true,true);
      }
    }

  if (reportDebug()) cout << "ParticlePairCombinedHistos::initialize() completed" << endl;
}


void ParticlePairCombinedHistos::loadHistograms(TFile * inputFile)
{
  TString fct = "loadHistograms(TFile * inputFile)";
  if (reportStart(fct))
    ;
  if (ptrFileExist(fct,inputFile)) return;
}

void ParticlePairCombinedHistos::calculate(ParticlePairDerivedHistos * pp,
                                           ParticlePairDerivedHistos * pm,
                                           ParticlePairDerivedHistos * mp,
                                           ParticlePairDerivedHistos * mm,
                                           double app, double apm, double amp, double amm)
{
  TString fct = "calculate(...)";
  if (reportStart(fct))
    ;
  ParticlePairAnalyzerConfiguration  & ac = *(ParticlePairAnalyzerConfiguration*) getConfiguration();

  if (ac.fillEta)
    {
    if (!sameDimensions(fct,pp->h_R2_etaEta, mm->h_R2_etaEta,pm->h_R2_etaEta, h_R2_etaEta)) return;
    h_R2_etaEta->Add(pp->h_R2_etaEta, mm->h_R2_etaEta, app, amm);
    h_R2_etaEta->Add(pm->h_R2_etaEta, apm);
    h_R2_etaEta->Add(mp->h_R2_etaEta, amp);

    if (!sameDimensions(fct,pp->h_R2_phiPhi, mm->h_R2_phiPhi,pm->h_R2_phiPhi, h_R2_phiPhi)) return;
    h_R2_phiPhi->Add(pp->h_R2_phiPhi, mm->h_R2_phiPhi, app, amm);
    h_R2_phiPhi->Add(pm->h_R2_phiPhi, apm);
    h_R2_phiPhi->Add(mp->h_R2_phiPhi, amp);

    h_R2_ptPt->Add(pp->h_R2_ptPt, mm->h_R2_ptPt, app, amm);
    h_R2_ptPt->Add(pm->h_R2_ptPt, apm);
    h_R2_ptPt->Add(mp->h_R2_ptPt, amp);

    if (!sameDimensions(fct,pp->h_R2_DetaDphi_shft, mm->h_R2_DetaDphi_shft,pm->h_R2_DetaDphi_shft, h_R2_DetaDphi_shft)) return;
    h_R2_DetaDphi_shft->Add(pp->h_R2_DetaDphi_shft, mm->h_R2_DetaDphi_shft, app, amm);
    h_R2_DetaDphi_shft->Add(pm->h_R2_DetaDphi_shft, apm);
    h_R2_DetaDphi_shft->Add(mp->h_R2_DetaDphi_shft, amp);

    if (ac.fillP2)
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

    if (ac.fillP2)
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

    if (ac.fillP2)
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

    if (ac.fillP2)
      {
      if (!sameDimensions(fct,pp->h_bf12_DetaDphi_shft, mm->h_bf12_DetaDphi_shft,pm->h_bf12_DetaDphi_shft, pm->h_bf21_DetaDphi_shft, h_BF_DetaDphi_shft)) return;
      h_BF_etaEta->Add(pp->h_bf12_etaEta, mm->h_bf12_etaEta, app, amm);
      h_BF_etaEta->Add(pm->h_bf12_etaEta, apm);
      h_BF_etaEta->Add(pm->h_bf21_etaEta, amp);
      h_BF_phiPhi->Add(pp->h_bf12_phiPhi, mm->h_bf12_phiPhi, app, amm);
      h_BF_phiPhi->Add(pm->h_bf12_phiPhi, apm);
      h_BF_phiPhi->Add(pm->h_bf21_phiPhi, amp);
      h_BF_DetaDphi_shft->Add(pp->h_bf12_DetaDphi_shft, mm->h_bf12_DetaDphi_shft, app, amm);
      h_BF_DetaDphi_shft->Add(pm->h_bf12_DetaDphi_shft, apm);
      h_BF_DetaDphi_shft->Add(pm->h_bf21_DetaDphi_shft, amp);
      }
    }

  if (ac.fillY)
    {
    h_R2_yY->Add(pp->h_R2_yY, mm->h_R2_yY, app, amm);
    h_R2_yY->Add(pm->h_R2_yY, apm);
    h_R2_yY->Add(mp->h_R2_yY, amp);
    h_R2_DyDphi_shft->Add(pp->h_R2_DyDphi_shft, mm->h_R2_DyDphi_shft, app, amm);
    h_R2_DyDphi_shft->Add(pm->h_R2_DyDphi_shft, apm);
    h_R2_DyDphi_shft->Add(mp->h_R2_DyDphi_shft, amp);

    if (ac.fillP2)
      {
      h_DptDpt_yY->Add(pp->h_DptDpt_yY, mm->h_DptDpt_yY, app, amm);
      h_DptDpt_yY->Add(pm->h_DptDpt_yY, apm);
      h_DptDpt_yY->Add(mp->h_DptDpt_yY, amp);
      h_DptDpt_DyDphi_shft->Add(pp->h_DptDpt_DyDphi_shft, mm->h_DptDpt_DyDphi_shft, app, amm);
      h_DptDpt_DyDphi_shft->Add(pm->h_DptDpt_DyDphi_shft, apm);
      h_DptDpt_DyDphi_shft->Add(mp->h_DptDpt_DyDphi_shft, amp);
      }

    if (ac.fillP2)
      {
      h_P2_yY->Add(pp->h_P2_yY, mm->h_P2_yY, app, amm);
      h_P2_yY->Add(pm->h_P2_yY, apm);
      h_P2_yY->Add(mp->h_P2_yY, amp);
      h_P2_DyDphi_shft->Add(pp->h_P2_DyDphi_shft, mm->h_P2_DyDphi_shft, app, amm);
      h_P2_DyDphi_shft->Add(pm->h_P2_DyDphi_shft, apm);
      h_P2_DyDphi_shft->Add(mp->h_P2_DyDphi_shft, amp);
      }

    if (ac.fillP2)
      {
      h_G2_yY->Add(pp->h_G2_yY, mm->h_G2_yY, app, amm);
      h_G2_yY->Add(pm->h_G2_yY, apm);
      h_G2_yY->Add(mp->h_G2_yY, amp);
      h_G2_DyDphi_shft->Add(pp->h_G2_DyDphi_shft, mm->h_G2_DyDphi_shft, app, amm);
      h_G2_DyDphi_shft->Add(pm->h_G2_DyDphi_shft, apm);
      h_G2_DyDphi_shft->Add(mp->h_G2_DyDphi_shft, amp);
      }

    if (ac.fillP2)
      {
      h_BF_yY->Add(pp->h_bf12_yY, mm->h_bf12_yY, app, amm);
      h_BF_yY->Add(pm->h_bf12_yY, apm);
      h_BF_yY->Add(pm->h_bf21_yY, amp);
      h_BF_DyDphi_shft->Add(pp->h_bf12_DyDphi_shft, mm->h_bf12_DyDphi_shft, app, amm);
      h_BF_DyDphi_shft->Add(pm->h_bf12_DyDphi_shft, apm);
      h_BF_DyDphi_shft->Add(pm->h_bf21_DyDphi_shft, amp);
      }
    }
  if (reportEnd("ParticlePairCombinedHistos",getName(),"calculate(...)"))
    ;
}
