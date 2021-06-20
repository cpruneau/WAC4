//
//  ParticleFilterEfficiency.cpp
//  MyMC
//
//  Created by Claude Pruneau on 12/12/16.
//  Copyright © 2016 Claude Pruneau. All rights reserved.
//

#include <TMath.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include "ParticleFilterEfficiency.hpp"

ClassImp(ParticleFilterEfficiency);

// ==========================================================================================
// CTOR
// Note: To turn off filtering on a specific variable, set the min large than the max.
// ==========================================================================================
ParticleFilterEfficiency::ParticleFilterEfficiency(double minPt,
                                                   double maxPt,
                                                   double minEta,
                                                   double maxEta,
                                                   TH1 * _efficiency)
:
ParticleFilter(minPt,maxPt,minEta,maxEta,1.0,-1.0, AllSpecies, Charged,  Live),
efficiency(_efficiency)
{
  // no ops
}

ParticleFilterEfficiency::ParticleFilterEfficiency(const ParticleFilterEfficiency & otherFilter)
:
ParticleFilter(otherFilter),
efficiency(otherFilter.efficiency)
{
  // no ops
}

ParticleFilterEfficiency & ParticleFilterEfficiency::operator=(const ParticleFilterEfficiency & otherFilter)
{
  if (this!=&otherFilter)
    {
    ParticleFilter::operator=(otherFilter);
    efficiency = (TH1*) otherFilter.efficiency->Clone();
    }
  return *this;
}


// ==========================================================================================
// DTOR
// ==========================================================================================
ParticleFilterEfficiency::~ParticleFilterEfficiency()
{
  if (efficiency) delete efficiency;
}


// ==========================================================================================
// accept/reject the given particle based on filter parameter
// Filtering is based on
// Charge : enum ChargeSelection   { AllCharges, Negative, Positive, Charged, Neutral };
// Species: enum SpeciesSelection  { AllSpecies, Photon, Lepton, Electron, Muon, Hadron, Pion, Kaon, Baryon, Proton, Lambda };
// pt     : accept conditionally if min_pt < pt <= max_pt  OR  if min_pt >= max_pt
// eta    : accept conditionally if min_eta< eta<= max_eta OR  if min_eta>= max_eta
// y      : accept conditionally if   min_y< y  <= max_y OR    if min_y>  = max_y
// ==========================================================================================
bool ParticleFilterEfficiency::accept(Particle & particle)
{
  bool accept = false;
  if (!ParticleFilter::accept(particle) ) return accept;
  TLorentzVector & momentum = particle.getMomentum();
  double r = gRandom->Rndm();
  double eff;
  int bin;
  if (efficiency->IsA() == TH1::Class()) // pt only
    {
    bin = efficiency->FindBin(momentum.Pt());
    eff = efficiency->GetBinContent(bin);
    if (r<=eff) accept = true;
    }
  else  if (efficiency->IsA() == TH2::Class())
    {
    bin = efficiency->FindBin(momentum.Eta(),momentum.Pt());
    eff = efficiency->GetBinContent(bin);
    if (r<=eff) accept = true;
    }
  else if (efficiency->IsA() == TH3::Class())
    {
    bin = efficiency->FindBin(momentum.Eta(),momentum.Phi(),momentum.Pt());
    eff = efficiency->GetBinContent(bin);
    if (r<=eff) accept = true;
    }
  return accept;
}

vector<ParticleFilter*> ParticleFilterEfficiency::createParticleFilters()
{
  vector<ParticleFilter*> filters;
//  filters.push_back( new ParticleFilterEfficiency(minPt, maxPt, minEta, maxEta, minY, maxY, ParticleFilterEfficiency::Hadron, ParticleFilterEfficiency::Charged,  ParticleFilterEfficiency::Live) );
//  filters.push_back( new ParticleFilterEfficiency(minPt, maxPt, minEta, maxEta, minY, maxY, ParticleFilterEfficiency::Hadron, ParticleFilterEfficiency::Positive, ParticleFilterEfficiency::Live) );
//  filters.push_back( new ParticleFilterEfficiency(minPt, maxPt, minEta, maxEta, minY, maxY, ParticleFilterEfficiency::Hadron, ParticleFilterEfficiency::Negative, ParticleFilterEfficiency::Live) );
//  filters.push_back( new ParticleFilterEfficiency(minPt, maxPt, minEta, maxEta, minY, maxY, ParticleFilterEfficiency::Pion,   ParticleFilterEfficiency::Charged,  ParticleFilterEfficiency::Live) );
//  filters.push_back( new ParticleFilterEfficiency(minPt, maxPt, minEta, maxEta, minY, maxY, ParticleFilterEfficiency::Pion,   ParticleFilterEfficiency::Positive, ParticleFilterEfficiency::Live) );
//  filters.push_back( new ParticleFilterEfficiency(minPt, maxPt, minEta, maxEta, minY, maxY, ParticleFilterEfficiency::Pion,   ParticleFilterEfficiency::Negative, ParticleFilterEfficiency::Live) );
//  filters.push_back( new ParticleFilterEfficiency(minPt, maxPt, minEta, maxEta, minY, maxY, ParticleFilterEfficiency::Kaon,   ParticleFilterEfficiency::Charged,  ParticleFilterEfficiency::Live) );
//  filters.push_back( new ParticleFilterEfficiency(minPt, maxPt, minEta, maxEta, minY, maxY, ParticleFilterEfficiency::Kaon,   ParticleFilterEfficiency::Positive, ParticleFilterEfficiency::Live) );
//  filters.push_back( new ParticleFilterEfficiency(minPt, maxPt, minEta, maxEta, minY, maxY, ParticleFilterEfficiency::Kaon,   ParticleFilterEfficiency::Negative, ParticleFilterEfficiency::Live) );
//  filters.push_back( new ParticleFilterEfficiency(minPt, maxPt, minEta, maxEta, minY, maxY, ParticleFilterEfficiency::Proton, ParticleFilterEfficiency::Charged,  ParticleFilterEfficiency::Live) );
//  filters.push_back( new ParticleFilterEfficiency(minPt, maxPt, minEta, maxEta, minY, maxY, ParticleFilterEfficiency::Proton, ParticleFilterEfficiency::Positive, ParticleFilterEfficiency::Live) );
//  filters.push_back( new ParticleFilterEfficiency(minPt, maxPt, minEta, maxEta, minY, maxY, ParticleFilterEfficiency::Proton, ParticleFilterEfficiency::Negative, ParticleFilterEfficiency::Live) );
  return filters;
}


