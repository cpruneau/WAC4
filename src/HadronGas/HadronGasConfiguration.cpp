// Copyright 2016 Chun Shen

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <cmath>
#include <iomanip>

#include "HadronGasConfiguration.hpp"

ClassImp(HadronGasConfiguration);

using namespace std;

HadronGasConfiguration::HadronGasConfiguration()
:
TaskConfiguration(),
pdgFileName(),
plotSystemProperties(true),
nT(50),     minT(100.0),  maxT(200.0),
nMass(50),  minMass(0.0), maxMass(3.0),
nMu(20),    minMu(0.0),   maxMu(40),
nMuB(20),   minMuB(0.0),  maxMuB(20),
nMuS(20),   minMuS(0.0),  maxMuS(20),
nMuQ(20),   minMuQ(0.0),  maxMuQ(20),
nP(200),    minP(0.0),    maxP(5.0),
plotParticleProperties(false),
nTypes(319),
nStableTypes(23)
{
  // no ops
}

HadronGasConfiguration::HadronGasConfiguration(const HadronGasConfiguration & source)
:
plotSystemProperties(source.plotSystemProperties),
nT(source.nT),       minT(source.minT),      maxT(source.maxT),
nMu(source.nMu),     minMu(source.minMu),    maxMu(source.maxMu),
nMuB(source.nMuB),   minMuB(source.minMuB),  maxMuB(source.maxMuB),
nMuS(source.nMuS),   minMuS(source.minMuS),  maxMuS(source.maxMuS),
nMuQ(source.nMuQ),   minMuQ(source.minMuQ),  maxMuQ(source.maxMuQ),
nP(source.nP),       minP(source.minP),      maxP(source.maxP),
plotParticleProperties(source.plotParticleProperties),
nTypes(source.nTypes),
nStableTypes(source.nStableTypes)
{
  // no ops
}

HadronGasConfiguration::~HadronGasConfiguration()
{
  // no ops
}

HadronGasConfiguration & HadronGasConfiguration::operator=(const HadronGasConfiguration & source)
{
  if (this!=&source)
    {
    plotSystemProperties = source.plotSystemProperties;
    nT   = source.nT;     minT   = source.minT;   maxT   = source.maxT;
    nMu  = source.nMu;    minMu  = source.minMu;  maxMu  = source.maxMu;
    nMuB = source.nMuB;   minMuB = source.minMuB; maxMuB = source.maxMuB;
    nMuS = source.nMuS;   minMuS = source.minMuS; maxMuS = source.maxMuS;
    nMuQ = source.nMuQ;   minMuQ = source.minMuQ; maxMuQ = source.maxMuQ;
    nP   = source.nP;     minP   = source.minP;   maxP   = source.maxP;
    plotParticleProperties = source.plotParticleProperties;
    nTypes       = source.nTypes;
    nStableTypes = source.nStableTypes;
    }
  return *this;
}

void HadronGasConfiguration::printConfiguration(ostream & os)
{
  os << "           pdgFileName: " << pdgFileName  << endl;
  os << "  plotSystemProperties: " << plotSystemProperties;
  os << "                    nT: " << nT    << "    minT: " << minT   << "    maxT: " <<maxT  << endl;
  os << "                 nMass: " << nMass << " minMass: " << minMass<< " maxMass: " <<maxMass  << endl;
  os << "                   nMu: " << nMu   << "   minMu: " << minMu  << "   maxMu: " <<maxMu  << endl;
  os << "                  nMuB: " << nMuB  << "  minMuB: " << minMuB << "  maxMuB: " <<maxMuB  << endl;
  os << "                  nMuB: " << nMuB  << "  minMuS: " << minMuS << "  maxMuS: " <<maxMuS  << endl;
  os << "                  nMuQ: " << nMuQ  << "  minMuQ: " << minMuQ << "  maxMuQ: " <<maxMuQ  << endl;
  os << "                    nP: " << nP    << "    minP: " << minP   << "    maxP: " <<maxP  << endl;
  os << "plotParticleProperties: " << plotParticleProperties  << endl;
  os << "                nTypes: " << nTypes  << endl;
  os << "          nStableTypes: " << nStableTypes  << endl;
}


