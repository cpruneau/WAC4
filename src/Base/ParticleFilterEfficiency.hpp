// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
#ifndef WAC_ParticleFilterEfficiency
#define WAC_ParticleFilterEfficiency
#include "TH1.h"
#include "TString.h"
#include "Particle.hpp"
#include "ParticleFilter.hpp"


class ParticleFilterEfficiency : public ParticleFilter
{
public:

  ParticleFilterEfficiency(double minPt,
                           double maxPt,
                           double minEta,
                           double maxEta,
                           TH1 * efficiency);
  ParticleFilterEfficiency(const ParticleFilterEfficiency & otherFilter);
  ParticleFilterEfficiency & operator=(const ParticleFilterEfficiency & otherFilter);
  virtual ~ParticleFilterEfficiency();
  virtual bool accept(Particle & particle);
  static vector<ParticleFilter*> createParticleFilters();

protected:

  TH1 * efficiency;

  ClassDef(ParticleFilterEfficiency,0)
};

#endif /* WAC_ParticleFilterEfficiency */
