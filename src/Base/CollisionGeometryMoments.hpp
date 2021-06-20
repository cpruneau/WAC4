// Author: Claude Pruneau   05/08/2020

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.  
 **********************************************************************/
#ifndef WAC_CollisionGeometryMoments
#define WAC_CollisionGeometryMoments
#include "TString.h"

// ***************************************************************************
// CollisionGeometryMoments
//
// Just a container of moments of the geometry of AA collisions
// ***************************************************************************
class CollisionGeometryMoments
{
public:

  CollisionGeometryMoments();
  CollisionGeometryMoments(const CollisionGeometryMoments & source);
  virtual  ~CollisionGeometryMoments()
  {
  // no ops.
  }
  CollisionGeometryMoments& operator=(const CollisionGeometryMoments & source);
  void reset();
  virtual void fill(double x ,double y);
  virtual void calculate();

  double m0; // counts...
  double m1x, m1y, m2x2, m2y2, m2xy;

  double meanX;
  double meanY;
  double varX;
  double varY;
  double varXY;
  double epsX;
  double epsY;
  double epsDenom;
  double epsMod;
  double psi2;
  double area;
  double cphiN[10];
  double sphiN[10];
  double rN[10];
  double psiN[10];
  double eccN[10];

  ClassDef(CollisionGeometryMoments,0)
};

#endif /* CollisionGeometryMoments_hpp */
