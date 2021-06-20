// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
#ifndef WAC_GlobalHistos
#define WAC_GlobalHistos
#include "Histograms.hpp"
#include "ParticleFilter.hpp"
#include "GlobalAnalyzerConfiguration.hpp"

// ==================================================
//
// Histograms of "global" observables.
//
// Observables are computed in the acceptance defined by
// particle filters. The filters can be set to select narrow
// or broad ranges of kinematic variables as well as species
// of particles.
//
// n:  number of selected particles in an  event
// e:  total energy of the selected particles in an event
// q:  total (net) electric charge of the selected particles in an event
// b:  total (net) baryon charge of the selected particles in an event
//
// The fill method of this class must be called at most once per event
// otherwise weird multiple counting will happen...
//
// ==================================================
class GlobalHistos : public Histograms
{
public:

  GlobalHistos(const TString &               _name,
               GlobalAnalyzerConfiguration * _configuration,
               vector<ParticleFilter*>       _particleFilters,
               LogLevel                      _debugLevel);
  virtual ~GlobalHistos() {}
  void createHistograms();
  void loadHistograms(TFile * inputFile);
  void fill(vector<double> & n, vector<double> & e, vector<double> & q, vector<double> & s, vector<double> & b, double weight);

  TString makeName(const TString & bn,const  TString & filterName1,const  TString & observableName1);
  TString makeName(const TString & bn,const  TString & filterName1,const  TString & observableName1,const  TString & filterName2,const  TString & observableName2);

protected:

  vector<ParticleFilter*> particleFilters;
  vector<TH1*>  h_n;
  vector<TH1*>  h_e;
  vector<TH1*>  h_q;
  vector<TH1*>  h_s;
  vector<TH1*>  h_b;
  vector<TH2*>  h_nVsN;
  vector<TH2*>  h_eVsN;
  vector<TH2*>  h_qVsN;
  vector<TH2*>  h_bVsN;
  vector<TH2*>  h_eVsE;
  vector<TH2*>  h_qVsE;
  vector<TH2*>  h_bVsE;
  vector<TH2*>  h_qVsQ;
  vector<TH2*>  h_bVsQ;
  vector<TH2*>  h_bVsB;

  ClassDef(GlobalHistos,0)
};

#endif /* WAC_GlobalHistos  */



