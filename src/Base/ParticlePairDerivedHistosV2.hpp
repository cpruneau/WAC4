//
// ParticlePairDerivedHistosV2.hpp
// MyMC
//
// created by Claude Pruneau on 9/23/16.
// Copyright © 2016 Claude Pruneau. All rights reserved.
//

#ifndef WAC_ParticlePairDerivedHistosV2
#define WAC_ParticlePairDerivedHistosV2
#include "Histograms.hpp"
#include "ParticleHistosV2.hpp"
#include "ParticlePairHistosV2.hpp"

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Two-Particle Correlation Functions
//////////////////////////////////////////////////////////////////////////////////////////////////////
class ParticlePairDerivedHistosV2 : public  Histograms
{
public:

  ParticlePairDerivedHistosV2(const TString & name,
                            ParticlePairAnalyzerConfiguration * configuration,
                            LogLevel  debugLevel);
  virtual ~ParticlePairDerivedHistosV2();
  virtual void loadHistograms(TFile * inputFile);
  virtual void createHistograms();
  //virtual double calculateWeightCorrection(TFile * fRaw, TFile * fCal);
  virtual void calculatePairDerivedHistograms(ParticleHistosV2 & part1Histos,
                                              ParticleHistosV2 & part2Histos,
                                              ParticlePairHistosV2 & pairHistos,
                                              double bincorrection);

  bool         fillEta;
  bool         fillY;
  bool         fillP2;
  unsigned int nBins_n1;
  float        min_n1;
  float        max_n1;
  unsigned int nBins_pt;
  float        min_pt;
  float        max_pt;
  unsigned int nBins_phi;
  float        min_phi;
  float        max_phi;
  unsigned int nBins_eta;
  float        min_eta;
  float        max_eta;
  unsigned int nBins_y;
  float        min_y;
  float        max_y;
  unsigned int nBins_Dphi;
  float        min_Dphi;
  float        max_Dphi;
  unsigned int nBins_Deta;
  float        min_Deta;
  float        max_Deta;
  unsigned int nBins_Dy;
  float        min_Dy;
  float        max_Dy;

  int          nBins_Dphi_shft;
  float        min_Dphi_shft;
  float        max_Dphi_shft;

  TH2* h_n1n1_ptpt;
  TH2* h_R2_ptpt;

  TH2* h_n1n1_phiPhi;
  TH2* h_R2_phiPhi;

  TH2* h_pt1pt1_phiPhi;
  TH2* h_DptDpt_phiPhi;
  TH2* h_P2_phiPhi;
  TH2* h_G2_phiPhi;

  TH2* h_n1n1_etaEta;
  TH2* h_R2_etaEta;

  TH2* h_pt1pt1_etaEta;
  TH2* h_DptDpt_etaEta;
  TH2* h_G2_etaEta;
  TH2* h_P2_etaEta;

  TH2* h_n1n1_DetaDphi;
  TH2* h_R2_DetaDphi;
  TH2* h_R2_DetaDphi_shft;

  TH2* h_pt1pt1_DetaDphi;
  TH2* h_DptDpt_DetaDphi;
  TH2* h_P2_DetaDphi;
  TH2* h_G2_DetaDphi;

  TH2* h_DptDpt_DetaDphi_shft;
  TH2* h_P2_DetaDphi_shft;
  TH2* h_G2_DetaDphi_shft;

  TH2* h_n1n1_yY;
  TH2* h_R2_yY;

  TH2* h_pt1pt1_yY;
  TH2* h_DptDpt_yY;
  TH2* h_G2_yY;
  TH2* h_P2_yY;

  TH2* h_n1n1_DyDphi;
  TH2* h_R2_DyDphi;
  TH2* h_R2_DyDphi_shft;

  TH2* h_pt1pt1_DyDphi;
  TH2* h_DptDpt_DyDphi;
  TH2* h_P2_DyDphi;
  TH2* h_G2_DyDphi;

  TH2* h_DptDpt_DyDphi_shft;
  TH2* h_P2_DyDphi_shft;
  TH2* h_G2_DyDphi_shft;

  ClassDef(ParticlePairDerivedHistosV2,1)

};

#endif /* WAC_ParticlePairDerivedHistos */
