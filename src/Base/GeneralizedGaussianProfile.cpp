// Author: Victor Gonzalez   02/10/19

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau. Victor Gonzalez.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
#include "GeneralizedGaussianProfile.hpp"
ClassImp(GeneralizedGaussianProfile);
#include <TVector2.h>
#include <TRandom.h>
#include "Event.hpp"

GeneralizedGaussianProfile::GeneralizedGaussianProfile(TaskConfiguration * configuration)
:
Task(),
profile(nullptr)
{
  configuration->useParticles    = true;
  configuration->useEventStream0 = true;
  setConfiguration(configuration);
}

GeneralizedGaussianProfile::~GeneralizedGaussianProfile()
{
  if (profile != nullptr) delete profile;
}

// initialize this analyzer
void GeneralizedGaussianProfile::initialize()
{
  if (reportStart("GeneralizedGaussianProfile",getName(),"initialize()"))
    ;
  GeneralizedGaussianConfiguration & conf = * (GeneralizedGaussianConfiguration *) getConfiguration();
  profile = new TF2("2DGenGauss","[0]*[1]*[2]/4.0/[3]/[4]/TMath::Gamma(1.0/[1])/TMath::Gamma(1.0/[2])*"
                    "TMath::Exp(-1.0*(TMath::Power(TMath::Abs(x/[3]),[1])+TMath::Power(TMath::Abs(y/[4]),[2])))");
  profile->SetParameters(conf.amplitude,conf.gammaeta,conf.gammaphi,conf.omegaeta,conf.omegaphi);
  profile->SetRange(-4.0,-TMath::PiOver2(),4.0,TMath::PiOver2());
  if (reportEnd("GeneralizedGaussianProfile",getName(),"initialize()"))
    ;
}


// =======================================================================================
// process one event
// =======================================================================================
void GeneralizedGaussianProfile::execute()
{
  Event & event = * eventStreams[0];

  int nParticles = event.getNParticles();
  for (int itrack = 0; itrack < nParticles; itrack++)
    {
    Particle *particle1 = event.getParticleAt(itrack);
    for (int jtrack = itrack+1; jtrack < nParticles; jtrack++)
      {
      Particle *particle2 = event.getParticleAt(jtrack);
      double deltaeta = particle1->getMomentum().Eta() - particle2->getMomentum().Eta();
      double deltaphi = TVector2::Phi_mpi_pi(particle1->getMomentum().Phi() - particle2->getMomentum().Phi());
      double weight; weight= profile->Eval(deltaeta,deltaphi) + 1.0;

      }
    }
}


