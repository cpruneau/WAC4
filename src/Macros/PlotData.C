
#include "TString.h"
#include "TRandom.h"
#include <vector>

int openRootInputFiles(TString & inputPathName,
                       vector<TString>  & fileNames,
                       vector<TFile*>   & inputFiles)
{
  cout << "<I> openRootInputFiles(...) Opening files..." << endl;
  TString fileName;
  for (unsigned int iFile=0;iFile<fileNames.size();iFile++)
    {
    fileName = inputPathName+fileNames[iFile];
    TFile * file = new TFile(fileName,"READ");
    if (file)
      {
      cout << "<I> openRootInputFiles() Successfully opened: " << fileName << endl;
      inputFiles.push_back(file);
      //file->ls();
      }
    else
      {
      cout << "<E> openRootInputFiles() Unable to open: " << fileName << endl;
      return 1;
      }
    }
  return 0;
}

int PlotData()
{
  TString includesPath = getenv("WAC_SRC");
  includesPath += "/Base/";
  gSystem->Load(includesPath+"CanvasConfiguration.hpp");
  gSystem->Load(includesPath+"GraphConfiguration.hpp");
  gSystem->Load(includesPath+"Plotter.hpp");
  gSystem->Load(includesPath+"MessageLogger.hpp");
  gSystem->Load("libBase.dylib");

  MessageLogger::LogLevel logLevel = MessageLogger::Info;
  CanvasConfiguration * landscapeLinear     = new CanvasConfiguration(CanvasConfiguration::LandscapeWide,CanvasConfiguration::Linear);
  CanvasConfiguration * landscapeLogY       = new CanvasConfiguration(CanvasConfiguration::LandscapeWide,CanvasConfiguration::LogY);
  vector<GraphConfiguration*>  graphConfigurations = GraphConfiguration::createConfigurationPalette(10,1);

  bool printGif = 0;
  bool printPdf = 1;
  bool printSvg = 0;
  bool printC   = 0;
  bool useColor = true;
  TString fileName;
  TString inputPathName     = getenv("WAC_OUTPUT_PATH");
  TString outputPathName    = getenv("WAC_OUTPUT_PATH");
  TString hepInputPathName  = "/Users/claudeapruneau/Documents/GitHub/WAC4/publishedData/Alice/ProtonProton/Eur.Phys.J.C.73-2013-2662/";
  outputPathName = hepInputPathName;
  vector<TString>  hepDataFileNames;
  vector<TFile*>   hepInputFiles;
  vector<DataGraph*> dataGraphs;

  hepDataFileNames.push_back("Eur.Phys.J.C.73-2013-P2662_Table1.root");
  int status = openRootInputFiles(hepInputPathName, hepDataFileNames, hepInputFiles);
  if (status>0) return 1;
  DataGraph * g;

  g = DataGraph::loadGraph("PP900Pt", "p_{T}(GeV)", "1/(2#pi p_{T}) dN/dp_{T}", "0.9 TeV", 0.0, 40.0, 1.0E-9, 1.0E4,
                           hepInputFiles[0], "Table 1","Hist1D_y1","Hist1D_y1_e1","Hist1D_y1_e2","Graph1D_y1");
  if (!g) return 1;
  g->setProperties(*graphConfigurations[0]);
  dataGraphs.push_back(g);

  g = DataGraph::loadGraph("PP2760Pt", "p_{T}(GeV)", "1/(2#pi p_{T}) dN/dp_{T}", "2.76 TeV", 0.0, 40.0, 1.0E-9, 1.0E4,
                           hepInputFiles[0], "Table 1","Hist1D_y2","Hist1D_y2_e1","Hist1D_y2_e2","Graph1D_y2");
  if (!g) return 1;
  g->setProperties(*graphConfigurations[1]);
  dataGraphs.push_back(g);

  g = DataGraph::loadGraph("PP7000Pt", "p_{T}(GeV)", "1/(2#pi p_{T}) dN/dp_{T}", "7.00 TeV", 0.0, 40.0, 1.0E-9, 1.0E4,
                           hepInputFiles[0], "Table 1","Hist1D_y3","Hist1D_y3_e1","Hist1D_y3_e2","Graph1D_y3");
  if (!g) return 1;
  g->setProperties(*graphConfigurations[2]);
  dataGraphs.push_back(g);

  Plotter * plotter = new Plotter("Plotter",MessageLogger::Info);
  plotter->setDefaultOptions(useColor);

  plotter->plot("Alice-PP-Pt", landscapeLogY, "p_{T}(GeV)", 0.0, 100.0,  "1/(2#pi p_{T}) dN/dp_{T}",1.0E-9, 1.0E4,
                dataGraphs, 0.6, 0.5, 0.8, 0.7, 0.035);

  plotter->printAllCanvas(outputPathName, printGif, printPdf, printSvg, printC);
  return 0;
}

