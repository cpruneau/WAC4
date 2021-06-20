//
//  GlobalHistos.cpp
//  MyMC
//
//  Created by Claude Pruneau on 9/23/16.
//  Copyright © 2016 Claude Pruneau. All rights reserved.
//
#include "GlobalHistos.hpp"
ClassImp(GlobalHistos);

GlobalHistos::GlobalHistos(const TString &               _name,
                           GlobalAnalyzerConfiguration * _configuration,
                           vector<ParticleFilter*>       _particleFilters,
                           LogLevel                      _debugLevel)
:
Histograms(_name,_configuration,_debugLevel),
particleFilters(_particleFilters),
h_n(),
h_e(),
h_q(),
h_s(),
h_b(),
h_nVsN(),
h_eVsN(),
h_qVsN(),
h_bVsN(),
h_eVsE(),
h_qVsE(),
h_bVsE(),
h_qVsQ(),
h_bVsQ(),
h_bVsB()
{
}

TString GlobalHistos::makeName(const TString & bn,const  TString & filterName1,const  TString & observableName1)
{
  TString name = bn;
  name += "_";
  name += filterName1;
  name += "_";
  name += observableName1;
  return name;
}


TString GlobalHistos::makeName(const TString & bn,const  TString & filterName1,const  TString & observableName1,const  TString & filterName2,const  TString & observableName2)
{
  TString name = bn;
  name += "_";
  name += filterName1;
  name += "_";
  name += observableName1;
  name += "_";
  name += filterName2;
  name += "_";
  name += observableName2;
  return name;
}

// for now use the same boundaries for eta and y histogram
void GlobalHistos::createHistograms()
{
  GlobalAnalyzerConfiguration & ac = *(GlobalAnalyzerConfiguration*)getConfiguration();
  TString bn = getName();
  TString name;
  unsigned int nParticleFilters = particleFilters.size();
  for (unsigned int k1=0; k1<nParticleFilters; k1++)
  {
  TString pfName1 = particleFilters[k1]->getName();
  h_n.push_back( createHistogram(makeName(bn,pfName1,"n"),  ac.nBins_n, ac.min_n,  ac.max_n, "n","N", true,true,true,false) );
  h_e.push_back( createHistogram(makeName(bn,pfName1,"e"),  ac.nBins_e, ac.min_e,  ac.max_e, "e","N", true,true,true,false) );
  h_q.push_back( createHistogram(makeName(bn,pfName1,"q"),  ac.nBins_q, ac.min_q,  ac.max_q, "q","N", true,true,true,false) );
  h_s.push_back( createHistogram(makeName(bn,pfName1,"s"),  ac.nBins_b, ac.min_b,  ac.max_b, "s","N", true,true,true,false) );
  h_b.push_back( createHistogram(makeName(bn,pfName1,"b"),  ac.nBins_b, ac.min_b,  ac.max_b, "b","N", true,true,true,false) );
  if (ac.fillCorrelationHistos)
    {
    for (unsigned int k2=k1; k2<nParticleFilters; k2++)
      {
      TString pfName2 = particleFilters[k2]->getName();
      if (k1!=k2)
        {
        h_nVsN.push_back( createHistogram(makeName(bn,pfName1,"n",pfName2,"n"), ac.nBins_n2, ac.min_n, ac.max_n, ac.nBins_n2, ac.min_n,  ac.max_n, "n", "n", "N", true,true,true,false) );
        h_eVsE.push_back( createHistogram(makeName(bn,pfName1,"e",pfName2,"e"), ac.nBins_e2, ac.min_e, ac.max_e, ac.nBins_e2, ac.min_e,  ac.max_e, "e", "e", "N", true,true,true,false) );
        h_qVsQ.push_back( createHistogram(makeName(bn,pfName1,"q",pfName2,"q"), ac.nBins_q2, ac.min_q, ac.max_q, ac.nBins_q2, ac.min_q,  ac.max_q, "q", "q", "N", true,true,true,false) );
        h_bVsB.push_back( createHistogram(makeName(bn,pfName1,"b",pfName2,"b"), ac.nBins_b2, ac.min_b, ac.max_b, ac.nBins_b2, ac.min_b,  ac.max_b, "b", "b", "N", true,true,true,false) );
        }
      h_eVsN.push_back( createHistogram(makeName(bn,pfName1,"n",pfName2,"e"), ac.nBins_n2, ac.min_n, ac.max_n, ac.nBins_e2, ac.min_e,  ac.max_e, "n", "e", "N", true,true,true,false) );
      h_qVsN.push_back( createHistogram(makeName(bn,pfName1,"n",pfName2,"q"), ac.nBins_n2, ac.min_n, ac.max_n, ac.nBins_q2, ac.min_q,  ac.max_q, "n", "q", "N", true,true,true,false) );
      h_bVsN.push_back( createHistogram(makeName(bn,pfName1,"n",pfName2,"b"), ac.nBins_n2, ac.min_n, ac.max_n, ac.nBins_b2, ac.min_b,  ac.max_b, "n", "b", "N", true,true,true,false) );
      h_qVsE.push_back( createHistogram(makeName(bn,pfName1,"e",pfName2,"q"), ac.nBins_e2, ac.min_e, ac.max_e, ac.nBins_q2, ac.min_q,  ac.max_q, "e", "q", "N", true,true,true,false) );
      h_bVsE.push_back( createHistogram(makeName(bn,pfName1,"e",pfName2,"b"), ac.nBins_e2, ac.min_e, ac.max_e, ac.nBins_b2, ac.min_b,  ac.max_b, "e", "b", "N", true,true,true,false) );
      h_bVsQ.push_back( createHistogram(makeName(bn,pfName1,"q",pfName2,"b"), ac.nBins_q2, ac.min_q, ac.max_q, ac.nBins_b2, ac.min_b,  ac.max_b, "q", "b", "N", true,true,true,false) );
      }
    }
  }
}

void GlobalHistos::loadHistograms(TFile * inputFile)
{
  if (!inputFile)
    {
    if (reportFatal()) cout << "Attempting to load GlobalHistos from an invalid file pointer" << endl;
    return;
    }
  GlobalAnalyzerConfiguration & ac = *(GlobalAnalyzerConfiguration*)getConfiguration();
  TString bn = getName();
  TString name;
  unsigned int nParticleFilters = particleFilters.size();
  for (unsigned int k1=0; k1<nParticleFilters; k1++)
  {
  TString pfName1 = particleFilters[k1]->getName();
  h_n.push_back( loadH1(inputFile, makeName(bn,pfName1,"n")) );
  h_e.push_back( loadH1(inputFile, makeName(bn,pfName1,"e")) );
  h_q.push_back( loadH1(inputFile, makeName(bn,pfName1,"q")) );
  h_s.push_back( loadH1(inputFile, makeName(bn,pfName1,"s")) );
  h_b.push_back( loadH1(inputFile, makeName(bn,pfName1,"b")) );
  if (ac.fillCorrelationHistos)
    {
    for (unsigned int k2=k1; k2<nParticleFilters; k2++)
      {
      TString pfName2 = particleFilters[k2]->getName();
      if (k1!=k2)
        {
        h_nVsN.push_back( loadH2(inputFile, makeName(bn,pfName1,"n",pfName2,"n")) );
        h_eVsE.push_back( loadH2(inputFile, makeName(bn,pfName1,"e",pfName2,"e")) );
        h_qVsQ.push_back( loadH2(inputFile, makeName(bn,pfName1,"q",pfName2,"q")) );
        h_bVsB.push_back( loadH2(inputFile, makeName(bn,pfName1,"b",pfName2,"b")) );
        }
      h_eVsN.push_back( loadH2(inputFile, makeName(bn,pfName1,"n",pfName2,"e")) );
      h_qVsN.push_back( loadH2(inputFile, makeName(bn,pfName1,"n",pfName2,"q")) );
      h_bVsN.push_back( loadH2(inputFile, makeName(bn,pfName1,"n",pfName2,"b")) );
      h_qVsE.push_back( loadH2(inputFile, makeName(bn,pfName1,"e",pfName2,"q")) );
      h_bVsE.push_back( loadH2(inputFile, makeName(bn,pfName1,"e",pfName2,"b")) );
      h_bVsQ.push_back( loadH2(inputFile, makeName(bn,pfName1,"q",pfName2,"b")) );
      }
    }
  }
}

// =============================================================================
// n : number of particles accepted by filters
// e : total energy accepted by filters
// q : net charge accepted by filters
// b : net baryon number accepted by filters
// =============================================================================
void GlobalHistos::fill(vector<double> & n, vector<double> & e, vector<double> & q, vector<double> & s, vector<double> & b, double weight)
{
  GlobalAnalyzerConfiguration & ac = *(GlobalAnalyzerConfiguration*)getConfiguration();
  unsigned int nParticleFilters = particleFilters.size();
  unsigned int nAll  = 0; // all
  unsigned int nND   = 0; // non diagonal only

  for (unsigned int k1=0; k1<nParticleFilters; k1++)
  {
  h_n[k1]->Fill(n[k1],weight);
  h_e[k1]->Fill(e[k1],weight);
  h_q[k1]->Fill(q[k1],weight);
  h_s[k1]->Fill(s[k1],weight);
  h_b[k1]->Fill(b[k1],weight);
  if (ac.fillCorrelationHistos)
    {
    //unsigned int n2 = nParticleFilters-k1;
    for (unsigned int k2=k1; k2<nParticleFilters; k2++)
      {
      if (k1!=k2)
        {
        h_nVsN[nND]->Fill(n[k1],n[k2],weight);
        h_eVsE[nND]->Fill(e[k1],e[k2],weight);
        h_qVsQ[nND]->Fill(q[k1],q[k2],weight);
        h_bVsB[nND]->Fill(b[k1],b[k2],weight);
        nND++;
        }
      h_eVsN[nAll]->Fill(n[k1],e[k2],weight);
      h_qVsN[nAll]->Fill(n[k1],q[k2],weight);
      h_bVsN[nAll]->Fill(n[k1],b[k2],weight);
      h_qVsE[nAll]->Fill(e[k1],q[k2],weight);
      h_bVsE[nAll]->Fill(e[k1],b[k2],weight);
      h_bVsQ[nAll]->Fill(q[k1],b[k2],weight);
      nAll++;
      }
    }
  }
}
