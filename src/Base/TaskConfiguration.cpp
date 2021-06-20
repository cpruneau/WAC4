// Author: Claude Pruneau   09/25/2019

/*************************************************************************
 * Copyright (C) 2019, Claude Pruneau.                                   *
 * All rights reserved.                                                  *
 * Based on the ROOT package and environment                             *
 *                                                                       *
 * For the licensing terms see LICENSE.                                  *
 *************************************************************************/
/**
 \class GenericEventAnalyzer
 \ingroup WAC

 Utility class used to define graph parameters
 */

#include "TaskConfiguration.hpp"
ClassImp(TaskConfiguration);


TaskConfiguration::TaskConfiguration()
:
name("TaskConfiguration"),
useParticles    (false),
useEventStream0 (false),
useEventStream1 (false),
useEventStream2 (false),
useEventStream3 (false),
loadHistograms  (false),
createHistograms(false),
scaleHistograms (false),
calculateDerivedHistograms(false),
saveHistograms  (false),
resetHistograms (false),
clearHistograms (false),
forceHistogramsRewrite(false),
subsampleAnalysis(false),
partialSave(false),
inputPath("./"),
outputPath("./"),

dataInputUsed(false),
dataInputPath(""),
dataInputFileName(""),
dataInputTreeName("tree"),
dataInputFileMinIndex(0),
dataInputFileMaxIndex(1),

dataOutputUsed(false),
dataOutputPath(""),
dataOutputFileName(""),
dataOutputTreeName("tree"),

dataConversionToWac(true),

dataSourceName(""),
collisionSystemName(""),
collisionEnergyName(""),
triggerName(""),
taskTypeName(""),
eventFilterName(""),
particleFilterName(""),
otherName(""),
rootInputFileName(""),
rootOuputFileName(""),
dataSourceTitle(""),
collisionSystemTitle(""),
collisionEnergyTitle(""),
triggerTitle(""),
taskTypeTitle(""),
eventFilterTitle(""),
particleFilterTitle("")
{
  /* no ops */
}

TaskConfiguration::TaskConfiguration(const TaskConfiguration & source)
:
name(             source.name),
useParticles(     source.useParticles),
useEventStream0(  source.useEventStream0),
useEventStream1(  source.useEventStream1),
useEventStream2(  source.useEventStream2),
useEventStream3(  source.useEventStream3),
loadHistograms(   source.loadHistograms ),
createHistograms( source.createHistograms ),
scaleHistograms( source.scaleHistograms ),
calculateDerivedHistograms( source.calculateDerivedHistograms ),
saveHistograms( source.saveHistograms ),
resetHistograms( source.resetHistograms ),
clearHistograms( source.clearHistograms ),
forceHistogramsRewrite( source.forceHistogramsRewrite ),
subsampleAnalysis( source.subsampleAnalysis ),
partialSave( source.partialSave ),
inputPath( source.inputPath ),
outputPath( source.outputPath ),

dataInputUsed( source.dataInputUsed ),
dataInputPath( source.dataInputPath ),
dataInputFileName( source.dataInputFileName ),
dataInputTreeName( source.dataInputTreeName),
dataInputFileMinIndex( source.dataInputFileMinIndex ),
dataInputFileMaxIndex( source.dataInputFileMaxIndex ),

dataOutputUsed(source.dataOutputUsed),
dataOutputPath(source.dataOutputPath),
dataOutputFileName(source.dataOutputFileName),
dataOutputTreeName(source.dataOutputTreeName),

dataConversionToWac(source.dataConversionToWac),

dataSourceName( source.dataSourceName ),
collisionSystemName( source.collisionSystemName ),
collisionEnergyName( source.collisionEnergyName ),
triggerName( source.triggerName ),
taskTypeName( source.taskTypeName ),
eventFilterName( source.eventFilterName ),
particleFilterName( source.particleFilterName ),
otherName( source.otherName ),
rootInputFileName( source.rootInputFileName ),
rootOuputFileName( source.rootOuputFileName ),
dataSourceTitle( source.dataSourceTitle ),
collisionSystemTitle( source.collisionSystemTitle ),
collisionEnergyTitle( source.collisionEnergyTitle ),
triggerTitle( source.triggerTitle ),
taskTypeTitle( source.taskTypeTitle ),
eventFilterTitle( source.eventFilterTitle ),
particleFilterTitle( source.particleFilterTitle )
{
  /* no ops */
}

TaskConfiguration & TaskConfiguration::operator=(const TaskConfiguration & source)
{
  // avoid copying onto itself...
  if (this!=&source)
    {
    name                   = source.name;
    useParticles           = source.useParticles;
    useEventStream0        = source.useEventStream0;
    useEventStream1        = source.useEventStream1;
    useEventStream2        = source.useEventStream2;
    useEventStream3        = source.useEventStream3;
    loadHistograms         = source.loadHistograms;
    createHistograms       = source.createHistograms;
    scaleHistograms        = source.scaleHistograms;
    calculateDerivedHistograms = source.calculateDerivedHistograms;
    saveHistograms         = source.saveHistograms;
    resetHistograms        = source.resetHistograms;
    clearHistograms        = source.clearHistograms;
    forceHistogramsRewrite = source.forceHistogramsRewrite;
    subsampleAnalysis      = source.subsampleAnalysis;
    partialSave            = source.partialSave;

    inputPath              = source.inputPath;
    outputPath             = source.outputPath;
    dataInputPath          = source.dataInputPath;
    dataInputFileName      = source.dataInputFileName;
    dataInputTreeName      = source.dataInputTreeName;
    dataInputFileMinIndex  = source.dataInputFileMinIndex;
    dataInputFileMaxIndex  = source.dataInputFileMaxIndex;
    dataSourceName         = source.dataSourceName;
    collisionSystemName    = source.collisionSystemName;
    collisionEnergyName    = source.collisionEnergyName;
    triggerName            = source.triggerName;
    taskTypeName           = source.taskTypeName;
    eventFilterName        = source.eventFilterName;
    particleFilterName     = source.particleFilterName;
    otherName              = source.otherName;
    rootInputFileName      = source.rootInputFileName;
    rootOuputFileName      = source.rootOuputFileName;
    dataSourceTitle        = source.dataSourceTitle;
    collisionSystemTitle   =  source.collisionSystemTitle;
    collisionEnergyTitle   =  source.collisionEnergyTitle;
    triggerTitle           =  source.triggerTitle;
    taskTypeTitle          =  source.taskTypeTitle;
    eventFilterTitle       =  source.eventFilterTitle;
    particleFilterTitle    =  source.particleFilterTitle;
    }
  return *this;
}


void TaskConfiguration::printConfiguration(ostream & os)
{
  os
  << " ------------------------------------------------------------" << endl
  << "                   outputPath: " << name << endl
  << "                 useParticles: " << useParticles << endl
  << "              useEventStream0: " << useEventStream0 << endl
  << "              useEventStream1: " << useEventStream1 << endl
  << "              useEventStream2: " << useEventStream2 << endl
  << "              useEventStream3: " << useEventStream3 << endl
  << "               loadHistograms: " << loadHistograms << endl
  << "             createHistograms: " << createHistograms << endl
  << "              scaleHistograms: " << scaleHistograms << endl
  << "   calculateDerivedHistograms: " << calculateDerivedHistograms << endl
  << "               saveHistograms: " << saveHistograms << endl
  << "              resetHistograms: " << resetHistograms << endl
  << "              clearHistograms: " << clearHistograms << endl
  << "       forceHistogramsRewrite: " << forceHistogramsRewrite << endl
  << "            subsampleAnalysis: " << subsampleAnalysis << endl
  << "                  partialSave: " << partialSave  << endl
  << "                    inputPath: " << inputPath << endl
  << "            rootInputFileName: " << rootInputFileName << endl
  << "                   outputPath: " << outputPath << endl
  << "            rootOuputFileName: " << rootOuputFileName << endl
  << " ------------------------------------------------------------" << endl;
}

TString TaskConfiguration::getInputRootFileName()
{
  TString inputName = inputPath;
  inputName += rootInputFileName;
  return inputName;
}

TString TaskConfiguration::getOutputRootFileName()
{
  TString outputName = outputPath;
  outputName += rootOuputFileName;
  return outputName;
}

TString TaskConfiguration::composeFileName(int option)
{
  TString name;
  switch (option)
    {
      case 0:
      name = dataSourceName;      // e.g., Pythia, AMPT, ALICE, etc
      name += "_";
      name += collisionSystemName; // e.g., pp, Pb-Pb, etc
      name += "_";
      name += collisionEnergyName; // e.g., 14 TeV
      name += "_";
      name += triggerName;         // e.g., Min Bias
      break;
      case 1:
      name = dataSourceName;      // e.g., Pythia, AMPT, ALICE, etc
      name += "_";
      name += collisionSystemName; // e.g., pp, Pb-Pb, etc
      name += "_";
      name += collisionEnergyName; // e.g., 14 TeV
      name += "_";
      name += triggerName;         // e.g., Min Bias
      name += "_";
      name += taskTypeName;        // e.g., Single, Pair, NuDyn, etc...
      break;

      case 2:
      name = dataSourceName;      // e.g., Pythia, AMPT, ALICE, etc
      name += "_";
      name += collisionSystemName; // e.g., pp, Pb-Pb, etc
      name += "_";
      name += collisionEnergyName; // e.g., 14 TeV
      name += "_";
      name += triggerName;         // e.g., Min Bias
      name += "_";
      name += taskTypeName;        // e.g., Single, Pair, NuDyn, etc...
      name += "_";
      name += eventFilterName;     // e.g., MultGt2Lt40
      break;

      case 3:
      name = dataSourceName;      // e.g., Pythia, AMPT, ALICE, etc
      name += "_";
      name += collisionSystemName; // e.g., pp, Pb-Pb, etc
      name += "_";
      name += collisionEnergyName; // e.g., 14 TeV
      name += "_";
      name += triggerName;         // e.g., Min Bias
      name += "_";
      name += taskTypeName;        // e.g., Single, Pair, NuDyn, etc...
      name += "_";
      name += eventFilterName;     // e.g., MultGt2Lt40
      name += "_";
      name += particleFilterName;
      break;
    }
  return name;
}

void TaskConfiguration::setRootOutputFileName(int option)
{
  rootOuputFileName = composeFileName(option);
}

TaskConfiguration * TaskConfiguration::defaultConfiguration = nullptr;

TaskConfiguration * TaskConfiguration::getDefaultConfiguration()
{
  if (!defaultConfiguration)
    {
    defaultConfiguration = new TaskConfiguration();
    }
  return defaultConfiguration;
}
