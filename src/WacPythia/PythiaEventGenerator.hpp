// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
#ifndef WAC_PythiaEventGenerator
#define WAC_PythiaEventGenerator
#include "TParticle.h"
#include "TClonesArray.h"
#include "TPythia8.h"
#include "TFile.h"
#include "TTree.h"
#include "NucleonNucleonCollisionGenerator.hpp"
#include "EventFilter.hpp"
#include "ParticleFilter.hpp"
#include "PythiaConfiguration.hpp"

class PythiaEventGenerator : public NucleonNucleonCollisionGenerator
{
public:


  PythiaEventGenerator(const TString &         _name,
                       PythiaConfiguration *   _configuration,
                       vector<EventFilter*>    _eventFilters,
                       vector<ParticleFilter*> _particleFilters,
                       LogLevel                _selectedLevel);
  virtual ~PythiaEventGenerator() {} 
  virtual void initialize();
  virtual void finalize();
  virtual void execute();
  virtual void generate(Particle * parent);

  TPythia8* pythia8; // = new TPythia8();

  // For WAC analyses
  int nMax; //  = 10000;
  TClonesArray* particles; // = new TClonesArray("TParticle", nMax);

  // For TTree file output
  // Set up the ROOT TFile and TTree.
  TFile *outputFile;
  Pythia8::Event *outputEvent;
  TTree *outputTree;

  bool standaloneMode;

  ClassDef(PythiaEventGenerator,0)
};

#endif /* WAC_PythiaEventGenerator */
