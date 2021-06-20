// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/

#ifndef WAC_PartitionCalculator
#define WAC_PartitionCalculator
#include <iostream>
#include "TClass.h"
#include "TH1.h"
#include "TFile.h"
#include "TString.h"
using namespace std;

class PartitionCalculator
{
public:

  PartitionCalculator();
  virtual ~PartitionCalculator() {}
  virtual void computePartitions(TH1* h, vector<double>& fractions, vector<double>& boundaries);

  ClassDef(PartitionCalculator,0)
};

#endif /* WAC_PartitionCalculator */
