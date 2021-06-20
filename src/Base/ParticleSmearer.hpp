// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
#ifndef WAC_ParticleSmearer
#define WAC_ParticleSmearer
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TLorentzVector.h"
#include "Particle.hpp"


class ParticleSmearer
{
public:

  ParticleSmearer(TH1 * _avgPt,
                  TH1 * _rmsPt,
                  TH1 * _avgPhi,
                  TH1 * _rmsPhi,
                  TH1 * _avgEta,
                  TH1 * _rmsEta);
  ParticleSmearer(const ParticleSmearer & otherFilter);
  ParticleSmearer & operator=(const ParticleSmearer & otherFilter);
  virtual ~ParticleSmearer();
  void smear(const TLorentzVector &in, TLorentzVector & out);

protected:

  void smearPt(double pt, double phi, double eta, double & smearedPt);
  void smearEta(double pt, double phi, double eta, double & smearedEta);
  void smearPhi(double pt, double phi, double eta, double & smearedPhi);

  TH1 * avgPt;
  TH1 * rmsPt;
  TH1 * avgPhi;
  TH1 * rmsPhi;
  TH1 * avgEta;
  TH1 * rmsEta;

  ClassDef(ParticleSmearer,0)
};

#endif /* WAC_ParticleSmearer */
