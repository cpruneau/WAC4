//
//  HadronGasVsTempHistograms.cpp
//  MyMC
//
//  Created by Claude Pruneau on 9/23/16.
//  Copyright © 2016 Claude Pruneau. All rights reserved.
//
#include "HadronGasVsTempHistograms.hpp"

ClassImp(HadronGasVsTempHistograms);

HadronGasVsTempHistograms::HadronGasVsTempHistograms(const TString & _name,
                                                     HadronGasConfiguration * _configuration,
                                                     LogLevel  _debugLevel)
:
Histograms(_name,_configuration,_debugLevel),
numberDensityVsT(nullptr),
energyDensityVsT(nullptr),
entropyDensityVsT(nullptr),
pressureVsT(nullptr),
nDensityVsT(),
eDensityVsT(),
sDensityVsT()
{
  // no ops
}

HadronGasVsTempHistograms::~HadronGasVsTempHistograms()
{

}

void HadronGasVsTempHistograms::createHistograms()
{
  HadronGasConfiguration & ac = *(HadronGasConfiguration*)getConfiguration();
  TString bn = getHistoBaseName();
  unsigned int nT = 1000*(ac.maxT-ac.minT);
  numberDensityVsT   = createHistogram(bn+TString("numberyDensityVsT"), nT, ac.minT, ac.maxT, "T (GeV)","n (fm^{-3})",     false,false,false,false);
  energyDensityVsT   = createHistogram(bn+TString("energyDensityVsT"),  nT, ac.minT, ac.maxT, "T (GeV)","e (GeV.fm^{-3})", false,false,false,false);
  entropyDensityVsT  = createHistogram(bn+TString("entropyDensityVsT"), nT, ac.minT, ac.maxT, "T (GeV)","s (fm^{-3})",     false,false,false,false);
  pressureVsT        = createHistogram(bn+TString("pressureVsT"),       nT, ac.minT, ac.maxT, "T (GeV)","p",               false,false,false,false);

  unsigned int nStableSpecies = ac.nStableTypes;
  nDensityVsT.clear();
  eDensityVsT.clear();
  sDensityVsT.clear();
  for (unsigned int iSpecies=0; iSpecies<nStableSpecies; iSpecies++)
  {
  TString bnSpecies = bn;
  bnSpecies += "_";
  bnSpecies += iSpecies;
  nDensityVsT.push_back(createHistogram(bnSpecies+TString("_nVsT"),  nT, ac.minT, ac.maxT, "T (GeV)","n (fm^{-3})",     false,false,false,false));
  eDensityVsT.push_back(createHistogram(bnSpecies+TString("_eVsT"),  nT, ac.minT, ac.maxT, "T (GeV)","e (GeV.fm^{-3})", false,false,false,false));
  sDensityVsT.push_back(createHistogram(bnSpecies+TString("_sVsT"),  nT, ac.minT, ac.maxT, "T (GeV)","s (fm^{-3})",     false,false,false,false));
  }
}


//________________________________________________________________________
void HadronGasVsTempHistograms::loadHistograms(TFile * inputFile)
{
  if (!inputFile)
    {
    if (reportFatal()) cout << "Attempting to load HadronGasVsTempHistograms from an invalid file pointer" << endl;
    return;
    }
  TString bn  = getHistoBaseName();
  numberDensityVsT   = loadH1(inputFile,bn+TString("numberyDensityVsT"));
  energyDensityVsT   = loadH1(inputFile,bn+TString("energyDensityVsT"));
  entropyDensityVsT  = loadH1(inputFile,bn+TString("entropyDensityVsT"));
  pressureVsT        = loadH1(inputFile,bn+TString("pressureVsT"));
  if (!numberDensityVsT)
    {
    if (reportError()) cout << "Could not load histogram: " << bn+TString("numberDensityVsT") << endl;
    return;
    }
}

void HadronGasVsTempHistograms::fill(HadronGas & hadronGas)
{
  //cout << " HadronGasVsTempHistograms::fill(HadronGas & hadronGas) --1--" << endl;
  double zero = 0;
  double temperature = hadronGas.getTemperature();
  int    iT  = energyDensityVsT->GetXaxis()->FindBin(temperature);
  ParticleTypeCollection & particleTypes       = hadronGas.getParticleTypes();
  ParticleTypeCollection & stableParticleTypes = hadronGas.getStableParticleTypes();
  vector<double> & particleDensities           = hadronGas.particleDensities;
  vector<double> & stableParticleDensities     = hadronGas.stableParticleDensities;
  int nSpecies       = particleTypes.size();
  int nStableSpecies = stableParticleTypes.size();
  numberDensityVsT   ->SetBinContent(iT, hadronGas.getNumberDensity() ); energyDensityVsT->SetBinError(iT,zero);
  energyDensityVsT   ->SetBinContent(iT, hadronGas.getEnergyDensity() ); energyDensityVsT->SetBinError(iT,zero);
  entropyDensityVsT  ->SetBinContent(iT, hadronGas.getEntropyDensity()); entropyDensityVsT->SetBinError(iT,zero);
  pressureVsT        ->SetBinContent(iT, hadronGas.getPressure()      ); pressureVsT->SetBinError(iT,zero);

  for (int iSpecies=0; iSpecies<nStableSpecies; iSpecies++)
  {

  //cout << "hadronGas.particleDensities[iSpecies]:" << hadronGas.particleDensities[iSpecies] << endl;
  //cout << "hadronGas.particleEnergies[iSpecies]:" << hadronGas.particleEnergies[iSpecies] << endl;
  //cout << "hadronGas.particleEntropies[iSpecies]:" << hadronGas.particleEntropies[iSpecies] << endl;

    nDensityVsT[iSpecies]->SetBinContent(iT, hadronGas.particleDensities[iSpecies] ); nDensityVsT[iSpecies]->SetBinError(iT,zero);
    eDensityVsT[iSpecies]->SetBinContent(iT, hadronGas.particleEnergies[iSpecies]  ); eDensityVsT[iSpecies]->SetBinError(iT,zero);
    sDensityVsT[iSpecies]->SetBinContent(iT, hadronGas.particleEntropies[iSpecies] ); sDensityVsT[iSpecies]->SetBinError(iT,zero);
  }
  //cout << " HadronGasVsTempHistograms::fill(HadronGas & hadronGas) --7--" << endl;

}

