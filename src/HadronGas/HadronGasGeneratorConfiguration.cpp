#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <cmath>
#include <iomanip>

#include "HadronGasGeneratorConfiguration.hpp"

ClassImp(HadronGasGeneratorConfiguration);

using namespace std;

HadronGasGeneratorConfiguration::HadronGasGeneratorConfiguration()
:
NucleonNucleonCollisionGeneratorConfiguration(),
standaloneMode(true),
nTkin(1),   minTkin(170.0), maxTkin(170.0),
nT(50),     minT(100.0),    maxT(200.0),
nMass(50),  minMass(0.0),   maxMass(3.0),
nMu(20),    minMu(0.0),     maxMu(40),
nMuB(20),   minMuB(0.0),    maxMuB(20),
nMuS(20),   minMuS(0.0),    maxMuS(20),
nMuQ(20),   minMuQ(0.0),    maxMuQ(20),
nP(200),    minP(0.0),      maxP(5.0),
generatorType(MomentumGenerator::CylindricalGaussPtUniformY),
volume(1.0),
totalMult(100),
momentumGeneratorParameters()
{
  // no ops
}

HadronGasGeneratorConfiguration::HadronGasGeneratorConfiguration(const HadronGasGeneratorConfiguration & source)
:
NucleonNucleonCollisionGeneratorConfiguration(source),
nT(source.nT),       minT(source.minT),      maxT(source.maxT),
nMu(source.nMu),     minMu(source.minMu),    maxMu(source.maxMu),
nMuB(source.nMuB),   minMuB(source.minMuB),  maxMuB(source.maxMuB),
nMuS(source.nMuS),   minMuS(source.minMuS),  maxMuS(source.maxMuS),
nMuQ(source.nMuQ),   minMuQ(source.minMuQ),  maxMuQ(source.maxMuQ),
nP(source.nP),       minP(source.minP),      maxP(source.maxP),
generatorType         (source.generatorType),
volume                (source.volume),
totalMult             (source.totalMult),
momentumGeneratorParameters(source.momentumGeneratorParameters)
{
  // no ops
}

HadronGasGeneratorConfiguration & HadronGasGeneratorConfiguration::operator=(const HadronGasGeneratorConfiguration & source)
{
  if (this!=&source)
    {
    NucleonNucleonCollisionGeneratorConfiguration::operator=(source);
    nT   = source.nT;     minT   = source.minT;   maxT   = source.maxT;
    nMu  = source.nMu;    minMu  = source.minMu;  maxMu  = source.maxMu;
    nMuB = source.nMuB;   minMuB = source.minMuB; maxMuB = source.maxMuB;
    nMuS = source.nMuS;   minMuS = source.minMuS; maxMuS = source.maxMuS;
    nMuQ = source.nMuQ;   minMuQ = source.minMuQ; maxMuQ = source.maxMuQ;
    nP   = source.nP;     minP   = source.minP;   maxP   = source.maxP;
    generatorType = source.generatorType;
    volume        = source.volume;
    totalMult     = source.totalMult;
    momentumGeneratorParameters = source.momentumGeneratorParameters;
    }
  return *this;
}

void HadronGasGeneratorConfiguration::printConfiguration(ostream & os)
{
  NucleonNucleonCollisionGeneratorConfiguration::printConfiguration(os);
  //os << "           pdgFileName: " << pdgFileName  << endl;
  //os << "  plotSystemProperties: " << plotSystemProperties;
  os << "                    nT: " << nT    << "    minT: " << minT   << "    maxT: " <<maxT  << endl;
  os << "                 nMass: " << nMass << " minMass: " << minMass<< " maxMass: " <<maxMass  << endl;
  os << "                   nMu: " << nMu   << "   minMu: " << minMu  << "   maxMu: " <<maxMu  << endl;
  os << "                  nMuB: " << nMuB  << "  minMuB: " << minMuB << "  maxMuB: " <<maxMuB  << endl;
  os << "                  nMuB: " << nMuB  << "  minMuS: " << minMuS << "  maxMuS: " <<maxMuS  << endl;
  os << "                  nMuQ: " << nMuQ  << "  minMuQ: " << minMuQ << "  maxMuQ: " <<maxMuQ  << endl;
  os << "                    nP: " << nP    << "    minP: " << minP   << "    maxP: " <<maxP  << endl;
  //os << "plotParticleProperties: " << plotParticleProperties  << endl;
  //os << "                nTypes: " << nTypes  << endl;
  //os << "          nStableTypes: " << nStableTypes  << endl;
  os << "         generatorType: " << generatorType << endl;
  os << "                Volume: " << volume << endl;
  os << "             totalMult: " << totalMult << endl;
  for (unsigned int k=0; k<momentumGeneratorParameters.size();k++)
    {
    os << "                     k: " << k << "    value: " << momentumGeneratorParameters[k] << endl;
    }
}
