
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

void gradientFinder(TH2* h, int iX, int iY, double & gx, double & gy)
{
double v11 = h->GetBinContent(iX,iY);
double v12 = h->GetBinContent(iX,iY+1);
double v21 = h->GetBinContent(iX+1,iY);
double v22 = h->GetBinContent(iX+1,iY+1);

double x1 = h->GetXaxis()->GetBinCenter(iX);
double x2 = h->GetXaxis()->GetBinCenter(iX+1);
double y1 = h->GetYaxis()->GetBinCenter(iY);
double y2 = h->GetYaxis()->GetBinCenter(iY+1);

double gxp = -(v21-v11)/(x2-x1); //  /((v21+v11)/2.0);
double gyp = -(v12-v11)/(y2-y1); //  /((v12+v11)/2.0);;
double g   = sqrt(gxp*gxp+gyp+gyp);
if (g>0)
  {
  gx = gxp/g;
  gy = gyp/g;
  TArrow * arrow = new  TArrow(x1, y1, x1+5.0*gx, y1+5.0*gy, 0.03);
  arrow->SetLineColor(2);
  arrow->SetLineWidth(4);
  arrow->Draw();
  }
else
  {
    gx = 0.0;
    gy = 0.0;
  }
cout << "gx:"<< gx << " gy:" << gy << endl;


}

double TwoDCubic(double *x, double *par)
{
  double xx   = x[0];
  double yy   = x[1];

  double v = 0;
  for (int iX=0; iX<3; iX++)
    {
    for (int iY=0; iY<3; iY++)
      {
      int index = 3*iX + iY;
      v += par[index]*TMath::Power(xx,iX)*TMath::Power(yy,iY);
      }
    }
  return v;
}

double GradXTwoDCubic(double *x, double *par)
{
  double xx   = x[0];
  double yy   = x[1];

  double v = 0;
  for (int iX=0; iX<3; iX++)
    {
    for (int iY=0; iY<3; iY++)
      {
      int index = 3*iX + iY;
      v += par[index]*double(iX)*TMath::Power(xx,iX-1)*TMath::Power(yy,iY);
      }
    }
  return v;
}

double GradYTwoDCubic(double *x, double *par)
{
  double xx   = x[0];
  double yy   = x[1];

  double v = 0;
  for (int iX=0; iX<3; iX++)
    {
    for (int iY=0; iY<3; iY++)
      {
      int index = 3*iX + iY;
      v += par[index]*double(iY)*TMath::Power(xx,iX)*TMath::Power(yy,iY-1);
      }
    }
  return v;
}



int PlotGradients()
{
//  TString includesPath = getenv("WAC_SRC");
//  includesPath += "/Base/";
//  gSystem->Load(includesPath+"CanvasCollection.hpp");
//  gSystem->Load(includesPath+"CanvasConfiguration.hpp");
//  gSystem->Load(includesPath+"TaskConfiguration.hpp");
//  gSystem->Load(includesPath+"EnhancedGraph.hpp");
//  gSystem->Load(includesPath+"Factory.hpp");
//  gSystem->Load(includesPath+"FunctionCollection.hpp");
//  gSystem->Load(includesPath+"GraphConfiguration.hpp");
//  gSystem->Load(includesPath+"HistogramCollection.hpp");
//  gSystem->Load(includesPath+"Histograms.hpp");
//  gSystem->Load("libBase.dylib");
//
//  MessageLogger::LogLevel logLevel = MessageLogger::Info;
//  vector<TString>  eventClassNames;
//  vector<TString>  eventClassTitles;
//  vector<TString>  observableNames;
//  vector<TString>  observableTitles;
//  vector<TString>  observableTitlesX;
//  vector<TString>  observableTitlesY;
//  vector<TString>  observableTitlesZ;
//  vector<double>  minX;
//  vector<double>  maxX;
//  vector<double>  minY;
//  vector<double>  maxY;
//  vector<double>  minZ;
//  vector<double>  maxZ;
//
//  vector<TH1*>     histograms;
//  vector<TString>  titles;
//  vector<Plotter*> plotters;
//  vector<GraphConfiguration*> graphConfigurations;
//  vector<GraphConfiguration*> graphConfigurations2D;
//  TString canvasName;
//
//  bool color = true;
//  if (color)
//    gStyle->SetPalette(1,0);
//  else
//    gStyle->SetPalette(7,0);
//  gStyle->SetOptStat(0);
//  gStyle->SetOptFit(0);
//  gStyle->SetOptDate(0);
//  gStyle->SetOptTitle(0);
//  gStyle->SetPadBorderMode(0);
//
//  CanvasConfiguration * landscapeLinear     = new CanvasConfiguration(CanvasConfiguration::SquareWide,CanvasConfiguration::Linear);
//  CanvasConfiguration * landscapeLogY       = new CanvasConfiguration(CanvasConfiguration::LandscapeWide,CanvasConfiguration::LogY);
//  CanvasConfiguration * landscapeLogZ       = new CanvasConfiguration(CanvasConfiguration::SquareWide,CanvasConfiguration::LogZ);
//  landscapeLogZ->leftMargin  = 0.2;
//  landscapeLogZ->rightMargin = 0.1;
//
//
//  GraphConfiguration * gc2D;
//  gc2D = new GraphConfiguration(2);
//  gc2D->plotOption = "ZCOL";
//  graphConfigurations2D.push_back(gc2D);
//  gc2D = new GraphConfiguration(2);
//  gc2D->plotOption = "BOX";
//  graphConfigurations2D.push_back(gc2D);


  // TString inputPathName  = getenv("WAC_REPOSITORY_PATH");
  TString inputPathName  = getenv("WAC_REPOSITORY_PATH");
  TString outputPathName = getenv("WAC_OUTPUT_PATH");

  //   /Users/claudeapruneau/Documents/GitHub/WAC/../WAC-DATA/OutputFiles//CollisionGeometryCGG.root
  TString fileName = inputPathName + "CG/Geom_CGGA.root";
  TFile *   inputFile = new TFile(fileName,"OLD");
  if (inputFile)
    {
    cout << "<I> Successfully opened: " << fileName << endl;
    }
  else
    {
    cout << "<E> Unable to open: " << fileName << endl;
    return 1;
    }


  //TH1 * h = (TH1*) inputFile->Get("CGGA_bGeq1Lt2_xyDistInteractions");
  TH1 * h = (TH1*) inputFile->Get("CGGA_bGeq16Lt20_xyDistInteractions");
  if (h)
    {
    cout << "<I> Successfully retrieved: " << "CGGA_bGeq1Lt2_xyDistInteractions" << endl;
    }
  else
    {
    cout << "<E> Unable to retrieve: " << "CGGA_bGeq1Lt2_xyDistInteractions" << endl;
    return 1;
    }

  TH2 * hClone = (TH2*) h->Clone();

  CanvasConfiguration * canvasConfig  = new CanvasConfiguration(CanvasConfiguration::SquareWide,CanvasConfiguration::Linear);
  canvasConfig->logx = 0;
  canvasConfig->logy = 0;
  canvasConfig->logz = 0;
  CanvasCollection * collection = new CanvasCollection();
  TCanvas * c2 = collection->createCanvas("c2", canvasConfig, 20);
  c2->SetLogx(0);
  c2->SetLogy(0);
  c2->SetLogz(0);

  hClone->Draw("SURF3");
  TCanvas * c1 = collection->createCanvas("c1", canvasConfig, 20);
  //hClone->Rebin2D(2,2);
  //hClone->Scale(1.0/4.0);
  hClone->Smooth(1);
  c1->SetLogx(0);
  c1->SetLogy(0);
  c1->SetLogz(0);
  hClone->Draw("ZCOL");  // hClone->Draw("SURF3");

  //  Plotter * plotter = new Plotter("Plotter",logLevel));
  //  plotters[0]->printAllCanvas(outputPathName);

//  int iX = 131;
//  int iY = 101;
  double g, gx, gy;
  TString fitOption = "WWSRQ";
  int kk = 4;

  vector<double> xp;
  vector<double> yp;
  vector<double> xg;
  vector<double> yg;

  int nx = hClone->GetNbinsX();

  double hMax = hClone->GetBinContent(100,100);

  int from = int(0.1*double(nx));
  int to   = int(0.9*double(nx));
  cout << " from:" << from << " to:" << to << endl;
  for (int iX=from; iX<to; iX+=5)
    {
    for (int iY=from; iY<to; iY+=5)
      {

      double value = h->GetBinContent(iX,iY);
      //cout << " value:" << value << " hMax:" << hMax << endl;
      if (value<0.0005*hMax) continue;

      double xLow  = h->GetXaxis()->GetBinCenter(iX-kk);
      double xHigh = h->GetXaxis()->GetBinCenter(iX+kk);
      double yLow  = h->GetYaxis()->GetBinCenter(iY-kk);
      double yHigh = h->GetYaxis()->GetBinCenter(iY+kk);

      TF2 * fxy = new TF2("fxy",TwoDCubic,xLow,xHigh,yLow,yHigh,9);
      TF2 * gfx = new TF2("gfx",GradXTwoDCubic,xLow,xHigh,yLow,yHigh,9);
      TF2 * gfy = new TF2("gfy",GradYTwoDCubic,xLow,xHigh,yLow,yHigh,9);


      fxy->SetParameter(0, value);
      for (int k=1;k<9;k++) fxy->SetParameter(k, 0.01);
      TFitResultPtr resultPtr = hClone->Fit(fxy, fitOption, "");
      //if (!resultPtr) continue;
      cout << " Fit is valid: " << resultPtr->IsValid() << endl;
      for (int k=0;k<9;k++)
        {
        gfx->SetParameter(k, fxy->GetParameter(k));
        gfy->SetParameter(k, fxy->GetParameter(k));
        }
      double x = hClone->GetXaxis()->GetBinCenter(iX);
      double y = hClone->GetYaxis()->GetBinCenter(iY);
      double ggx = -gfx->Eval(x,y);
      double ggy = -gfy->Eval(x,y);
      if (ggx!=0 && ggy!=0)
        {
        g = sqrt(ggx*ggx + ggy*ggy);
        gx = ggx/g;
        gy = ggy/g;
        }
      else
        {
        g = 0; gx = 0; gy = 0;
        }

      cout << " x: " << x << " y: " << y << endl;
      cout << "gx: " << gx << " gy: " << gy << endl;
      xp.push_back(x);
      yp.push_back(y);
      xg.push_back(gx);
      yg.push_back(gy);
      delete fxy;
      delete gfx;
      delete gfy;

      }
    }

  TCanvas * c3 = collection->createCanvas("c3", canvasConfig, 20);
  c3->SetLogx(0);
  c3->SetLogy(0);
  c3->SetLogz(0);
  h->Draw("ZCOL");  // hClone->Draw("SURF3");

  //fxy->Draw("ZCOL");
  //vector<TArrow*> arrows;
  TArrow * arrow;
  for (unsigned int k=0; k<xp.size(); k++)
    {
    double x = xp[k];
    double y = yp[k];
    double dx = x + 1.6*xg[k];
    double dy = y + 1.6*yg[k];
    arrow = new  TArrow(x,y,dx,dy,0.01);
    int color = k%4;
    switch (color)
      {
        case 0:    arrow->SetLineColor(1); break;
        case 1:    arrow->SetLineColor(2); break;
        case 2:    arrow->SetLineColor(4); break;
        case 3:    arrow->SetLineColor(6); break;
      }

    arrow->SetLineWidth(4);
    arrow->Draw();
    }



//  for (int iX = 100; iX<140; iX+=10)
//    {
//    for (int iY= 100; iY<140; iY+=10)
//      {
//      gradientFinder(hClone, iX, iY, gx,  gy);
//      }
//    }

  return 0;
}
