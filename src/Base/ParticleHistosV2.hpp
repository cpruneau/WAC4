// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
#ifndef WAC_ParticleHistosV2
#define WAC_ParticleHistosV2
#include "Histograms.hpp"
#include "Particle.hpp"
#include "ParticleDigit.hpp"
#include "ParticleAnalyzerConfiguration.hpp"
#include "TLorentzVector.h"


class ParticleHistosV2 : public Histograms
{
public:

  ParticleHistosV2(const TString & collectionName,
                 ParticleAnalyzerConfiguration * analysisConfiguration,
                  LogLevel  debugLevel);
  virtual ~ParticleHistosV2();
  virtual void createHistograms();
  virtual void loadHistograms(TFile * inputFile);
  virtual void fill(vector<ParticleDigit*> & particles, double weight);
  virtual void fill(Particle & particle, double weight);
  virtual void fillMultiplicity(double nAccepted, double totalEnergy, double weight);
  virtual void calculateDerivedHistograms();

  inline int getPtBinFor(float v) const
  {
  int index = 1+double(nBins_pt)*(v-min_pt)/range_pt;
  if (index>int(nBins_pt)) index = -1;
  return index;
  }

  inline int getPhiBinFor(float v) const
  {
  if (v<0.0) v += TMath::TwoPi();
  int index = 1+ double(nBins_phi)*(v-min_phi)/range_phi;
  if (index>int(nBins_phi)) index = -1;
  return index;
  }

  inline int getEtaBinFor(float v) const
  {
  int index = 1+ double(nBins_eta)*(v-min_eta)/range_eta;
  if (index>int(nBins_eta)) index = -1;
  return index;
  }

  inline int getYBinFor(float v) const
  {
  int index = 1+double(nBins_y)*(v-min_y)/range_y;
  if (index>int(nBins_y)) index = -1;
  return index;
  }

  ////////////////////////////////////////////////////////////////////////////
  // Data Members - Histograms
  ////////////////////////////////////////////////////////////////////////////
  bool fillEta;
  bool fillY;
  bool fillP2;
  unsigned int nBins_n1;
  float        min_n1;
  float        max_n1;
  unsigned int nBins_pt;
  float        min_pt;
  float        max_pt;
  float        range_pt;
  unsigned int nBins_phi;
  float        min_phi;
  float        max_phi;
  float        range_phi;
  unsigned int nBins_eta;
  float        min_eta;
  float        max_eta;
  float        range_eta;
  unsigned int nBins_y;
  float        min_y;
  float        max_y;
  float        range_y;

  //! Primary histograms

  TH1 * h_n1;
  TH1 * h_n1_eTotal;
  TH1 * h_n1_pt;
  TH1 * h_n1_ptXS;  // 1/pt dN/dptdy
  TH1 * h_n1_phi;
  TH1 * h_spt_phi;
  TH1 * h_n1_eta;
  TH2 * h_n1_phiEta;
  TH1 * h_spt_eta;
  TH2 * h_spt_phiEta;

  TH1 * h_n1_y;
  TH2 * h_n1_phiY;
  TH1 * h_spt_y;
  TH2 * h_spt_phiY;

  //! Derived  histograms
  TH1 * h_pt_phi;
  TH1 * h_pt_eta;
  TH2 * h_pt_phiEta;
  TH1 * h_pt_y;
  TH2 * h_pt_phiY;

    ClassDef(ParticleHistosV2,0)

};

#endif /* WAC_ParticleHistosV2  */



