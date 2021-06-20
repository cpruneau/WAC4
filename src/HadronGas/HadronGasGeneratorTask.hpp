// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/

#ifndef WAC_HadronGasGeneratorTask
#define WAC_HadronGasGeneratorTask
#include "HadronGasGeneratorConfiguration.hpp"
#include "NucleonNucleonCollisionGenerator.hpp"
#include "HadronGas.hpp"
#include "HadronGasHistograms.hpp"
#include "MomentumGenerator.hpp"

class HadronGasGeneratorTask : public NucleonNucleonCollisionGenerator
{
public:

  HadronGasGeneratorTask(const TString & name,
                         HadronGasGeneratorConfiguration * _configuration,
                         LogLevel _selectedLevel);
  virtual ~HadronGasGeneratorTask() {} 
  virtual void initialize();
  virtual void execute();
  virtual void generate(Particle * parent);
  
protected:

  ParticleTypeCollection *   particleTypes;
  ParticleTypeCollection *   stableParticleTypes;
  vector<HadronGas*>         hadronGases;
  vector<MomentumGenerator*> momentumGenerators;
  TH1 * relativeAbundances;
  TH1 * relativeAbundancesGas;
  ClassDef(HadronGasGeneratorTask,0)
};

#endif /* WAC_HadronGasGeneratorTask */
