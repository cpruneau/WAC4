////////////////////////////////////////////////////
// Basic Function
////////////////////////////////////////////////////

//namespace ROOT::Math;
#include "TSystem.h"
#include "TStyle.h"
#include "TMath.h"
#include "TPad.h"
#include "TF1.h"
#include "TF2.h"
#include "TRandom.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TCanvas.h"
#include "TString.h"
#include "TLatex.h"
#include "TLine.h"
#include "TArrow.h"
#include "TLegend.h"

////////////////////////////////////////////////////
// Create a new directory
////////////////////////////////////////////////////
void createDirectory(const TString & dirName)
{
  gSystem->mkdir(dirName,1);
}

////////////////////////////////////////////////////
// Set Default Style for Plots
////////////////////////////////////////////////////
void setDefaultOptions(bool color=0)
{
  if (color)
    gStyle->SetPalette(1,0);
  else
    gStyle->SetPalette(7,0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  gStyle->SetOptDate(0);
  gStyle->SetOptTitle(0);
  gStyle->SetPadBorderMode(0);
  
}

////////////////////////////////////////////////////
// Create a canvas
////////////////////////////////////////////////////
TCanvas * createCanvas(const TString & canvasName,
                       int x, int y, int xSize, int ySize,
                       float leftMargin, float topMargin, float rightMargin, float bottomMargin,
                       int logX=0, int logY=0, int logZ=0,
                       float theta=45, float phi=55)
{
  TCanvas * canvas = new TCanvas(canvasName,canvasName,x,y,xSize,ySize);
  canvas->SetLogx(logX);
  canvas->SetLogy(logY);
  canvas->SetLogz(logZ);
  canvas->SetRightMargin(rightMargin);
  canvas->SetLeftMargin(leftMargin);
  canvas->SetBottomMargin(bottomMargin);
  canvas->SetTopMargin(topMargin);
  canvas->SetTheta(theta);
  canvas->SetPhi(phi);
  
  canvas->SetFillColor(kWhite);
  canvas->SetFillStyle(1001);
  canvas->SetBorderSize(0);
  canvas->SetBorderMode(0);
  
  return canvas;
}

////////////////////////////////////////////////////
// Create a pad
////////////////////////////////////////////////////
TPad * createPad(const TString & name, const TString title,
                 float x1, float y1, float x2, float y2,
                 float leftMargin,
                 float topMargin,
                 float rightMargin,
                 float bottomMargin,
                 int   fillColor=0,
                 int   fillStyle=0,
                 bool  doDraw=true)
{
  TPad* pad = new TPad(name, title, x1, y1, x2, y2);
  pad->SetLeftMargin(leftMargin);
  pad->SetRightMargin(rightMargin);
  pad->SetTopMargin(topMargin);
  pad->SetBottomMargin(bottomMargin);
  pad->SetFillColor(fillColor);
  pad->SetFillStyle(fillStyle);
  pad->SetTicks();
  if (doDraw)
  {
    pad->Draw();
    pad->cd(0);
  }
  return pad;
}

////////////////////////////////////////////////////
// Create a label within an existing pad
////////////////////////////////////////////////////
TLatex * createLabel(double x, double y, int color, int fontType, double fontSize, const TString & text, bool doDraw=true)
{
  TLatex * label;
  label = new TLatex(x,y,text);
  label->SetTextColor(color);
  //label->SetTextFont(fontType);
  label->SetTextSize(fontSize);
  if (doDraw) label->Draw();
  return label;
}

////////////////////////////////////////////////////
// Create standard legend
////////////////////////////////////////////////////
TLegend * createLegend(float x1, float y1, float x2, float y2, int fontType, float fontSize)
{
  TLegend *legend = new TLegend(x1,y1,x2,y2);
  //legend->SetTextFont(fontType);
  legend->SetTextSize(fontSize);
  legend->SetFillColor(0);
  legend->SetBorderSize(0);
  return legend;
}

////////////////////////////////////////////////////
// Create simple line
////////////////////////////////////////////////////
TLine * createLine(float x1, float y1, float x2, float y2, int style, int color, int width, bool doDraw=true)
{
  TLine *line = new TLine(x1,y1,x2,y2);
  line->SetLineStyle(style);
  line->SetLineColor(color);
  line->SetLineWidth(width);
  line->Draw();
  return line;
}

////////////////////////////////////////////////////
// Create Arrow Line
////////////////////////////////////////////////////
TArrow * createArrow(float x1, float y1, float x2, float y2, float arrowSize, Option_t* option, int style, int color, int width, bool doDraw=true)
{
  TArrow *line = new TArrow(x1,y1,x2,y2,arrowSize,option);
  line->SetLineStyle(style);
  line->SetLineColor(color);
  line->SetLineWidth(width);
  line->Draw();
  return line;
}


////////////////////////////////////////////////////
// Set (1D) Histrogram Plotting Attributes
////////////////////////////////////////////////////
void setHistoProperties(TH1 * h, int lineColor, int lineStyle, int lineWidth,
                        int nXDivisions, float xTitleSize, float xTitleOffset, float xLabelSize, float xLabelOffset, const TString & xTitle,
                        int nYDivisions, float yTitleSize, float yTitleOffset, float yLabelSize, float yLabelOffset, const TString & yTitle)
{
  cout << "Setting properties of histo: " << h->GetName() << endl;
  h->SetLineColor(lineColor);
  h->SetLineStyle(lineStyle);
  h->SetLineWidth(lineWidth);
  TAxis * xAxis = (TAxis *) h->GetXaxis();
  TAxis * yAxis = (TAxis *) h->GetYaxis();
  xAxis->SetNdivisions(nXDivisions);
  xAxis->SetTitleSize(xTitleSize);
  xAxis->SetTitleOffset(xTitleOffset);
  xAxis->SetTitle(xTitle);
  xAxis->SetLabelSize(xLabelSize);
  xAxis->SetLabelOffset(xLabelOffset);
  yAxis->SetNdivisions(nYDivisions);
  yAxis->SetTitleSize(yTitleSize);
  yAxis->SetTitleOffset(yTitleOffset);
  yAxis->SetLabelSize(yLabelSize);
  yAxis->SetLabelOffset(yLabelOffset);
  yAxis->SetTitle(yTitle);
}

////////////////////////////////////////////////////
// Set (2D) Histrogram Plotting Attributes
////////////////////////////////////////////////////
void setHistoProperties(TH2 * h,
                        int nXDivisions, float xTitleSize, float xTitleOffset, float xLabelSize, float xLabelOffset, const TString & xTitle,
                        int nYDivisions, float yTitleSize, float yTitleOffset, float yLabelSize, float yLabelOffset, const TString & yTitle,
                        int nZDivisions, float zTitleSize, float zTitleOffset, float zLabelSize, float zLabelOffset, const TString & zTitle)
{
  cout << "Setting properties of histo: " << h->GetName() << endl;
  TAxis * xAxis = (TAxis *) h->GetXaxis();
  TAxis * yAxis = (TAxis *) h->GetYaxis();
  TAxis * zAxis = (TAxis *) h->GetZaxis();
  xAxis->SetNdivisions(nXDivisions);
  xAxis->SetTitleSize(xTitleSize);
  xAxis->SetTitleOffset(xTitleOffset);
  xAxis->SetTitle(xTitle);
  xAxis->SetLabelSize(xLabelSize);
  xAxis->SetLabelOffset(xLabelOffset);
  
  yAxis->SetNdivisions(nYDivisions);
  yAxis->SetTitleSize(yTitleSize);
  yAxis->SetTitleOffset(yTitleOffset);
  yAxis->SetTitle(yTitle);
  yAxis->SetLabelSize(yLabelSize);
  yAxis->SetLabelOffset(yLabelOffset);
  
  zAxis->SetNdivisions(nZDivisions);
  zAxis->SetTitleSize(zTitleSize);
  zAxis->SetTitleOffset(zTitleOffset);
  zAxis->SetTitle(zTitle);
  zAxis->SetLabelSize(zLabelSize);
  zAxis->SetLabelOffset(zLabelOffset);
}

////////////////////////////////////////////////////
// Set (1D) Function Plotting Attributes
////////////////////////////////////////////////////
void setFunctionProperties(TF1 * f, int npx, int lineColor, int lineStyle, int lineWidth,
                           int nXDivisions, float xTitleSize, float xTitleOffset, float xLabelSize, float xLabelOffset, const TString & xTitle,
                           int nYDivisions, float yTitleSize, float yTitleOffset, float yLabelSize, float yLabelOffset, const TString & yTitle)
{
  cout << "Setting properties of function: " << f->GetName() << endl;
  //TAxis * xAxis = (TAxis *) f->GetXaxis();
  //TAxis * yAxis = (TAxis *) f->GetYaxis();
  
  f->SetNpx(npx);
  f->SetLineColor(lineColor);
  f->SetLineStyle(lineStyle);
  f->SetLineWidth(lineWidth);
  
  f->GetXaxis()->SetNdivisions(nXDivisions);
  f->GetXaxis()->SetTitleSize(xTitleSize);
  f->GetXaxis()->SetTitleOffset(xTitleOffset);
  f->GetXaxis()->SetTitle(xTitle);
  f->GetXaxis()->SetLabelSize(xLabelSize);
  f->GetXaxis()->SetLabelOffset(xLabelOffset);
  f->GetYaxis()->SetNdivisions(nYDivisions);
  f->GetYaxis()->SetTitleSize(yTitleSize);
  f->GetYaxis()->SetTitleOffset(yTitleOffset);
  f->GetYaxis()->SetTitle(yTitle);
  f->GetYaxis()->SetLabelSize(yLabelSize);
  f->GetYaxis()->SetLabelOffset(yLabelOffset);
  cout << "Done" << endl;
}

////////////////////////////////////////////////////
// Set (1D) Function Plotting Attributes
////////////////////////////////////////////////////
void setFunctionProperties(TF2 * f, int npx, int npy,
                           int nXDivisions, float xTitleSize, float xTitleOffset, float xLabelSize, float xLabelOffset, const TString & xTitle,
                           int nYDivisions, float yTitleSize, float yTitleOffset, float yLabelSize, float yLabelOffset, const TString & yTitle,
                           int nZDivisions, float zTitleSize, float zTitleOffset, float zLabelSize, float zLabelOffset, const TString & zTitle)
{
  cout << "Setting properties of function: " << f->GetName() << endl;
  f->SetNpx(npx);
  f->SetNpx(npy);

  f->GetXaxis()->SetNdivisions(nXDivisions);
  f->GetXaxis()->SetTitleSize(xTitleSize);
  f->GetXaxis()->SetTitleOffset(xTitleOffset);
  f->GetXaxis()->SetTitle(xTitle);
  f->GetXaxis()->SetLabelSize(xLabelSize);
  f->GetXaxis()->SetLabelOffset(xLabelOffset);
  
  f->GetYaxis()->SetNdivisions(nYDivisions);
  f->GetYaxis()->SetTitleSize(yTitleSize);
  f->GetYaxis()->SetTitleOffset(yTitleOffset);
  f->GetYaxis()->SetTitle(yTitle);
  f->GetYaxis()->SetLabelSize(yLabelSize);
  f->GetYaxis()->SetLabelOffset(yLabelOffset);
  
  f->GetZaxis()->SetNdivisions(nZDivisions);
  f->GetZaxis()->SetTitleSize(zTitleSize);
  f->GetZaxis()->SetTitleOffset(zTitleOffset);
  f->GetZaxis()->SetTitle(zTitle);
  f->GetZaxis()->SetLabelSize(zLabelSize);
  f->GetZaxis()->SetLabelOffset(zLabelOffset);
  
}

////////////////////////////////////////////////////
// Set (1D) Graph Attributes
////////////////////////////////////////////////////
void setGraphProperties(TGraph * g, int lineColor, int lineStyle, int lineWidth, int markerColor, int markerStyle, float markerSize,
                           int nXDivisions, float xTitleSize, float xTitleOffset, float xLabelSize, float xLabelOffset, const TString & xTitle,
                           int nYDivisions, float yTitleSize, float yTitleOffset, float yLabelSize, float yLabelOffset, const TString & yTitle)
{
  cout << "Setting properties of graph: " << endl;
  
  g->SetLineColor(lineColor);
  g->SetLineStyle(lineStyle);
  g->SetLineWidth(lineWidth);
  g->SetMarkerColor(markerColor);
  g->SetMarkerStyle(markerStyle);
  g->SetMarkerSize(markerSize);
  g->SetFillColor(0);
  
  g->GetXaxis()->SetNdivisions(nXDivisions);
  g->GetXaxis()->SetTitleSize(xTitleSize);
  g->GetXaxis()->SetTitleOffset(xTitleOffset);
  g->GetXaxis()->SetTitle(xTitle);
  g->GetXaxis()->SetLabelSize(xLabelSize);
  g->GetXaxis()->SetLabelOffset(xLabelOffset);
  g->GetYaxis()->SetNdivisions(nYDivisions);
  g->GetYaxis()->SetTitleSize(yTitleSize);
  g->GetYaxis()->SetTitleOffset(yTitleOffset);
  g->GetYaxis()->SetTitle(yTitle);
  g->GetYaxis()->SetLabelSize(yLabelSize);
  g->GetYaxis()->SetLabelOffset(yLabelOffset);
  cout << "Done" << endl;
}


////////////////////////////////////////////////////
// Print Canvas
////////////////////////////////////////////////////
void printCanvas(TCanvas * canvas, const TString & directoryName="./", bool printGif=0, bool printPdf=1, bool printSvg=0, bool printC=0)
{
  createDirectory(directoryName);
  TString fileName = directoryName + canvas->GetName();
  if (printGif) canvas->Print(fileName+".gif");
  if (printPdf) canvas->Print(fileName+".pdf");
  if (printSvg) canvas->Print(fileName+".svg");
  if (printC)   canvas->Print(fileName+".C");
}

////////////////////////////////////////////////////
// 2nd order polynomial
////////////////////////////////////////////////////
double Poly2(double *xx, double *par)
{
  double x = xx[0];
  double a = par[0];
  double b = par[1];
  double c = par[2];
  return a*x*x+b*x+c;
}


////////////////////////////////////////////////////
// Uniform Distribution (a,b)
////////////////////////////////////////////////////
double Uniform(double *xx, double *par)
{
  double x = xx[0];
  double a = par[0];
  double b = par[1];
  double v = par[2];
  if (x<a)
    return 0;
  else if (x<b)
    return v;
  else
    return 0;
}

////////////////////////////////////////////////////
// Cumulative Integral of Uniform Distribution (a,b)
////////////////////////////////////////////////////
double CumulativeUniform(double *xx, double *par)
{
  double x = xx[0];
  double a = par[0];
  double b = par[1];
  double v = par[2];
  if (x<a)
    return 0;
  else if (x<b)
    return (x-a)/(b-a);
  else
    return 1;
}

////////////////////////////////////////////////////
// Log Normal Distribution (mu,sigma)
////////////////////////////////////////////////////
double LogNormal(double *xx, double *par)
{
  double x = xx[0];
  double mu = par[0];
  double sigma = par[1];
  double arg = (log(x)-mu)/sigma;
  double v = exp(-0.5*arg*arg)/(x*sqrt(2.*3.1415927)*sigma);
  return v;
}


////////////////////////////////////////////////////
// Exponential Distribution (lambda)
////////////////////////////////////////////////////
double Exponential(double *xx, double *par)
{
  double x = xx[0];
  double lambda = par[0];
  double v = exp(-x/lambda)/lambda;
  return v;
}


////////////////////////////////////////////////////
// Gamma Distribution
////////////////////////////////////////////////////
double GammaDistribution(double *xx, double *par)
{
  double x = xx[0];
  double alpha = par[0];
  double beta  = par[1];
  double v = TMath::GammaDist(x, alpha, 0.0, beta);
  return v;
}


////////////////////////////////////////////////////
// Beta Distribution
////////////////////////////////////////////////////
double BetaDistribution(double *xx, double *par)
{
  double x = xx[0];
  double alpha = par[0];
  double beta  = par[1];
  double v = TMath::BetaDist(x, alpha, beta);
  return v;
}




////////////////////////////////////////////////////
// Gaussian Distribution (mu1, sigma1)
////////////////////////////////////////////////////
double Gaussian(double *xx, double *par)
{
  double x      = xx[0];
  double mu1    = par[0];
  double sigma1 = par[1];
  double v;
  double arg1 = (x-mu1)/sigma1;
  v = exp(-0.5*arg1*arg1)/(sqrt(2*3.1415927)*sigma1);
  return v;
}


////////////////////////////////////////////////////
// Gaussian Distribution (mu1, sigma1)
////////////////////////////////////////////////////
double polGaussian(double *xx, double *par)
{
  double x      = xx[0];
  double amp    = par[0];
  double mu1    = par[1];
  double sigma1 = par[2];
  double a0     = par[3];
  //cout << "mu:" << mu1 << endl;
  //double a1     = par[4];
  double v;
  double arg1 = (x-mu1)/sigma1;
  //v = a0+a1*x+(amp*exp(-0.5*arg1*arg1)/sqrt(2*3.1415927)/sigma1);
  v = a0+amp*exp(-0.5*arg1*arg1)/(sqrt(2*3.1415927)/sigma1);
  return v;
}


////////////////////////////////////////////////////
// Double Gaussian Distribution (mu1, sigma1, mu2, sigma2)
////////////////////////////////////////////////////
double DoubleGaussian(double *xx, double *par)
{
  double x = xx[0];
  double mu1    = par[0];
  double sigma1 = par[1];
  double mu2    = par[2];
  double sigma2 = par[3];
  double f      = par[4];
  double v1;
  double v2;
  double v;
  double arg1 = (x-mu1)/sigma1;
  v1 = exp(-0.5*arg1*arg1);
  double arg2 = (x-mu2)/sigma2;
  v2 = exp(-0.5*arg2*arg2);
  v = (f*v1+(1-f)*v2)/(sqrt(2*3.1415927)*(f*sigma1+(1-f)*sigma2));
  return v;
}

////////////////////////////////////////////////////
// x * Double Gaussian Distribution (mu1, sigma1, mu2, sigma2)
////////////////////////////////////////////////////
double xDoubleGaussian(double *xx, double *par)
{
  double x = xx[0];
  double mu1    = par[0];
  double sigma1 = par[1];
  double mu2    = par[2];
  double sigma2 = par[3];
  double f      = par[4];
  double v1;
  double v2;
  double v;
  double arg1 = (x-mu1)/sigma1;
  v1 = exp(-0.5*arg1*arg1);
  double arg2 = (x-mu2)/sigma2;
  v2 = exp(-0.5*arg2*arg2);
  v = x*(f*v1+(1-f)*v2)/(sqrt(2*3.1415927)*(f*sigma1+(1-f)*sigma2));
  return v;
}

////////////////////////////////////////////////////
// Asymetric Gaussian Distribution (mu.sigma1, sigma2)
////////////////////////////////////////////////////
double AssymetricGaussian(double *xx, double *par)
{
  double x = xx[0];
  double mu = par[0];
  double sigma1 = par[1];
  double sigma2 = par[2];
  double v;
  double arg;
  if (x<mu)
    {
    arg = (x-mu)/sigma1;
    v = 2.*exp(-0.5*arg*arg)/sqrt(2*3.1415927)/(sigma1+sigma2);
    }
  else
    {
    arg = (x-mu)/sigma2;
    v = 2.*exp(-0.5*arg*arg)/sqrt(2*3.1415927)/(sigma1+sigma2);
    }
  return v;
}

////////////////////////////////////////////////////
// x* Asymetric Gaussian Distribution (mu.sigma1, sigma2)
////////////////////////////////////////////////////
double xAssymetricGaussian(double *xx, double *par)
{
  double x = xx[0];
  double mu = par[0];
  double sigma1 = par[1];
  double sigma2 = par[2];
  double v;
  double arg;
  if (x<mu)
    {
    arg = (x-mu)/sigma1;
    v = 2.*x*exp(-0.5*arg*arg)/sqrt(2*3.1415927)/(sigma1+sigma2);
    }
  else
    {
    arg = (x-mu)/sigma2;
    v = 2.*x*exp(-0.5*arg*arg)/sqrt(2*3.1415927)/(sigma1+sigma2);
    }
  return v;
}

////////////////////////////////////////////////////
// ChiSquare Distribution (n)
////////////////////////////////////////////////////
double ChiSquare(double *xx, double *par)
{
  double z = xx[0];
  double n = par[0];
  double arg = -0.5*n*log(2) -log( TMath::Gamma(n/2)) + (0.5*n - 1)*log(z) - z/2;
  double v = exp(arg);
  return v;
}


////////////////////////////////////////////////////
// ChiSquare Per DoF Distribution (n)
////////////////////////////////////////////////////
double ChiSquarePerDof(double *xx, double *par)
{
  double n = par[0];
  double z = n*xx[0];
  double arg = -0.5*n*log(2) -log( TMath::Gamma(n/2)) + (0.5*n - 1)*log(z) - z/2;
  double v = n*exp(arg);
  return v;
}


////////////////////////////////////////////////////
// Breit Wigner Per DoF Distribution (n)
////////////////////////////////////////////////////
double BreitWigner(double *xx, double *par)
{
  double x     = xx[0];
  double x0    = par[0];
  double halfGamma = par[1]/2.;
  double arg = x-x0;
  double v = halfGamma/3.1415927/(halfGamma*halfGamma + arg*arg);
  return v;
}


////////////////////////////////////////////////////
// Maxwell Boltzmann
////////////////////////////////////////////////////
double MaxwellBoltzman(double *xx, double *par)
{
  double x = xx[0];
  double a = par[0];
  double a2 = a*a;
  double a3 = a*a2;
  double v = sqrt(2/3.1415927)*x*x*exp(-x*x/2/a2)/a3;
  return v;
}


////////////////////////////////////////////////////
// Rapidity (of beam)
// vs p of beam along z axis
////////////////////////////////////////////////////
double Rapidity(double *xx, double *par)
{
  double p = xx[0];
  double mass = par[0];
  double e = sqrt(p*p+mass*mass);
  double r = 0.5*log((e+p)/(e-p));
  if (fabs(p-100)<1) cout << p << "  " << r << endl;
  if (fabs(p-450)<1) cout << p << "  " << r << endl;
  if (fabs(p-7000)<100) cout << p << "  " << r << endl;
  return r;
}



////////////////////////////////////////////////////
// Weibull Distribution 
////////////////////////////////////////////////////
double Weibull(double *xx, double *par)
{
  double x     = xx[0];
  if (x<0) return 0;
  double sigma = par[0];
  double eta   = par[1];
  if (sigma<=0) return 0;
  double arg = x/sigma;
  double lnarg = log(arg);
  double v = exp(log(eta)-log(sigma)+(eta-1)*lnarg - exp(eta*lnarg));
  return v;
}


////////////////////////////////////////////////////
// t-Student Distribution (n)
////////////////////////////////////////////////////
double tStudent(double *xx, double *par)
{
  double x     = xx[0];
  double nu    = par[0];
  double arg   = -0.5*(nu+1)*log(1+x*x/nu);
  double gg    = TMath::Gamma((nu+1)/2)/TMath::Gamma(nu/2);
  double v     = gg*exp(arg)/sqrt(nu*3.1415927);
  return v;
}

////////////////////////////////////////////////////
// Binomial Distribution
////////////////////////////////////////////////////
double BinomialDist(double n, double N, double p)
{
  double arg   = n*log(p) + (N-n)*log(1-p);
  double v     = TMath::Binomial(N,n)*exp(arg);
  return v;
}


////////////////////////////////////////////////////
// Round Distribution
////////////////////////////////////////////////////
double RoundDist(double *xx, double *par)
{
  double x     = TMath::Abs(xx[0]);
  double norm    = par[0];
  double v;
  if (x<1)
    {
    v = norm*(1-0.5*x*x*x*x);
    }
  else
    {
    v = norm*TMath::Gaus(x,0.,0.5,2);
    }
  return v;
}


double CorrelationModel(double *xx, double *par)
{
  double eta1  = xx[0];
  double eta2  = xx[1];
  double sigmaAvg = par[0];
  double sigmaDif = par[1];
  double dEta = eta1-eta2;
  double etaAvg = 0.5*(eta1+eta2);
  double arg3 = dEta/sigmaDif;
  double arg4 = etaAvg/sigmaAvg;
  return 1000*exp(-0.5*arg3*arg3)*exp(-0.5*arg4*arg4)/(2.*3.1415927*sigmaDif*sigmaAvg);
}



double RapidityJacobian(double *xx, double *par)
{
  double y      = xx[0];
  double ratio  = par[0];
  double coshy  = cosh(y);
  double arg    = (1+ratio*ratio)*coshy*coshy;
  return sqrt(1-1./arg);
}



double LandauDist(double *xx, double *par)
{
  double x      = xx[0];
  double mpv    = par[0];
  double sigma  = par[1];
  //double r =  exp(-(x-mpv)*(x-mpv)/2/sigma/sigma);
  //cout << r << endl;
  double r = TMath::Landau(x, mpv, sigma,0);
  return r;
}

// Calculate the dE/dx vs beta*gamma
// x: beta*gamma
// k1: constant = Z/A
// k2: constant
// K/A = 0.307075 MeV g^-1 cm^2 for A=1 g mol^-1
// mass_e = mass of electron in MeV.
double BetheBlochBetaGamma(double *xx, double *par)
{
  double K      = 0.307075;
  double mass_e = 0.5109;
  double z2     = 1;//par[0]; // square of charge of projectile
  double Z      = 29;
  double A      = 63;
  double ZA     = Z/A; //par[1]; // Z/A of target Copper
  //double I      = 0.000322;//copper par[2]; // average ionizaion potential 322 for Cu
  double I      = 10*Z/1000000.; //
  double I2     = I*I;
  double mass_A = 0.938*A;
  double massRatio = mass_e;

  double betaGamma   = xx[0];
  double betaGammaSq = betaGamma*betaGamma;
  double betaSq = betaGammaSq/(1+betaGammaSq);
  double gamma  = betaGamma/sqrt(betaSq);
  double Tmax   = 2*mass_e*betaGammaSq/(1+2*gamma*massRatio+massRatio*massRatio);
  double v = K*z2*ZA*(0.5*log(2*mass_e*betaGammaSq*Tmax/I2) - betaSq)/betaSq;
  return v;
}

// Calculate the dE/dx vs P
// p:  momentum
// x:  bet*gamma
// k1: constant
// k2: constant
//
double BetheBlochP(double *xx, double *par)
{
  double K      = 0.307075;
  double mass_e = 0.5109;
  double z2     = 1;//par[0]; // square of charge of projectile
  double Z      = 29;
  double A      = 63;
  double ZA     = Z/A; //par[1]; // Z/A of target Copper
  //double I      = 0.000322;//copper par[2]; // average ionizaion potential 322 for Cu
  double I      = 10*Z/1000000.; //
  double I2     = I*I;
  double mass_A = 0.938*A;
  double massRatio = mass_e;
  
  double p           = xx[0];
  double mass_proj   = par[0];
  double betaGamma   = p/mass_proj;
  double betaGammaSq = betaGamma*betaGamma;
  double betaSq = betaGammaSq/(1+betaGammaSq);
  double gamma  = betaGamma/sqrt(betaSq);
  double Tmax   = 2*mass_e*betaGammaSq/(1+2*gamma*massRatio+massRatio*massRatio);
  double v = K*z2*ZA*(0.5*log(2*mass_e*betaGammaSq*Tmax/I2) - betaSq)/betaSq;
  return v/1.36; // units of MIP


}


// Calculate a relative error for a product of two quantities (assumed
// uncorrelated.
double relError(double v1, double ev1, double v2, double ev2)
{
  double re1, re2;
  if (v1>0)
    re1 = ev1/v1;
  else
    re1 = 0;
  if (v2>0)
    re2 = ev2/v2;
  else
    re2 = 0;
  return sqrt(re1*re1+re2*re2);
}

////////////////////////////////////////////////////
// Class CanvasConfig
////////////////////////////////////////////////////
class CanvasConfig
{
public:
  
  CanvasConfig()
  :
  x(20),
  y(20),
  width(700),
  height(400),
  leftMargin(0.25),
  topMargin(0.03),
  rightMargin(0.03),
  bottomMargin(0.2),
  logx(0),
  logy(0),
  logz(0),
  theta(45.0),
  phi(55.0)
  {}
  
  CanvasConfig(const CanvasConfig & canvasConfig)
  :
  x(canvasConfig.x),
  y(canvasConfig.y),
  width(canvasConfig.width),
  height(canvasConfig.height),
  leftMargin(canvasConfig.leftMargin),
  topMargin(canvasConfig.topMargin),
  rightMargin(canvasConfig.rightMargin),
  bottomMargin(canvasConfig.bottomMargin),
  logx(canvasConfig.logx),
  logy(canvasConfig.logy),
  logz(canvasConfig.logz),
  theta(canvasConfig.theta),
  phi(canvasConfig.phi)
  {}
  
  int x;
  int y;
  int width;
  int height;

  float leftMargin;
  float topMargin;
  float rightMargin;
  float bottomMargin;
  
  bool logx;
  bool logy;
  bool logz;
  
  float theta;
  float phi;
  
};


////////////////////////////////////////////////////
// Create a canvas
////////////////////////////////////////////////////
TCanvas * createCanvas(const TString & canvasName, const CanvasConfig & canvasConfig, int inc=0)
{
  TCanvas * canvas = new TCanvas(canvasName,canvasName,canvasConfig.x+inc*20,canvasConfig.y,canvasConfig.width,canvasConfig.height);
  canvas->SetLogx(canvasConfig.logx);
  canvas->SetLogy(canvasConfig.logy);
  canvas->SetLogz(canvasConfig.logz);
  canvas->SetRightMargin(canvasConfig.rightMargin);
  canvas->SetLeftMargin(canvasConfig.leftMargin);
  canvas->SetBottomMargin(canvasConfig.bottomMargin);
  canvas->SetTopMargin(canvasConfig.topMargin);
  canvas->SetTheta(canvasConfig.theta);
  canvas->SetPhi(canvasConfig.phi);
  
  canvas->SetFillColor(kWhite);
  canvas->SetFillStyle(1001);
  canvas->SetBorderSize(0);
  canvas->SetBorderMode(0);
  
  return canvas;
}


////////////////////////////////////////////////////
// Class GraphConfig
////////////////////////////////////////////////////
class GraphConfig
{
  public:
  
  GraphConfig()
:
  npx(100),
  npy(100),
  lineColor(1),
  lineStyle(1),
  lineWidth(1),
  markerColor(1),
  markerStyle(20),
  markerSize(1.1),
nXDivisions(4),
xTitleSize(0.07),
xTitleOffset(1.1),
xLabelSize(0.06),
xLabelOffset(0.01),
xTitle("x"),
nYDivisions(4),
yTitleSize(0.07),
yTitleOffset(1.1),
yLabelSize(0.06),
yLabelOffset(0.01),
yTitle("y"),
nZDivisions(4),
zTitleSize(0.07),
zTitleOffset(1.1),
zLabelSize(0.06),
zLabelOffset(0.01),
zTitle("z")
  {
  
  }
  
  int npx;
  int npy;
  
  int lineColor;
  int lineStyle;
  int lineWidth;
  
  int markerColor;
  int markerStyle;
  float markerSize;
  
  int   nXDivisions;
  float xTitleSize;
  float xTitleOffset;
  float xLabelSize;
  float xLabelOffset;
  const TString xTitle;
  
  int   nYDivisions;
  float yTitleSize;
  float yTitleOffset;
  float yLabelSize;
  float yLabelOffset;
  const TString yTitle;
  
  int   nZDivisions;
  float zTitleSize;
  float zTitleOffset;
  float zLabelSize;
  float zLabelOffset;
  const TString zTitle;
  
};



void setHistoProperties(TH1 * h, const GraphConfig & graphConfig)
{
  cout << "Setting properties of histo: " << h->GetTitle() << endl;
  h->SetLineColor(graphConfig.lineColor);
  h->SetLineStyle(graphConfig.lineStyle);
  h->SetLineWidth(graphConfig.lineWidth);
  TAxis * xAxis = (TAxis *) h->GetXaxis();
  xAxis->SetNdivisions(graphConfig.nXDivisions);
  xAxis->SetTitleSize(graphConfig.xTitleSize);
  xAxis->SetTitleOffset(graphConfig.xTitleOffset);
  //xAxis->SetTitle(graphConfig.xTitle);
  xAxis->SetLabelSize(graphConfig.xLabelSize);
  xAxis->SetLabelOffset(graphConfig.xLabelOffset);
  TAxis * yAxis = (TAxis *) h->GetYaxis();
  yAxis->SetNdivisions(graphConfig.nYDivisions);
  yAxis->SetTitleSize(graphConfig.yTitleSize);
  yAxis->SetTitleOffset(graphConfig.yTitleOffset);
  yAxis->SetLabelSize(graphConfig.yLabelSize);
  yAxis->SetLabelOffset(graphConfig.yLabelOffset);
  //yAxis->SetTitle(graphConfig.yTitle);
}


void setHistoProperties(TH1 * h, const GraphConfig & graphConfig, const TString & xTitle, const TString & yTitle)
{
  cout << "Setting properties of histo: " << h->GetTitle() << endl;
  h->SetLineColor(graphConfig.lineColor);
  h->SetLineStyle(graphConfig.lineStyle);
  h->SetLineWidth(graphConfig.lineWidth);
  TAxis * xAxis = (TAxis *) h->GetXaxis();
  xAxis->SetNdivisions(graphConfig.nXDivisions);
  xAxis->SetTitleSize(graphConfig.xTitleSize);
  xAxis->SetTitleOffset(graphConfig.xTitleOffset);
  xAxis->SetTitle(xTitle);
  xAxis->SetLabelSize(graphConfig.xLabelSize);
  xAxis->SetLabelOffset(graphConfig.xLabelOffset);
  TAxis * yAxis = (TAxis *) h->GetYaxis();
  yAxis->SetNdivisions(graphConfig.nYDivisions);
  yAxis->SetTitleSize(graphConfig.yTitleSize);
  yAxis->SetTitleOffset(graphConfig.yTitleOffset);
  yAxis->SetLabelSize(graphConfig.yLabelSize);
  yAxis->SetLabelOffset(graphConfig.yLabelOffset);
  yAxis->SetTitle(yTitle);
}

void setHistoProperties(TH2 * h, const GraphConfig & graphConfig, const TString & xTitle, const TString & yTitle, const TString & zTitle)
{
  cout << "Setting properties of histo: " << h->GetTitle() << endl;
  TAxis * xAxis = (TAxis *) h->GetXaxis();
  xAxis->SetNdivisions(graphConfig.nXDivisions);
  xAxis->SetTitleSize(graphConfig.xTitleSize);
  xAxis->SetTitleOffset(graphConfig.xTitleOffset);
  xAxis->SetTitle(xTitle);
  xAxis->SetLabelSize(graphConfig.xLabelSize);
  xAxis->SetLabelOffset(graphConfig.xLabelOffset);
  TAxis * yAxis = (TAxis *) h->GetYaxis();
  yAxis->SetNdivisions(graphConfig.nYDivisions);
  yAxis->SetTitleSize(graphConfig.yTitleSize);
  yAxis->SetTitleOffset(graphConfig.yTitleOffset);
  yAxis->SetLabelSize(graphConfig.yLabelSize);
  yAxis->SetLabelOffset(graphConfig.yLabelOffset);
  yAxis->SetTitle(yTitle);
  TAxis * zAxis = (TAxis *) h->GetZaxis();
  zAxis->SetNdivisions(graphConfig.nZDivisions);
  zAxis->SetTitleSize(graphConfig.zTitleSize);
  zAxis->SetTitleOffset(graphConfig.zTitleOffset);
  zAxis->SetLabelSize(graphConfig.zLabelSize);
  zAxis->SetLabelOffset(graphConfig.zLabelOffset);
  zAxis->SetTitle(zTitle);
}


void setGraphProperties(TGraph * g, const GraphConfig & graphConfig)
{
  cout << "Setting properties of graph: " << g->GetTitle() << endl;
  g->SetLineColor(graphConfig.lineColor);
  g->SetLineStyle(graphConfig.lineStyle);
  g->SetLineWidth(graphConfig.lineWidth);
  g->SetMarkerColor(graphConfig.markerColor);
  g->SetMarkerStyle(graphConfig.markerStyle);
  g->SetMarkerSize(graphConfig.markerSize);
  g->SetFillColor(0);
  TAxis * xAxis = (TAxis *) g->GetXaxis();
  xAxis->SetNdivisions(graphConfig.nXDivisions);
  xAxis->SetTitleSize(graphConfig.xTitleSize);
  xAxis->SetTitleOffset(graphConfig.xTitleOffset);
  xAxis->SetTitle(graphConfig.xTitle);
  xAxis->SetLabelSize(graphConfig.xLabelSize);
  xAxis->SetLabelOffset(graphConfig.xLabelOffset);
  TAxis * yAxis = (TAxis *) g->GetYaxis();
  yAxis->SetNdivisions(graphConfig.nYDivisions);
  yAxis->SetTitleSize(graphConfig.yTitleSize);
  yAxis->SetTitleOffset(graphConfig.yTitleOffset);
  yAxis->SetLabelSize(graphConfig.yLabelSize);
  yAxis->SetLabelOffset(graphConfig.yLabelOffset);
  yAxis->SetTitle(graphConfig.yTitle);
}

void setGraphProperties(TGraph * g, const GraphConfig & graphConfig, const TString & xTitle, const TString & yTitle)
{
  cout << "Setting properties of graph: " << g->GetTitle() << endl;
  g->SetLineColor(graphConfig.lineColor);
  g->SetLineStyle(graphConfig.lineStyle);
  g->SetLineWidth(graphConfig.lineWidth);
  g->SetMarkerColor(graphConfig.markerColor);
  g->SetMarkerStyle(graphConfig.markerStyle);
  g->SetMarkerSize(graphConfig.markerSize);
  g->SetFillColor(0);
  TAxis * xAxis = (TAxis *) g->GetXaxis();
  xAxis->SetNdivisions(graphConfig.nXDivisions);
  xAxis->SetTitleSize(graphConfig.xTitleSize);
  xAxis->SetTitleOffset(graphConfig.xTitleOffset);
  xAxis->SetTitle(xTitle);
  xAxis->SetLabelSize(graphConfig.xLabelSize);
  xAxis->SetLabelOffset(graphConfig.xLabelOffset);
  TAxis * yAxis = (TAxis *) g->GetYaxis();
  yAxis->SetNdivisions(graphConfig.nYDivisions);
  yAxis->SetTitleSize(graphConfig.yTitleSize);
  yAxis->SetTitleOffset(graphConfig.yTitleOffset);
  yAxis->SetLabelSize(graphConfig.yLabelSize);
  yAxis->SetLabelOffset(graphConfig.yLabelOffset);
  yAxis->SetTitle(yTitle);
}


void setFunctionProperties(TF1 * f, const GraphConfig & graphConfig)
{
  cout << "Setting properties of function: " << f->GetName() << endl;
  f->SetNpx(graphConfig.npx);
  f->SetLineColor(graphConfig.lineColor);
  f->SetLineStyle(graphConfig.lineStyle);
  f->SetLineWidth(graphConfig.lineWidth);
  f->SetMarkerColor(graphConfig.markerColor);
  f->SetMarkerStyle(graphConfig.markerStyle);
  f->SetMarkerSize(graphConfig.markerSize);
  f->SetFillColor(0);
  TAxis * xAxis = (TAxis *) f->GetXaxis();
  xAxis->SetNdivisions(graphConfig.nXDivisions);
  xAxis->SetTitleSize(graphConfig.xTitleSize);
  xAxis->SetTitleOffset(graphConfig.xTitleOffset);
  xAxis->SetTitle(graphConfig.xTitle);
  xAxis->SetLabelSize(graphConfig.xLabelSize);
  xAxis->SetLabelOffset(graphConfig.xLabelOffset);
  TAxis * yAxis = (TAxis *) f->GetYaxis();
  yAxis->SetNdivisions(graphConfig.nYDivisions);
  yAxis->SetTitleSize(graphConfig.yTitleSize);
  yAxis->SetTitleOffset(graphConfig.yTitleOffset);
  yAxis->SetLabelSize(graphConfig.yLabelSize);
  yAxis->SetLabelOffset(graphConfig.yLabelOffset);
  yAxis->SetTitle(graphConfig.yTitle);
}
