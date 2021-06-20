// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
#ifndef WAC_CollisionGeometryGenerator
#define WAC_CollisionGeometryGenerator
#include "Task.hpp"
#include "CollisionGeometryConfiguration.hpp"
#include "NucleusGenerator.hpp"

class CollisionGeometryGenerator : public Task
{
public:

  CollisionGeometryGenerator(const TString &                  _name,
                             CollisionGeometryConfiguration * _configuration,
                             vector<EventFilter*>             _eventFilters,
                             LogLevel                         _requiredLevel=Info);
  
  virtual ~CollisionGeometryGenerator();

  virtual void initialize();
  virtual void clear();
  virtual void reset();
  virtual void execute();

  Nucleus & getNucleusA() { return eventStreams[0]->getNucleusA(); }
  Nucleus & getNucleusB() { return eventStreams[0]->getNucleusB(); }
  NucleusGenerator  * getNucleusGeneratorA() { return nucleusGeneratorA; }
  NucleusGenerator  * getNucleusGeneratorB() { return nucleusGeneratorB; }

protected:

  NucleusGenerator  * nucleusGeneratorA;
  NucleusGenerator  * nucleusGeneratorB;
  double minB, minBSq, maxB, maxBSq;
  double nnCrossSection;
  double maxNNDistanceSq;

  ClassDef(CollisionGeometryGenerator,0)
};

#endif /* WAC_CollisionGeometryGenerator */
