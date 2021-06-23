//
//  CollisionGeometryHistograms.cpp
//  MyMC
//
//  Created by Claude Pruneau on 9/23/16.
//  Copyright © 2016 Claude Pruneau. All rights reserved.
// ===========================================================
#include "CollisionGeometryHistograms.hpp"
#include <vector>
using std::vector;

ClassImp(CollisionGeometryHistograms);

CollisionGeometryHistograms::CollisionGeometryHistograms(const TString & collectionName,
                                                         CollisionGeometryConfiguration * _configuration,
                                                         LogLevel  debugLevel)
:
Histograms(collectionName,_configuration,debugLevel),
h_nProcessedVsB(0),
h_nAcceptedVsB(0),
h_nPart(0),
h_nBinary(0),
h_nPartVsB_Prof(0),
h_nBinaryVsB_Prof(0),
h_nPartVsB(0),
h_nBinaryVsB(0),
h_crossSection(0)
{
  // no ops
}

void CollisionGeometryHistograms::createHistograms()
{
  if (reportStart("CollisionGeometryHistograms",getName(),"createHistograms()"))
    ;

  CollisionGeometryConfiguration & ac = * (CollisionGeometryConfiguration*) getConfiguration();
  TString bn = getHistoBaseName();
  h_nProcessedVsB     = createHistogram(bn+TString("nProcessedVsB"),         ac.nBins_b,      ac.min_b,       ac.max_b,    "b (fm)",   "Counts", 0, 1);
  h_nAcceptedVsB      = createHistogram(bn+TString("nAcceptedVsB"),          ac.nBins_b,      ac.min_b,       ac.max_b,    "b (fm)",   "Counts", 0, 1);
  h_nPartVsB          = createHistogram(bn+TString("nPartVsB"),              ac.nBins_b,      ac.min_b,       ac.max_b,    ac.nBins_nPart,   ac.min_nPart,   ac.max_nPart,   "b (fm)", "N_{Part}", "Counts",0,1);
  h_nBinaryVsB        = createHistogram(bn+TString("nBinaryVsB"),            ac.nBins_b,      ac.min_b,       ac.max_b,    ac.nBins_nBinary, ac.min_nBinary, ac.max_nBinary, "b (fm)", "N_{Bin}",  "Counts",0,1);
  h_nPartVsB_Prof     = createProfile(bn+TString("nPartVsB_Prof"),           ac.nBins_b,      ac.min_b,       ac.max_b,    "b (fm)",   "<N_{Part}>",1);
  h_nPartSqVsB_Prof   = createProfile(bn+TString("nPartSqVsB_Prof"),         ac.nBins_b,      ac.min_b,       ac.max_b,    "b (fm)",   "<N_{Part}^{2}>",1);
  h_nBinaryVsB_Prof   = createProfile(bn+TString("nBinaryVsB_Prof"),         ac.nBins_b,      ac.min_b,       ac.max_b,    "b (fm)",   "<N_{Bin}>",   1);
  h_nBinarySqVsB_Prof = createProfile(bn+TString("nBinarySqVsB_Prof"),       ac.nBins_b,      ac.min_b,       ac.max_b,    "b (fm)",   "<N_{Bin}^{2}>", 1);

  h_nPart                 = createHistogram(bn+TString("nPart"),              ac.nBins_nPart,  ac.min_nPart,   ac.max_nPart,   "N_{Part}", "Counts", 0, 1);
  h_nBinaryVsNPart_Prof   = createProfile(bn+TString("nBinaryVsNPart_Prof"),  ac.nBins_nPart,  ac.min_nPart,   ac.max_nPart,   "N_{Part}", "<N_{Bin}>", 1);
  h_nBinarySqVsNPart_Prof = createProfile(bn+TString("nBinarySqVsNPart_Prof"),ac.nBins_nPart,  ac.min_nPart,   ac.max_nPart,   "N_{Part}", "<N_{Bin}^{2}>", 1);
  h_bVsNPart_Prof         = createProfile(bn+TString("bVsNPart_Prof"),        ac.nBins_nPart,  ac.min_nPart,   ac.max_nPart,   "N_{Part}", "<b>", 1);
  h_bSqVsNPart_Prof       = createProfile(bn+TString("bSqVsNPart_Proff"),     ac.nBins_nPart,  ac.min_nPart,   ac.max_nPart,   "N_{Part}", "<b^{2}>", 1);

  h_nBinary                = createHistogram(bn+TString("nBinary"),            ac.nBins_nBinary,ac.min_nBinary, ac.max_nBinary, "N_{Bin}",  "Counts", 0, 1);
  h_nPartVsNBinary_Prof    = createProfile(bn+TString("nPartVsNBinary_Prof"),  ac.nBins_nBinary,ac.min_nBinary, ac.max_nBinary, "N_{Bin}",  "<N_{Part}>", 0, 1);
  h_nPartSqVsNBinary_Prof  = createProfile(bn+TString("nPartSqVsNBinary_Prof"),ac.nBins_nBinary,ac.min_nBinary, ac.max_nBinary, "N_{Bin}",  "<N_{Part}^{2}>", 0, 1);
  h_bVsNBinary_Prof        = createProfile(bn+TString("bVsNBinary_Prof"),      ac.nBins_nBinary,ac.min_nBinary, ac.max_nBinary, "N_{Bin}",  "<b>", 0, 1);
  h_bSqVsNBinary_Prof      = createProfile(bn+TString("bSqVsNBinary_Prof"),    ac.nBins_nBinary,ac.min_nBinary, ac.max_nBinary, "N_{Bin}",  "<b^{2}>", 0, 1);

  h_crossSection           = createHistogram(bn+TString("Xsect"),               100,             0.0,            100.0,          "Xsect",    "Counts", 0, 1);
  h_nPartVsXsect_Prof      = createProfile(bn+TString("nPartVsXsect_Prof"),     100,             0.0,            100.0,          "Xsect",    "<N_{Part}>", 0, 1);
  h_nPartSqVsXsect_Prof    = createProfile(bn+TString("nPartSqVsXsect_Prof"),   100,             0.0,            100.0,          "Xsect",    "<N_{Part}^{2}>", 0, 1);
  h_nBinaryVsXsect_Prof    = createProfile(bn+TString("nBinaryVsXsect_Prof"),   100,             0.0,            100.0,          "Xsect",    "<N_{Bin}>", 0, 1);
  h_nBinarySqVsXsect_Prof  = createProfile(bn+TString("nBinarySqVsXsect_Prof"), 100,             0.0,            100.0,          "Xsect",    "<N_{Bin}^{2}>", 0, 1);
  h_bVsXsect_Prof          = createProfile(bn+TString("bVsXsect_Prof"),         100,             0.0,            100.0,          "Xsect",    "<b>", 0, 1);
  h_bSqVsXsect_Prof        = createProfile(bn+TString("bSqVsXsect_Prof"),       100,             0.0,            100.0,          "Xsect",    "<b^{2}>", 0, 1);

  h_xyDistInteractions     = createHistogram(bn+TString("xyDistInteractions"),   400, -20.0, 20.0,   400, -20.0, 20.0, "x (fm)",  "y (fm)",   "Counts", 0, 1);
  h_xyDistNucleons         = createHistogram(bn+TString("xyDistNucleons"),   400, -20.0, 20.0,   400, -20.0, 20.0, "x (fm)",  "y (fm)",   "Counts", 0, 1);
  // Derived Histograms
  h_nPartRmsVsB            = createHistogram(bn+TString("nPartRmsVsB"),            ac.nBins_b,      ac.min_b,       ac.max_b,      "b (fm)",   "RMS(N_{Part})");
  h_nPartOmegaVsB          = createHistogram(bn+TString("nPartOmegaVsB"),          ac.nBins_b,      ac.min_b,       ac.max_b,      "b (fm)",   "#omega(N_{part})");
  h_nPartR2VsB             = createHistogram(bn+TString("nPartR2VsB"),             ac.nBins_b,      ac.min_b,       ac.max_b,      "b (fm)",   "R_{2}(N_{part})");
  h_nBinaryRmsVsB          = createHistogram(bn+TString("nBinaryRmsVsB"),          ac.nBins_b,      ac.min_b,       ac.max_b,      "b (fm)",   "RMS(N_{Bin})");
  h_nBinaryOmegaVsB        = createHistogram(bn+TString("nBinaryOmegaVsB"),        ac.nBins_b,      ac.min_b,       ac.max_b,      "b (fm)",   "#omega(N_{Bin})");
  h_nBinaryR2VsB           = createHistogram(bn+TString("nBinaryR2VsB"),           ac.nBins_b,      ac.min_b,       ac.max_b,      "b (fm)",   "R_{2}(N_{bin})");

  h_nBinaryRmsVsNPart      = createHistogram(bn+TString("nBinaryRmsVsNPart"),      ac.nBins_nPart,  ac.min_nPart,   ac.max_nPart,   "N_{Part}", "RMS(N_{Bin})");
  h_nBinaryOmegaVsNPart    = createHistogram(bn+TString("nBinaryOmegaVsNPart"),    ac.nBins_nPart,  ac.min_nPart,   ac.max_nPart,   "N_{Part}", "#omega(N_{Bin})");
  h_nBinaryR2VsNPart       = createHistogram(bn+TString("nBinaryR2VsNPart"),       ac.nBins_nPart,  ac.min_nPart,   ac.max_nPart,   "N_{Part}", "R_{2}(N_{bin})");
  h_bRmsVsNPart            = createHistogram(bn+TString("bRmsVsNPart"),            ac.nBins_nPart,  ac.min_nPart,   ac.max_nPart,   "N_{Part}", "RMS(b)");
  h_bOmegaVsNPart          = createHistogram(bn+TString("bOmegaVsNPart"),          ac.nBins_nPart,  ac.min_nPart,   ac.max_nPart,   "N_{Part}", "#omega(b)");
  h_bR2VsNPart             = createHistogram(bn+TString("bR2VsNPart"),             ac.nBins_nPart,  ac.min_nPart,   ac.max_nPart,   "N_{Part}", "R_{2}(b)");

  h_nPartRmsVsNBinary      = createHistogram(bn+TString("nPartRmsVsNBinary"),      ac.nBins_nBinary,ac.min_nBinary, ac.max_nBinary, "N_{Bin}", "RMS(N_{Bin})");
  h_nPartOmegaVsNBinary    = createHistogram(bn+TString("nPartOmegaVsNBinary"),    ac.nBins_nBinary,ac.min_nBinary, ac.max_nBinary, "N_{Bin}", "#omega(N_{Bin})");
  h_nPartR2VsNBinary       = createHistogram(bn+TString("nPartR2VsNBinary"),       ac.nBins_nBinary,ac.min_nBinary, ac.max_nBinary, "N_{Bin}", "R_{2}(N_{bin})");
  h_bRmsVsNBinary          = createHistogram(bn+TString("bRmsVsNBinary"),          ac.nBins_nBinary,ac.min_nBinary, ac.max_nBinary, "N_{Bin}", "RMS(b)");
  h_bOmegaVsNBinary        = createHistogram(bn+TString("bOmegaVsNBinary"),        ac.nBins_nBinary,ac.min_nBinary, ac.max_nBinary, "N_{Bin}", "#omega(b)");
  h_bR2VsNBinary           = createHistogram(bn+TString("bR2VsNBinary"),           ac.nBins_nBinary,ac.min_nBinary, ac.max_nBinary, "N_{Bin}", "R_{2}(b)");

  h_nPartRmsVsXsect        = createHistogram(bn+TString("nPartRmsVsXsect"),        100,             0.0,            100.0,          "Xsect", "RMS(N_{Part})");
  h_nPartOmegaVsXsect      = createHistogram(bn+TString("nPartOmegaVsXsect"),      100,             0.0,            100.0,          "Xsect", "#omega(N_{Part})");
  h_nPartR2VsXsect         = createHistogram(bn+TString("nPartR2VsXsect"),         100,             0.0,            100.0,          "Xsect", "R_{2}(N_{Part})");

  h_nBinaryRmsVsXsect      = createHistogram(bn+TString("nNBinaryRmsVsXsect"),     100,             0.0,            100.0,          "Xsect", "RMS(N_{Bin})");
  h_nBinaryOmegaVsXsect    = createHistogram(bn+TString("nBinaryOmegaVsXsect"),    100,             0.0,            100.0,          "Xsect", "#omega(N_{Bin})");
  h_nBinaryR2VsXsect       = createHistogram(bn+TString("nBinaryR2VsXsect"),       100,             0.0,            100.0,          "Xsect", "R_{2}(N_{Bin})");

  h_bRmsVsXsect            = createHistogram(bn+TString("bRmsVsXsect"),            100,             0.0,            100.0,          "Xsect", "RMS(b)",0,1);
  h_bOmegaVsXsect          = createHistogram(bn+TString("bOmegaVsXsect"),          100,             0.0,            100.0,          "Xsect", "#omega(b)");
  h_bR2VsXsect             = createHistogram(bn+TString("bR2VsXsect"),             100,             0.0,            100.0,          "Xsect", "R_{2}(b)");

//  bValues10[0] = 0.0;
//  bValues10[1] = 3.39927;
//  bValues10[2] = 4.79929;
//  bValues10[3] = 6.79951;
//  bValues10[4] = 8.39898;
//  bValues10[5] = 9.79733;
//  bValues10[6] = 10.9965;
//  bValues10[7] = 11.9972;
//  bValues10[8] = 12.9963;
//  bValues10[9] = 13.7987;
//  bValues10[10] = 14.7986;
//  bValues10[11] = 20.0;

//  h_xyNNIntVsB         = createHistogram(bn+"xyNNIntVsB",       ac.nBins_b,  ac.min_b,  ac.max_b,  40, -10.0, 10.0, 40, -10.0, 10.0, "b", "x", "y", "Counts",0,1);
//  h_varXVsB_Prof       = createProfile(bn+"varXVsB_Prof",       ac.nBins_b,  ac.min_b,  ac.max_b,  "b",  "Var[x]", 1);
//  h_varYVsB_Prof       = createProfile(bn+"varYVsB_Prof",       ac.nBins_b,  ac.min_b,  ac.max_b,  "b",  "Var[y]", 1);
//  h_covXYVsB_Prof      = createProfile(bn+"covXYVsB_Prof",      ac.nBins_b,  ac.min_b,  ac.max_b,  "b",  "Cov[x,y]", 1);
//  h_epsilonXVsB_Prof   = createProfile(bn+"epsilonXVsB_Prof",   ac.nBins_b,  ac.min_b,  ac.max_b,  "b",  "#epsilon_{x}", 1);
//  h_epsilonYVsB_Prof   = createProfile(bn+"epsilonYVsB_Prof",   ac.nBins_b,  ac.min_b,  ac.max_b,  "b",  "#epsilon_{y}", 1);
//  h_epsilonXYVsB_Prof  = createProfile(bn+"epsilonXYVsB_Prof",  ac.nBins_b,  ac.min_b,  ac.max_b,  "b",  "|#epsilon|", 1);
//  h_epsilonXYVsB       = createHistogram(bn+TString("epsilonXYVsB"), ac.nBins_b,  ac.min_b,  ac.max_b,  40, -1.0, 1.0,  40, -1.0, 1.0, "b", "#epsilon_{x}", "#epsilon_{y}", "Counts",0,1);;
//  h_psi2VsB_Prof       = createProfile(bn+"psi2VsB_Prof",            ac.nBins_b,  ac.min_b,  ac.max_b,  "b",  "#psi_{2}", 1);
//  h_psi2VsB            = createHistogram(bn+TString("psi2VsB"),      ac.nBins_b,  ac.min_b,  ac.max_b,  40, -TMath::Pi(), TMath::Pi(), "b", "#psi_2", "Counts",0,1);

  if (reportEnd("CollisionGeometryHistograms",getName(),"createHistograms()"))
    ;
}

//________________________________________________________________________
void CollisionGeometryHistograms::loadHistograms(TFile * inputFile)
{
  if (reportStart("CollisionGeometryHistograms",getName(),"loadHistograms(TFile * inputFile)"))
    ;
  if (!inputFile)
    {
    if (reportFatal("CollisionGeometryHistograms",getName(),"loadHistograms(TFile * inputFile) inputFile is a nullptr"))
      {
      exit(1);
      }
    }
  CollisionGeometryConfiguration & ac = * (CollisionGeometryConfiguration*) getConfiguration();
  TString bn = getHistoBaseName();

  if (reportEnd("CollisionGeometryHistograms",getName(),"loadHistograms(TFile * inputFile)"))
    ;
}

void CollisionGeometryHistograms::fill(Event & event, double weight)
{

  double impactPar = event.getImpactParameter();
  double nPart     = event.getNParticipants();
  double nBinary   = event.getNBinaryCollisions();
  double xSect     = event.getCrossSection();

//  if (reportInfo("CollisionGeometryHistograms",getName(),"loadHistograms(TFile * inputFile)"))
//    {
//    cout<< " impactPar :" << impactPar << endl;
//    cout<< "     nPart :" << nPart << endl;
//    cout<< "   nBinary :" << nBinary << endl;
//    cout<< "     xSect :" << xSect << endl;
//    }

  // Impact parameter, cross-section, nParticipants, nBinaries, etc.
  h_nAcceptedVsB           ->Fill(impactPar, weight);
  h_nPartVsB               ->Fill(impactPar, nPart,            weight);
  h_nBinaryVsB             ->Fill(impactPar, nBinary,          weight);
  h_nPartVsB_Prof          ->Fill(impactPar, nPart,            weight);
  h_nPartSqVsB_Prof        ->Fill(impactPar, nPart*nPart,      weight);
  h_nBinaryVsB_Prof        ->Fill(impactPar, nBinary,          weight);
  h_nBinarySqVsB_Prof      ->Fill(impactPar, nBinary*nBinary,  weight);


  h_nPart                  ->Fill(nPart,  weight);
  h_nBinaryVsNPart_Prof    ->Fill(nPart,  nBinary,             weight);
  h_nBinarySqVsNPart_Prof  ->Fill(nPart,  nBinary*nBinary,     weight);
  h_bVsNPart_Prof          ->Fill(nPart,  impactPar,           weight);
  h_bSqVsNPart_Prof        ->Fill(nPart,  impactPar*impactPar, weight);

  h_nBinary                ->Fill(nBinary, weight);
  h_nPartVsNBinary_Prof    ->Fill(nBinary, nPart,               weight);
  h_nPartSqVsNBinary_Prof  ->Fill(nBinary, nPart*nPart,         weight);
  h_bVsNBinary_Prof        ->Fill(nBinary, impactPar,           weight);
  h_bSqVsNBinary_Prof      ->Fill(nBinary, impactPar*impactPar, weight);

  h_crossSection           ->Fill(xSect, weight);
  h_nPartVsXsect_Prof      ->Fill(xSect, nPart,           weight);
  h_nPartSqVsXsect_Prof    ->Fill(xSect, nPart*nPart,     weight);
  h_nBinaryVsXsect_Prof    ->Fill(xSect, nBinary,         weight);
  h_nBinarySqVsXsect_Prof  ->Fill(xSect, nBinary*nBinary, weight);
  h_bVsXsect_Prof          ->Fill(xSect, impactPar,           weight);
  h_bSqVsXsect_Prof        ->Fill(xSect, impactPar*impactPar, weight);

  // Geometry

  const vector<Particle*> interactions = event.getNucleonNucleonInteractions();
  for (unsigned int k=0;k<interactions.size(); k++)
    {
    const TLorentzVector & position = interactions[k]->getPosition();
    h_xyDistInteractions->Fill( position.X(), position.Y() );
    }

  Nucleus & nucleusA =  event.getNucleusA();
  Nucleus & nucleusB =  event.getNucleusB();
  vector<Particle*> nucleonsA = nucleusA.getChildren();
  vector<Particle*> nucleonsB = nucleusB.getChildren();
  for (unsigned int k=0;k<nucleonsA.size(); k++)
    {
    const TLorentzVector & position = nucleonsA[k]->getPosition();
    h_xyDistNucleons->Fill( position.X(), position.Y() );
    }
  for (unsigned int k=0;k<nucleonsB.size(); k++)
    {
    const TLorentzVector & position = nucleonsB[k]->getPosition();
    h_xyDistNucleons->Fill( position.X(), position.Y() );
    }
}

void CollisionGeometryHistograms::noFill(Event & event, double weight)
{

  double impactPar = event.getImpactParameter();
  double nPart     = event.getNParticipants();
  double nBinary   = event.getNBinaryCollisions();
  double xSect     = event.getCrossSection();

//  if (reportInfo("CollisionGeometryHistograms",getName(),"loadHistograms(TFile * inputFile)"))
//    {
//    cout<< " impactPar :" << impactPar << endl;
//    cout<< "     nPart :" << nPart << endl;
//    cout<< "   nBinary :" << nBinary << endl;
//    cout<< "     xSect :" << xSect << endl;
//    }

  // Impact parameter, cross-section, nParticipants, nBinaries, etc.
  h_nProcessedVsB->Fill(impactPar, weight);
}


void CollisionGeometryHistograms::calculateDerivedHistograms()
{
  if (reportStart("CollisionGeometryHistograms",getName(),"calculateDerivedHistograms()"))
    ;
//  double area    = 1.0; //h_crossSection->GetBinContent(4);
//  double ratio1  = nEvents>0      ? nEventsWColl/nEvents : 1.0e-30;
//  double eRatio1 = nEvents>0      ? eNEventWColl/nEvents : 1.0e-30;
//  double ratio2  = nEventNcollGE0 ? double(nEventNcollGE1)/double(nEventNcollGE0) : 1.0e-30;
//  double eRatio2 = nEventNcollGE0 ? sqrt(double(nEventNcollGE1))/double(nEventNcollGE0) : 1.0e-30;
//  double xSect1  = ratio1*area;
//  double eXSect1 = eRatio1*area;
//  double xSect2  = ratio2*area;
//  double eXSect2 = eRatio2*area;

//  if (reportInfo("CollisionGeometryHistograms",getName(),"calculateDerivedHistograms()"))
//    {
//    cout << "                         nEvents: " <<  nEvents << endl;
//    cout << "                    nEventsWColl: " <<  nEventsWColl << endl;
//    cout << "            nEventsWColl/nEvents: " <<  ratio1 << " +-" << eRatio1 << endl;
//    cout << "                  nEventNcollGE0: " <<  nEventNcollGE0 << endl;
//    cout << "                  nEventNcollGE1: " <<  nEventNcollGE1 << endl;
//    cout << "   nEventNcollGE1/nEventNcollGE0: " <<  ratio2 << " +-" << eRatio2 << endl;
//    cout << "            xSect1 = ratio1*area: " <<  xSect1 << " +-" << eXSect1 << endl;
//    cout << "            xSect2 = ratio2*area: " <<  xSect2 << " +-" << eXSect2 << endl;
//    }

  calculateRms(h_nPartVsB_Prof,       h_nPartSqVsB_Prof,       h_nPartRmsVsB,       h_nPartOmegaVsB,       h_nPartR2VsB);
  calculateRms(h_nBinaryVsB_Prof,     h_nBinarySqVsB_Prof,     h_nBinaryRmsVsB,     h_nBinaryOmegaVsB,     h_nBinaryR2VsB);

  calculateRms(h_nBinaryVsNPart_Prof, h_nBinarySqVsNPart_Prof, h_nBinaryRmsVsNPart, h_nBinaryOmegaVsNPart, h_nBinaryR2VsNPart);
  calculateRms(h_bVsNPart_Prof,       h_bSqVsNPart_Prof,       h_bRmsVsNPart,       h_bOmegaVsNPart,       h_bR2VsNPart);

  calculateRms(h_nPartVsNBinary_Prof, h_nPartSqVsNBinary_Prof, h_nPartRmsVsNBinary, h_nPartOmegaVsNBinary, h_nPartR2VsNBinary);
  calculateRms(h_bVsNBinary_Prof,     h_bSqVsNBinary_Prof,     h_bRmsVsNBinary,     h_bOmegaVsNBinary,     h_bR2VsNBinary);

  calculateRms(h_nPartVsXsect_Prof,   h_nPartSqVsXsect_Prof,   h_nPartRmsVsXsect,   h_nPartOmegaVsXsect,   h_nPartR2VsXsect);
  calculateRms(h_nBinaryVsXsect_Prof, h_nBinarySqVsXsect_Prof, h_nBinaryRmsVsXsect, h_nBinaryOmegaVsXsect, h_nBinaryR2VsXsect);
  calculateRms(h_bVsXsect_Prof,       h_bSqVsXsect_Prof,       h_bRmsVsXsect,       h_bOmegaVsXsect,       h_bR2VsXsect);

  if (reportEnd("CollisionGeometryHistograms",getName(),"calculateDerivedHistograms()"))
    ;
}

void CollisionGeometryHistograms::calculateRms(TProfile * h1, TProfile * h1Sq,   TH1* h1Rms, TH1* h1Omega, TH1* h1R2)
{
  if (reportStart("CollisionGeometryHistograms",getName(),"calculateRms(...)"))
    ;
  double v, ev, v2, ev2, rms, erms, omega, eomega, R2, eR2;
  int nBins = h1->GetNbinsX();
  for (int iBin=1; iBin<nBins; iBin++)
    {
    v = h1->GetBinContent(iBin);
    ev = 0.0;
    v2 = h1Sq->GetBinContent(iBin);
    ev2 = 0.0;
    rms = v2 - v*v;
    if (rms>0) rms = sqrt(rms);
    erms = 0.0;

    if (v>0)
      {
      omega  = rms*rms/v;
      eomega = 0.0;
      R2 = (v2-v)/v/v - 1.0;
      eR2 = 0;
      }
    else
      {
      omega  = 0.0;
      eomega = 0.0;
      R2     = 0.0;
      eR2    = 0;
      }

    //cout << " iBin: " << iBin << " v:" << v << " v2:" << v2 << " rms:" << rms << " omega:" << omega << " R2:" << R2 << endl;

    h1Rms->SetBinContent(iBin, rms);
    h1Rms->SetBinError  (iBin, erms);

    h1Omega->SetBinContent(iBin, omega);
    h1Omega->SetBinError(iBin, eomega);

    h1R2->SetBinContent(iBin, R2);
    h1R2->SetBinError(iBin, eR2);
    }
  if (reportEnd("CollisionGeometryHistograms",getName(),"calculateRms(...)"))
    ;
}
