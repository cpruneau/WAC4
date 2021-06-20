#ifndef WAC_ParticleType
#define WAC_ParticleType
#include <iostream>
#include <iomanip>
#include <vector>
#include "TString.h"
#include "SelectionGenerator.hpp"
#include "ParticleDecayMode.hpp"

using namespace std;
class ParticleDecayMode;

//statistics = 1;  // Fermi-Dirac
//statistics = -1;  // Bose-Einstein

class ParticleType
{
private:

  TString name;       // ParticleType name: e.g., pip
  TString title;      // ParticleType title: e.g., #pi^{+}
  int privateCode;    // Code used in this package only (convenience)
  int pdgCode;        // Monte-Carlo number according PDG
  double mass;        // ParticleType mass (GeV)
  double width;       // decay width
  int gSpin;          // spin degeneracy
  int gIsospin;       // isospin degeneracy
  int baryon;         // baryon number
  int strange;        // strangeness
  int charm;          // charmness
  int bottom;         // bottomness
  int leptonElectron; // Electron lepton number
  int leptonMuon;     // Muon lepton number
  int leptonTau;      // Tau lepton number
  int charge;         // charge
  bool stable;        // defines whether this ParticleType is stable
  bool weakStable;    // particle is weakly stable (i.e., decays by weak-decay)
  int statistics;     // Bosons or Fermions
  std::vector<ParticleDecayMode> decayModes;
  SelectionGenerator * decayRndmSelector;

public:
  ParticleType();
  ParticleType(int monval_in, TString & name_in, TString & title_in, double mass_in,
               double width_in, int gSpin_in, int baryon_in, int strange_in,
               int charm_in, int bottom_in, int gIsospin_in, int charge_in);
  ParticleType(const ParticleType & source);
  virtual ~ParticleType();
  ParticleType & operator=(const ParticleType & source);


  void setupDecayGenerator();
  void generateDecay(int&n,int * pid);

  TString getName()     { return name;}
  TString getTitle()    { return title;}
  int    getAntiParticlePdgCode() const;
  int    getPdgCode() const        { return pdgCode;}
  int    getPrivateCode() const    { return privateCode;}
  double getMass() const           { return mass;  } // GeV
  double getWidth() const          { return width; } // GeV
  double getLifeTime() const       { return (width>0.0) ? (6.582E-25/width) : 1.0E37; }

  int    getBaryon() const         { return baryon;}
  int    getCharge() const         { return charge;}
  int    getStrange() const        { return strange;}
  int    getCharm() const          { return charm;}
  int    getBottom() const         { return bottom;}
  int    getLeptonElectron() const { return leptonElectron;}
  int    getLeptonMuon() const     { return leptonMuon;}
  int    getLeptonTau() const      { return leptonTau;}
  int    getSpinFactor() const     { return gSpin;}
  int    getIsoSpinFactor() const  { return gIsospin;      }
  int    getStatistics() const     { return statistics; }


//
  void setName(const TString & _name)  { name    = _name;}
  void setTitle(const TString & _title){ title   = _title;}
  void setPdgCode(int value)           { pdgCode = value; }
  void setPrivateCode(int value)       { privateCode = value;}
  void setMass(double value)           { mass    = value;  } // GeV
  void setWidth(double value)          { width   = value; } // GeV
  void setBaryon(int value)            { baryon  = value;}
  void setCharge(int value)            { charge  = value;}
  void setStrange(int value)           { strange = value;}
  void setCharm(int value)             { charm   = value;}
  void setBottom(int value)            { bottom  = value;}
  void setLeptonElectron(int value)    { leptonElectron = value;}
  void setLeptonMuon(int value)        { leptonMuon   = value;}
  void setLeptonTau(int value)         { leptonTau    = value;}
  void setSpinFactor(double value)     { gSpin        = value;}
  void setIsoSpinFactor(double value)  { gIsospin     = value;}
  void setStatistics(int value)        { statistics   = value;}

  bool     isFermion() const         { return statistics==1; }
  bool     isBoson() const           { return statistics==-1; }
  bool     isGluon() const           { return pdgCode==21; }
  bool     isPhoton() const          { return pdgCode==22; }
  bool     isZBoson() const          { return pdgCode==23; }
  bool     isWBoson() const          { return pdgCode==24; }
  bool     isHiggsBoson() const      { return pdgCode==25; }
  bool     isGaugeBoson() const      { return pdgCode==21 || pdgCode==22 || pdgCode==23 || pdgCode==24; }
  bool     isLepton() const          { return leptonElectron!=0 || leptonMuon!=0 || leptonTau!=0;       }
  bool     isElectron() const        { return pdgCode==11 || pdgCode==-11;                              }
  bool     isMuon() const            { return pdgCode==13 || pdgCode==-13;                              }
  bool     isTau() const             { return pdgCode==15 || pdgCode==-15;                              }
  bool     isMeson() const           { return (statistics==-1);                           }
  bool     isPion() const            { return pdgCode ==211 || pdgCode==111 || pdgCode==-211;     }
  bool     isKaon() const            { return pdgCode ==321 || pdgCode==-321 || pdgCode==311 || pdgCode==-311; }
  bool     isCharged() const         { return charge!=0;     }
  bool     isBaryon() const          { return baryon!=0;     }
  bool     isHadron() const          { return baryon!=0  || ( (statistics==-1)&&(TMath::Abs(pdgCode)>40) );  }
  bool     isStrange() const         { return strange!=0;    }
  bool     isCharm() const           { return charm!=0;      }
  bool     isBottom() const          { return bottom!=0;     }
  bool     isQuark() const           { return pdgCode==1 || pdgCode==2 || pdgCode==3 || pdgCode==4 || pdgCode==5 || pdgCode==6; }
  bool     isProton() const          { return pdgCode==2212;  }
  bool     isNeutron() const         { return pdgCode==2112;  }
  bool     isNucleon() const         { return pdgCode==2212 || pdgCode==2112;  }
  bool     isAntiProton() const      { return pdgCode==-2212;  }
  bool     isAntiNeutron() const     { return pdgCode==-2112;  }
  bool     isAntiNucleon() const     { return pdgCode==-2212 || pdgCode==-2112;  }
  bool     isDecay() const           { return pdgCode==1000001;  }
  bool     isInteraction() const     { return pdgCode==1000010;  }
  bool     isPPInteraction() const   { return pdgCode==1000011;  }
  bool     isPNInteraction() const   { return pdgCode==1000012;  }
  bool     isNNInteraction() const   { return pdgCode==1000013;  }

  int    getNDecayModes() const    { return decayModes.size();   }
  bool   isStable() const          { return stable; }
  bool   isWeakStable() const      { return weakStable;}
  void   addDecayMode(double branchingRatio,
                    std::vector<int> children);
  void   addDecayMode(ParticleDecayMode &decayMode);
  ParticleDecayMode & getDecayMode(int i) { return decayModes[i];}
  ParticleDecayMode & generateDecayMode();
  void   setStable(bool s)     { stable = s; }
  void   setWeakStable(bool s) { weakStable = s; }
  ostream & printProperties(ostream & os);
  ostream & printDecayProperties(ostream & os);

  static  ParticleType * getDecayModeType();
  static  ParticleType * getInteractionType();
  static  ParticleType * getPPInteractionType();
  static  ParticleType * getPNInteractionType();
  static  ParticleType * getNNInteractionType();
  static  ParticleType * getProtonType();
  static  ParticleType * getNeutronType();
  static  ParticleType * getNucleusType();

protected:

  static  ParticleType * protonType;
  static  ParticleType * neutronType;
  static  ParticleType * nucleusType;
  static  ParticleType * decayModeType;
  static  ParticleType * interactionType;
  static  ParticleType * protonProtonInteractionType;
  static  ParticleType * protonNeutronInteractionType;
  static  ParticleType * neutronNeutronInteractionType;



  ClassDef(ParticleType,0)
};

#endif  

