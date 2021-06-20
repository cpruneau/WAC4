//
//  ParticlePairFilter.cpp
//  MyMC
//
//  Created by Claude Pruneau on 12/12/16.
//  Copyright © 2016 Claude Pruneau. All rights reserved.
//

#include "ParticlePairFilter.hpp"
ClassImp(ParticlePairFilter);

//////////////////////////////////////////////////////////////////////////////////////////
// CTOR
//////////////////////////////////////////////////////////////////////////////////////////
ParticlePairFilter::ParticlePairFilter(double minDpt,
                                       double maxDpt,
                                       double minDphi,
                                       double maxDphi,
                                       double minDeta,
                                       double maxDeta)
:
min_dpt    (minDpt),
max_dpt    (maxDpt),
min_dphi   (minDphi),
max_dphi   (maxDphi),
min_deta   (minDeta),
max_deta   (maxDeta)
{
  // no ops
}

//////////////////////////////////////////////////////////////////////////////////////////
// DTOR
//////////////////////////////////////////////////////////////////////////////////////////
ParticlePairFilter::~ParticlePairFilter()
{
  // no ops
}

//////////////////////////////////////////////////////////////////////////////////////////
// Accept/reject the given pair of particles
//////////////////////////////////////////////////////////////////////////////////////////
bool ParticlePairFilter::accept(Particle & particle1, Particle & particle2)
{
//  ParticleType & type1 = particle1.getType();
//  ParticleType & type2 = particle2.getType();
  if (particle1.isDecayed() )          return false;
  if (particle2.isDecayed() )          return false;
  return true;
}
