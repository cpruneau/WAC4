// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
#ifndef WAC_CollisionGeometryGradientHistograms
#define WAC_CollisionGeometryGradientHistograms
#include "Histograms.hpp"
#include "CollisionGeometryConfiguration.hpp"

class CollisionGeometryGradientHistograms : public Histograms
{
public:

  CollisionGeometryGradientHistograms(const TString & collectionName,
                                      CollisionGeometryConfiguration * configuration,
                                      LogLevel  debugLevel);
  virtual ~CollisionGeometryGradientHistograms() { }
  virtual void createHistograms();
  virtual void loadHistograms(TFile * inputFile);
  virtual void getRadiusAndGradient(double x, double y, double & radius, double & gx, double & gy);

protected:

  TH2      * h_edge;
  TH2      * h_radius;
  TH2      * h_gx;
  TH2      * h_gy;

  ClassDef(CollisionGeometryGradientHistograms,0)

};

#endif /* WAC_CollisionGeometryGradientHistograms  */



