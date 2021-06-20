// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.                                 *
 * All rights reserved.                                                *
 * Based on the ROOT package and environment                           *
 *                                                                     *
 * For the licensing terms see LICENSE.                                *
 **********************************************************************/

#ifndef WAC_TaskConfiguration
#define WAC_TaskConfiguration
#include <ostream>
#include "TObject.h"
#include "TString.h"
using std::ostream;
using std::endl;

////////////////////////////////////////////////////////////////////////////////////////////////////////
// TaskConfiguration of a given object or task
////////////////////////////////////////////////////////////////////////////////////////////////////////
class TaskConfiguration : public TObject 
{
public:

  TaskConfiguration();
  TaskConfiguration(const TaskConfiguration & source);
  virtual ~TaskConfiguration() {}
  TaskConfiguration & operator=(const TaskConfiguration & source);

  inline void setName(const TString & _name) {  name = _name;  }
  virtual void printConfiguration(ostream & os);

  TString composeFileName(int option=0);
  void    setRootOutputFileName(int option=0);
  TString getName() const { return name;  }
  TString getHistoBaseName() const {  return name;  }
  TString getInputRootFileName();
  TString getOutputRootFileName();

  // /////////////////////////////////////////////////
  // Data members
  // /////////////////////////////////////////////////
  TString name;
  bool useParticles;
  bool useEventStream0;
  bool useEventStream1;
  bool useEventStream2;
  bool useEventStream3;
  bool loadHistograms;
  bool createHistograms;
  bool scaleHistograms;
  bool calculateDerivedHistograms;
  bool saveHistograms;
  bool resetHistograms;
  bool clearHistograms;
  bool forceHistogramsRewrite;
  bool subsampleAnalysis;
  bool partialSave;
  TString inputPath;
  TString outputPath;

  bool    dataInputUsed;
  TString dataInputPath;
  TString dataInputFileName;
  TString dataInputTreeName;
  int     dataInputFileMinIndex;
  int     dataInputFileMaxIndex;

  bool    dataOutputUsed;
  TString dataOutputPath;
  TString dataOutputFileName;
  TString dataOutputTreeName;

  bool    dataConversionToWac;


  TString dataSourceName;      // e.g., Pythia, AMPT, ALICE, etc
  TString collisionSystemName; // e.g., pp, PbPb, etc
  TString collisionEnergyName; // e.g., 14TeV
  TString triggerName;         // e.g., MB
  TString taskTypeName;        // e.g., Single, Pair, NuDyn, etc...
  TString eventFilterName;     // e.g., MultGt2Lt40
  TString particleFilterName;  // e.g., HA, HC, HP, PiP, PiM
  TString otherName;           // e.g., Special, WideAccept, etc

  TString rootInputFileName;
  TString rootOuputFileName;

  TString dataSourceTitle;      // e.g., Pythia, AMPT, ALICE, etc
  TString collisionSystemTitle; // e.g., pp, Pb-Pb, etc
  TString collisionEnergyTitle; // e.g., 14 TeV
  TString triggerTitle;         // e.g., Min Bias
  TString taskTypeTitle;        // e.g., Single, Pair, NuDyn, etc...
  TString eventFilterTitle;     // e.g., MultGt2Lt40
  TString particleFilterTitle;  // e.g., HA, HC, HP, PiP, PiM


private:

  static TaskConfiguration * defaultConfiguration;

public:

  static TaskConfiguration * getDefaultConfiguration();

  ClassDef(TaskConfiguration,0)
  
};

#endif /* TaskConfiguration_hpp */
