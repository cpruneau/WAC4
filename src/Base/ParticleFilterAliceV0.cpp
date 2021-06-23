//
//  ParticleFilterAliceV0.cpp
//  MyMC
//
//  Created by Claude Pruneau on 12/12/16.
//  Copyright © 2016 Claude Pruneau. All rights reserved.
//

#include <TMath.h>
#include "ParticleFilterAliceV0.hpp"

ClassImp(ParticleFilterAliceV0);

// ==========================================================================================
// CTOR
// Note: To turn off filtering on a specific variable, set the min large than the max.
// ==========================================================================================
ParticleFilterAliceV0::ParticleFilterAliceV0(V0Selection       v0Selected,
                                             SpeciesSelection  pidSelected,
                                             ChargeSelection   chargeSelected,
                                             StatusSelection   statusSelected,
                                             double minPtRequested,
                                             double maxPtRequested)
:
ParticleFilter(minPtRequested,maxPtRequested,1.0,-1.0,1.0,-1.0,pidSelected,chargeSelected,statusSelected),
v0Selected(v0Selected),
min_eta_v0A(2.8),
max_eta_v0A(5.1),
min_eta_v0C(-3.7),
max_eta_v0C(-1.7)
{
  // no ops
}

ParticleFilterAliceV0::ParticleFilterAliceV0(const ParticleFilterAliceV0 & otherFilter)
:ParticleFilter(otherFilter),
v0Selected(otherFilter.v0Selected),
min_eta_v0A(otherFilter.min_eta_v0A),
max_eta_v0A(otherFilter.max_eta_v0A),
min_eta_v0C(otherFilter.min_eta_v0C),
max_eta_v0C(otherFilter.max_eta_v0C)
{
// noops
}

ParticleFilterAliceV0 & ParticleFilterAliceV0::operator=( const ParticleFilterAliceV0 & otherFilter)
{
  if (this!=&otherFilter)
    {
    ParticleFilter::operator=(otherFilter);
    v0Selected = otherFilter.v0Selected;
    min_eta_v0A = otherFilter.min_eta_v0A;
    max_eta_v0A = otherFilter.max_eta_v0A;
    min_eta_v0C = otherFilter.min_eta_v0C;
    max_eta_v0C = otherFilter.max_eta_v0C;
    }
  return *this;
}



// ==========================================================================================
// DTOR
// ==========================================================================================
ParticleFilterAliceV0::~ParticleFilterAliceV0()
{
  // no ops
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
bool ParticleFilterAliceV0::accept(Particle & particle)
{
  ParticleType & type = particle.getType();
  if (!acceptStatus(particle.isLive()) )            return false;
  if (!acceptCharge(type.getCharge()))              return false;
  // -- if (!acceptPid(TMath::Abs(type.getPdgCode())))    return false;
  if (pidSelected==ByPDG)
    {
    if (!acceptPid(particle.getType().getPdgCode())) return false;
    }
  else  if (pidSelected==ByUserPID)
    {
    if (!acceptPid(particle.getPid()))                 return false;
    }
  else
    {
    if (!acceptType(particle.getType()))               return false;
    }
  TLorentzVector & momentum = particle.getMomentum();
  if (filteringOnPt  && !acceptPt(momentum.Pt()))      return false;
  bool accepting = true;
  double eta = momentum.Eta();
  switch (v0Selected)
    {
      case V0A:   accepting = (min_eta_v0A<eta)&&(eta<= max_eta_v0A); break;
      case V0C:   accepting = (min_eta_v0C<eta)&&(eta<= max_eta_v0C); break;
      case V0M:   accepting = ((min_eta_v0A<eta)&&(eta<= max_eta_v0A)) ||
      ((min_eta_v0C<eta)&&(eta<= max_eta_v0C));break;
    }
  return accepting;
}

// ==========================================================================================
// Creates a short filter name based on the PID and charge accepted
// ==========================================================================================
TString ParticleFilterAliceV0::getName() const
{
  TString name;
  switch (v0Selected)
    {
      default: name = "WakyWaky"; break;
      case V0A:     name = "V0A"; break;
      case V0C:     name = "V0C"; break;
      case V0M:     name = "V0M"; break;
    }
  return name;
}

// ==========================================================================================
// Creates a short filter title  based on the PID and charge accepted
// ==========================================================================================
TString ParticleFilterAliceV0::getTitle() const
{
  TString name;
  switch (v0Selected)
    {
      default: name = "WakyWaky"; break;
      case V0A:     name = "V0A"; break;
      case V0C:     name = "V0C"; break;
      case V0M:     name = "V0M"; break;
    }
  return name;
}



// ==========================================================================================
// Creates a long filter name based on the PID and charge accepted
// as well as the pT, eta, and y minimum and maximum accepted values.
// To avoid floating point values, all floats are multiplied by 1000.
// ==========================================================================================
TString ParticleFilterAliceV0::getLongName() const
{
  TString name = getName();
  if (filteringOnPt)
    {
    name += "PtGeq";
    name += int(1000.0*min_pt);
    name += "Lt";
    name += int(1000.0*max_pt);
    }
  if (filteringOnEta)
    {
    name += "EtaGeq";
    name += int(1000.0*min_eta);
    name += "Lt";
    name += int(1000.0*max_eta);
    }
  if (filteringOnY)
    {
    name += "YGeq";
    name += int(1000.0*min_y);
    name += "Lt";
    name += int(1000.0*max_y);
    }
  return name;
}


// ==========================================================================================
// Creates a long filter name based on the PID and charge accepted
// as well as the pT, eta, and y minimum and maximum accepted values.
// To avoid floating point values, all floats are multiplied by 1000.
// ==========================================================================================
TString ParticleFilterAliceV0::getLongTitle() const 
{
  TString name = getTitle();
  if (filteringOnPt)   name += Form(" %g < p_{T} < %g;",min_pt,max_pt);
  if (filteringOnEta)  name += Form(" %g < #eta < %g;",min_eta,max_eta);
  if (filteringOnY)    name += Form(" %g < Y < %g",min_y,max_y);
  return name;
}


void ParticleFilterAliceV0::printProperties( ostream & os)
{
  os << "          Filter Name: " << getLongName() << endl;
  os << "                Title: " << getLongTitle() << endl;
  os << "          pidSelected: " << pidSelected << endl;
  os << "       chargeSelected: " << chargeSelected << endl;
  os << "       statusSelected: " << statusSelected << endl;
  os << "           v0Selected: " << v0Selected << endl;
  os << "        filteringOnPt: " << filteringOnPt << endl;
  os << "       filteringOnEta: " << filteringOnEta << endl;
  os << "         filteringOnY: " << filteringOnY << endl;
  os << "               min_pt: " << min_pt << endl;
  os << "               max_pt: " << max_pt << endl;
  os << "          min_eta_v0A: " << min_eta_v0A << endl;
  os << "          max_eta_v0A: " << max_eta_v0A << endl;
  os << "          min_eta_v0C: " << min_eta_v0C << endl;
  os << "          max_eta_v0C: " << max_eta_v0C << endl;


}
