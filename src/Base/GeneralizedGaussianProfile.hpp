// Author: Victor Gonzalez on 02/10/19

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
#ifndef WAC_GeneralizedGaussianProfile
#define WAC_GeneralizedGaussianProfile

// Base class for introducing effects on individual tracks or pairs once generated


#include <TF2.h>
#include "GeneralizedGaussianConfiguration.hpp"
#include "Task.hpp"

class GeneralizedGaussianProfile : public Task
{
public:
  
  GeneralizedGaussianProfile(TaskConfiguration * configuration);
  virtual ~GeneralizedGaussianProfile();
  virtual void initialize();
  virtual void execute();
  TF2  * getProfile() { return profile; }

protected:

  TF2 *profile;

    ClassDef(GeneralizedGaussianProfile,0)
};

#endif /* WAC_GeneralizedGaussianProfile */
