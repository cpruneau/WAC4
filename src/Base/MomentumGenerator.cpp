//
//  MomentumGenerator.hpp
//  Created by Victor Gonzalez on 20190206 based in the work of Claude Pruneau and the ALICE AliPhysics SW framework
//  Copyright © 2016-2019 Claude Pruneau. All rights reserved.
//
#include "MomentumGenerator.hpp"
ClassImp(MomentumGenerator);

double probDensity(double p, double mass, double temperature, double mu, double stat)
{
  double e     = sqrt(p*p + mass*mass);
  double beta  = 1.0/temperature;
  double arg   = beta*(e-mu);
  double density = p*p/(exp(arg)+stat);
  return density;
}


MomentumGenerator::MomentumGenerator(const TString & _partName, double _mass, double _tMin, double _tMax, double _tWidth, double _stat, TRandom * _selectedRandom)
:
partName(_partName),
mass(_mass),
tMin(_tMin),
tMax(_tMax),
tWidth(_tWidth),
stat(_stat),
histograms(),
random(_selectedRandom)
{
  if (tWidth<=0.0 || tMax<=tMin)
    {
    cout << "<F> MomentumGenerator::MomentumGenerator(...) tWidth<0.0" << endl;
    exit(1);
    }
  int nT = (tMax-tMin)/tWidth;
  double zero = 0.0;
  for (int k=0; k<nT; k++)
    {
    TString hName = partName;
    hName += k;
    double temp   = tMin + tWidth*double(k);
    double mu     = 0.0;
    int nBinsP    = 1000;
    double minP   = 0.0;
    double maxP   = 10.0;
    double dp     = (maxP-minP)/double(nBinsP);
    TH1 * h = new TH1D(hName,hName, nBinsP, minP, maxP);
    double p = -dp;
    double v;
    for (int j=0; j<nBinsP; j++)
      {
      p += dp;
      v = probDensity(p,mass,temp,mu,stat);
      h->SetBinContent(j,v);
      h->SetBinError(j,zero);
      }
    histograms.push_back(h);
    }
}

MomentumGenerator::MomentumGenerator(TRandom * _selectedRandom)
:
mass(0.0),
tMin(0.0),
tMax(0.0),
tWidth(0.0),
stat(0.0),
histograms(),
random(_selectedRandom)
{
}


TLorentzVector  MomentumGenerator::generate(GeneratorType generatorType, vector<double>& parameters, TH1 * histogramP)
{
  TLorentzVector momentum;
  double px, py, pz, pt, mt, p, e, mass, ptSq, temp, sigma;
  double phi, theta, cosTh, y;
  switch (generatorType)
    {
      case IsotropicGaussP:
      // generate isotropic distribution
      // p is Gaussian distributed
      // parameters[0] : mass
      // parameters[1] : minimum p
      // parameters[2] : width
      mass  = parameters[0];
      phi   = TMath::TwoPi()*random->Rndm( );
      cosTh = -1.0 + 2.0*random->Rndm( );
      theta = acos(cosTh);
      p     = random->Gaus(parameters[1],parameters[2] );
      e     = sqrt(mass*mass + p*p);
      pt    = p*sin(theta);
      px    = pt*cos(phi);
      py    = pt*sin(phi);
      pz    = p*cosTh;
      break;

      case IsotropicExpP:
      // generate isotropic distribution
      // p is exponential distributed
      // exp( -t/tau )
      // parameters[0] : mass
      // parameters[1] : tau
      mass  = parameters[0];
      phi   = TMath::TwoPi()*random->Rndm( );
      cosTh = -1.0 + 2.0*random->Rndm( );
      theta = acos(cosTh);
      p     = random->Exp(parameters[1]);
      e     = sqrt(mass*mass + p*p);
      pt    = p*sin(theta);
      px    = pt*cos(phi);
      py    = pt*sin(phi);
      pz    = p*cosTh;
      break;

      case IsotropicUniformP:
      // generate isotropic distribution
      // p is uniform distributed
      // parameters[0] : mass
      // parameters[1] : minimum p
      // parameters[2] : width of the generated range
      mass  = parameters[0];
      phi   = TMath::TwoPi()*random->Rndm( );
      cosTh = -1.0 + 2.0*random->Rndm( );
      theta = acos(cosTh);
      p     = parameters[1] + random->Rndm()*parameters[2];
      e     = sqrt(mass*mass + p*p);
      pt    = p*sin(theta);
      px    = pt*cos(phi);
      py    = pt*sin(phi);
      pz    = p*cosTh;
      break;

      case IsotropicUniformDensity:
      // generate isotropic distribution
      // uniform density
      // parameters[0] : mass
      // parameters[1] : minimum p
      // parameters[2] : width of the generated range in p^2
      mass  = parameters[0];
      phi   = TMath::TwoPi()*random->Rndm( );
      cosTh = -1.0 + 2.0*random->Rndm( );
      theta = acos(cosTh);
      p     = parameters[1] + random->Rndm()*parameters[2];
      e     = sqrt(mass*mass + p*p);
      pt    = p*sin(theta);
      px    = pt*cos(phi);
      py    = pt*sin(phi);
      pz    = p*cosTh;
      break;

      case IsotropicMaxwellP:
      // generate isotropic distribution
      // Maxwell Boltzmann
      // parameters[0] : mass
      // parameters[1] : inverse temperature
      mass = parameters[0];
      px = random->Gaus(0.0,parameters[1] );
      py = random->Gaus(0.0,parameters[1] );
      pz = random->Gaus(0.0,parameters[1] );
      e  = sqrt(mass*mass + px*px + py*py + pz*pz);
      break;

      case IsotropicHistoP:
      // generate isotropic distribution
      // p determined by histogram
      // parameters[0] : mass
      mass  = parameters[0];
      phi   = TMath::TwoPi()*random->Rndm( );
      cosTh = -1.0 + 2.0*random->Rndm( );
      theta = acos(cosTh);
      p     = histogramP->GetRandom();
      e     = sqrt(mass*mass + p*p);
      pt    = p*sin(theta);
      px    = pt*cos(phi);
      py    = pt*sin(phi);
      pz    = p*cosTh;
      break;

      case CylindricalGaussPtUniformY:
      // generate cylindrical distribution
      // parameters[0] : mass
      // parameters[1] : minimum rapidity
      // parameters[2] : width of rapidity window
      // parameters[3] : width of Gauss pt
      mass  = parameters[0];
      phi   = TMath::TwoPi()*random->Rndm( );
      px    = random->Gaus(0.0,parameters[3] );
      py    = random->Gaus(0.0,parameters[3] );
      ptSq  = px*px + py*py;
      mt    = sqrt(mass*mass + ptSq);
      y     = parameters[1]+parameters[2]*random->Rndm( );
      e     = mt*cosh(y);
      pz    = mt*sinh(y);
      px    = pt*cos(phi);
      py    = pt*sin(phi);
      break;

      case CylindricalExpPtUniformY:
      // generate cylindrical distribution
      // parameters[0] : mass
      // parameters[1] : minimum rapidity
      // parameters[2] : width of rapidity window
      // parameters[3] : average pt
      mass  = parameters[0];
      phi   = TMath::TwoPi()*random->Rndm( );
      pt    = random->Exp(parameters[3]);
      mt    = sqrt(mass*mass + pt*pt);
      y     = parameters[1]+parameters[2]*random->Rndm( );
      e     = mt*cosh(y);
      pz    = mt*sinh(y);
      px    = pt*cos(phi);
      py    = pt*sin(phi);
      break;

      case CylindricalMaxwellPtUniformY:
      // generate cylindrical distribution
      // parameters[0] : mass
      // parameters[1] : minimum rapidity
      // parameters[2] : width of rapidity window
      // parameters[3] : Gauss Width
//      mass  = parameters[0];
//      temp  = parameters[3];
//      sigma = sqrt(mass*temp);
//      px    = random->Gaus(0.0,sigma);
//      py    = random->Gaus(0.0,sigma);
//      pz    = random->Gaus(0.0,sigma);
//      e     = sqrt(px*px+py*py+pz*pz+mass*mass);

      mass  = parameters[0];
      temp  = parameters[3];
      p     = random->Exp(temp);
      e     = sqrt(p*p+mass*mass);
      phi   = TMath::TwoPi()*random->Rndm( );
      cosTh = -1.0 + 2.0*random->Rndm( );
      theta = acos(cosTh);
      pt    = p*sin(theta);
      px    = pt*cos(phi);
      py    = pt*sin(phi);
      pz    = p*cosTh;

      //ptSq  = px*px + py*py;
      //mt    = sqrt(mass*mass + ptSq);
      //y     = parameters[1]+parameters[2]*random->Rndm( );
      //e     = mt*cosh(y);
      //pz    = mt*sinh(y);
      break;

      case CylindricalRadialFlowUniformY:
      // generate cylindrical distribution
      // parameters[0] : mass
      // parameters[1] : minimum rapidity
      // parameters[2] : width of rapidity window
      // parameters[3] : inverse temperature
      mass  = parameters[0];
      px    = random->Gaus(0.0,parameters[2] );
      py    = random->Gaus(0.0,parameters[2] );
      pt    = sqrt(px*px + py*py);
      mt    = sqrt(mass*mass + pt*pt);
      y     = parameters[1]+parameters[2]*random->Rndm( );
      e     = mt*cosh(y);
      pz    = mt*sinh(y);
      break;

      case CylindricalHistoPtUniformY:
      // generate cylindrical distribution
      // parameters[0] : mass
      // parameters[1] : minimum rapidity
      // parameters[2] : width of rapidity window
      mass  = parameters[0];
      pt    = histogramP->GetRandom();
      phi   = TMath::TwoPi()*random->Rndm( );
      mt    = sqrt(mass*mass + pt*pt);
      y     = parameters[1]+parameters[2]*random->Rndm( );
      e     = mt*cosh(y);
      px    = pt*cos(phi);
      py    = pt*sin(phi);
      pz    = mt*sinh(y);
      break;
    }
  momentum.SetPxPyPzE (px,py,pz,e);
  return momentum;
}

TLorentzVector  MomentumGenerator::generate(double temperature)
{
  TLorentzVector momentum;

  double px, py, pz, pt, mt, p, e, ptSq, temp, sigma;
  double phi, theta, cosTh, y;

  phi   = TMath::TwoPi()*random->Rndm( );
  cosTh = -1.0 + 2.0*random->Rndm( );
  theta = acos(cosTh);
  int k = 0;
  if (temperature <= tMin) k = 0;
  else if (temperature >= tMax) k = -1 + histograms.size();
  else
    {
    k = (temperature-tMin)/tWidth;
    if (tWidth>histograms.size())
      {
      cout << "<F> MomentumGenerator::generate(double temperature) Internal error." << endl;
      exit(1);
      }
    }
  p     = histograms[k]->GetRandom();
  e     = sqrt(mass*mass + p*p);
  pt    = p*sin(theta);
  px    = pt*cos(phi);
  py    = pt*sin(phi);
  pz    = p*cosTh;
  momentum.SetPxPyPzE (px,py,pz,e);
  return momentum;
}
