// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
/**
 \class Task
 \ingroup WAC

 Class defining Task
 */

#include "ParticlePairHistosV2.hpp"

ClassImp(ParticlePairHistosV2);

ParticlePairHistosV2::ParticlePairHistosV2(const TString & name,
                                           ParticlePairAnalyzerConfiguration * configuration,
                                           LogLevel  debugLevel)
:
Histograms(name,configuration,debugLevel),
fillEta(configuration->fillEta),
fillY(configuration->fillY),
fillP2(configuration->fillP2),
nBins_n2(configuration->nBins_n2),
min_n2(configuration->min_n2),
max_n2(configuration->max_n2),
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
nBins_Dphi(configuration->nBins_Dphi),
min_Dphi(configuration->min_Dphi),
max_Dphi(configuration->max_Dphi),
nBins_Deta(configuration->nBins_Deta),
min_Deta(configuration->min_Deta),
max_Deta(configuration->max_Deta),
range_Deta(configuration->max_Deta-configuration->min_Deta),
nBins_Dy(configuration->nBins_Dy),
min_Dy(configuration->min_Dy),
max_Dy(configuration->max_Dy),
range_Dy(configuration->max_Dy-configuration->min_Dy),
h_n2(nullptr),
h_n2_ptpt(nullptr),
h_n2_etaEta(nullptr),
h_DptDpt_etaEta(nullptr),
h_n2_phiPhi(nullptr),
h_DptDpt_phiPhi(nullptr),
h_n2_yY(nullptr),
h_DptDpt_yY(nullptr),
h_n2_DetaDphi(nullptr),
h_DptDpt_DetaDphi(nullptr),
h_n2_DyDphi(nullptr),
h_DptDpt_DyDphi(nullptr)
{
  setClassName("ParticlePairHistosV2");
  setInstanceName(name);
}

ParticlePairHistosV2::~ParticlePairHistosV2()
{
  // no ops
}

void ParticlePairHistosV2::createHistograms()
{
  if (reportDebug()) cout << "ParticlePairHistosV2::initialize() Started." << endl;
  TString bn = getHistoBaseName();

  h_n2          = createHistogram(bn+TString("n2"),         nBins_n2,  min_n2, max_n2, "n_{2}", "Yield", 1,1,1,0);
  h_n2_ptpt     = createHistogram(bn+TString("n2_ptpt"),    nBins_pt,  min_pt, max_pt, nBins_pt, min_pt, max_pt,   "p_{T,1}",  "p_{T,2}", "N_{2}", 1,1,1,0);
  h_n2_phiPhi   = createHistogram(bn+TString("n2_phiPhi"),  nBins_phi, min_phi, max_phi, nBins_phi, min_phi, max_phi, "#varphi_{1}", "#varphi_{2}", "N_{2}", 1,1,1,0);

  if (fillP2)
    {
    h_DptDpt_phiPhi = createHistogram(bn+TString("ptpt_phiPhi"),   nBins_phi, min_phi, max_phi, nBins_phi, min_phi, max_phi, "#varphi_{1}", "#varphi_{2}", "p_{T}xp_{T}",1,1,0,0);
    }

  if (fillEta)
    {
    h_n2_etaEta   = createHistogram(bn+TString("n2_etaEta"),   nBins_eta,  min_eta, max_eta, nBins_eta, min_eta, max_eta, "#eta_{1}", "#eta_{2}", "N_{2}", 1,1,1,0);
    h_n2_DetaDphi = createHistogram(bn+TString("n2_DetaDphi"), nBins_Deta, min_Deta, max_Deta, nBins_Dphi, min_Dphi, max_Dphi, "#Delta#eta", "#Delta#phi", "N_{2}", 1,1,1,0);
    if (fillP2)
      {
      h_DptDpt_etaEta = createHistogram(bn+TString("ptpt_etaEta"), nBins_eta, min_eta, max_eta, nBins_eta, min_eta, max_eta, "#eta_{1}", "#eta_{2}", "p_{T}xp_{T}", 1,1,0,0);
      h_DptDpt_DetaDphi = createHistogram(bn+TString("ptpt_DetaDphi"),nBins_Deta, min_Deta, max_Deta, nBins_Dphi, min_Dphi, max_Dphi, "#Delta#eta", "#Delta#phi", "ptpt", 1,1,1,0);
      }
    }

  if (fillY)
    {
    h_n2_yY     = createHistogram(bn+TString("n2_yY"),     nBins_y,  min_y,  max_y,  nBins_y, min_y, max_y, "y_{1}","y_{2}", "N_{2}", 1,1,0,0);
    h_n2_DyDphi = createHistogram(bn+TString("n2_DyDphi"), nBins_Dy, min_Dy, max_Dy, nBins_Dphi, min_Dphi, max_Dphi, "#Delta#y", "#Delta#phi", "N_{2}", 1,1,1,0);
    if (fillP2)
      {
      h_DptDpt_yY    = createHistogram(bn+TString("ptpt_yY"),  nBins_y,  min_y, max_y, nBins_y, min_y, max_y, "y_{1}","y_{2}", "p_{T}xp_{T}", 1,1,0,0);
      h_DptDpt_DyDphi = createHistogram(bn+TString("ptpt_DyDphi"),nBins_Dy, min_Dy, max_Dy, nBins_Dphi, min_Dphi, max_Dphi, "#Delta#y", "#Delta#phi", "ptpt", 1,1,1,0);
      }
    }
  if (reportDebug()) cout << "ParticlePairHistosV2::initialize() Completed." << endl;
}

//________________________________________________________________________
void ParticlePairHistosV2::loadHistograms(TFile * inputFile)
{
  if (!inputFile)
    {
    cout << "-Fatal- Attempting to load ParticleHistosV2 from an invalid file pointer" << endl;
    return;
    }
}

void ParticlePairHistosV2::fill(vector<ParticleDigit*> & particle1, vector<ParticleDigit*> & particle2, bool same, double weight)
{
  double nPairs = 0;


  int iG;

  for (unsigned int iPart_1=0; iPart_1<particle1.size(); iPart_1++)
    {
    float pt1 = particle1[iPart_1]->pt;
    float dpt1 = pt1; // - avgPt1
    unsigned int iPt_1  = particle1[iPart_1]->iPt;
    unsigned int iPhi_1 = particle1[iPart_1]->iPhi;
    unsigned int iEta_1 = particle1[iPart_1]->iEta;
    unsigned int iY_1   = particle1[iPart_1]->iY;

    for (unsigned int iPart_2=(same?iPart_1+1: 0); iPart_2<particle2.size(); iPart_2++)
      {
      float pt2 = particle2[iPart_2]->pt;
      float dpt2 = pt2; // - avgPt2
      unsigned int iPt_2  = particle2[iPart_2]->iPt;
      unsigned int iPhi_2 = particle2[iPart_2]->iPhi;
      unsigned int iEta_2 = particle2[iPart_2]->iEta;
      unsigned int iY_2   = particle2[iPart_2]->iY;


      nPairs += 1.0;
      iG = h_n2_ptpt->GetBin(iPt_1,iPt_2);
      h_n2_ptpt->AddBinContent(iG,  weight);

      iG = h_n2_phiPhi->GetBin(iPhi_1,iPhi_2);
      h_n2_phiPhi->AddBinContent(iG,  weight);
      if (fillP2)
        {
        h_DptDpt_phiPhi ->AddBinContent(iG,weight*dpt1*dpt2);
        }
      if (same)
        {
        nPairs++;
        iG = h_n2_ptpt->GetBin(iPt_2,iPt_1);
        h_n2_ptpt->AddBinContent(iG,  weight);
        iG = h_n2_phiPhi->GetBin(iPhi_2,iPhi_1);
        h_n2_phiPhi->AddBinContent(iG,  weight);
        if (fillP2)
          {
          h_DptDpt_phiPhi->AddBinContent(iG,weight*pt1*pt2);
          }
        }

      if (fillEta)
        {
        // delta-eta maps onto a 2n-1 range i.e., 0 to 2n-2
        int iDeltaEta  = iEta_1-iEta_2 + nBins_eta-1;
        int iDeltaPhi  = iPhi_1-iPhi_2;
        if (iDeltaPhi < 0) iDeltaPhi += nBins_phi;

        iG = h_n2_etaEta->GetBin(iEta_1,iEta_2);
        h_n2_etaEta->AddBinContent(iG,weight);
        if (fillP2)
          {
          h_DptDpt_etaEta->AddBinContent(iG,weight*pt1*pt2);
          }

        iG = h_n2_DetaDphi->GetBin(iDeltaEta+1,iDeltaPhi+1);
        h_n2_DetaDphi->AddBinContent(iG,weight);
        if (fillP2)
          {
          h_DptDpt_DetaDphi->AddBinContent(iG,weight*pt1*pt2);
          }
        if (same)
          {
          // delta-eta maps onto a 2n-1 range i.e., 0 to 2n-2
          iDeltaEta  = iEta_2-iEta_1 + nBins_eta-1;
          iDeltaPhi  = iPhi_2-iPhi_1;
          if (iDeltaPhi < 0) iDeltaPhi += nBins_phi;
          iG = h_n2_etaEta->GetBin(iEta_2,iEta_1);
          h_n2_etaEta->AddBinContent(iG,weight);
          if (fillP2)
            {
            h_DptDpt_etaEta->AddBinContent(iG,weight*pt1*pt2);
            }

          iG = h_n2_DetaDphi->GetBin(iDeltaEta+1,iDeltaPhi+1);
          h_n2_DetaDphi->AddBinContent(iG,weight);
          if (fillP2)
            {
            h_DptDpt_DetaDphi->AddBinContent(iG,weight*pt1*pt2);
            }
          }
        }

      if (fillY)
        {
        int iDeltaY   = iY_1-iY_2 + nBins_y-1;
        int iDeltaPhi  = iPhi_1-iPhi_2;
        if (iDeltaPhi < 0) iDeltaPhi += nBins_phi;

        iG = h_n2_yY->GetBin(iY_1+1,iY_2+1);
        h_n2_yY->AddBinContent(iG,weight);
        if (fillP2)
          {
          h_DptDpt_yY->AddBinContent(iG,weight*pt1*pt2);
          }

        iG = h_n2_DyDphi->GetBin(iDeltaY+1,iDeltaPhi+1);
        h_n2_DyDphi  ->AddBinContent(iG,weight);
        if (fillP2)
          {
          h_DptDpt_DyDphi->AddBinContent(iG,weight*pt1*pt2);
          }
        if (same)
          {
          int iDeltaY   = iY_2-iY_1 + nBins_y-1;
          int iDeltaPhi  = iPhi_2-iPhi_1;
          if (iDeltaPhi < 0) iDeltaPhi += nBins_phi;

          iG = h_n2_yY->GetBin(iY_1+1,iY_2+1);
          h_n2_yY->AddBinContent(iG,weight);
          if (fillP2)
            {
            h_DptDpt_yY->AddBinContent(iG,weight*pt1*pt2);
            }

          iG = h_n2_DyDphi->GetBin(iDeltaY+1,iDeltaPhi+1);
          h_n2_DyDphi  ->AddBinContent(iG,weight);
          if (fillP2)
            {
            h_DptDpt_DyDphi->AddBinContent(iG,weight*pt1*pt2);
            }
          }
        }
      }
    }

  // Update number of entries
  h_n2_ptpt->SetEntries(h_n2_ptpt->GetEntries()+nPairs);
  h_n2_phiPhi->SetEntries(h_n2_phiPhi->GetEntries()+nPairs);
  if (fillP2)
    {
    h_DptDpt_phiPhi->SetEntries(h_DptDpt_phiPhi->GetEntries()+nPairs);
    }
  if (fillEta)
    {
    h_n2_etaEta->SetEntries(h_n2_etaEta->GetEntries()+nPairs);
    h_n2_DetaDphi->SetEntries(h_n2_DetaDphi->GetEntries()+nPairs);
    if (fillP2)
      {
      h_DptDpt_etaEta->SetEntries(h_DptDpt_etaEta->GetEntries()+nPairs);
      h_DptDpt_DetaDphi->SetEntries(h_DptDpt_DetaDphi->GetEntries()+nPairs);
      }
    }

  if (fillY)
    {
    h_n2_yY->SetEntries(h_n2_yY->GetEntries()+nPairs);
    h_n2_DyDphi->SetEntries(h_n2_DyDphi->GetEntries()+nPairs);
    if (fillP2)
      {
      h_DptDpt_yY->SetEntries(h_DptDpt_yY->GetEntries()+nPairs);
      h_DptDpt_DyDphi->SetEntries(h_DptDpt_DyDphi->GetEntries()+nPairs);
      }
    }
  h_n2->Fill(double(nPairs),weight);
}

