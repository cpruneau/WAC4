#ifndef WAC_AACollisionGenerator
#define WAC_AACollisionGenerator
#include "Task.hpp"
#include "EventFilter.hpp"
#include "ParticleFilter.hpp"
#include "CollisionGeometry.hpp"
#include "PythiaConfiguration.hpp"


/*!
  Task class designed to produce AA collisions as superpositions of nBinary nucleon-nucleon collisions. The does not generate the nucleon-nucleon collisions
but instead call another task class that does. This task assumes an event is already loaded with "interactions" (instance of the class Particle) that carry a type for the
interactions, a momentum, and a position. Nucleon-nucleon collision positions can for instance be generated with the CollisionGeometry class.
 */
class AACollisionGenerator : public Task
{
public:

  AACollisionGenerator();
  virtual ~AACollisionGenerator() {}
  virtual void initialize();
  virtual void finalize();
  virtual void execute();

  NucleonNucleonCollisionGenerator * getCollisionGenerator()
  {
  return nnCollisionGenerator;
  }

  void setNucleonNucleonCollisionGenerator(NucleonNucleonCollisionGenerator  * _nnCollisionGenerator);

  // ==============================================================================================================
  // Data Members
  // ==============================================================================================================  
protected:
  NucleonNucleonCollisionGenerator * nnCollisionGenerator;
  
  ClassDef(AACollisionGenerator,0)
};


#endif /*WAC_AACollisionGenerator*/
