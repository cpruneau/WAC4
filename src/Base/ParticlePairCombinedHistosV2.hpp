//
//  ParticlePairCombinedHistosV2.hpp
//  MyMC
//
//  Created by Claude Pruneau on 12/18/16.
//  Copyright © 2016 Claude Pruneau. All rights reserved.
//

#ifndef WAC_ParticlePairCombinedHistosV2
#define WAC_ParticlePairCombinedHistosV2
#include "ParticlePairDerivedHistosV2.hpp"

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Two-Particle Correlation Combinations (CI, CD)
//////////////////////////////////////////////////////////////////////////////////////////////////////
class ParticlePairCombinedHistosV2 : public Histograms
{
public:

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  // CTOR
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ParticlePairCombinedHistosV2(const TString & name,
                             ParticlePairAnalyzerConfiguration * configuration,
                             LogLevel  debugLevel);
  virtual ~ParticlePairCombinedHistosV2();
  virtual void createHistograms();
  virtual void loadHistograms(TFile * inputFile);
  virtual void calculate(ParticlePairDerivedHistosV2 * pp,
                         ParticlePairDerivedHistosV2 * pm,
                         ParticlePairDerivedHistosV2 * mp,
                         ParticlePairDerivedHistosV2 * mm,
                         double app, double apm, double amp, double amm);

  //void calculateRR_Q3D(ClusterHistos * ch);

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Data Members - Computed
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

  int   nBins_Dphi_shft;
  float min_Dphi_shft;
  float max_Dphi_shft;

  TH2* h_R2_etaEta;
  TH2* h_R2_phiPhi;
  TH2* h_R2_ptpt;
  TH2* h_R2_DetaDphi_shft;
  TH2* h_R2_yY;
  TH2* h_R2_DyDphi_shft;

  TH2* h_DptDpt_etaEta;
  TH2* h_DptDpt_phiPhi;
  TH2* h_DptDpt_DetaDphi_shft;
  TH2* h_DptDpt_yY;
  TH2* h_DptDpt_DyDphi_shft;

  TH2* h_P2_etaEta;
  TH2* h_P2_phiPhi;
  TH2* h_P2_DetaDphi_shft;
  TH2* h_P2_yY;
  TH2* h_P2_DyDphi_shft;

  TH2* h_G2_etaEta;
  TH2* h_G2_phiPhi;
  TH2* h_G2_DetaDphi_shft;
  TH2* h_G2_yY;
  TH2* h_G2_DyDphi_shft;

  TH2* h_BF_etaEta;
  TH2* h_BF_phiPhi;
  TH2* h_BF_DetaDphi_shft;
  TH2* h_BF_yY;
  TH2* h_BF_DyDphi_shft;

  
    ClassDef(ParticlePairCombinedHistosV2,1)
};



#endif /* WAC_ParticlePairCombinedHistosV2 */
