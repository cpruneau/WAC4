// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
/**
 \class ParticleDigit
 \ingroup WAC

 Class defining ParticleDigit
 */
#include <ostream>
#include "ParticleDigit.hpp"
#include "Factory.hpp"


ClassImp(Factory<ParticleDigit>);

ClassImp(ParticleDigit);

ParticleDigit::ParticleDigit()
:
iY(0),
iEta(0),
iPhi(0),
iPt(0),
pt(0),
e(0)
{  }

ParticleDigit::ParticleDigit(unsigned int _iY,
                             unsigned int _iEta,
                             unsigned int _iPhi,
                             unsigned int _iPt,
                             double       _pt,
                             double       _e)
:
iY(_iY),
iEta(_iEta),
iPhi(_iPhi),
iPt(_iPt),
pt(_pt),
e(_e)
{  }

ParticleDigit::~ParticleDigit()
{  }

ParticleDigit::ParticleDigit(const ParticleDigit& other)
:
iY(other.iY),
iEta(other.iEta),
iPhi(other.iPhi),
iPt(other.iPt),
pt(other.pt),
e(other.e)
{  }

ParticleDigit & ParticleDigit::operator=(const ParticleDigit & other)
{
  if (&other != this)
    {
    iY   = other.iY;
    iEta = other.iEta;
    iPhi = other.iPhi;
    iPt  = other.iPt;
    pt   = other.pt;
    e    = other.e;
    }
  return *this;
}



void ParticleDigit::reset()
{
  iY   = 0;
  iEta = 0;
  iPhi = 0;
  iPt  = 0;
  pt   = 0.0;
  e    = 0.0;
}



//!
//! Print the properties of this ParticleDigit at the given output
//!
void ParticleDigit::printProperties(ostream & output)
{
  output << "-------------------------------------" << endl;
  output << "              iY: " << iY << endl;
  output << "            iEta: " << iEta << endl;
  output << "            iPhi: " << iPhi << endl;
  output << "             iPt: " << iPt << endl;
  output << "              pt: " << pt << endl;
  output << "               e: " << e  << endl;
}


int ParticleDigit::factorySize = 5000;
Factory<ParticleDigit> * ParticleDigit::factory = 0;
Factory<ParticleDigit> * ParticleDigit::getFactory()
{
  if (!factory)
    {
    factory = new Factory<ParticleDigit>();
    factory->initialize(factorySize);
    }
  return factory;
}
