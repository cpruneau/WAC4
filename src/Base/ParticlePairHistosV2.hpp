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

 Class defining ParticlePairHistosV2
 */

#ifndef WAC_ParticlePairHistosV2
#define WAC_ParticlePairHistosV2

#include "Histograms.hpp"
#include "ParticlePairAnalyzerConfiguration.hpp"
#include "Particle.hpp"
#include "ParticleDigit.hpp"

class ParticlePairHistosV2 : public Histograms
{
public:

  ParticlePairHistosV2(const TString & name,
                     ParticlePairAnalyzerConfiguration * configuration,
                     LogLevel  debugLevel);
  virtual ~ParticlePairHistosV2();
  virtual void createHistograms();
  virtual void fill(vector<ParticleDigit*> & particle1, vector<ParticleDigit*> & particle2, bool same, double weight);

  virtual void loadHistograms(TFile * inputFile);

  inline int getPtBinFor(float v) const
  {
  int index = 1+double(nBins_pt)*(v-min_pt)/range_pt;
  if (index>int(nBins_pt)) index = -1;
  return index;
  }

  inline int getPhiBinFor(float v) const
  {
  if (v<0.0) v += TMath::TwoPi();
  int index = 1+double(nBins_phi)*(v-min_phi)/range_phi;
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
  int index = 1+ double(nBins_y)*(v-min_y)/range_y;
  if (index>int(nBins_y)) index = -1;
  return index;
  }

  ////////////////////////////////////////////////////////////////////////////
  // Data Members - Histograms
  ////////////////////////////////////////////////////////////////////////////

  bool fillEta;
  bool fillY;
  bool fillP2;
  unsigned int nBins_n2;
  float        min_n2;
  float        max_n2;
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
  unsigned int nBins_Dphi;
  float        min_Dphi;
  float        max_Dphi;
  unsigned int nBins_Deta;
  float        min_Deta;
  float        max_Deta;
  float        range_Deta;
  unsigned int nBins_Dy;
  float        min_Dy;
  float        max_Dy;
  float        range_Dy;

  TH1 * h_n2;

  TH2 * h_n2_ptpt;

  TH2 * h_n2_etaEta;
  TH2 * h_DptDpt_etaEta;

  TH2 * h_n2_phiPhi;
  TH2 * h_DptDpt_phiPhi;
  
  TH2 * h_n2_yY;
  TH2 * h_DptDpt_yY;

  TH2* h_n2_DetaDphi;
  TH2* h_DptDpt_DetaDphi;

  TH2* h_n2_DyDphi;
  TH2* h_DptDpt_DyDphi;

     ClassDef(ParticlePairHistosV2,0)
};


#endif /* WAC_ParticlePairHistosV2  */
