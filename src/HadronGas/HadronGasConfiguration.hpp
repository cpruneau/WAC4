#ifndef WAC_HadronGasConfiguration
#define WAC_HadronGasConfiguration
#include "TaskConfiguration.hpp"

using namespace std;

class HadronGasConfiguration : public TaskConfiguration
{
public:

  HadronGasConfiguration();
  HadronGasConfiguration(const HadronGasConfiguration & config);
  virtual ~HadronGasConfiguration();
  HadronGasConfiguration & operator=(const HadronGasConfiguration & config);
  void printConfiguration(ostream & os);

  TString pdgFileName;
  bool plotSystemProperties;
  int nT;    double minT,    maxT;
  int nMass; double minMass, maxMass;
  int nMu;   double minMu,   maxMu;
  int nMuB;  double minMuB,  maxMuB;
  int nMuS;  double minMuS,  maxMuS;
  int nMuQ;  double minMuQ,  maxMuQ;
  int nP;    double minP,    maxP;
  
  bool plotParticleProperties;
  int nTypes;
  int nStableTypes;

  ClassDef(HadronGasConfiguration,0)
};


#endif  // WAC_HadronGasConfiguration

