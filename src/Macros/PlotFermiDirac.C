
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

double probDensity(double p, double mass, double temperature, double mu, double stat)
{
  double e     = sqrt(p*p + mass*mass);
  double beta  = 1.0/temperature;
  double arg   = beta*(e-mu);
  double density = p*p/(exp(arg)+stat);
  return density;
}


int PlotFermiDirac()
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
  outputPathName += "FD/";
  vector<DataGraph*> dataGraphs;

  double temperature = 0.180;
  double mass        = 0.938;
  double mu          = 0.0;
  double v;
  double zero   = 0.0;
  int nBinsP    = 1000;
  double minP   = 0.0;
  double maxP   = 6.0;
  double dp     = (maxP-minP)/double(nBinsP);

  TString hName = "BE";
  TH1 * hBE = new TH1D(hName,hName, nBinsP, minP, maxP);
  TH1 * hBEx = new TH1D(hName+"XS",hName+"XS", nBinsP, minP, maxP);
  hName = "FD";
  TH1 * hFD = new TH1D(hName,hName, nBinsP, minP, maxP);
  TH1 * hFDx = new TH1D(hName+"XS",hName+"XS", nBinsP, minP, maxP);
  hName = "MB";
  TH1 * hMB = new TH1D(hName,hName, nBinsP, minP, maxP);
  TH1 * hMBx = new TH1D(hName+"XS",hName+"XS", nBinsP, minP, maxP);

  hName = "BEtoMB";
  TH1 * rBEtoMB  = new TH1D(hName,hName, nBinsP, minP, maxP);
  TH1 * rBEtoMBx = new TH1D(hName+"XS",hName+"XS", nBinsP, minP, maxP);

  hName = "FDtoMB";
  TH1 * rFDtoMB  = new TH1D(hName,hName, nBinsP, minP, maxP);
  TH1 * rFDtoMBx = new TH1D(hName+"XS",hName+"XS", nBinsP, minP, maxP);

  double p = -dp;
  for (int j=0; j<nBinsP; j++)
    {
    //double p, double mass, double temperature, double mu, double stat
    p += dp;
    // BE
    v = probDensity(p,mass,temperature,mu,-1.0);
    hBE->SetBinContent(j,v);
    hBE->SetBinError(j,zero);
    hBEx->SetBinContent(j,v/p/p);
    hBEx->SetBinError(j,zero);

    v = probDensity(p,mass,temperature,mu,+1.0);
    hFD->SetBinContent(j,v);
    hFD->SetBinError(j,zero);
    hFDx->SetBinContent(j,v/p/p);
    hFDx->SetBinError(j,zero);

    v = probDensity(p,mass,temperature,mu,0.0);
    hMB->SetBinContent(j,v);
    hMB->SetBinError(j,zero);
    hMBx->SetBinContent(j,v/p/p);
    hMBx->SetBinError(j,zero);
    }

  rBEtoMB->Divide(hBE,hMB);
  rBEtoMBx->Divide(hBEx,hMBx);

  rFDtoMB->Divide(hFD,hMB);
  rFDtoMBx->Divide(hFDx,hMBx);

  vector<TH1*> histograms;
  vector<TH1*> histogramsXS;
  vector<TH1*> histogramsRatio;
  vector<TH1*> histogramsRatioXS;
  vector<TString> legends;
  vector<TString> legendsRatios;

  histograms.push_back(hBE);
  histograms.push_back(hFD);
  histograms.push_back(hMB);
  histogramsXS.push_back(hBEx);
  histogramsXS.push_back(hFDx);
  histogramsXS.push_back(hMBx);

  histogramsRatio.push_back(rBEtoMB);
  histogramsRatio.push_back(rFDtoMB);
  histogramsRatioXS.push_back(rBEtoMBx);
  histogramsRatioXS.push_back(rFDtoMBx);

  legends.push_back("BE");
  legends.push_back("FD");
  legends.push_back("MB");
  legendsRatios.push_back("BE/MB");
  legendsRatios.push_back("FD/MB");

  Plotter * plotter = new Plotter("Plotter",MessageLogger::Info);
  plotter->setDefaultOptions(useColor);

  TString canvasName = "Protons";
  plotter->plot(canvasName, landscapeLogY, graphConfigurations,
                "p_{T} (GeV)", minP,maxP,
                "dN/dp",1.0E-12, 1.0E-1,
                histograms,legends, 0.6, 0.5, 0.8, 0.7, 0.035);
  plotter->plot(canvasName+"Xs", landscapeLogY, graphConfigurations,
                "p_{T} (GeV)", minP,maxP,
                "1/p^{2} dN/dp",1.0E-12, 1.0E-1,
                histogramsXS,legends, 0.6, 0.5, 0.8, 0.7, 0.035);
  canvasName = "ProtonsRatios";
  plotter->plot(canvasName, landscapeLinear, graphConfigurations,
                "p_{T} (GeV)", minP,1.0,
                "Ratio of dN/dy",0.95, 1.05,
                histogramsRatio,legendsRatios, 0.6, 0.7, 0.8, 0.85, 0.035);
  plotter->plot(canvasName+"Xs", landscapeLinear, graphConfigurations,
                "p_{T} (GeV)", minP,1.0,
                "Ratio",0.96, 1.04,
                histogramsRatioXS,legendsRatios, 0.6, 0.7, 0.8, 0.85, 0.035);

  plotter->printAllCanvas(outputPathName, printGif, printPdf, printSvg, printC);
  return 0;
}

