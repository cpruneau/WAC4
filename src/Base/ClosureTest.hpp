// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/

#ifndef WAC_ClosureTest
#define WAC_ClosureTest
#include <iostream>
#include "TClass.h"
#include "TH1D.h"
#include "TH1F.h"
#include "TH2D.h"
#include "TH2F.h"
#include "TH3D.h"
#include "TH3F.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TFile.h"
#include "TString.h"
#include "Task.hpp"
using namespace std;

class ClosureTest : public Task
{
public:

  // Ratio      : detectorLevel/generatorLevel
  // Difference : detectorLevel - generatorLevel
  // Relative   : (detectorLevel - generatorLevel)/generatorLevel

  enum ClosureTestMethod    { Ratio, Difference, Relative };


  TString inputPath;
  TString outputPath;
  TString generatorLevelFileName;  // Truth
  TString detectorLevelFileName;   // Measured
  TString closureFileName;         // Comparison
  ClosureTestMethod selectedMethod;

  ClosureTest(const TString &   _inputPath,
              const TString &   _generatorLevel,
              const TString &   _detectorLevel,
              const TString &   _outputPath,
              const TString &   _closureFileName,
              ClosureTestMethod _selectedMethod=Ratio,
              MessageLogger::LogLevel debugLevel=Info);
  virtual ~ClosureTest() {}
  virtual void execute();

  ClassDef(ClosureTest,0)
};

#endif /* WAC_ClosureTest */
