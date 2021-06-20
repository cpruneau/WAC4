
#include "TString.h"
#include "TRandom.h"
//#include "ParticlePlotter.hpp"
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


int PlotGeometry()
{
  TString includesPath = getenv("WAC_SRC");
  includesPath += "/Base/";
  gSystem->Load(includesPath+"CanvasCollection.hpp");
  gSystem->Load(includesPath+"CanvasConfiguration.hpp");
  gSystem->Load(includesPath+"TaskConfiguration.hpp");
  gSystem->Load(includesPath+"EnhancedGraph.hpp");
  gSystem->Load(includesPath+"Factory.hpp");
  gSystem->Load(includesPath+"FunctionCollection.hpp");
  gSystem->Load(includesPath+"GraphConfiguration.hpp");
  gSystem->Load(includesPath+"HistogramCollection.hpp");
  gSystem->Load(includesPath+"Histograms.hpp");
  gSystem->Load("libBase.dylib");

  MessageLogger::LogLevel logLevel = MessageLogger::Info;
  vector<TString>  eventClassNames;
  vector<TString>  eventClassTitles;
  vector<TString>  observableNames;
  vector<TString>  observableTitles;
  vector<TString>  observableTitlesX;
  vector<TString>  observableTitlesY;
  vector<TString>  observableTitlesZ;
  vector<double>  minX;
  vector<double>  maxX;
  vector<double>  minY;
  vector<double>  maxY;
  vector<double>  minZ;
  vector<double>  maxZ;

  vector<TH1*>     histograms;
  vector<TString>  titles;
  vector<Plotter*> plotters;
  vector<GraphConfiguration*> graphConfigurations;
  vector<GraphConfiguration*> graphConfigurations2D;
  TString canvasName;

  bool color = true;
  if (color)
    gStyle->SetPalette(1,0);
  else
    gStyle->SetPalette(7,0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  gStyle->SetOptDate(0);
  gStyle->SetOptTitle(0);
  gStyle->SetPadBorderMode(0);

  CanvasConfiguration * landscapeLinear     = new CanvasConfiguration(CanvasConfiguration::SquareWide,CanvasConfiguration::Linear);
  CanvasConfiguration * landscapeLogY       = new CanvasConfiguration(CanvasConfiguration::LandscapeWide,CanvasConfiguration::LogY);
  CanvasConfiguration * landscapeLogZ       = new CanvasConfiguration(CanvasConfiguration::SquareWide,CanvasConfiguration::LogZ);
  landscapeLogZ->leftMargin  = 0.2;
  landscapeLogZ->rightMargin = 0.1;


  GraphConfiguration * gc2D;
  gc2D = new GraphConfiguration(2);
  gc2D->plotOption = "ZCOL";
  graphConfigurations2D.push_back(gc2D);
  gc2D = new GraphConfiguration(2);
  gc2D->plotOption = "BOX";
  graphConfigurations2D.push_back(gc2D);


  // TString inputPathName  = getenv("WAC_REPOSITORY_PATH");
  TString inputPathName  = getenv("WAC_OUTPUT_PATH");
  TString outputPathName = getenv("WAC_OUTPUT_PATH");

  //   /Users/claudeapruneau/Documents/GitHub/WAC/../WAC-DATA/OutputFiles//CollisionGeometryCGG.root
  TString fileName;
  vector<TString>  fileNames;
  vector<TFile*>   inputFiles;
  fileNames.push_back("CollisionGeometryCGGA.root");
  for (unsigned int iFile=0;iFile<fileNames.size();iFile++)
    {
    fileName = inputPathName+fileNames[iFile];
    TFile * file = new TFile(fileName,"OLD");
    if (file)
      {
      cout << "<I> PlotGeometry() Successfully opened: " << fileName << endl;
      inputFiles.push_back(file);
      //file->ls();
      }
    else
      {
      cout << "<E> PlotGeometry() Unable to open: " << fileName << endl;
      return 1;
      }
    }

  plotters.push_back(new Plotter("Plotter",logLevel));

  unsigned int iEventClass = 0;


  if (false)
    {
    eventClassNames.clear();
    observableNames.clear();
    observableTitles.clear();
    observableTitlesX.clear();
    observableTitlesY.clear();
    observableTitlesZ.clear();
    minX.clear();
    maxX.clear();
    minY.clear();
    maxY.clear();
    minZ.clear();
    maxZ.clear();

    eventClassNames.push_back("CGGA_bGeq0Lt0.5");   eventClassTitles.push_back("0<b<0.5 fm");
    eventClassNames.push_back("CGGA_bGeq0.5Lt1");   eventClassTitles.push_back("0.5<b<1 fm");
    eventClassNames.push_back("CGGA_bGeq1Lt2");     eventClassTitles.push_back("1<b<2 fm");
    eventClassNames.push_back("CGGA_bGeq2Lt3");     eventClassTitles.push_back("2<b<3 fm");
    eventClassNames.push_back("CGGA_bGeq3Lt4");     eventClassTitles.push_back("3<b<4 fm");
    eventClassNames.push_back("CGGA_bGeq4Lt6");     eventClassTitles.push_back("4<b<6 fm");
    eventClassNames.push_back("CGGA_bGeq6Lt8");     eventClassTitles.push_back("6<b<8 fm");
    eventClassNames.push_back("CGGA_bGeq8Lt10");    eventClassTitles.push_back("8<b<10 fm");
    eventClassNames.push_back("CGGA_bGeq10Lt12");   eventClassTitles.push_back("10<b<12 fm");
    eventClassNames.push_back("CGGA_bGeq12Lt16");   eventClassTitles.push_back("12<b<16 fm");
    eventClassNames.push_back("CGGA_bGeq16Lt20");   eventClassTitles.push_back("16<b<20 fm");

    observableNames.push_back("xyDistNucleons");
    observableTitles.push_back("xy_{nn}");
    observableTitlesX.push_back("x (fm)");
    observableTitlesY.push_back("y (fm)");
    observableTitlesZ.push_back("Counts");
    minX.push_back(-20.0);
    maxX.push_back( 20.0);
    minY.push_back(-20.0);
    maxY.push_back( 20.0);
    observableNames.push_back("xyDistInteractions");
    observableTitles.push_back("xy_{int}");
    observableTitlesX.push_back("x (fm)");
    observableTitlesY.push_back("y (fm)");
    observableTitlesZ.push_back("Counts");
    minX.push_back(-20.0);
    maxX.push_back( 20.0);
    minY.push_back(-20.0);
    maxY.push_back( 20.0);

    for (unsigned int iEventClass = 0; iEventClass<eventClassNames.size();iEventClass++)
      {
      histograms.clear();
      canvasName = eventClassNames[iEventClass];
      canvasName += "_xyDistributions";
      for (unsigned int iObservable = 0; iObservable<observableNames.size();iObservable++)
        {
        TString histoName = eventClassNames[iEventClass];
        histoName += "_";
        histoName += observableNames[iObservable];
        cout << "   ======================  Fetching: " << histoName << endl;
        TH1 * h =  (TH1*) inputFiles[0]->Get(histoName);
        if (h)
          {
          histograms.push_back(h);
          titles.push_back(eventClassTitles[iEventClass]);
          }
        else
          {
          cout << "Abort -- Histogram not found: " << histoName << endl;
          return 1;
          }
        }
      plotters[0]->plot(canvasName,landscapeLogZ,graphConfigurations2D,
                        observableTitlesX[0], minX[0], maxX[0],
                        observableTitlesY[0], minY[0], maxY[0],
                        histograms,titles,0.65, 1.0, 0.75, 1.2, 0.045);
      }
    }

  if (true)
    {
    eventClassNames.clear();
    observableNames.clear();
    observableTitles.clear();
    observableTitlesX.clear();
    observableTitlesY.clear();
    observableTitlesZ.clear();
    minX.clear();
    maxX.clear();
    minY.clear();
    maxY.clear();
    minZ.clear();
    maxZ.clear();

    eventClassNames.push_back("CGGA_bGeq0Lt0.5");   eventClassTitles.push_back("0<b<0.5 fm");
    eventClassNames.push_back("CGGA_bGeq0.5Lt1");   eventClassTitles.push_back("0.5<b<1 fm");
    eventClassNames.push_back("CGGA_bGeq1Lt2");     eventClassTitles.push_back("1<b<2 fm");

    eventClassNames.push_back("CGGA_bGeq2Lt3");     eventClassTitles.push_back("2<b<3 fm");
    eventClassNames.push_back("CGGA_bGeq3Lt4");     eventClassTitles.push_back("3<b<4 fm");
    eventClassNames.push_back("CGGA_bGeq4Lt6");     eventClassTitles.push_back("4<b<6 fm");

    eventClassNames.push_back("CGGA_bGeq6Lt8");     eventClassTitles.push_back("6<b<8 fm");
    eventClassNames.push_back("CGGA_bGeq8Lt10");    eventClassTitles.push_back("8<b<10 fm");
    eventClassNames.push_back("CGGA_bGeq10Lt12");   eventClassTitles.push_back("10<b<12 fm");

    eventClassNames.push_back("CGGA_bGeq12Lt16");   eventClassTitles.push_back("12<b<16 fm");

    observableNames.push_back("b");
    observableTitles.push_back("b");
    observableTitlesX.push_back("f (fm)");
    observableTitlesY.push_back("Counts");
    minX.push_back(  0.0);
    maxX.push_back(20.0);
    minY.push_back(  10.0);
    maxY.push_back(10000.0);


    observableNames.push_back("nBinaryVsNPart_Prof");
    observableTitles.push_back("N_{Bin}");
    observableTitlesX.push_back("N_{part}");
    observableTitlesY.push_back("<N_{Bin}>");
    minX.push_back(  0.0);
    maxX.push_back(500.0);
    minY.push_back(  1.0);
    maxY.push_back(10000.0);

    graphConfigurations = GraphConfiguration::createConfigurationPalette(eventClassNames.size(),1);
    for (unsigned int iObservable = 0; iObservable<observableNames.size();iObservable++)
      {
      histograms.clear();
      canvasName = "PbPb_";
      canvasName += observableNames[iObservable];
    for (unsigned int iEventClass = 0; iEventClass<eventClassNames.size();iEventClass++)
      {

        TString histoName = eventClassNames[iEventClass];
        histoName += "_";
        histoName += observableNames[iObservable];
        cout << "   ======================  Fetching: " << histoName << endl;
        TH1 * h =  (TH1*) inputFiles[0]->Get(histoName);
        if (h)
          {
          histograms.push_back(h);
          titles.push_back(eventClassTitles[iEventClass]);
          }
        else
          {
          cout << "Abort -- Histogram not found: " << histoName << endl;
          return 1;
          }
        }
      plotters[0]->plot(canvasName,landscapeLogY,graphConfigurations,
                        observableTitlesX[iObservable], minX[iObservable], maxX[iObservable],
                        observableTitlesY[iObservable], minY[iObservable], maxY[iObservable],
                        histograms,titles,0.5, 0.25, 0.65, 0.55, 0.03);
      }
    }

  if (true)
    {
    eventClassNames.clear();
    observableNames.clear();
    observableTitles.clear();
    observableTitlesX.clear();
    observableTitlesY.clear();
    observableTitlesZ.clear();
    minX.clear();
    maxX.clear();
    minY.clear();
    maxY.clear();
    minZ.clear();
    maxZ.clear();

    eventClassNames.push_back("CGGA_bGeq0Lt0.5");   eventClassTitles.push_back("0<b<0.5 fm");
    eventClassNames.push_back("CGGA_bGeq0.5Lt1");   eventClassTitles.push_back("0.5<b<1 fm");
    eventClassNames.push_back("CGGA_bGeq1Lt2");     eventClassTitles.push_back("1<b<2 fm");

    eventClassNames.push_back("CGGA_bGeq2Lt3");     eventClassTitles.push_back("2<b<3 fm");
    eventClassNames.push_back("CGGA_bGeq3Lt4");     eventClassTitles.push_back("3<b<4 fm");
    eventClassNames.push_back("CGGA_bGeq4Lt6");     eventClassTitles.push_back("4<b<6 fm");

    eventClassNames.push_back("CGGA_bGeq6Lt8");     eventClassTitles.push_back("6<b<8 fm");
    eventClassNames.push_back("CGGA_bGeq8Lt10");    eventClassTitles.push_back("8<b<10 fm");
    eventClassNames.push_back("CGGA_bGeq10Lt12");   eventClassTitles.push_back("10<b<12 fm");

    eventClassNames.push_back("CGGA_bGeq12Lt16");   eventClassTitles.push_back("12<b<16 fm");
 

    observableNames.push_back("nPartVsB_Prof");
    observableTitles.push_back("n_{Part}");
    observableTitlesX.push_back("f (fm)");
    observableTitlesY.push_back("<n_{Part}>");
    minX.push_back(  0.0);
    maxX.push_back(20.0);
    minY.push_back(  1.0);
    maxY.push_back(1000.0);



    observableNames.push_back("nBinaryVsB_Prof");
    observableTitles.push_back("n_{Bin}");
    observableTitlesX.push_back("f (fm)");
    observableTitlesY.push_back("<n_{Bin}>");
    minX.push_back(  0.0);
    maxX.push_back(20.0);
    minY.push_back(  1.0);
    maxY.push_back(10000.0);



    graphConfigurations = GraphConfiguration::createConfigurationPalette(eventClassNames.size(),1);
    for (unsigned int iObservable = 0; iObservable<observableNames.size();iObservable++)
      {
      histograms.clear();
      canvasName = "PbPb_";
      canvasName += observableNames[iObservable];
    for (unsigned int iEventClass = 0; iEventClass<eventClassNames.size();iEventClass++)
      {

        TString histoName = eventClassNames[iEventClass];
        histoName += "_";
        histoName += observableNames[iObservable];
        cout << "   ======================  Fetching: " << histoName << endl;
        TH1 * h =  (TH1*) inputFiles[0]->Get(histoName);
        if (h)
          {
          histograms.push_back(h);
          titles.push_back(eventClassTitles[iEventClass]);
          }
        else
          {
          cout << "Abort -- Histogram not found: " << histoName << endl;
          return 1;
          }
        }
      plotters[0]->plot(canvasName,landscapeLogY,graphConfigurations,
                        observableTitlesX[iObservable], minX[iObservable], maxX[iObservable],
                        observableTitlesY[iObservable], minY[iObservable], maxY[iObservable],
                        histograms,titles,0.25, 0.25, 0.4, 0.55, 0.03);
      }
    }

  if (true)
    {
    eventClassNames.clear();
    observableNames.clear();
    observableTitles.clear();
    observableTitlesX.clear();
    observableTitlesY.clear();
    observableTitlesZ.clear();
    minX.clear();
    maxX.clear();
    minY.clear();
    maxY.clear();
    minZ.clear();
    maxZ.clear();

    eventClassNames.push_back("CGGA_bGeq0Lt0.5");   eventClassTitles.push_back("0<b<0.5 fm");
    eventClassNames.push_back("CGGA_bGeq0.5Lt1");   eventClassTitles.push_back("0.5<b<1 fm");
    eventClassNames.push_back("CGGA_bGeq1Lt2");     eventClassTitles.push_back("1<b<2 fm");

    eventClassNames.push_back("CGGA_bGeq2Lt3");     eventClassTitles.push_back("2<b<3 fm");
    eventClassNames.push_back("CGGA_bGeq3Lt4");     eventClassTitles.push_back("3<b<4 fm");
    eventClassNames.push_back("CGGA_bGeq4Lt6");     eventClassTitles.push_back("4<b<6 fm");

    eventClassNames.push_back("CGGA_bGeq6Lt8");     eventClassTitles.push_back("6<b<8 fm");
    eventClassNames.push_back("CGGA_bGeq8Lt10");    eventClassTitles.push_back("8<b<10 fm");
    eventClassNames.push_back("CGGA_bGeq10Lt12");   eventClassTitles.push_back("10<b<12 fm");

    eventClassNames.push_back("CGGA_bGeq12Lt16");   eventClassTitles.push_back("12<b<16 fm");

    observableNames.push_back("nPart");
    observableTitles.push_back("N_{part}");
    observableTitlesX.push_back("N_{part}");
    observableTitlesY.push_back("Counts");
    minX.push_back(  0.0);
    maxX.push_back(500.0);
    minY.push_back(  1.0);
    maxY.push_back(400000.0);

    observableNames.push_back("nBinary");
    observableTitles.push_back("N_{Bin}");
    observableTitlesX.push_back("N_{Bin}");
    observableTitlesY.push_back("Counts");
    minX.push_back(  0.0);
    maxX.push_back(2500.0);
    minY.push_back(  1.0);
    maxY.push_back(100000.0);

    graphConfigurations = GraphConfiguration::createConfigurationPalette(eventClassNames.size(),1);
    for (unsigned int iObservable = 0; iObservable<observableNames.size();iObservable++)
      {
      histograms.clear();
      canvasName = "PbPb_";
      canvasName += observableNames[iObservable];
    for (unsigned int iEventClass = 0; iEventClass<eventClassNames.size();iEventClass++)
      {

        TString histoName = eventClassNames[iEventClass];
        histoName += "_";
        histoName += observableNames[iObservable];
        cout << "   ======================  Fetching: " << histoName << endl;
        TH1 * h =  (TH1*) inputFiles[0]->Get(histoName);
        if (h)
          {
          histograms.push_back(h);
          titles.push_back(eventClassTitles[iEventClass]);
          }
        else
          {
          cout << "Abort -- Histogram not found: " << histoName << endl;
          return 1;
          }
        }
      plotters[0]->plot(canvasName,landscapeLogY,graphConfigurations,
                        observableTitlesX[iObservable], minX[iObservable], maxX[iObservable],
                        observableTitlesY[iObservable], minY[iObservable], maxY[iObservable],
                        histograms,titles,0.5, 0.55, 0.65, 0.85, 0.03);
      }
    }


  plotters[0]->printAllCanvas(outputPathName);

  return 0;
}
