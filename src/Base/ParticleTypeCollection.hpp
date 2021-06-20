#ifndef WAC_ParticleTypeCollection
#define WAC_ParticleTypeCollection
#include <iostream>
#include "TString.h"
#include <string>
#include <ostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include "Collection.hpp"
#include "ParticleType.hpp"

using namespace std;
class ParticleType;

class ParticleTypeCollection : public Collection<ParticleType>
{
public:

  ParticleTypeCollection();
  virtual ~ParticleTypeCollection();
  void readFromFile(const TString & inputFileName);
  void sortByMass();
  ParticleTypeCollection * extractCollection(int option);
  int findIndexForPdgCode(int pdgCode);
  int findIndexForPrivateCode(int privateCode);
  void resolveTypes();
  ParticleType * findPdgCode(int pdgCode);
  ParticleType * findPrivateCode(int privateCode);
  
  ParticleType * getParticleType(int index)
  {
    return objects[index];
  }

  ParticleType * operator[](int index)
  {
    return objects[index];
  }

  vector<int> getListOfPdgCodes();

  ostream & printProperties(ostream & os);
  ostream & printDecayProperties(ostream & os);

  static ParticleTypeCollection * masterParticleCollection;
  static ParticleTypeCollection * getMasterParticleCollection();


  ClassDef(ParticleTypeCollection,0)
};


#endif  // WAC_ParticleTypeCollection

