
#include "TString.h"
#include "TRandom.h"
//#include "TFile.h"
//#include "AnalysisConfiguration.hpp"
//#include "NuDynHistos.hpp"
//#include "NuDynDerivedHistos.hpp"
//#include "CanvasConfiguration.hpp"
//#include "HistogramCollection.hpp"
//#include "GraphConfiguration.hpp"
//#include "CanvasConfiguration.hpp"
//#include "TRint.h"


//R__LOAD_LIBRARY(/Users/claudeapruneau/opt/WAC/lib/libBase.dylib)

int ParticleSubsampleAnalysis()
{
  cout << "ParticleSubsampleAnalysis() Starting" << endl;

  TString includesPath = getenv("WAC_SRC");
  includesPath += "/Base/";
  gSystem->Load(includesPath+"MessageLogger.hpp");
  gSystem->Load(includesPath+"Task.hpp");
  gSystem->Load(includesPath+"TaskConfiguration.hpp");
  gSystem->Load(includesPath+"HistogramCollection.hpp");
  gSystem->Load(includesPath+"Histograms.hpp");
  gSystem->Load(includesPath+"SubSampleStatCalculator.hpp");
  gSystem->Load("libBase.dylib");

  MessageLogger::LogLevel  debugLevel = MessageLogger::Info;

  TString inputPathName  = getenv("WAC_INPUT_PATH");
  TString outputPathName = getenv("WAC_OUTPUT_PATH");

  inputPathName  += "/PYTHIA/7TEV/";
  outputPathName += "/PYTHIA/7TEV/";
  gSystem->mkdir(outputPathName,1);

  //TString inputFileNameBase = "PYTHIA_softOnHardOff_Singles_LUMP_Wide_MB_";
  //TString outputFileName = "PYTHIA_softOnHardOff_Singles_LUMP_Wide_MB_Sum0_99";
  TString inputFileBaseName = "PYTHIA_pp_7TeV_softOnHardOff_Global_";
  TString outputFileName    = "PYTHIA_pp_7TeV_softOnHardOff_Global_Sum";
  int nFiles = 10;
  vector<TString *> fileNames;
  cout << "      input Path Name: " << inputPathName << endl;
  cout << " input File Base Name: " << inputFileBaseName << endl;
  cout << "     output Path Name: " << outputPathName << endl;
  cout << "     output File Name: " << outputFileName << endl;
  for (int iFile=0; iFile<nFiles; iFile++)
  {
  TString  fName = inputFileBaseName;
  fName += iFile;
  fileNames.push_back(new TString(fName));
  cout << "   file Name [" << iFile << "]: " << fName << endl;
  }
  SubSampleStatCalculator * calculator = new SubSampleStatCalculator(inputPathName,fileNames,outputPathName,outputFileName,debugLevel);
  calculator->execute();

  return 0;

}
