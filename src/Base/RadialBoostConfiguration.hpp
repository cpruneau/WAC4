// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
#ifndef WAC_RadialBoostConfiguration
#define WAC_RadialBoostConfiguration
#include "TaskConfiguration.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Configuration of a given analysis
////////////////////////////////////////////////////////////////////////////////////////////////////////
class RadialBoostConfiguration : public TaskConfiguration
{
public:
  
  RadialBoostConfiguration();
  RadialBoostConfiguration(const RadialBoostConfiguration & source);
  virtual ~RadialBoostConfiguration(){}
  RadialBoostConfiguration & operator=(const RadialBoostConfiguration & source);
  TString getBaseName() { return baseName; }
  void printConfiguration(ostream & os);

  //!
  //!  Slope coefficient
  //!
  double param_a;
  //!
  //!  Exponent of the ratio r/r_max
  //!
  double param_b;
  //!
  //!  maximum value of beta allowed in the generation.
  //!
  double betaMaximum;

  //!
  //!  Base name (beyond $WAC_REPOSITOTY) of the input file.
  //!
  TString baseName;

  // Histogram parameters
  int nBins_phi;  double min_phi;  double max_phi;
  int nBins_r;    double min_r;    double max_r;
  int nBins_beta; double min_beta; double max_beta;

  ClassDef(RadialBoostConfiguration,0)
};

#endif /* WAC_RadialBoostConfiguration */
