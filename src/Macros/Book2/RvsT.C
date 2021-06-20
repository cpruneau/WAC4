#include "BasicFunctions.C"

// Complute and plot the evolution of the universe scale R as a function of time
// for a pressureless dust universe.
//

double FlatUniverse(double *xx, double *par)
{
  double x = xx[0]; // time in units of hubble time
  double arg = 1.5 * t;
  return TMath::Power(arg,0.666666);
}



void NewtonianCosmoRvsTime()
{
  setDefaultOptions(0);
  float leftMargin   = 0.20;
  float topMargin    = 0.03;
  float rightMargin  = 0.03;
  float bottomMargin = 0.15;
  int   nXDivisions  = 6;
  float xTitleSize   = 0.08;
  float xTitleOffset = 0.8;
  float xLabelSize   = 0.07;
  float xLabelOffset = 0.01;
  int   nYDivisions  = 6;
  float yTitleSize   = 0.08;
  float yTitleOffset = 1.1;
  float yLabelSize   = 0.07;
  float yLabelOffset = 0.02;

  const TString xTitle = "#Delta t/t_H";
  const TString xTitle = "R";

  vector<TF1*>    functions;
  vector<TString> legends;
  vector<int> lineColor; lineColor.assign(3,1);
  vector<int> lineStyle; lineStyle.assign(3,1); lineStyle[1] = 2; lineStyle[2] = 4;
  vector<int> lineWidth; lineWidth.assign(3,2);

  double xMin = -1.0000;
  double xMax = 6.0.;
  double yMin = 0.0;
  double yMax = 6.0;

  TString canvasName = "NewtonianCosmoRvsTime";
  TCanvas * canvas = createCanvas(canvasName,2,2,700,500,leftMargin,topMargin,rightMargin,bottomMargin,0,1);

  vector<double> t;

  TString fctName = "R1";
  functions.push_back( new TF1("R1",FlatUniverse,xMin,xMax,1) );
  functions.push_back( new TF1("R2",OpenUniverse,xMin,xMax,1) );
  functions.push_back( new TF1("R3",CloseUniverse,xMin,xMax,1) );
  legends.push_back( "#Omega_0 = 1.0 (flat)");
  legends.push_back( "#Omega_0 = 0.5 (open)");
  legends.push_back( "#Omega_0 = 2.0 (close)");

  for (unsigned int k=0; k<3; k++)
    {
    setFunctionProperties(functions[k], 800, lineColor[k],lineStyle[k],lineWidth[k],
                          nXDivisions,xTitleSize,xTitleOffset,xLabelSize,xLabelOffset,xTitle,
                          nYDivisions,yTitleSize,yTitleOffset,yLabelSize,yLabelOffset,yTitle);
    if (k==0)
      {
      functions[k]->SetMinimum(yMin);
      functions[k]->SetMaximum(yMax);
      functions[k]->Draw();
      }
    else
      functions[k]->Draw("SAME");
    }
  
  //legend
  TLegend * legend = createLegend(0.3, 0.2, 0.6,0.5, 0, 0.05);
  for (int k=0; k<3; k++)legend->AddEntry(functions[k],legends[k]);
  legend->Draw();

  
  printCanvas(canvas_1, "./", 1, 1, 0, 0);
  
}
