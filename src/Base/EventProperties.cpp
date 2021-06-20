// Author: Claude Pruneau   12/12/16

/*************************************************************************
 * Copyright (C) 2019, Claude Pruneau.                                   *
 * All rights reserved.                                                  *
 * Based on the ROOT package and environment                             *
 *                                                                       *
 * For the licensing terms see LICENSE.                                  *
 *************************************************************************/
/**
 \class CanvasConfiguration
 \ingroup WAC

 Utility class used to define the parameters of a root canvas
 */
#include "EventProperties.hpp"
ClassImp(EventProperties);


EventProperties::EventProperties()
:
zProjectile(0),
aProjectile(0),
nPartProjectile(0),
zTarget(0),
aTarget(0),
nPartTarget(0),
nPartTotal(0),
nBinaryTotal(0),
impactParameter(0),
centrality(0),
multiplicity(0),
other(0),
nFilters(10),
nFiltered(10,0.0),
eFiltered(10,0.0),
qFiltered(10,0.0),
sFiltered(10,0.0),
bFiltered(10,0.0)
{
// no ops
}

EventProperties::EventProperties(const EventProperties & source)
:
zProjectile(source.zProjectile),
aProjectile(source.aProjectile),
nPartProjectile(source.nPartProjectile),
zTarget(source.zTarget),
aTarget(source.aTarget),
nPartTarget(source.nPartTarget),
nPartTotal(source.nPartTotal),
nBinaryTotal(source.nBinaryTotal),
impactParameter(source.impactParameter),
centrality(source.centrality),
multiplicity(source.multiplicity),
other(source.other),
nFilters(source.nFilters),
nFiltered(source.nFiltered),
eFiltered(source.eFiltered),
qFiltered(source.qFiltered),
sFiltered(source.sFiltered),
bFiltered(source.bFiltered)
{
// no ops
}

EventProperties & EventProperties::operator=(const EventProperties & source)
{
  if (this!=&source)
    {
    zProjectile      =  source.zProjectile;
    aProjectile      =  source.aProjectile;
    nPartProjectile  =  source.nPartProjectile;
    zTarget          =  source.zTarget;
    aTarget          =  source.aTarget;
    nPartTarget      =  source.nPartTarget;
    nPartTotal       =  source.nPartTotal;
    nBinaryTotal     =  source.nBinaryTotal;
    impactParameter  =  source.impactParameter;
    centrality       =  source.centrality;
    multiplicity     =  source.multiplicity;
    other      =  source.other;
    nFilters   =  source.nFilters;
    nFiltered  =  source.nFiltered;
    eFiltered  =  source.eFiltered;
    qFiltered  =  source.qFiltered;
    sFiltered  =  source.sFiltered;
    bFiltered  =  source.bFiltered;
    }
  return *this;
}


EventProperties::~EventProperties()
{}

void EventProperties::clear()
{
  zProjectile     = 0;
  aProjectile     = 0;
  nPartProjectile = 0;
  zTarget         = 0;
  aTarget         = 0;
  nPartTarget     = 0;
  nPartTotal      = 0;
  nBinaryTotal    = 0;
  impactParameter = 0;
  centrality      = 0;
  multiplicity    = 0;
  other           = 0;
  nFilters        = 10;
  double zero     = 0.0;
  nFiltered.clear(); nFiltered.assign(nFilters,zero);
  eFiltered.clear(); eFiltered.assign(nFilters,zero);
  qFiltered.clear(); qFiltered.assign(nFilters,zero);
  sFiltered.clear(); sFiltered.assign(nFilters,zero);
  bFiltered.clear(); bFiltered.assign(nFilters,zero);
}

void EventProperties::reset()
{
  zProjectile     = 0;
  aProjectile     = 0;
  nPartProjectile = 0;
  zTarget         = 0;
  aTarget         = 0;
  nPartTarget     = 0;
  nPartTotal      = 0;
  nBinaryTotal    = 0;
  impactParameter = 0;
  centrality      = 0;
  multiplicity    = 0;
  other           = 0;
  nFilters        = 10;
  double zero     = 0.0;
  for (int k=0; k<nFilters; k++)
  {
  nFiltered[k] = zero;
  eFiltered[k] = zero;
  qFiltered[k] = zero;
  sFiltered[k] = zero;
  bFiltered[k] = zero;
  }
}

void EventProperties::fill(vector<double> & n,  // number of particles accepted by filter #i
                           vector<double> & e,  // total energy of particles accepted by filter #i
                           vector<double> & q,  // total charge of particles accepted by filter #i
                           vector<double> & s,  // total strangeness of particles accepted by filter #i
                           vector<double> & b)
{
  int nFilters = nFiltered.size();
  for (int iFilter=0; iFilter<nFilters; iFilter++)
    {
    nFiltered[iFilter] += n[iFilter];
    eFiltered[iFilter] += e[iFilter];
    qFiltered[iFilter] += q[iFilter];;
    sFiltered[iFilter] += s[iFilter];;
    bFiltered[iFilter] += b[iFilter];;
    }
}


void EventProperties::printProperties(ostream & output)
{
  output << "===============================================" << endl;
  output << "Event Properties" << endl;
  output << "===============================================" << endl;
  output << "          zProjectile : " << zProjectile << endl;     // atomic number projectile
  output << "          aProjectile : " << aProjectile << endl;     // mass number projectile
  output << "      nPartProjectile : " << nPartProjectile << endl; // number of participants  projectile
  output << "              zTarget : " << zTarget << endl;         // atomic number target
  output << "              aTarget : " << aTarget << endl;         // mass number target
  output << "          nPartTarget : " << nPartTarget << endl;     // number of participants  target
  output << "           nPartTotal : " << nPartTotal << endl;      // total number of participants
  output << "         nBinaryTotal : " << nBinaryTotal << endl;    // total number of binary collisions
  output << "      impactParameter : " << impactParameter << endl; // nucleus-nucleus center distance in fm
  output << "           centrality : " << centrality << endl;      // fraction cross section value
  output << "         multiplicity : " << multiplicity << endl;    // nominal multiplicity in the reference range
  output << "                other : " << other << endl;           // other value of interest
  nFilters = nFiltered.size();
  output << "             nFilters : " << nFilters << endl;
  for (int k=0;k<nFilters;k++)
  {
  output 
    << setw(5) << k << " "
  << scientific << setw(10)<< setprecision(5) << nFiltered[k]
  << scientific << setw(10)<< setprecision(5) << eFiltered[k]
  << scientific << setw(10)<< setprecision(5) << qFiltered[k]
  << scientific << setw(10)<< setprecision(5) << sFiltered[k]
  << scientific << setw(10)<< setprecision(5) << bFiltered[k] << endl;
  }
}
