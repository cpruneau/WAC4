// Author: Claude Pruneau   09/25/2019

/*************************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *************************************************************************/

#ifndef EventProperties_hpp
#define EventProperties_hpp
#include <vector>
#include "Particle.hpp"

class EventProperties
{
public:

  EventProperties();
  EventProperties(const EventProperties & source);
  EventProperties & operator=(const EventProperties & source);
  virtual ~EventProperties();
  virtual void clear();
  virtual void reset();
  virtual void printProperties(ostream & output);

  virtual void fill(vector<double> & nFiltered,  // number of particles accepted by filter #i
                    vector<double> & eFiltered,  // total energy of particles accepted by filter #i
                    vector<double> & qFiltered,  // total charge of particles accepted by filter #i
                    vector<double> & sFiltered,  // total strangeness of particles accepted by filter #i
                    vector<double> & bFiltered);

  // ================================================
  // Data Members
  // ================================================
  unsigned int    zProjectile;     // atomic number projectile
  unsigned int    aProjectile;     // mass number projectile
  unsigned int    nPartProjectile; // number of participants  projectile
  unsigned int    zTarget;         // atomic number target
  unsigned int    aTarget;         // mass number target
  unsigned int    nPartTarget;     // number of participants  target
  unsigned int    nPartTotal;      // total number of participants
  unsigned int    nBinaryTotal;    // total number of binary collisions
  double impactParameter; // nucleus-nucleus center distance in fm
  double centrality;      // fraction cross section value
  double multiplicity;    // nominal multiplicity in the reference range
  double other;           // other value of interest
  unsigned int    particlesCounted;   // number of particles provided internally by event generator/reader
  unsigned int    particlesAccepted;  // number of particles accepted by event generator/reader's particle filter

  // From GlobalAnalyzer
  int    nFilters;
  vector<double> nFiltered;  // number of particles accepted by filter #i
  vector<double> eFiltered;  // total energy of particles accepted by filter #i
  vector<double> qFiltered;  // total charge of particles accepted by filter #i
  vector<double> sFiltered;  // total strangeness of particles accepted by filter #i
  vector<double> bFiltered;  // total baryon number of particles accepted by filter #i

  ClassDef(EventProperties,0)

};




#endif /* EventProperties_hpp */
