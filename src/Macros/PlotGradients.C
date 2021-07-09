
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

void symmetrize(TH2* h)
{
  int n = h->GetNbinsX();
  int nHalf = n/2;
  cout << " n: " << n << " nHalf:" << nHalf << endl;

  double v1,v2,v3,v4,avg;
  for (int iX=1;iX<=nHalf;iX++)
    {
    for (int iY=1;iY<=nHalf;iY++)
      {
      v1 = h->GetBinContent(nHalf+iX,  nHalf+iY);
      v2 = h->GetBinContent(nHalf+1-iX,nHalf+iY);
      v3 = h->GetBinContent(nHalf+1-iX,nHalf+1-iY);
      v4 = h->GetBinContent(nHalf+iX,  nHalf+1-iY);
      avg = (v1+v2+v3+v4)/4.0;
      h->SetBinContent(nHalf+iX,  nHalf+iY,  avg);
      h->SetBinContent(nHalf+1-iX,nHalf+iY,  avg);
      h->SetBinContent(nHalf+1-iX,nHalf+1-iY,avg);
      h->SetBinContent(nHalf+iX,  nHalf+1-iY,avg);
      }
    }
}



void analyzeGradients(TH2 * work, TH2* edge, TH2* radius, TH2* hGx, TH2* hGy,
                      vector<double> & xArray, vector<double> & yArray, vector<double> & gxArray, vector<double> & gyArray,
                      double minimumFraction)
{
  symmetrize(work);
  work->Smooth(1);
  double x,y, xs, ys, g, gx, gy, a, b, r0, r, rMax;
  TString fitOption = "WWSRQ";
  int    kk        = 4;
  int    nx        = work->GetNbinsX();
  int    nHalf     = nx/2;
  double hMax      = work->GetBinContent(100,100);
  double threshold = minimumFraction*hMax;
  int    from      = nHalf+1;  // int(0.1*double(nx));
  int    to        = nx; // int(0.9*double(nx));
                         //cout << " from:" << from << " to:" << to << endl;
  for (int iX=from; iX<to; iX+=1)
    {
    for (int iY=from; iY<to; iY+=1)
      {
      double value = work->GetBinContent(iX,iY);
      //cout << " value:" << value << " hMax:" << hMax << endl;
      rMax = -9999.0;
      if (value>=threshold)
        {
        // calculate gradient
        double xLow  = work->GetXaxis()->GetBinCenter(iX-kk);
        double xHigh = work->GetXaxis()->GetBinCenter(iX+kk);
        double yLow  = work->GetYaxis()->GetBinCenter(iY-kk);
        double yHigh = work->GetYaxis()->GetBinCenter(iY+kk);
        TF2 * fxy = new TF2("fxy",TwoDCubic,xLow,xHigh,yLow,yHigh,9);
        TF2 * gfx = new TF2("gfx",GradXTwoDCubic,xLow,xHigh,yLow,yHigh,9);
        TF2 * gfy = new TF2("gfy",GradYTwoDCubic,xLow,xHigh,yLow,yHigh,9);
        fxy->SetParameter(0, value);
        for (int k=1;k<9;k++) fxy->SetParameter(k, 0.01);
        TFitResultPtr resultPtr = work->Fit(fxy, fitOption, "");
        //cout << " Fit is valid: " << resultPtr->IsValid() << endl;
        for (int k=0;k<9;k++)
          {
          gfx->SetParameter(k, fxy->GetParameter(k));
          gfy->SetParameter(k, fxy->GetParameter(k));
          }
        x = work->GetXaxis()->GetBinCenter(iX);
        y = work->GetYaxis()->GetBinCenter(iY);
        double ggx = -gfx->Eval(x,y);
        double ggy = -gfy->Eval(x,y);
        if (ggx<0) ggx = 0;
        if (ggy<0) ggy = 0;
        if (ggx!=0 || ggy!=0)
          {
          g = sqrt(ggx*ggx + ggy*ggy);
          gx = ggx/g;
          gy = ggy/g;
          // distance from y axis along gradient
          if (gx>0)
            {
            a = gy/gx;
            b = y - a*x;
            if (b<0) b = 0.0;
            r = sqrt(x*x + (y-b)*(y-b));
            }
          else
            {
            gx = 0;
            a = 99999.0; // infinite really...
            b = 0;
            r = sqrt(x*x+y*y);
            }
          // distance to the edge along gradient
          double xs, ys;
          rMax = -99999;
          if (a>1.0)
            {
            for (int jY=iY+1;jY<=nx;jY++)
              {
              ys = work->GetYaxis()->GetBinCenter(jY);
              xs = (ys-y)/a + x;
              int jX    = work->GetXaxis()->FindBin(xs);
              double c  = work->GetBinContent(jX,jY);
              //cout << "Case > iX:" << iX << " iY:" << iY << " x:" << x << " y:" << y << " jX:" << jX << " jY:" <<  jY << " xs:" << xs << " ys:" << ys << " c:" << c << " threshold:" << threshold << endl;
              if (c<threshold)
                {
                ys = work->GetYaxis()->GetBinCenter(jY);
                rMax = sqrt(xs*xs + (ys-b)*(ys-b) );
                break;
                }
              }
            }
          else
            {
            for (int jX=iX+1;jX<=nx;jX++)
              {
              xs = work->GetXaxis()->GetBinCenter(jX);
              ys = y + a*(xs-x);
              int jY    = work->GetYaxis()->FindBin(ys);
              double c  = work->GetBinContent(jX,jY);
              //cout << "Case < iX:" << iX << " iY:" << iY << " x:" << x << " y:" << y << " jX:" << jX << " jY:" <<  jY << " xs:" << xs << " ys:" << ys << " c:" << c << " threshold:" << threshold << endl;
              if (c<threshold)
                {
                ys = work->GetYaxis()->GetBinCenter(jY);
                rMax = sqrt(xs*xs + (ys-b)*(ys-b) );
                break;
                }

              }
            }
          edge->Fill(xs,ys);
          }
        xArray.push_back(x);
        yArray.push_back(y);
        gx = gx*r/rMax;
        gy = gy*r/rMax;
        gxArray.push_back(gx);
        gyArray.push_back(gy);

        double radiusRatio;
        if (rMax>0)
          radiusRatio = r/rMax;
        else
          radiusRatio = 0.00001;
        int iiX = iX-nHalf;
        int iiY = iY-nHalf;
        hGx->SetBinContent(nHalf+iiX,     nHalf+iiY, gx);
        hGy->SetBinContent(nHalf+iiX,     nHalf+iiY, gy);
        radius->SetBinContent(nHalf+iiX,  nHalf+iiY, radiusRatio);

        hGx->SetBinContent(nHalf+1-iiX,   nHalf+iiY, gx);
        hGy->SetBinContent(nHalf+1-iiX,   nHalf+iiY, gy);
        radius->SetBinContent(nHalf+1-iiX,nHalf+iiY, radiusRatio);

        hGx->SetBinContent(nHalf+1-iiX,   nHalf+1-iiY, gx);
        hGy->SetBinContent(nHalf+1-iiX,   nHalf+1-iiY, gy);
        radius->SetBinContent(nHalf+1-iiX,nHalf+1-iiY, radiusRatio);

        hGx->SetBinContent(nHalf+iiX,     nHalf+1-iiY, gx);
        hGy->SetBinContent(nHalf+iiX,     nHalf+1-iiY, gy);
        radius->SetBinContent(nHalf+iiX,  nHalf+1-iiY, radiusRatio);

        delete fxy;
        delete gfx;
        delete gfy;
        }
      else
        {
        int iiX = iX-nHalf;
        int iiY = iY-nHalf;
        double small = 0.0001;
        hGx->SetBinContent(nHalf+iiX,     nHalf+iiY, small);
        hGy->SetBinContent(nHalf+iiX,     nHalf+iiY, small);
        radius->SetBinContent(nHalf+iiX,  nHalf+iiY, small);

        hGx->SetBinContent(nHalf+1-iiX,   nHalf+iiY, small);
        hGy->SetBinContent(nHalf+1-iiX,   nHalf+iiY, small);
        radius->SetBinContent(nHalf+1-iiX,nHalf+iiY, small);

        hGx->SetBinContent(nHalf+1-iiX,   nHalf+1-iiY, small);
        hGy->SetBinContent(nHalf+1-iiX,   nHalf+1-iiY, small);
        radius->SetBinContent(nHalf+1-iiX,nHalf+1-iiY, small);

        hGx->SetBinContent(nHalf+iiX,     nHalf+1-iiY, small);
        hGy->SetBinContent(nHalf+iiX,     nHalf+1-iiY, small);
        radius->SetBinContent(nHalf+iiX,  nHalf+1-iiY, small);
        }
      } // iX
    }   // iY
}


int PlotGradients()
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
  gSystem->Load(includesPath+"MessageLogger.hpp");
  gSystem->Load("libBase.dylib");


  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  gStyle->SetOptDate(0);
  gStyle->SetPadBorderMode(0);

  MessageLogger::LogLevel logLevel = MessageLogger::Info;

  CanvasConfiguration * landscapeLogZ       = new CanvasConfiguration(CanvasConfiguration::SquareWide,CanvasConfiguration::Linear);
  landscapeLogZ->leftMargin  = 0.15;
  landscapeLogZ->rightMargin = 0.15;
  GraphConfiguration * gc2D;
  gc2D = new GraphConfiguration(2);
  gc2D->plotOption = "ZCOL";

  // TString inputPathName  = getenv("WAC_REPOSITORY_PATH");
  TString inputPathName  = getenv("WAC_REPOSITORY_PATH");
  TString outputPathName = getenv("WAC_OUTPUT_PATH");

  TString          observableName = "_xyDistInteractions";
  TString          histoName;
  TString          canvasName;
  vector<TString>  eventClassNames;
  vector<TString>  eventClassTitles;
  vector<TFile*>   inputFiles;
  vector<TH2*>     histos;
  vector<TH2*>     histosEdge;
  vector<TH2*>     histosRadius;
  vector<TH2*>     histosGx;
  vector<TH2*>     histosGy;

  vector<double>   bRanges ={0.0, 0.5, 1.0, 2.0, 3.0, 4.0, 4.0, 6.0, 8.0, 10.0, 12.0, 16.0, 20.0};
  double           minimumFraction = 0.0005;

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


  //   /Users/claudeapruneau/Documents/GitHub/WAC/../WAC-DATA/OutputFiles//CollisionGeometryCGG.root
  TString fileName = inputPathName + "PbPbGeometry/Geom_PbPb_CGGA.root";
  TFile *   inputFile = new TFile(fileName,"OLD");
  if (inputFile)
    {
    cout << "<I> --------------------------------------------------------------------------------------------------------------" << endl;
    cout << "<I> Successfully opened: " << fileName << endl;
    cout << "<I> --------------------------------------------------------------------------------------------------------------" << endl;
    }
  else
    {
    cout << "<I> --------------------------------------------------------------------------------------------------------------" << endl;
    cout << "<E> Unable to open: " << fileName << endl;
    cout << "<I> --------------------------------------------------------------------------------------------------------------" << endl;
    return 1;
    }

  fileName = outputPathName + "CG/Geom_CGGA_Gradients.root";
  TFile *   outputFile = new TFile(fileName,"RECREATE");
  if (outputFile)
    {
    cout << "<I> --------------------------------------------------------------------------------------------------------------" << endl;
    cout << "<I> Successfully opened: " << fileName << endl;
    cout << "<I> --------------------------------------------------------------------------------------------------------------" << endl;
    }
  else
    {
    cout << "<I> --------------------------------------------------------------------------------------------------------------" << endl;
    cout << "<E> Unable to open: " << fileName << endl;
    cout << "<I> --------------------------------------------------------------------------------------------------------------" << endl;
    return 1;
    }

  //for (unsigned int k=0; k<1; k++)
  for (unsigned int k=0; k<eventClassNames.size(); k++)
    {
    histoName = eventClassNames[k];
    histoName += observableName;
    TH2 * h = (TH2*) inputFile->Get(histoName);
    if (h)
      {
      cout << "<I> Successfully retrieved: " << histoName << endl;
      cout << "<I> --------------------------------------------------------------------------------------------------------------" << endl;
      }
    else
      {
      cout << "<E> Unable to retrieve: " << histoName << endl;
      cout << "<E> ----- ABORT --------------------------------------------------------------------------------------------------" << endl;
      cout << "<E> --------------------------------------------------------------------------------------------------------------" << endl;
      return 1;
      }
    histos.push_back(h);
    }

  cout << "<I> --------------------------------------------------------------------------------------------------------------" << endl;
  cout << "<I> Let's get busy..." << endl;
  cout << "<I> --------------------------------------------------------------------------------------------------------------" << endl;

  TCanvas * workCanvas = new TCanvas();

  Plotter * plotter = new Plotter("Plotter",logLevel);




  for (unsigned int k=0; k<histos.size(); k++)
    {
    workCanvas->cd();
    TH2 * hClone1 = (TH2*) histos[k]->Clone();
    TH2 * hClone2 = (TH2*) histos[k]->Clone();
    histoName = eventClassNames[k];
    histoName += "_edge";
    histosEdge.push_back(  new TH2D(histoName,histoName, 200,-20.0, 20.0, 200,-20.0, 20.0) );
    histoName = eventClassNames[k];
    histoName += "_radius";
    histosRadius.push_back(  new TH2D(histoName,histoName, 200,-20.0, 20.0, 200,-20.0, 20.0) );

    histoName = eventClassNames[k];
    histoName += "_gx";
    histosGx.push_back(  new TH2D(histoName,histoName, 200,-20.0, 20.0, 200,-20.0, 20.0) );
    histoName = eventClassNames[k];
    histoName += "_gy";
    histosGy.push_back(  new TH2D(histoName,histoName, 200,-20.0, 20.0, 200,-20.0, 20.0) );

    vector<double> xArray;
    vector<double> yArray;
    vector<double> gxArray;
    vector<double> gyArray;

    analyzeGradients(hClone1,histosEdge[k],histosRadius[k], histosGx[k], histosGy[k], xArray, yArray, gxArray, gyArray,minimumFraction);
    canvasName = "CG_PbPb_";
    canvasName += eventClassNames[k];
    canvasName += "_gradients";
    plotter->plot(canvasName,landscapeLogZ,gc2D,
                  "x (fm)", -20.0, 20.0,
                  "y (fm)", -20.0, 20.0,
                  "Counts",  20.0, -20.0,
                  hClone2);
    vector<TArrow*> arrows;
    for (unsigned int j=0; j<xArray.size(); j++)
      {
      double x = xArray[j];
      double y = yArray[j];
      double dx = x + 2.0*gxArray[j];
      double dy = y + 2.0*gyArray[j];
      TArrow * arrow = new  TArrow(x,y,dx,dy,0.01);
      int color = j%4;
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

    canvasName = "CG_PbPb_";
    canvasName += eventClassNames[k];
    canvasName += "_edge";
    plotter->plot(canvasName,landscapeLogZ,gc2D,
                  "x (fm)", -20.0, 20.0,
                  "y (fm)", -20.0, 20.0,
                  "Counts",  20.0, -20.0,
                  histosEdge[k]);

    canvasName = "CG_PbPb_";
    canvasName += eventClassNames[k];
    canvasName += "_radius";
    plotter->plot(canvasName,landscapeLogZ,gc2D,
                  "x (fm)", -20.0, 20.0,
                  "y (fm)", -20.0, 20.0,
                  "Radius (fm)",  0.001, 1.1,
                  histosRadius[k]);
    }

  for (unsigned int k=0; k<histosGx.size(); k++)
    {
    histosEdge[k]->Write();
    histosRadius[k]->Write();
    histosGx[k]->Write();
    histosGy[k]->Write();
    }
  //outputFile->Close();
  outputPathName += "CG/";
  plotter->printAllCanvas(outputPathName);

  return 0;
}
