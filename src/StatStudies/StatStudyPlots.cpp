
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

//void plot(int n, TH1** h, GraphConfiguration** g, HistogramCollection * hc, int rebin=1);

//
//// r = f2/f1
//void calculateRatio(double f1,double ef1,double f2,double ef2,double & r,double & er)
//{
//  double ref1, ref2, rer;
//  if (f2!=0)
//    {
//    ref1   = ef1/f1;
//    ref2   = ef2/f2;
//    rer    = ref1*ref1 + ref2*ref2;
//    r = f1/f2;
//    er = r*sqrt(rer);
//    }
//  else
//    {
//    r = 0.0;
//    er = 0.0;
//    }
//}
//
//
//// calculation carried out assuming no correlation between the different factors...
//// r = f2/f1/f1
//void calculateR2(double f1,double ef1,double f2,double ef2,double & r2,double & er2)
//{
//  // calculate ratio and error for r2 = f2/f1/f1
//  double ref1, ref2, rer2;
//  if (f1>0)
//    {
//    ref1 = ef1/f1;
//    ref2   = ef2/f2;
//    rer2   = 4*ref1*ref1 + ref2*ref2;
//    r2 = f2/(f1*f1) - 1.0;
//    er2 = r2*sqrt(rer2);
//    }
//  else
//    {
//    r2 = 0.0;
//    er2 = 0.0;
//    }
//}
//
//// calculation carried out assuming no correlation between the different factors...
//void calculateR2(double f1_1,double ef1_1,double f1_2,double ef1_2,double f2,double ef2,double & r2,double & er2)
//{
//  // calculate ratio and error for r2 = f2/f1_1/f1_2
//
//  double ref1_1, ref1_2, ref2, rer2;
//  if (f1_1>0 & f1_2>0)
//    {
//    ref1_1 = ef1_1/f1_1;
//    ref1_2 = ef1_2/f1_2;
//    ref2   = ef2/f2;
//    rer2   = ref1_1*ref1_1 + ref1_2*ref1_2 + ref2*ref2;
//    r2 = f2/(f1_1*f1_2) - 1.0;
//    er2 = r2*sqrt(rer2);
//    }
//  else
//    {
//    r2 = 0.0;
//    er2 = 0.0;
//    }
//}
//
//void calculateNudyn(double r2_11,double er2_11,double r2_12,double er2_12,double r2_22,double er2_22,double & nudyn,double & enudyn)
//{
//  nudyn = r2_11 + r2_22 - 2.0*r2_12;
//  enudyn = sqrt(er2_11*er2_11 + er2_22*er2_22 + 4.0*er2_12*er2_12);
//}
//
//
//void calculateR2(TH1* h1, TH1* h2, TH1* hr)
//{
//  double v1, ev1, v2, ev2, r, er;
//  int nBins = h1->GetNbinsX();
//  for (int iBin = 1; iBin <=nBins; iBin++)
//    {
//    v1   = h1->GetBinContent(iBin);
//    ev1  = h1->GetBinError(iBin);
//    v2   = h2->GetBinContent(iBin);
//    ev2  = h2->GetBinError(iBin);
//    calculateR2(v1,ev1,v2,ev2,r,er);
//    hr->SetBinContent(iBin, r);
//    hr->SetBinError(iBin,   er);
//    }
//}
//
//void calculateR2(TH1* h1_1, TH1* h1_2, TH1* h2_12, TH1* hr)
//{
//  double v1_1, ev1_1, v1_2, ev1_2, v2, ev2, r, er;
//  int nBins = h1_1->GetNbinsX();
//  for (int iBin = 1; iBin <=nBins; iBin++)
//    {
//    v1_1   = h1_1->GetBinContent(iBin);
//    ev1_1  = h1_1->GetBinError(iBin);
//    v1_2   = h1_2->GetBinContent(iBin);
//    ev1_2  = h1_2->GetBinError(iBin);
//    v2     = h2_12->GetBinContent(iBin);
//    ev2    = h2_12->GetBinError(iBin);
//    calculateR2(v1_1,ev1_1,v1_2,ev1_2,v2,ev2,r,er);
//    hr->SetBinContent(iBin, r);
//    hr->SetBinError(iBin,   er);
//    }
//}
//
//void calculateNuDyn(TH1* r2_11, TH1* r2_12, TH1* r2_22, TH1* nudyn)
//{
//  double v2_11, ev2_11, v2_12, ev2_12, v2_22, ev2_22, nu, enu;
//  int nBins = r2_11->GetNbinsX();
//  for (int iBin = 1; iBin <=nBins; iBin++)
//    {
//    v2_11   = r2_11->GetBinContent(iBin);
//    ev2_11  = r2_11->GetBinError(iBin);
//    v2_12   = r2_12->GetBinContent(iBin);
//    ev2_12  = r2_12->GetBinError(iBin);
//    v2_22   = r2_22->GetBinContent(iBin);
//    ev2_22  = r2_22->GetBinError(iBin);
//    calculateNudyn(v2_11,ev2_11,v2_12,ev2_12,v2_22,ev2_22,nu,enu);
//    nudyn->SetBinContent(iBin, nu);
//    nudyn->SetBinError(iBin,   enu);
//    }
//}
//// Calculate r = h2/h1
//void calculateRatio(TH1 * h1, TH1 * h2, TH1 * ratio)
//{
//  double v1, ev1, v2, ev2, r, er;
//  int nBins = h1->GetNbinsX();
//  for (int iBin = 1; iBin <=nBins; iBin++)
//    {
//    v1   = h1->GetBinContent(iBin);
//    ev1  = h1->GetBinError(iBin);
//    v2   = h2->GetBinContent(iBin);
//    ev2  = h2->GetBinError(iBin);
//    calculateRatio(v1,ev1,v2,ev2,r,er);
//
//    cout << ratio->GetName() << " iBin:" << iBin << "  r:" << r << "   er:" << er << endl;
//    ratio->SetBinContent(iBin, r);
//    ratio->SetBinError  (iBin, er);
//    }
//}
//
//void calculateMeanAndStd(TH1 * h, int nBins, double &mean, double &eMean, double &std)
//{
//  double m1  = 0;
//  double em1 = 0;
//  double m2  = 0;
//  double v, ev;
//  // calculate mean value of the nBins
//  for (int iBin=1; iBin<=nBins; iBin++)
//    {
//    v  = h->GetBinContent(iBin);
//    ev = h->GetBinError(iBin);
//    m1 += v;
//    em1 += ev*ev;
//    }
//  mean = m1/double(nBins);
//  eMean = sqrt(em1)/double(nBins);
//
//  for (int iBin=1; iBin<=nBins; iBin++)
//    {
//    v  = h->GetBinContent(iBin)-mean;
//    m2 += v*v;
//    }
//  m2 = m2/double(nBins-1);
//  std = sqrt(m2);
//}
//
//void extractGauss(TH1* h, double & mean, double & sigma)
//{
//  TF1* h_fct = h->GetFunction("gaus");
//  mean  = h_fct->GetParameter(1);
//  sigma = h_fct->GetParameter(2);
//  cout << h->GetName() << " mean:" << mean << "  sigma:" << sigma << endl;
//}
//
//void compareRatio(TString name,
//                  double num,  double eNum,
//                  double dem,  double eDem,
//                  double & predictedRatio, double & predictedError,
//                  double ratioMean,  double  ratioSigma)
//{
//  double predictedToActualMean, predictedToActualSigma;
//
//  predictedRatio  = num/dem;
//  predictedToActualMean = predictedRatio/ratioMean;
//
//  // assume no correlations are present...
//  double re1, re2;
//  re1 = eNum/num;
//  re2 = eDem/dem;
//  predictedError  = predictedRatio*sqrt(re1*re1 + re2*re2);
//  predictedToActualSigma = predictedError/ratioSigma;
//
//  cout << "Compare ratio for " << name << endl;
//  cout << "Uncorrelated errors" << endl;
//  cout << "Predicted Ratio Mean  :" << predictedRatio  << " Actual Ratio Mean:" << ratioMean << " Ratio of Predicted to actual Ratio Mean: " << predictedToActualMean << endl;
//  cout << "Predicted Ratio Sigma :" << predictedError << " Actual Ratio Sigma:" << ratioSigma << " Ratio of Predicted to actual Ratio Sigma: " << predictedToActualSigma << endl;
//
//  predictedError  = predictedRatio*sqrt( fabs((1.0- 2.0*predictedRatio)*re1*re1 + re2*re2));
//  predictedToActualSigma = predictedError/ratioSigma;
//
//  cout << "Binomial  errors" << endl;
//  cout << "Predicted Ratio Mean  :" << predictedRatio  << " Actual Ratio Mean:" << ratioMean << " Ratio of Predicted to actual Ratio Mean: " << predictedToActualMean << endl;
//  cout << "Predicted Ratio Sigma :" << predictedError << " Actual Ratio Sigma:" << ratioSigma << " Ratio of Predicted to actual Ratio Sigma: " << predictedToActualSigma << endl;
//
//
//
//}
//
//
//void compareR2(TString name,
//               double f2_mean, double f2_sigma,
//               double f1_mean, double f1_sigma,
//               double & predictedR2Mean, double & predictedR2Sigma,
//               double r2_mean, double r2_sigma)
//{
//  double predictedToActualMean, predictedToActualSigma;
//
//  predictedR2Mean = f2_mean/(f1_mean*f1_mean);
//  predictedToActualMean = predictedR2Mean/r2_mean;
//  double ref2 = f2_sigma/f2_mean;
//  double ref1 = f1_sigma/f1_mean;
//  predictedR2Sigma = predictedR2Mean * sqrt(ref2*ref2 + 4.0*ref1*ref1);
//  predictedToActualSigma = predictedR2Sigma/r2_sigma;
//  cout << "Compare r2 for " <<  name << endl;
//  cout << "Predicted r2 Mean  :" << predictedR2Mean  << "  Actual r2 Mean:" << r2_mean << " Ratio of Predicted to actual r2 Mean: " << predictedToActualMean << endl;
//  cout << "Predicted r2 Sigma :" << predictedR2Sigma << " Actual r2 Sigma:" << r2_sigma << " Ratio of Predicted to actual r2 Sigma: " << predictedToActualSigma << endl;
//}
//
//void compareR2(TString name,
//               double f2_mean, double f2_sigma,
//               double f1_1_mean, double f1_1_sigma,
//               double f1_2_mean, double f1_2_sigma,
//               double & predictedR2Mean, double & predictedR2Sigma,
//               double r2_mean, double r2_sigma)
//{
//  double predictedToActualMean, predictedToActualSigma;
//  predictedR2Mean = f2_mean/(f1_1_mean*f1_2_mean);
//  predictedToActualMean = predictedR2Mean/r2_mean;
//  double ref2 = f2_sigma/f2_mean;
//  double ref11 = f1_1_sigma/f1_1_mean;
//  double ref12 = f1_2_sigma/f1_2_mean;
//  predictedR2Sigma = predictedR2Mean * sqrt(ref2*ref2 + ref11*ref11 + ref12*ref12);
//  predictedToActualSigma = predictedR2Sigma/r2_sigma;
//
//  cout << "Compare r2 for " <<  name << endl;
//  cout << "Predicted r2 Mean  :" << predictedR2Mean  << "  Actual r2 Mean:" << r2_mean << " Ratio of Predicted to actual r2 Mean: " << predictedToActualMean << endl;
//  cout << "Predicted r2 Sigma :" << predictedR2Sigma << " Actual r2 Sigma:" << r2_sigma << " Ratio of Predicted to actual r2 Sigma: " << predictedToActualSigma << endl;
//}


int StatStudyPlots()
{
  TString includesPath = "/Users/claudeapruneau/Documents/GitHub/WAC/Base/";
  gSystem->Load(includesPath+"CanvasCollection.hpp");
  gSystem->Load(includesPath+"CanvasConfiguration.hpp");
  gSystem->Load(includesPath+"TaskConfiguration.hpp");
  gSystem->Load(includesPath+"EnhancedGraph.hpp");
  gSystem->Load(includesPath+"Factory.hpp");
  gSystem->Load(includesPath+"FunctionCollection.hpp");
  gSystem->Load(includesPath+"GraphConfiguration.hpp");
  gSystem->Load(includesPath+"HistogramCollection.hpp");
  gSystem->Load(includesPath+"Histograms.hpp");
  gSystem->Load(includesPath+"RandomGenerators.hpp");
  gSystem->Load(includesPath+"RapidityGenerator.hpp");
  gSystem->Load(includesPath+"Task.hpp");
  gSystem->Load(includesPath+"TaskCollection.hpp");
  gSystem->Load(includesPath+"Property.hpp");
  gSystem->Load(includesPath+"MessageLogger.hpp");
  gSystem->Load(includesPath+"AnalysisConfiguration.hpp");
  gSystem->Load(includesPath+"Event.hpp");
  gSystem->Load(includesPath+"EventFilter.hpp");
  gSystem->Load(includesPath+"EventHistos.hpp");
  gSystem->Load(includesPath+"EventLoop.hpp");
  gSystem->Load(includesPath+"GeneralizedGaussianConfiguration.hpp");
  gSystem->Load(includesPath+"GeneralizedGaussianProfile.hpp");
  gSystem->Load(includesPath+"GeneratorConfiguration.hpp");
  gSystem->Load(includesPath+"TwoPartCorrelationAnalyzer.hpp");
  gSystem->Load(includesPath+"Particle.hpp");
  gSystem->Load(includesPath+"ParticleFilter.hpp");
  gSystem->Load(includesPath+"ParticleHistos.hpp");
  gSystem->Load(includesPath+"ParticlePairCombinedHistos.hpp");
  gSystem->Load(includesPath+"ParticlePairDerivedHistos.hpp");
  gSystem->Load(includesPath+"ParticlePairFilter.hpp");
  gSystem->Load(includesPath+"ParticlePairHistos.hpp");
  gSystem->Load(includesPath+"TrackAndPairConfiguration.hpp");
  gSystem->Load(includesPath+"NuDynTask.hpp");
  gSystem->Load(includesPath+"NuDynHistos.hpp");
  gSystem->Load(includesPath+"NuDynDerivedHistos.hpp");
  gSystem->Load(includesPath+"StatStudyHistograms.hpp");
  gSystem->Load(includesPath+"Plotter.hpp");
  gSystem->Load(includesPath+"StatStudyPlotter.hpp");
  gSystem->Load("libBase.dylib");

  int nSubSamples = 1000;

  HistogramCollection * histogramCollection = new HistogramCollection("Collection",100);
  histogramCollection->setDefaultOptions(1);
  CanvasCollection    * canvasCollection    = new CanvasCollection();
  CanvasConfiguration * canvasConfiguration = new CanvasConfiguration(CanvasConfiguration::Landscape,CanvasConfiguration::Linear);
  GraphConfiguration  ** graphConfigurations = new GraphConfiguration*[10];
  for (int iGraph=0;iGraph<10;iGraph++)
    {
    graphConfigurations[iGraph] = new GraphConfiguration(1,iGraph);
    }

  TString inputPath = "/Users/claudeapruneau/Documents/GitHub/run/StatStudy/";
  TString outputPath = "/Users/claudeapruneau/Documents/GitHub/run/StatStudy/";
  int nModels            = 5;
  TFile ** inputFiles = new TFile* [nModels];
  StatStudyHistograms ** statStudy = new StatStudyHistograms*[nModels];
  StatStudyHistograms ** statStudy2 = new StatStudyHistograms*[nModels];
  StatStudyPlotter    ** plotter   = new StatStudyPlotter*[nModels];
  TString nameBase = "StatStudySet";
  TString name, title;
  TString ** modelName  = new TString*[nModels];
  TString ** modelTitle = new TString*[nModels];
  TString canvasNameBase;
  //for (int iModel=0;iModel<nModels;iModel++)
    for (int iModel=0;iModel<nModels;iModel++)
    {
    name = nameBase;
    name += iModel;
    modelName[iModel]  = new TString(name);
    canvasNameBase = name;
    canvasNameBase += "_";
    title = "Set ";
    title += iModel;

    modelTitle[iModel]  = new TString(title);
    inputFiles[iModel] = new TFile(inputPath+name+".root","OLD");;
    statStudy[iModel]  = new StatStudyHistograms(inputFiles[iModel], name ,MessageLogger::Debug);
    plotter[iModel]    = new StatStudyPlotter();
    //plotter[iModel]->makePlots(*modelName[iModel],statStudy[iModel],canvasConfiguration,graphConfigurations);
    plotter[iModel]->makePlots(canvasNameBase,statStudy[iModel],canvasConfiguration,graphConfigurations);
    plotter[iModel]->printAllCanvas(outputPath);
    }

  StatStudyPlotter * compPlotter = new StatStudyPlotter();
  compPlotter->makeComparisonPlots("ModelComparisonSampleSize_", canvasConfiguration,3,statStudy,modelTitle,graphConfigurations);
  compPlotter->printAllCanvas(outputPath);

  StatStudyPlotter * compPlotter2 = new StatStudyPlotter();
  statStudy++;statStudy++;
  modelTitle++;modelTitle++;
   compPlotter2->makeComparisonPlots("ModelComparisonPartMult_", canvasConfiguration,3,statStudy,modelTitle,graphConfigurations);
   compPlotter2->printAllCanvas(outputPath);

  return 0;
}

