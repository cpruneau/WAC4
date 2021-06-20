// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
/**
 \class SubSampleStatCalculator
 \ingroup WAC

 Class defining SubSampleStatCalculator
 */
#include "HistogramCollection.hpp"
#include "SubSampleStatCalculator.hpp"
#include "TParameter.h"

ClassImp(SubSampleStatCalculator);


SubSampleStatCalculator::SubSampleStatCalculator(const TString &   _inputPath,
                                                 const TString &   _inputFileNameBase,
                                                 unsigned int      _nInputFiles,
                                                 const TString &   _outputPath,
                                                 const TString &   _outputFileName,
                                                 MessageLogger::LogLevel debugLevel)
:
Task(),
inputPath(_inputPath),
inputFileNameBase(_inputFileNameBase),
nInputFiles(_nInputFiles),
outputPath(_outputPath),
outputFileName(_outputFileName),
weight(0),
sumWeights(0)
{
  setReportLevel(debugLevel);
}

void SubSampleStatCalculator::execute()
{
  if (reportInfo("SubSampleStatCalculator",getName(),"execute()"))
    {
    cout << "Starting subsample analysis of nFiles: " << nInputFiles << endl;
    cout << "                           Input path: " << inputPath << endl;
    }
  TString inputFileName;
  TFile * inputFile;
  TFile * firstInputFile;
  sumWeights = 0.0;
  weight = 0.0;

  HistogramCollection * collectionAvg = new HistogramCollection("Sum",getReportLevel());
  HistogramCollection * collection;
  TString parameterName = "EventProcessed";

  for (unsigned int iFile=0; iFile<nInputFiles; iFile++)
  {
  inputFileName = inputFileNameBase;
  inputFileName += "_";
  inputFileName += iFile;

  inputFile = openRootFile(inputPath, inputFileName, "READ");
  if (!isTaskOk()) return;
  weight = readParameter(inputFile,parameterName);
  if (!isTaskOk()) return;
  if (reportDebug ("SubSampleStatCalculator",getName(),"execute()")) cout << "Loading histograms" << endl;
  if (iFile==0)
    {
    firstInputFile = inputFile;
    collectionAvg->loadCollection(inputFile);
    if (reportDebug ("SubSampleStatCalculator",getName(),"execute()")) cout << "First Load completed."  << endl;
    sumWeights = weight;
  }
  else
    {
    collection = new HistogramCollection(inputFileName,getReportLevel());;
    collection->loadCollection(inputFile);
    collectionAvg->squareDifferenceCollection(*collection, sumWeights, weight, (iFile==(nInputFiles-1)) ? nInputFiles : -iFile);
    sumWeights += weight;
    delete collection;
    delete inputFile;
    if (reportDebug ("SubSampleStatCalculator",getName(),"execute()")) cout << "sumWeights : " <<  sumWeights << endl;
    }
  }
  TFile * outputFile = openRootFile(outputPath, outputFileName, "RECREATE");
  if (!isTaskOk()) return;
  collectionAvg->saveHistograms(outputFile);
  delete collectionAvg;
  delete outputFile;
  delete firstInputFile;
  if (reportInfo ("SubSampleStatCalculator",getName(),"execute()")) cout << "Subsample analysis completed." << endl;
}

