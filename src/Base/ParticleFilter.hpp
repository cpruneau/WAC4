// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
#ifndef WAC_ParticleFilter
#define WAC_ParticleFilter
#include "TString.h"
#include "Particle.hpp"

//////////////////////////////////////////////////////////////////////////////////////////
// Single Particle Filter
//
// chargeSelected:
// case -1:    accepts negative only
// case  0:    accepts neutral only
// case  1:    accepts positive only
// case  999:  accepts all
//////////////////////////////////////////////////////////////////////////////////////////



class ParticleFilter
{
public:

  enum ChargeSelection    { AllCharges, Negative, Positive, Charged, Neutral, NotUsed };
  enum SpeciesSelection   { AllSpecies, Photon, Lepton, Electron, Muon, Hadron, Pion, Kaon, Baryon, Proton, Lambda, Charm, ByPDG, ByUserPID };
  enum StatusSelection    { AllStatus,  Live, Decayed, Interaction };

  ParticleFilter(double minPt,
                 double maxPt,
                 double minEta,
                 double maxEta,
                 double minY,
                 double maxY,
                 SpeciesSelection pidSelected = Hadron,
                 ChargeSelection  chargeSelected = Charged,
                 StatusSelection   statusSelected  = Live);

  ParticleFilter(double minPt,
                 double maxPt,
                 double minEta,
                 double maxEta,
                 double minY,
                 double maxY,
                 vector<int> & pdgSelected,
                 StatusSelection   statusSelected  = Live);

  ParticleFilter(const ParticleFilter & otherFilter);
  ParticleFilter & operator=(const ParticleFilter & otherFilter);
  virtual ~ParticleFilter() {}
  virtual bool accept(Particle & particle);
  virtual bool acceptStatus(bool decayed);
  virtual bool acceptCharge(double charge);
  virtual bool acceptPid(int  pid);
  virtual bool acceptType(ParticleType & type); 
  inline  bool acceptPt(double pt)
  {
    return (min_pt<pt) &&  (pt<= max_pt);
  }
  inline bool acceptEta(double eta)
  {
    return (min_eta<eta) && (eta<= max_eta);
  }
  inline bool acceptY(double y)
  {
    return (min_y<y) && (y<= max_y);
  }
  virtual TString getName() const;
  virtual TString getTitle() const;
  virtual TString getLongName() const;
  virtual TString getLongTitle() const;

  void setSpeciesSelection(SpeciesSelection selection)  {  pidSelected    = selection; }
  void setChargeSelection (ChargeSelection  selection)  {  chargeSelected = selection; }
  void setStatusSelection (StatusSelection  selection)  {  statusSelected = selection; }
  void setFilteringOnPt   (bool selection)              {  filteringOnPt  = selection; }
  void setFilteringOnEta  (bool selection)              {  filteringOnEta = selection; }
  void setFilteringOnY    (bool selection)              {  filteringOnY   = selection; }
  void setPtRange(double min, double max)
  {
  min_pt  = min;
  max_pt  = max;
  filteringOnPt =  (min_pt < max_pt) ? true : false;
  }

  void setEtaRange(double min, double max)
  {
  min_eta  = min;
  max_eta  = max;
  filteringOnEta =  (min_eta < max_eta) ? true : false;
  }

  void setYRange(double min, double max)
  {
  min_y  = min;
  max_y  = max;
  filteringOnY =  (min_y < max_y) ? true : false;
  }


  SpeciesSelection getSpeciesSelection() const {  return pidSelected;    }
  ChargeSelection  getChargeSelection () const {  return chargeSelected; }
  StatusSelection  getStatusSelection () const {  return statusSelected; }
  bool isFilteringOnPt   () const              {  return filteringOnPt;  }
  bool isFilteringOnEta  () const              {  return filteringOnEta; }
  bool isFilteringOnY    () const              {  return filteringOnY;   }
  double getMinPt        () const              {  return min_pt;         }
  double getMaxPt        () const              {  return max_pt;         }
  double getMinEta       () const              {  return min_eta;        }
  double getMaxEta       () const              {  return max_eta;        }
  double getMinY         () const              {  return min_y;          }
  double getMaxY         () const              {  return max_y;          }

  virtual void printProperties(  ostream & os);

  static vector<ParticleFilter*> createHadronFilters(double minPt, double maxPt, double minEta, double maxEta, double minY, double maxY);
  static vector<ParticleFilter*> createChargedHadronFilters(double minPt, double maxPt, double minEta, double maxEta, double minY, double maxY);
  static vector<ParticleFilter*> createPlusMinusHadronFilters(double minPt, double maxPt, double minEta, double maxEta, double minY, double maxY);
  static vector<ParticleFilter*> createMultiplePdgFilters(double minPt, double maxPt, double minEta, double maxEta, double minY, double maxY,vector<int> & pdgAccepted);

protected:

  SpeciesSelection  pidSelected;
  ChargeSelection   chargeSelected;
  StatusSelection   statusSelected;

  //! Toggle switch: whether to filter based on pT
  bool   filteringOnPt;
  //! Toggle switch: whether to filter based on pseudorapidity - eta
  bool   filteringOnEta;
  //! Toggle switch: whether to filter based on rapidity - y
  bool   filteringOnY;
  double min_pt;
  double max_pt;
  double min_eta;
  double max_eta;
  double min_y;
  double max_y;
  vector<int> pdgAccepted;

  ClassDef(ParticleFilter,0)
};

#endif /* WAC_ParticleFilter */
