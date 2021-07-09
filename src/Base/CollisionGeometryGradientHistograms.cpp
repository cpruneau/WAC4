//
//  CollisionGeometryGradientHistograms.cpp
//  MyMC
//
//  Created by Claude Pruneau on 9/23/16.
//  Copyright © 2016 Claude Pruneau. All rights reserved.
// ===========================================================
#include "CollisionGeometryGradientHistograms.hpp"


CollisionGeometryGradientHistograms::CollisionGeometryGradientHistograms(const TString & collectionName,
                                                                         CollisionGeometryConfiguration * _configuration,
                                                                         LogLevel  debugLevel)
:
Histograms(collectionName,_configuration,debugLevel),
h_edge(0),
h_radius(0),
h_gx(0),
h_gy(0)
{
  // no ops
}

void CollisionGeometryGradientHistograms::createHistograms()
{
  if (reportStart("CollisionGeometryGradientHistograms",getName(),"createHistograms()"))
    ;
  CollisionGeometryConfiguration & ac = * (CollisionGeometryConfiguration*) getConfiguration();
  TString bn = getHistoBaseName();
  h_edge     = createHistogram(bn+TString("edge"),   200,-20.0, 20.0, 200,-20.0, 20.0,    "x (fm)", "y (fm)",  "Edge", 0, 1);
  h_radius   = createHistogram(bn+TString("radius"), 200,-20.0, 20.0, 200,-20.0, 20.0,    "x (fm)", "y (fm)",  "r/r_{max}", 0, 1);
  h_gx       = createHistogram(bn+TString("gx"),     200,-20.0, 20.0, 200,-20.0, 20.0,    "x (fm)", "y (fm)",  "g_{x}", 0, 1);
  h_gy       = createHistogram(bn+TString("gy"),     200,-20.0, 20.0, 200,-20.0, 20.0,    "x (fm)", "y (fm)",  "g_{y}", 0, 1);
  if (reportEnd("CollisionGeometryGradientHistograms",getName(),"createHistograms()"))
    ;
}

//________________________________________________________________________
void CollisionGeometryGradientHistograms::loadHistograms(TFile * inputFile)
{
  if (reportStart("CollisionGeometryGradientHistograms",getName(),"loadHistograms(TFile * inputFile)"))
    ;
  if (!inputFile)
    {
    if (reportFatal("CollisionGeometryGradientHistograms",getName(),"loadHistograms(TFile * inputFile) inputFile is a nullptr"))
      {
      exit(1);
      }
    }
  CollisionGeometryConfiguration & ac = * (CollisionGeometryConfiguration*) getConfiguration();
  TString bn = getHistoBaseName();
  h_edge   = loadH2(inputFile,bn+TString("edge"));
  h_radius = loadH2(inputFile,bn+TString("radius"));
  h_gx     = loadH2(inputFile,bn+TString("gx"));
  h_gy     = loadH2(inputFile,bn+TString("gy"));
  if (reportEnd("CollisionGeometryGradientHistograms",getName(),"loadHistograms(TFile * inputFile)"))
    ;
}

void CollisionGeometryGradientHistograms::getRadiusAndGradient(double x, double y, double & radius, double & gx, double & gy)
{
  int iX = h_radius->GetXaxis()->FindBin(x);
  int iY = h_radius->GetYaxis()->FindBin(y);
  radius = h_radius->GetBinContent(iX,iY);
  gx     = h_gx->GetBinContent(iX,iY);
  gy     = h_gy->GetBinContent(iX,iY);
}
