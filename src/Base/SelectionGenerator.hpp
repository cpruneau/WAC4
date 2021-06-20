// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.                                 *
 * All rights reserved.                                                *
 * Based on the ROOT package and environment                           *
 *                                                                     *
 * For the licensing terms see LICENSE.                                *
 **********************************************************************/

#ifndef WAC_SelectionGenerator
#define WAC_SelectionGenerator

//////////////////////////////////////////////////////////////////////////
// SelectionGenerator
//
// Produces a random integer based
// on a probability distribution.
//
////////////////////////////////////////////////////////////////////////
#include <vector>
#include "TRandom.h"

class SelectionGenerator
{
public:
  SelectionGenerator(std::vector<double> probabilities);
  virtual ~SelectionGenerator(){}
  virtual int generate();
  int nPartitions() const
  {
    return cProbs.size();
  }

protected:

  std::vector<double> cProbs;
  ClassDef(SelectionGenerator,0)
};

#endif /* SelectionGenerator_hpp */
