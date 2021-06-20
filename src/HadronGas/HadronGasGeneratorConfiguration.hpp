#ifndef WAC_HadronGasGeneratorConfiguration
#define WAC_HadronGasGeneratorConfiguration
#include "NucleonNucleonCollisionGeneratorConfiguration.hpp"
#include "MomentumGenerator.hpp"

using namespace std;

class HadronGasGeneratorConfiguration : public NucleonNucleonCollisionGeneratorConfiguration
{
public:

  HadronGasGeneratorConfiguration();
  HadronGasGeneratorConfiguration(const HadronGasGeneratorConfiguration & config);
  virtual ~HadronGasGeneratorConfiguration() {}
  HadronGasGeneratorConfiguration & operator=(const HadronGasGeneratorConfiguration & config);
  void printConfiguration(ostream & os);

  int nT;    double minT,    maxT;
  int nMass; double minMass, maxMass;
  int nMu;   double minMu,   maxMu;
  int nMuB;  double minMuB,  maxMuB;
  int nMuS;  double minMuS,  maxMuS;
  int nMuQ;  double minMuQ,  maxMuQ;
  int nP;    double minP,    maxP;

  MomentumGenerator::GeneratorType generatorType;
  double volume;
  double totalMult;
  vector<double> momentumGeneratorParameters;

  //TString pdgFileName;
  //bool plotSystemProperties;

  //bool plotParticleProperties;
  //int nTypes;

  ClassDef(HadronGasGeneratorConfiguration,0)
};


#endif  // WAC_HadronGasGeneratorConfiguration

