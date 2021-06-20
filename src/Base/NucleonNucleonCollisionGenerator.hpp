#ifndef WAC_NucleonNucleonCollisionGenerator
#define WAC_NucleonNucleonCollisionGenerator
#include "Task.hpp"
#include "ParticleFilter.hpp"
#include "CollisionGeometry.hpp"
#include "NucleonNucleonCollisionGeneratorConfiguration.hpp"

// ==============================================================================================================
// Abstract class for the generation of NN collisions
// ==============================================================================================================
class NucleonNucleonCollisionGenerator : public Task
{
public:
  NucleonNucleonCollisionGenerator();
  virtual ~NucleonNucleonCollisionGenerator(){}
  virtual void generate(Particle * parent);

protected:


  static NucleonNucleonCollisionGenerator * defaultNNCollisionGenerator;
public:
  static NucleonNucleonCollisionGenerator * getDefaultNNCollisionGenerator(); 

  ClassDef(NucleonNucleonCollisionGenerator,0)
};


#endif /*WAC_NucleonNucleonCollisionGenerator */
