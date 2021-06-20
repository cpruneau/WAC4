#ifndef WAC_ParticleDecayMode
#define WAC_ParticleDecayMode
#include <iostream>
#include <iomanip>
#include <vector>
#include "TString.h"
#include "ParticleType.hpp"
#include "ParticleTypeCollection.hpp"

using namespace std;
class ParticleType;
class ParticleTypeCollection;

class ParticleDecayMode
{
private:
  double branchingRatio;
  vector<int> childrenCodes;
  vector<ParticleType*> childrenTypes;

public:
  ParticleDecayMode();
  ParticleDecayMode(const ParticleDecayMode & source);
  virtual ~ParticleDecayMode();
  ParticleDecayMode & operator=(const ParticleDecayMode & source);

  void   setBranchingRatio(double _branchingRatio) { branchingRatio = _branchingRatio; }
  double getBranchingRatio() const                 { return branchingRatio;            }
  int    getNChildren() const                      { return childrenCodes.size();      }
  void   addChild(int pdgCode)                     { childrenCodes.push_back(pdgCode); }
  void   addChild(ParticleType* type)              { childrenTypes.push_back(type);    }
  vector<int> & getChildrenPdgCodes()              { return childrenCodes;             }
  vector<ParticleType*> & getChildrenTypes()       { return childrenTypes;             }
  ParticleType & getChildType(int index)
  {
  return * childrenTypes[index];
  }
  int getChildPdgCode(int index)
  {
  return childrenCodes[index];
  }
  void resolveTypes(ParticleTypeCollection * collection);
  ostream & printProperties(ostream & os);

  ClassDef(ParticleDecayMode,0)
};

#endif  // WAC_ParticleDecayMode

