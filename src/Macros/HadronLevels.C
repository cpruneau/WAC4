#include "TString.h"
#include "TRandom.h"
#include "TLine.h"


TLine * createLine(float x1, float y1, float x2, float y2, int style, int color, int width);
void drawLevels(int n, double *energies, double minX, double maxX, int style, int color, int width);

int HadronLevels()
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
  gSystem->Load(includesPath+"RandomGenerators.hpp");
  gSystem->Load(includesPath+"Plotter.hpp");
  gSystem->Load("libBase.dylib");

  CanvasConfiguration * canvasConfiguration = new CanvasConfiguration(CanvasConfiguration::Landscape,CanvasConfiguration::Linear);
  GraphConfiguration  ** graphConfigurations = new GraphConfiguration*[40];
  for (int iGraph=0;iGraph<40;iGraph++)
    {
    graphConfigurations[iGraph] = new GraphConfiguration(1,iGraph%10);
    }
  TString inputPath  = "/Users/claudeapruneau/Documents/GitHub/run/PythiaStudies/";
  TString outputPath = "/Users/claudeapruneau/Documents/GitHub/WAC-DATA/OutputFiles/";

  TString fileBaseName  = "HadronLevels_";
  TString histoName, histoTitle;
  TString fileName;

  Plotter * plotter = new Plotter("HadronLevels", MessageLogger::Info);
  plotter->setDefaultOptions(1);
  TH1 * h = new TH1F("h","h",5,0.0,5.0);
  graphConfigurations[0]->xLabelSize = 0.08;
  graphConfigurations[0]->xLabelOffset = 0.01;
  h->GetXaxis()->SetBinLabel(1,"N");
  h->GetXaxis()->SetBinLabel(2,"#Delta");
  h->GetXaxis()->SetBinLabel(3,"#Lambda");
  h->GetXaxis()->SetBinLabel(4,"#Xi");
  h->GetXaxis()->SetBinLabel(5,"#Omega");
  plotter->plot("EnergyLevels",
                canvasConfiguration,
                graphConfigurations[0],
                "", 0.0, 5.0,
                "E (MeV)", 0.0, 3.0,
                h,
                "",
                 2.0, 0.0, 2.2, 0.1,
                 0.0);

  double unstrangeBaryonLow = 0.0;
  double unstrangeBaryonHi = 0.5;
  double unstrangeBaryonModelLow = 0.5;
  double unstrangeBaryonModelHi = 1.0;
  int dataStyle  = 1;
  int dataColor  = 2;
  int dataWidth  = 3;
  int modelStyle = 2;
  int modelColor = 4;
  int modelWidth = 2;

  double nd[5] = { 0.938, 1.1, 1.5, 1.8, 2.1};
  double nm[5] = { 0.938, 1.15, 1.45, 1.83, 2.19};
  double ld[5] = { 1.2, 1.3, 1.5, 1.8, 2.1};
  double lm[5] = { 1.21, 1.35, 1.45, 1.83, 2.19};
  drawLevels(5, nd, 0.0, 0.5, dataStyle,dataColor,dataWidth);
  drawLevels(5, nm, 0.5, 1.0, modelStyle,modelColor,modelWidth);
  drawLevels(5, nd, 1.0, 1.5, dataStyle,dataColor,dataWidth);
  drawLevels(5, nm, 1.5, 2.0, modelStyle,modelColor,modelWidth);
  drawLevels(5, nd, 2.0, 2.5, dataStyle,dataColor,dataWidth);
  drawLevels(5, nm, 2.5, 3.0, modelStyle,modelColor,modelWidth);
  drawLevels(5, nd, 3.0, 3.5, dataStyle,dataColor,dataWidth);
  drawLevels(5, nm, 3.5, 4.0, modelStyle,modelColor,modelWidth);
  drawLevels(5, nd, 4.0, 4.5, dataStyle,dataColor,dataWidth);
  drawLevels(5, nm, 4.5, 5.0, modelStyle,modelColor,modelWidth);

  plotter->printAllCanvas(outputPath);

  return 0;
}


void drawLevels(int n, double *energies, double minX, double maxX, int style, int color, int width)
{
  TLine * line;
  for (int k=0; k<n; k++)
  {
  line =  createLine(minX, energies[k], maxX, energies[k], style,color,width);
  }
}

TLine * createLine(float x1, float y1, float x2, float y2, int style, int color, int width)
{
  TLine *line = new TLine(x1,y1,x2,y2);
  line->SetLineStyle(style);
  line->SetLineColor(color);
  line->SetLineWidth(width);
  line->Draw();
  return line;
}
