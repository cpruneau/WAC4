//  Created by Claude Pruneau on 6/19/2020.
//  Copyright © 2020 Claude Pruneau. All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <TStyle.h>
#include <TROOT.h>

int RunPythiaAnalysisAA()
{
  TString includesPath = getenv("WAC_SRC");
  includesPath += "/Base/";
  gSystem->Load(includesPath+"MessageLogger.hpp");
  gSystem->Load(includesPath+"TaskConfiguration.hpp");
  gSystem->Load(includesPath+"Task.hpp");
  gSystem->Load(includesPath+"TaskIterator.hpp");
  gSystem->Load(includesPath+"GraphConfiguration.hpp");
  gSystem->Load(includesPath+"Collection.hpp");
  gSystem->Load(includesPath+"CanvasCollection.hpp");
  gSystem->Load(includesPath+"HistogramCollection.hpp");
  gSystem->Load(includesPath+"CanvasConfiguration.hpp");
  gSystem->Load(includesPath+"Histograms.hpp");
  gSystem->Load(includesPath+"Plotter.hpp");
  gSystem->Load(includesPath+"Particle.hpp");
  gSystem->Load(includesPath+"ParticleType.hpp");
  gSystem->Load(includesPath+"ParticleTypeCollection.hpp");
  gSystem->Load(includesPath+"ParticleDecayMode.hpp");
  gSystem->Load(includesPath+"ParticleAnalyzer.hpp");
  gSystem->Load(includesPath+"ParticlePairAnalyzer.hpp");
  gSystem->Load(includesPath+"SubSampleStatCalculator.hpp");
  gSystem->Load("libBase.dylib");

  includesPath = getenv("WAC_SRC");
  includesPath += "/WacPythia/";
  gSystem->Load(includesPath+"PythiaConfiguration.hpp");
  gSystem->Load(includesPath+"PythiaEventGenerator.hpp");
  gSystem->Load(includesPath+"PythiaEventReader.hpp");
  gSystem->Load("libWacPythia.dylib");
  // ==================================================================================

  MessageLogger::LogLevel messageLevel =  MessageLogger::Info; // MessageLogger::Debug; //
  unsigned long nEventRequested   = 10000;
  unsigned long nEventReported    = 1000;
  unsigned long nEventPartialSave = 1000;
  bool    partialSave             = true;
  bool    subsampleAnalysis       = true;
  int     beamType                = 2212;
  double  beamEnergy              = 7000.0; // GeV
  double  minBias                 = true; // alternative is AliceV0
  TString outputFileNameBase      = "PYTHIA_pp_7TeV_inelastic_";
  TString inputPathName           = getenv("WAC_INPUT_PATH");
  TString outputPathName          = getenv("WAC_OUTPUT_PATH");
  inputPathName  += "/PYTHIA/7TEV/";
  //outputPathName += "/PYTHIA/7TEV/BetterTest/";  // radialBoostConfig->param_a      = 0.08;
  outputPathName += "/PYTHIA/7TEV/BetterTest2/";  // radialBoostConfig->param_a     = 0.4;
  //outputPathName += "/PYTHIA/7TEV/BetterTest3/";  // radialBoostConfig->param_a     = 0.99;
  gSystem->mkdir(outputPathName,1);

  std::cout << "==================================================================================" << std::endl;
  std::cout << "PYTHIA Model Analysis" << endl;
  std::cout << "==================================================================================" << std::endl;
  TString pdgDataTable = getenv("WAC_SRC");
  pdgDataTable += "/EOS/pdg.dat";
  ParticleTypeCollection * particles = ParticleTypeCollection::getMasterParticleCollection(); //ParticleTypeCollection::getMasterParticleCollection();
  particles->readFromFile(pdgDataTable);
  //particles->printProperties(std::cout);

  CollisionGeometryConfiguration * cggConfig = new CollisionGeometryConfiguration();
  cggConfig->forceHistogramsRewrite = true;
  cggConfig->nnCrossSection         = 6.4; // cross section must be adjusted for beam energy.
  cggConfig->outputPath             = outputPathName;
  cggConfig->rootOuputFileName      = outputFileNameBase;


  vector<TString*> pythiaOptions;
  pythiaOptions.push_back( new TString("Init:showChangedSettings = on") );      // list changed settings
  pythiaOptions.push_back( new TString("Init:showChangedParticleData = off") ); // list changed particle data
  pythiaOptions.push_back( new TString("Next:numberCount = 10000") );            // print message every n events
  pythiaOptions.push_back( new TString("Next:numberShowInfo = 1") );            // print event information n times
  pythiaOptions.push_back( new TString("Next:numberShowProcess = 0") );         // print process record n times
  pythiaOptions.push_back( new TString("Next:numberShowEvent = 0") );
  pythiaOptions.push_back( new TString("SoftQCD:inelastic = on") );             // All inelastic processes
  //pythiaOptions.push_back( new TString("SoftQCD:all = on") );                   // Allow total sigma = elastic/SD/DD/ND
                                                                                // pythiaOptions.push_back(  new TString("HardQCD:all = on");
  pythiaOptions.push_back( new TString("130:mayDecay = no") ); //K0s decay off
  pythiaOptions.push_back( new TString("310:mayDecay = no") ); //K0s decay off
  pythiaOptions.push_back( new TString("311:mayDecay = no") ); //K0  decay off
  pythiaOptions.push_back( new TString("3112:mayDecay = no") );
  pythiaOptions.push_back( new TString("3122:mayDecay = no") );
  pythiaOptions.push_back( new TString("3222:mayDecay = no") );
  pythiaOptions.push_back( new TString("3212:mayDecay = no") );

  pythiaOptions.push_back( new TString("3322:mayDecay = no") );
  pythiaOptions.push_back( new TString("3312:mayDecay = no") );
  pythiaOptions.push_back( new TString("3334:mayDecay = no") );

  PythiaConfiguration * pythiaConfig = new PythiaConfiguration();
  pythiaConfig->beam                = beamType;  // PDG Code   proton is 2212
  pythiaConfig->target              = beamType;
  pythiaConfig->energy              = beamEnergy;
  pythiaConfig->options             = pythiaOptions;
  pythiaConfig->useEventStream0     = true;
  pythiaConfig->standaloneMode      = false;   // use interactions generated by the geometry generator
  pythiaConfig->dataOutputUsed      = false;
  pythiaConfig->dataConversionToWac = true;
  pythiaConfig->dataOutputFileName  = "Pythia_pp_7000.root";
  pythiaConfig->dataOutputTreeName  = "PythiaTree";
  pythiaConfig->dataOutputPath      = outputPathName;
  pythiaConfig->dataInputUsed       = false;
  pythiaConfig->dataInputFileName   = "Pythia_pp_7000_10Mevents.root";
  pythiaConfig->dataInputTreeName   = "PythiaTree";
  pythiaConfig->dataInputPath       = getenv("WAC_INPUT_DATA_PATH");
  pythiaConfig->saveHistograms      = false;

  GlobalAnalyzerConfiguration * globalConfigND = new GlobalAnalyzerConfiguration();
  globalConfigND->useEventStream0             = true;
  globalConfigND->loadHistograms              = false;
  globalConfigND->createHistograms            = true;
  globalConfigND->scaleHistograms             = true;
  globalConfigND->calculateDerivedHistograms  = false;
  globalConfigND->saveHistograms              = true;
  globalConfigND->forceHistogramsRewrite      = true;
  globalConfigND->subsampleAnalysis           = subsampleAnalysis;
  globalConfigND->partialSave                 = partialSave;
  globalConfigND->outputPath                  = outputPathName;
  globalConfigND->rootOuputFileName           = outputFileNameBase;
  globalConfigND->countParticles              = true;
  globalConfigND->setEvent                    = true;
  globalConfigND->fillCorrelationHistos       = false;
  globalConfigND->nBins_n  = 250;
  globalConfigND->nBins_n2 = 40;
  globalConfigND->min_n    = 0.0;
  globalConfigND->max_n    = 20000;
  globalConfigND->nBins_e  = 400;
  globalConfigND->nBins_e2 = 20;
  globalConfigND->min_e    = 0.0;
  globalConfigND->max_e    = 40000.0;
  globalConfigND->nBins_q  = 200;
  globalConfigND->nBins_q2 = 20;
  globalConfigND->min_q    = -100.0;
  globalConfigND->max_q    = 100.0;
  globalConfigND->nBins_b  = 200;
  globalConfigND->nBins_b2 = 200;
  globalConfigND->min_b    = -20.0;
  globalConfigND->max_b    = 20.0;
  globalConfigND->validate();
  GlobalAnalyzerConfiguration * globalConfigWD = new GlobalAnalyzerConfiguration(*globalConfigND);
  globalConfigWD->setEvent = false;
  GlobalAnalyzerConfiguration * globalConfigBS = new GlobalAnalyzerConfiguration(*globalConfigND);


  ParticleAnalyzerConfiguration * p1ConfigND = new ParticleAnalyzerConfiguration();
  p1ConfigND->useEventStream0             = true;
  p1ConfigND->loadHistograms              = false;
  p1ConfigND->createHistograms            = true;
  p1ConfigND->scaleHistograms             = true;
  p1ConfigND->calculateDerivedHistograms  = true;
  p1ConfigND->saveHistograms              = true;
  p1ConfigND->forceHistogramsRewrite      = true;
  p1ConfigND->partialSave                 = partialSave;
  p1ConfigND->subsampleAnalysis           = subsampleAnalysis;
  p1ConfigND->inputPath                   = inputPathName;
  p1ConfigND->outputPath                  = outputPathName;
  p1ConfigND->rootOuputFileName           = outputFileNameBase;
  p1ConfigND->nBins_n1                    = 120;
  p1ConfigND->min_n1                      = 10.0;
  p1ConfigND->max_n1                      = 12000.0;
  p1ConfigND->nBins_eTot                  = 100;
  p1ConfigND->min_eTot                    = 0.0;
  p1ConfigND->max_eTot                    = 1000000.0;

  p1ConfigND->nBins_pt    = 200;
  p1ConfigND->min_pt      = 0.00;
  p1ConfigND->max_pt      = 20.0;
  p1ConfigND->nBins_eta   = 80;
  p1ConfigND->min_eta     = -4;
  p1ConfigND->max_eta     =  4;
  p1ConfigND->nBins_y     = 80;
  p1ConfigND->min_y       = -4;
  p1ConfigND->max_y       = 4;
  p1ConfigND->nBins_phi   = 36;
  p1ConfigND->min_phi     = 0.0;
  p1ConfigND->max_phi     = 2.0*3.1415927;
  p1ConfigND->validate();
  ParticleAnalyzerConfiguration * p1ConfigWD = new ParticleAnalyzerConfiguration(*p1ConfigND);
  ParticleAnalyzerConfiguration * p1ConfigBS = new ParticleAnalyzerConfiguration(*p1ConfigND);


  ParticlePairAnalyzerConfiguration * p2ConfigND = new ParticlePairAnalyzerConfiguration();
  p2ConfigND->useEventStream0             = true;
  p2ConfigND->createHistograms            = true;
  p2ConfigND->scaleHistograms             = true;
  p2ConfigND->calculateDerivedHistograms  = true;
  p2ConfigND->calculateCombinedHistograms = true;
  p2ConfigND->saveHistograms              = true;
  p2ConfigND->forceHistogramsRewrite      = true;
  p2ConfigND->partialSave                 = partialSave;
  p2ConfigND->subsampleAnalysis           = subsampleAnalysis;
  p2ConfigND->inputPath                   = inputPathName;
  p2ConfigND->outputPath                  = outputPathName;
  p2ConfigND->rootOuputFileName           = outputFileNameBase;
  p2ConfigND->nBins_pt    = 18;
  p2ConfigND->min_pt      = 0.200;
  p2ConfigND->max_pt      = 2.0;
  p2ConfigND->nBins_eta   = 40;
  p2ConfigND->min_eta     = -2;
  p2ConfigND->max_eta     = 2;
  p2ConfigND->nBins_y     = 40;
  p2ConfigND->min_y       = -2;
  p2ConfigND->max_y       = 2;
  p2ConfigND->nBins_phi   = 36;
  p2ConfigND->min_phi     = 0.0;
  p2ConfigND->max_phi     = 2.0*3.1415927;

  RadialBoostConfiguration * radialBoostConfig = new RadialBoostConfiguration();
  radialBoostConfig->useEventStream0   = true;
  radialBoostConfig->loadHistograms    = true;
  radialBoostConfig->createHistograms  = true;
  radialBoostConfig->scaleHistograms   = true;
  radialBoostConfig->saveHistograms         = true;
  radialBoostConfig->forceHistogramsRewrite = true;
  radialBoostConfig->max_r       = 16.0;
  radialBoostConfig->param_a     = 0.4;
  radialBoostConfig->param_b     = 1.0;
  radialBoostConfig->betaMaximum = 0.999;
  radialBoostConfig->baseName    = "CGGA";// CG/PbPb/PbPb_Geom_Gradients.root";
  radialBoostConfig->inputPath   = "/Users/claudeapruneau/Documents/GitHub/WAC4/data//OutputFiles/CG/PbPb/"; //  PbPb_Geom_Gradients.root;
  radialBoostConfig->rootInputFileName = "Geom_CGGA_Gradients.root";
  radialBoostConfig->outputPath        = outputPathName;
  radialBoostConfig->rootOuputFileName = outputFileNameBase;

  // =========================================
  // Setup all event filters
  // =========================================

  vector<EventFilter*> eventFiltersGeomGen;
  vector<EventFilter*> eventFiltersGeomAna;
  vector<EventFilter*> eventFiltersPythiaGen;
  vector<EventFilter*> eventFiltersPythiaAna;
  vector<EventFilter*> eventFiltersGlobalAna;
  vector<EventFilter*> eventFiltersP1Ana;
  vector<EventFilter*> eventFiltersP2Ana;
  vector<EventFilter*> eventFiltersRadialBoost;

  int centralityOptionGeom    = 0;
  int centralityOptionPythia  = 0;
  int centralityOptionGlobal  = 0;
  int centralityOptionP1      = 0;
  int centralityOptionP2      = 0;
  int centralityOptionRadial  = 0;

  EventFilter * openEventFilter = new EventFilter(EventFilter::MinBias,0.0,0.0);

  switch (centralityOptionGeom)
    {
      default:
      case 0: // Minimum bias only -- open wide
      {
      eventFiltersGeomGen.push_back( openEventFilter);
      eventFiltersGeomAna.push_back( openEventFilter);
      }
      break;

      case 1:  // Bins in impact parameter b
      {
      eventFiltersGeomGen.push_back( openEventFilter);
      vector<double> limits = { 0.0, 1.0, 2.0, 3.0, 4.0, 6.0, 8.0, 10.0, 12.0, 16.0, 20.0};
      eventFiltersGeomAna = EventFilter::createEventFilterSet(EventFilter::ImpactParameter,limits);
      }
      break;
    }

  switch (centralityOptionPythia)
    {
      default:
      case 0: // Minimum bias only -- open wide
      {
      eventFiltersPythiaGen.push_back( openEventFilter);
      }
      break;
    }

  switch (centralityOptionGlobal)
    {
      default:
      case 0: // Minimum bias only -- open wide
      {
      eventFiltersGlobalAna.push_back( openEventFilter);
      }
      break;

      case 1:  // Bins in impact parameter b
      vector<double> limits = { 0.0, 1.0, 2.0, 3.0, 4.0, 6.0, 8.0, 10.0, 12.0, 16.0, 20.0};
      eventFiltersGlobalAna = EventFilter::createEventFilterSet(EventFilter::ImpactParameter,limits);
      break;
    }

  switch (centralityOptionP1)
    {
      default:
      case 0: // Minimum bias only -- open wide
      {
      eventFiltersP1Ana.push_back( openEventFilter);
      }
      break;

      case 1:  // Bins in impact parameter b
      {
      vector<double> limits = { 0.0, 1.0, 2.0, 3.0, 4.0, 6.0, 8.0, 10.0, 12.0, 16.0, 20.0};
      eventFiltersP1Ana = EventFilter::createEventFilterSet(EventFilter::ImpactParameter,limits);
      }
      break;

      case 2:  // Min bias in V0M multiplicity
      {
      eventFiltersP1Ana.push_back( new EventFilter(EventFilter::ParticleFilter0,1.0,1000.0) );
      }
      break;

      case 3:
      {
      vector<double> limits = { 0.0, 1.0, 2.0, 3.0, 4.0, 6.0, 8.0, 10.0, 12.0, 16.0, 20.0};
      eventFiltersP1Ana = EventFilter::createEventFilterSet(EventFilter::ParticleFilter0,limits);
      }
      break;
      break;
    }

  switch (centralityOptionP2)
    {
      default:
      case 0: // Minimum bias only -- open wide
      {
      eventFiltersP2Ana.push_back( openEventFilter);
      }
      break;

      case 1:  // Bins in impact parameter b
      {
      vector<double> limits = { 0.0, 1.0, 2.0, 3.0, 4.0, 6.0, 8.0, 10.0, 12.0, 16.0, 20.0};
      eventFiltersP2Ana = EventFilter::createEventFilterSet(EventFilter::ImpactParameter,limits);
      }
      break;

      case 2:  // Min bias in V0M multiplicity
      {
      eventFiltersP2Ana.push_back( new EventFilter(EventFilter::ParticleFilter0,1.0,1000.0) );
      }
      break;

      case 3:
      {
      vector<double> limits = { 0.0, 1.0, 2.0, 3.0, 4.0, 6.0, 8.0, 10.0, 12.0, 16.0, 20.0};
      eventFiltersP2Ana = EventFilter::createEventFilterSet(EventFilter::ParticleFilter0,limits);
      }
      break;
    }

  switch (centralityOptionRadial)
    {
      default:
      case 0:
      {
      vector<double> limits = { 0.0, 0.5, 1.0, 2.0, 3.0, 4.0, 6.0, 8.0, 10.0, 12.0, 16.0, 20.0};
      eventFiltersRadialBoost = EventFilter::createEventFilterSet(EventFilter::ImpactParameter,limits);
      }
      break;
    }



//  limits.push_back(0.0);
//  //limits.push_back(2.0);   // 90-100%
//  limits.push_back(7.0);   // 80-90%
//  //limits.push_back(10.0);  // 70-80%
//  limits.push_back(12.0);  // 60-70%
//  //limits.push_back(16.0);  // 50-60%
//  limits.push_back(20.0);  // 40-50%
//  //limits.push_back(28.0);  // 30-40%
//  limits.push_back(38.0);  // 20-30%
//  //limits.push_back(53.0);  // 10-20%
//  limits.push_back(400.0); // 0-10%



  // =========================================
  // Setup all particle filters
  // =========================================

  vector<ParticleFilter*>  particleFiltersPythiaGen;
  vector<ParticleFilter*>  particleFiltersGlobalAna ;
  vector<ParticleFilter*>  particleFiltersP1Ana;
  vector<ParticleFilter*>  particleFiltersP2Ana;

  ParticleFilter* openParticleFilter     = new ParticleFilter(0.001, 100.0, -6.0, 6.0, 10.0, -10.0,ParticleFilter::Hadron,ParticleFilter::Charged);
  ParticleFilter* aliceV0ParticleFilter  = new ParticleFilterAliceV0();
  ParticleFilter* aliceTpcParticleFilter = new ParticleFilter(0.200, 100.0, -1.0, 1.0, 10.0, -10.0,ParticleFilter::Hadron,ParticleFilter::Charged);

  particleFiltersPythiaGen.push_back( openParticleFilter     );
  particleFiltersGlobalAna.push_back( aliceV0ParticleFilter  );
  particleFiltersGlobalAna.push_back( aliceTpcParticleFilter );

  int analysisOptionP1 = 0;
  int analysisOptionP2 = 0;

  switch (analysisOptionP1)
    {
      default:
      case 0:  particleFiltersP1Ana.push_back( openParticleFilter      ); break;
      case 1:  particleFiltersP1Ana.push_back( aliceV0ParticleFilter   ); break;
      case 2:  particleFiltersP1Ana.push_back( aliceTpcParticleFilter  ); break;
      case 3:  particleFiltersP1Ana = ParticleFilter::createChargedHadronFilters(0.2, 10.0, -6.0, 6.0, 10.0, -10.0); break;
      case 4:  particleFiltersP1Ana = ParticleFilter::createChargedHadronFilters(0.2, 10.0, -1.0, 1.0, 10.0, -10.0); break;
      case 5:  particleFiltersP1Ana = ParticleFilter::createPlusMinusHadronFilters(0.2, 10.0, -1.0, 1.0, 10.0, -10.0); break;
    }

  switch (analysisOptionP2)
    {
      default:
      case 0:  particleFiltersP2Ana.push_back( openParticleFilter      ); break;
      case 1:  particleFiltersP2Ana.push_back( aliceV0ParticleFilter   ); break;
      case 2:  particleFiltersP2Ana.push_back( aliceTpcParticleFilter ); break;
      case 3:  particleFiltersP2Ana = ParticleFilter::createChargedHadronFilters(0.2, 10.0, -6.0, 6.0, 10.0, -10.0); break;
      case 4:  particleFiltersP2Ana = ParticleFilter::createChargedHadronFilters(0.2, 10.0, -1.0, 1.0, 10.0, -10.0); break;
      case 5:  particleFiltersP2Ana = ParticleFilter::createPlusMinusHadronFilters(0.2, 10.0, -1.0, 1.0, 10.0, -10.0); break;
    }


  vector<unsigned int> comb;
  comb.push_back(0);comb.push_back(3); p2ConfigND->combinations.push_back( comb ); comb.clear();
  comb.push_back(0);comb.push_back(1); comb.push_back(3);comb.push_back(4); p2ConfigND->combinations.push_back( comb );comb.clear();
  comb.push_back(0);comb.push_back(2); comb.push_back(3);comb.push_back(5); p2ConfigND->combinations.push_back( comb );comb.clear();
  comb.push_back(1);comb.push_back(0); comb.push_back(4);comb.push_back(3); p2ConfigND->combinations.push_back( comb );comb.clear();
  comb.push_back(1);comb.push_back(4); p2ConfigND->combinations.push_back( comb );  comb.clear();
  comb.push_back(1);comb.push_back(2); comb.push_back(4);comb.push_back(5); p2ConfigND->combinations.push_back( comb );comb.clear();
  comb.push_back(2);comb.push_back(0); comb.push_back(5);comb.push_back(3); p2ConfigND->combinations.push_back( comb );comb.clear();
  comb.push_back(2);comb.push_back(1); comb.push_back(5);comb.push_back(4); p2ConfigND->combinations.push_back( comb );comb.clear();
  comb.push_back(2);comb.push_back(5); p2ConfigND->combinations.push_back( comb );

  // ND : no decay
  // WD : with decays
  // BS : with boost

  CollisionGeometryGenerator * collisionGeomGenerator = new CollisionGeometryGenerator("CGG", cggConfig,eventFiltersGeomGen,messageLevel);
  CollisionGeometryAnalyzer  * collisionGeomAnalyzer  = new CollisionGeometryAnalyzer ("CGGA",cggConfig,eventFiltersGeomAna,messageLevel);
  PythiaEventGenerator * pythiaGenerator         = new PythiaEventGenerator("PythiaEventGenerator",pythiaConfig,eventFiltersPythiaGen,particleFiltersPythiaGen, messageLevel);
  ParticleDecayerTask  * decayerTask             = new ParticleDecayerTask();
  RadialBoostTask      * radialBoostTask         = new RadialBoostTask ("Boost",radialBoostConfig, eventFiltersRadialBoost, messageLevel);

  GlobalAnalyzer   * globalAnalyzerND            = new GlobalAnalyzer(  "GlobalND",globalConfigND,eventFiltersGlobalAna,particleFiltersGlobalAna,messageLevel);
  GlobalAnalyzer   * globalAnalyzerWD            = new GlobalAnalyzer(  "GlobalWD",globalConfigWD,eventFiltersGlobalAna,particleFiltersGlobalAna,messageLevel);
  GlobalAnalyzer   * globalAnalyzerBS            = new GlobalAnalyzer(  "GlobalBs",globalConfigBS,eventFiltersGlobalAna,particleFiltersGlobalAna,messageLevel);
  ParticleAnalyzer * particleAnalyzerND          = new ParticleAnalyzer("P1ND", p1ConfigND, eventFiltersP1Ana, particleFiltersP1Ana, messageLevel);
  ParticleAnalyzer * particleAnalyzerWD          = new ParticleAnalyzer("P1WD", p1ConfigWD, eventFiltersP1Ana, particleFiltersP1Ana, messageLevel);
  ParticleAnalyzer * particleAnalyzerBS          = new ParticleAnalyzer("P1BS", p1ConfigBS, eventFiltersP1Ana, particleFiltersP1Ana, messageLevel);

//  vector<unsigned int> comb;
//  comb.push_back(0);comb.push_back(3); p2ConfigND->combinations.push_back( comb ); comb.clear();
//  comb.push_back(0);comb.push_back(1); comb.push_back(3);comb.push_back(4); p2ConfigND->combinations.push_back( comb );comb.clear();
//  comb.push_back(0);comb.push_back(2); comb.push_back(3);comb.push_back(5); p2ConfigND->combinations.push_back( comb );comb.clear();
//  comb.push_back(1);comb.push_back(0); comb.push_back(4);comb.push_back(3); p2ConfigND->combinations.push_back( comb );comb.clear();
//  comb.push_back(1);comb.push_back(4); p2ConfigND->combinations.push_back( comb );  comb.clear();
//  comb.push_back(1);comb.push_back(2); comb.push_back(4);comb.push_back(5); p2ConfigND->combinations.push_back( comb );comb.clear();
//  comb.push_back(2);comb.push_back(0); comb.push_back(5);comb.push_back(3); p2ConfigND->combinations.push_back( comb );comb.clear();
//  comb.push_back(2);comb.push_back(1); comb.push_back(5);comb.push_back(4); p2ConfigND->combinations.push_back( comb );comb.clear();
//  comb.push_back(2);comb.push_back(5); p2ConfigND->combinations.push_back( comb );
//  ParticlePairAnalyzer * particlePairAnalyzerND  = new ParticlePairAnalyzer("P2ND", p2ConfigND, eventFiltersP2Ana, particleFiltersP2Ana, messageLevel);
//  ParticlePairAnalyzer * particlePairAnalyzerWD  = new ParticlePairAnalyzer("P2WD", p2ConfigWD, eventFiltersP2Ana, particleFiltersP2Ana, messageLevel);
//  ParticlePairAnalyzer * particlePairAnalyzerBS  = new ParticlePairAnalyzer("P2BS", p2ConfigBS, eventFiltersP2Ana, particleFiltersP2Ana, messageLevel);

  TaskIterator * masterTask = new TaskIterator();
  masterTask->setName("IteratorTask");
  masterTask->setNEventRequested(nEventRequested);
  masterTask->setNEventReported(nEventReported);
  masterTask->setReportLevel(messageLevel);
  masterTask->setNEventPartialSave(nEventPartialSave);
  masterTask->setPartialSave(partialSave);
  masterTask->setSubsampleAnalysis(subsampleAnalysis);
  masterTask->addSubtask( collisionGeomGenerator );
  masterTask->addSubtask( collisionGeomAnalyzer  );
  masterTask->addSubtask( pythiaGenerator        );
  masterTask->addSubtask( globalAnalyzerND       );
  masterTask->addSubtask( particleAnalyzerND     );
  //masterTask->addSubtask( particlePairAnalyzerND );
  masterTask->addSubtask( radialBoostTask );
  //masterTask->addSubtask( decayerTask );
  //masterTask->addSubtask( globalAnalyzerWD );
  //masterTask->addSubtask( particleAnalyzerWD );
  //masterTask->addSubtask( particlePairAnalyzerWD );
  masterTask->addSubtask( globalAnalyzerBS );
  masterTask->addSubtask( particleAnalyzerBS );
  //masterTask->addSubtask( particlePairAnalyzerBS );
  masterTask->run();

  return 0;
}
