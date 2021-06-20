//
//  ParticleFilter.cpp
//  MyMC
//
//  Created by Claude Pruneau on 12/12/16.
//  Copyright © 2016 Claude Pruneau. All rights reserved.
//

#include <TMath.h>
#include "ParticleFilter.hpp"

ClassImp(ParticleFilter);

// ==========================================================================================
// CTOR
// Note: To turn off filtering on a specific variable, set the min large than the max.
// ==========================================================================================
ParticleFilter::ParticleFilter(double minPt,
                               double maxPt,
                               double minEta,
                               double maxEta,
                               double minY,
                               double maxY,
                               SpeciesSelection _pidSelected,
                               ChargeSelection  _chargeSelected,
                               StatusSelection  _statusSelected)
:
pidSelected     (_pidSelected),
chargeSelected  (_chargeSelected),
statusSelected  (_statusSelected),
filteringOnPt   (maxPt  > minPt),
filteringOnEta  (maxEta > minEta),
filteringOnY    (maxY   > minY),
min_pt          (minPt),
max_pt          (maxPt),
min_eta         (minEta),
max_eta         (maxEta),
min_y           (minY),
max_y           (maxY),
pdgAccepted()
{
  // no ops
}

ParticleFilter::ParticleFilter(double minPt,
                               double maxPt,
                               double minEta,
                               double maxEta,
                               double minY,
                               double maxY,
                               vector<int> & pdgSelected,
                               StatusSelection _statusSelected)
:
pidSelected     (ByPDG),
chargeSelected  (NotUsed),
statusSelected  (_statusSelected),
filteringOnPt   (maxPt  > minPt),
filteringOnEta  (maxEta > minEta),
filteringOnY    (maxY   > minY),
min_pt          (minPt),
max_pt          (maxPt),
min_eta         (minEta),
max_eta         (maxEta),
min_y           (minY),
max_y           (maxY),
pdgAccepted     (pdgSelected)
{
  // no ops
}

ParticleFilter::ParticleFilter(const ParticleFilter & otherFilter)
:
pidSelected     (otherFilter.pidSelected),
chargeSelected  (otherFilter.chargeSelected),
statusSelected  (otherFilter.statusSelected),
filteringOnPt   (otherFilter.filteringOnPt),
filteringOnEta  (otherFilter.filteringOnEta),
filteringOnY    (otherFilter.filteringOnY),
min_pt          (otherFilter.min_pt),
max_pt          (otherFilter.max_pt),
min_eta         (otherFilter.min_eta),
max_eta         (otherFilter.max_eta),
min_y           (otherFilter.min_y),
max_y           (otherFilter.max_y),
pdgAccepted     (otherFilter.pdgAccepted)
{
  // no ops
}

ParticleFilter & ParticleFilter::operator=(const ParticleFilter & otherFilter)
{
  if (this!=&otherFilter)
    {
    pidSelected    = otherFilter.pidSelected;
    chargeSelected = otherFilter.chargeSelected;
    statusSelected = otherFilter.statusSelected;
    filteringOnPt  = otherFilter.filteringOnPt;
    filteringOnEta = otherFilter.filteringOnEta;
    filteringOnY   = otherFilter.filteringOnY;
    min_pt         = otherFilter.min_pt;
    max_pt         = otherFilter.max_pt;
    min_eta        = otherFilter.min_eta;
    max_eta        = otherFilter.max_eta;
    min_y          = otherFilter.min_y;
    max_y          = otherFilter.max_y;
    pdgAccepted    = otherFilter.pdgAccepted;
    }
  return *this;
}



bool ParticleFilter::acceptCharge(double charge)
{
  bool accepting = true;
  switch (chargeSelected)
    {
      case AllCharges: accepting = true; break;
      case Negative:   accepting = (charge < 0); break;
      case Positive:   accepting = (charge > 0); break;
      case Charged:    accepting = (charge != 0); break;
      case Neutral:    accepting = (charge == 0); break;
      case NotUsed:    accepting = true; break;
    }
  return accepting;
}

bool ParticleFilter::acceptPid(int pid)
{
  bool accepting = false;
  for (unsigned int k=0; k<pdgAccepted.size(); k++)
    {
    if (pid==pdgAccepted[k])
      {
      accepting = true; break;
      }
    }
  return accepting;
}

bool ParticleFilter::acceptType(ParticleType & type)
{
  bool accepting = true;
  int  pid;
  switch (pidSelected)
    {
      case AllSpecies: accepting = true; break;
      case Photon:   accepting = type.isPhoton();   break;
      case Lepton:   accepting = type.isLepton();   break;
      case Electron: accepting = type.isElectron(); break;
      case Muon:     accepting = type.isMuon();     break;
      case Hadron:   accepting = type.isHadron();   break;
      case Pion:     accepting = type.isPion();     break;
      case Kaon:     accepting = type.isKaon();     break;
      case Baryon:   accepting = type.isBaryon();   break;
      case Proton:   accepting = type.isProton() || type.isAntiProton(); break;
      case Lambda:
      pid = type.getPdgCode();
      accepting  = (pid == 3122) || (pid == -3122); break;
      case ByPDG:     accepting = false; break; // this should not happen -- handled by acceptPid()
      case ByUserPID: accepting = false; break; // this should not happen -- handled by acceptPid()
    }
  return accepting;
}

bool ParticleFilter::acceptStatus(bool liveStatus)
{
  bool accepting = true;
  switch (statusSelected)
    {
      case AllStatus: break;
      case Live:        accepting = liveStatus;   break;
      case Decayed:     accepting = !liveStatus;  break;
      case Interaction: accepting = !liveStatus;  break;
    }
  return accepting;
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
bool ParticleFilter::accept(Particle & particle)
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
  if (filteringOnEta && !acceptEta(momentum.Eta()))    return false;
  if (filteringOnY   && !acceptY(momentum.Rapidity())) return false;
  return true;
}

// ==========================================================================================
// Creates a short filter name based on the PID and charge accepted
// ==========================================================================================
TString ParticleFilter::getName() const
{
  TString name;
  switch (pidSelected)
    {
      default: name = "WakyWaky"; break;
      case AllSpecies: name = "All"; break;
      case Photon:     name = "Gam"; break;
      case Lepton:     name = "Lep"  ; break;
      case Electron:   name = "El"; break;
      case Muon:       name = "Mu"; break;
      case Hadron:     name = "H"; break;
      case Pion:       name = "Pi"; break;
      case Kaon:       name = "K"; break;
      case Baryon:     name = "B"; break;
      case Proton:     name = "P"; break;
      case Lambda:     name = "Lam"; break;
      case ByPDG:      name = "PDG";
      for (unsigned int k=0; k<pdgAccepted.size();k++)
        {
        if (pdgAccepted[k]>0)
          {
          name += pdgAccepted[k];
          }
        else
          {
          name += "M";
          name += TMath::Abs(pdgAccepted[k]);
          }
        if (k+1<pdgAccepted.size()) name += "_";
        }
      break;
      case ByUserPID:  name = "User";
      for (unsigned int k=0; k<pdgAccepted.size();k++)
        {
        if (pdgAccepted[k]>0)
          {
          name += pdgAccepted[k];
          }
        else
          {
          name += "M";
          name += TMath::Abs(pdgAccepted[k]);
          }
        if (k+1<pdgAccepted.size()) name += "_";
        }
      break;
    }

  if (pidSelected == Photon) return name;

  switch (chargeSelected)
    {
      default: name = "WakyWakyWakyWaky"; break;
      case AllCharges:   name += "All"; break; // all charges
      case Negative: name += "M"; break; // minus
      case Positive: name += "P"; break;
      case Charged:  name += "C"; break;
      case Neutral:  name += "0"; break;
      case NotUsed:               break;
    }
  return name;
}

// ==========================================================================================
// Creates a short filter title  based on the PID and charge accepted
// ==========================================================================================
TString ParticleFilter::getTitle() const
{
  TString name;
  switch (pidSelected)
    {
      case AllSpecies:name = "l,h"; break;
      case Photon:   name = "#gamma"; break;
      case Lepton:   name = "l"  ; break;
      case Electron: name = "e"; break;
      case Muon:     name = "#muon"; break;
      case Hadron:   name = "h"; break;
      case Pion:     name = "#pi"; break;
      case Kaon:     name = "K"; break;
      case Baryon:   name = "Baryon"; break;
      case Proton:   name = "p"; break;
      case Lambda:   name = "#Lambda"; break;
      case ByPDG:      name = "PDG";
      for (unsigned int k=0; k<pdgAccepted.size();k++)
        {
        if (pdgAccepted[k]>0)
          {
          name += pdgAccepted[k];
          }
        else
          {
          name += "M";
          name += TMath::Abs(pdgAccepted[k]);
          }
        if (k+1<pdgAccepted.size()) name += "_";
        }
      break;
      case ByUserPID:  name = "User";
      for (unsigned int k=0; k<pdgAccepted.size();k++)
        {
        if (pdgAccepted[k]>0)
          {
          name += pdgAccepted[k];
          }
        else
          {
          name += "M";
          name += TMath::Abs(pdgAccepted[k]);
          }
        }
    }

  if (pidSelected == Photon) return name;


  switch (chargeSelected)
    {
      case AllCharges:name += "^{0,#pm}"; break; // all charges
      case Negative: name += "^{-}";   break; // minus
      case Positive: name += "^{+}";   break;
      case Charged:  name += "^{#pm}"; break;
      case Neutral:  name += "^{0}";   break;
      case NotUsed:                    break;
    }
  return name;
}

// ==========================================================================================
// Creates a long filter name based on the PID and charge accepted
// as well as the pT, eta, and y minimum and maximum accepted values.
// To avoid floating point values, all floats are multiplied by 1000.
// ==========================================================================================
TString ParticleFilter::getLongName() const
{
  TString name = getName();
  if (filteringOnPt)
    {
    name += "PtGeq";
    if (min_pt<0.0) name += "M";
    name += abs(int(1000.0*min_pt));
    name += "Lt";
    if (max_pt<0.0) name += "M";
    name += abs(int(1000.0*max_pt));
    }
  if (filteringOnEta)
    {
    name += "EtaGeq";
    if (min_eta<0.0) name += "M";
    name += abs(int(1000.0*min_eta));
    name += "Lt";
    if (max_eta<0.0) name += "M";
    name += abs(int(1000.0*max_eta));
    }
  if (filteringOnY)
    {
    name += "YGeq";
    if (min_y<0.0) name += "M";
    name += abs(int(1000.0*min_y));
    name += "Lt";
    if (max_y<0.0) name += "M";
    name += abs(int(1000.0*max_y));
    }
  return name;
}


// ==========================================================================================
// Creates a long filter name based on the PID and charge accepted
// as well as the pT, eta, and y minimum and maximum accepted values.
// To avoid floating point values, all floats are multiplied by 1000.
// ==========================================================================================
TString ParticleFilter::getLongTitle()  const
{
  TString name = getTitle();
  if (filteringOnPt)   name += Form(" %g < p_{T} < %g;",min_pt,max_pt);
  if (filteringOnEta)  name += Form(" %g < #eta < %g;",min_eta,max_eta);
  if (filteringOnY)    name += Form(" %g < Y < %g",min_y,max_y);
  return name;
}


void ParticleFilter::printProperties( ostream & os)
{
  os << "          Filter Name: " << getLongName() << endl;
  os << "                Title: " << getLongTitle() << endl;
  os << "    Filter Short Name: " << getName() << endl;
  os << "          Short Title: " << getTitle() << endl;
  os << "          pidSelected: " << pidSelected << endl;
  os << "       chargeSelected: " << chargeSelected << endl;
  os << "       statusSelected: " << statusSelected << endl;
  os << "        filteringOnPt: " << filteringOnPt << endl;
  os << "       filteringOnEta: " << filteringOnEta << endl;
  os << "         filteringOnY: " << filteringOnY << endl;
  os << "               min_pt: " << min_pt << endl;
  os << "               max_pt: " << max_pt << endl;
  os << "              min_eta: " << min_eta << endl;
  os << "              max_eta: " << max_eta << endl;
  os << "                min_y: " << min_y << endl;
  os << "                max_y: " << max_y << endl;

  if (pdgAccepted.size()>0)
    {
    os << "  pdg values Accepted: ";
    for (unsigned int k=0; k<pdgAccepted.size();k++)
      {
      os << pdgAccepted[k] << ", ";
      }
    os << endl;
    }
}


vector<ParticleFilter*> ParticleFilter::createChargedHadronFilters(double minPt, double maxPt, double minEta, double maxEta, double minY, double maxY)
{
  vector<ParticleFilter*> filters;

  filters.push_back( new ParticleFilter(minPt, maxPt, minEta, maxEta, minY, maxY, ParticleFilter::Hadron, ParticleFilter::Charged,  ParticleFilter::Live) );
  filters.push_back( new ParticleFilter(minPt, maxPt, minEta, maxEta, minY, maxY, ParticleFilter::Hadron, ParticleFilter::Positive, ParticleFilter::Live) );
  filters.push_back( new ParticleFilter(minPt, maxPt, minEta, maxEta, minY, maxY, ParticleFilter::Hadron, ParticleFilter::Negative, ParticleFilter::Live) );
  filters.push_back( new ParticleFilter(minPt, maxPt, minEta, maxEta, minY, maxY, ParticleFilter::Pion,   ParticleFilter::Charged,  ParticleFilter::Live) );
  filters.push_back( new ParticleFilter(minPt, maxPt, minEta, maxEta, minY, maxY, ParticleFilter::Pion,   ParticleFilter::Positive, ParticleFilter::Live) );
  filters.push_back( new ParticleFilter(minPt, maxPt, minEta, maxEta, minY, maxY, ParticleFilter::Pion,   ParticleFilter::Negative, ParticleFilter::Live) );
  filters.push_back( new ParticleFilter(minPt, maxPt, minEta, maxEta, minY, maxY, ParticleFilter::Kaon,   ParticleFilter::Charged,  ParticleFilter::Live) );
  filters.push_back( new ParticleFilter(minPt, maxPt, minEta, maxEta, minY, maxY, ParticleFilter::Kaon,   ParticleFilter::Positive, ParticleFilter::Live) );
  filters.push_back( new ParticleFilter(minPt, maxPt, minEta, maxEta, minY, maxY, ParticleFilter::Kaon,   ParticleFilter::Negative, ParticleFilter::Live) );
  filters.push_back( new ParticleFilter(minPt, maxPt, minEta, maxEta, minY, maxY, ParticleFilter::Proton, ParticleFilter::Charged,  ParticleFilter::Live) );
  filters.push_back( new ParticleFilter(minPt, maxPt, minEta, maxEta, minY, maxY, ParticleFilter::Proton, ParticleFilter::Positive, ParticleFilter::Live) );
  filters.push_back( new ParticleFilter(minPt, maxPt, minEta, maxEta, minY, maxY, ParticleFilter::Proton, ParticleFilter::Negative, ParticleFilter::Live) );
  return filters;
}

// Produce 6 particle filters:  pi+, K+, p, pi-, K-, pbar
vector<ParticleFilter*> ParticleFilter::createPlusMinusHadronFilters(double minPt, double maxPt, double minEta, double maxEta, double minY, double maxY)
{
  vector<ParticleFilter*> filters;
  filters.push_back( new ParticleFilter(minPt, maxPt, minEta, maxEta, minY, maxY, ParticleFilter::Pion,   ParticleFilter::Positive, ParticleFilter::Live) );
  filters.push_back( new ParticleFilter(minPt, maxPt, minEta, maxEta, minY, maxY, ParticleFilter::Kaon,   ParticleFilter::Positive, ParticleFilter::Live) );
  filters.push_back( new ParticleFilter(minPt, maxPt, minEta, maxEta, minY, maxY, ParticleFilter::Proton, ParticleFilter::Positive, ParticleFilter::Live) );
  filters.push_back( new ParticleFilter(minPt, maxPt, minEta, maxEta, minY, maxY, ParticleFilter::Pion,   ParticleFilter::Negative, ParticleFilter::Live) );
  filters.push_back( new ParticleFilter(minPt, maxPt, minEta, maxEta, minY, maxY, ParticleFilter::Kaon,   ParticleFilter::Negative, ParticleFilter::Live) );
  filters.push_back( new ParticleFilter(minPt, maxPt, minEta, maxEta, minY, maxY, ParticleFilter::Proton, ParticleFilter::Negative, ParticleFilter::Live) );
  return filters;
}

vector<ParticleFilter*> ParticleFilter::createHadronFilters(double minPt, double maxPt, double minEta, double maxEta, double minY, double maxY)
{
  vector<ParticleFilter*> filters;
  filters.push_back( new ParticleFilter(minPt, maxPt, minEta, maxEta, minY, maxY, ParticleFilter::Hadron, ParticleFilter::Charged,  ParticleFilter::Live) );
  filters.push_back( new ParticleFilter(minPt, maxPt, minEta, maxEta, minY, maxY, ParticleFilter::Pion,   ParticleFilter::Charged,  ParticleFilter::Live) );
  filters.push_back( new ParticleFilter(minPt, maxPt, minEta, maxEta, minY, maxY, ParticleFilter::Kaon,   ParticleFilter::Charged,  ParticleFilter::Live) );
  filters.push_back( new ParticleFilter(minPt, maxPt, minEta, maxEta, minY, maxY, ParticleFilter::Proton, ParticleFilter::Charged,  ParticleFilter::Live) );
  return filters;
}

vector<ParticleFilter*> ParticleFilter::createMultiplePdgFilters(double minPt, double maxPt, double minEta, double maxEta, double minY, double maxY, vector<int> & pdgAccepted)
{
  vector<ParticleFilter*> filters;
  vector<int> pdgs;
  for (unsigned int k=0;k<pdgAccepted.size();k++)
    {
    pdgs.clear();
    pdgs.push_back( pdgAccepted[k] );
    filters.push_back( new ParticleFilter(minPt, maxPt, minEta, maxEta, minY, maxY, pdgs, ParticleFilter::Live) );
    }
  return filters;
}
