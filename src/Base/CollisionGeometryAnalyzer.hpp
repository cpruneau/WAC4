// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
#ifndef WAC_CollisionGeometryAnalyzer
#define WAC_CollisionGeometryAnalyzer
#include "Task.hpp"
#include "CollisionGeometry.hpp"
#include "CollisionGeometryHistograms.hpp"
#include "CollisionGeometryConfiguration.hpp"

//!
//! Open the root file named "fileName" located on the path "inputPath", using options specified by "ioOption".
//! @param inputPath path where to find or create the file.
//! @param fileName name of the root file to open for i/o.
//! @param ioOption i/o options.
//! @return Pointer to the file if successfully open or a null pointer if the file could not be opened.
//!
class CollisionGeometryAnalyzer : public Task
{
public:

  CollisionGeometryAnalyzer(const TString &                  _name,
                            CollisionGeometryConfiguration * _configuration,
                            vector<EventFilter*>             _eventFilters,
                            LogLevel                         _requiredLevel);
  virtual ~CollisionGeometryAnalyzer() {}
  virtual void execute();
  virtual void createHistograms();
  virtual void loadHistograms(TFile * inputFile);

  CollisionGeometry * getCollisionGeometry() { return collisionGeometry; }

protected:

  CollisionGeometry * collisionGeometry;

  ClassDef(CollisionGeometryAnalyzer,0)
};

#endif /* WAC_CollisionGeometryAnalyzer */
