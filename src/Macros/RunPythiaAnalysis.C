//  Created by Claude Pruneau on 6/19/2020.
//  Copyright © 2020 Claude Pruneau. All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <TStyle.h>
#include <TROOT.h>

int RunPythiaAnalysis()
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

  MessageLogger::LogLevel messageLevel = MessageLogger::Info; // MessageLogger::Debug; //
  unsigned long nEventRequested   = 199999;
  unsigned long nEventReported    = 10000;
  unsigned long nEventPartialSave =  10000;
  bool    partialSave             = true;
  bool    subsampleAnalysis       = true;
  double  beamEnergy              = 7000.0; // GeV
  double  minBias                 = true; // alternative is AliceV0
  TString outputFileNameBase      = "PYTHIA_pp_7TeV_inelastic_";
  TString inputPathName           = getenv("WAC_INPUT_PATH");
  TString outputPathName          = getenv("WAC_OUTPUT_PATH");
  inputPathName  += "/PYTHIA/7TEV/";
  outputPathName += "/PYTHIA/7TEV/SpeedTest/";
  gSystem->mkdir(outputPathName,1);

  std::cout << "==================================================================================" << std::endl;
  std::cout << "PYTHIA Model Analysis - Single Particle Histograms" << endl;
  std::cout << "==================================================================================" << std::endl;
  TString pdgDataTable = getenv("WAC_SRC");
  pdgDataTable += "/EOS/pdg.dat";
  ParticleTypeCollection * particles = ParticleTypeCollection::getMasterParticleCollection(); //ParticleTypeCollection::getMasterParticleCollection();
  particles->readFromFile(pdgDataTable);
  particles->printProperties(std::cout);

  vector<TString*> pythiaOptions;
  pythiaOptions.push_back( new TString("Init:showChangedSettings = on") );      // list changed settings
  pythiaOptions.push_back( new TString("Init:showChangedParticleData = off") ); // list changed particle data
  pythiaOptions.push_back( new TString("Next:numberCount = 10000") );            // print message every n events
  pythiaOptions.push_back( new TString("Next:numberShowInfo = 1") );            // print event information n times
  pythiaOptions.push_back( new TString("Next:numberShowProcess = 0") );         // print process record n times
  pythiaOptions.push_back( new TString("Next:numberShowEvent = 0") );
  pythiaOptions.push_back( new TString("SoftQCD:inelastic = on") );             // All inelastic processes
  pythiaOptions.push_back( new TString("ColourReconnection:reconnect = on") );

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
/*
 To generate enhanced charm production -- received from Pet Christianssen. Jul 16-2021.
  // Initialize PYTHIA minbias Generator.
   Pythia8::Pythia pythia;
   pythia.readString("Beams:eCM = 5000."); // 5 TeV pp
   //  pythia.readString("Beams:eCM = 13000."); // 13 TeV pp
   pythia.readString("HardQCD:gg2ccbar = on");
   pythia.readString("HardQCD:qqbar2ccbar = on");

   // Prevent charm hadrons from decaying.
   pythia.readString("ParticleDecays:limitTau0 = on");
   pythia.readString("ParticleDecays:tau0Max = 0");

   pythia.init();


  I select the final state hadrons like this:
      // Final
       if (!pythia.event[i].isFinal())
         continue;

       // Hadron
       if(!pythia.event[i].isHadron())
         continue;

       // print out all charm hadrons
       const int pdg = TMath::Abs(pythia.event[i].id()%10000); // only 3 digits meson
       // or 4 digits baryon
       if(pdg < 1000) { // meson
         if(int(pdg/100) != 4)
           continue;
       } else {

         if(int(pdg/1000) != 4)
           continue;
       }
*/

  //pythia.readString("310:mayDecay = no");//K0s decay off
  //pythia.readString("ParticleDecays:limitTau0 = on");
  //pythia.readString("ParticleDecays:tau0Max = 10");


  //pythiaOptions.push_back( new TString("SoftQCD:all = on") );                   // Allow total sigma = elastic/SD/DD/ND
                                                                                // pythiaOptions.push_back(  new TString("HardQCD:all = on");
  PythiaConfiguration * pythiaConfig = new PythiaConfiguration();
  pythiaConfig->beam                = 2212;  // PDG Code   proton is 2212
  pythiaConfig->target              = 2212;
  pythiaConfig->energy              = beamEnergy;
  pythiaConfig->options             = pythiaOptions;
  pythiaConfig->useEventStream0     = true;
  pythiaConfig->standaloneMode      = true; 
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

  GlobalAnalyzerConfiguration * globalConfig1 = new GlobalAnalyzerConfiguration();
  globalConfig1->useEventStream0             = true;
  globalConfig1->loadHistograms              = false;
  globalConfig1->createHistograms            = true;
  globalConfig1->scaleHistograms             = true;
  globalConfig1->calculateDerivedHistograms  = false;
  globalConfig1->saveHistograms              = true;
  globalConfig1->forceHistogramsRewrite      = true;
  globalConfig1->subsampleAnalysis           = subsampleAnalysis;
  globalConfig1->partialSave                 = partialSave;
  globalConfig1->outputPath                  = outputPathName;
  globalConfig1->rootOuputFileName           = outputFileNameBase;
  globalConfig1->countParticles              = true;
  globalConfig1->setEvent                    = true;
  globalConfig1->fillCorrelationHistos       = true;
  globalConfig1->nBins_n  = 400;
  globalConfig1->nBins_n2 = 400;
  globalConfig1->min_n    = -0.5;
  globalConfig1->max_n    = 399.5;
  globalConfig1->nBins_e  = 200;
  globalConfig1->nBins_e2 = 20;
  globalConfig1->min_e    = 0.0;
  globalConfig1->max_e    = 1000.0;
  globalConfig1->nBins_q  = 200;
  globalConfig1->nBins_q2 = 20;
  globalConfig1->min_q    = -100.0;
  globalConfig1->max_q    = 100.0;
  globalConfig1->nBins_b  = 200;
  globalConfig1->nBins_b2 = 200;
  globalConfig1->min_b    = -20.0;
  globalConfig1->max_b    = 20.0;
  globalConfig1->validate();
  GlobalAnalyzerConfiguration * globalConfig2 = new GlobalAnalyzerConfiguration(*globalConfig1);
  globalConfig2->setEvent               = false;
  globalConfig2->fillCorrelationHistos  = true;

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
  p1ConfigND->nBins_n1                    = 200;
  p1ConfigND->min_n1                      = 0.0;
  p1ConfigND->max_n1                      = 200.0;
  p1ConfigND->nBins_eTot                  = 100;
  p1ConfigND->min_eTot                    = 0.0;
  p1ConfigND->max_eTot                    = 1000.0;

  p1ConfigND->nBins_pt    = 200;
  p1ConfigND->min_pt      = 0.00;
  p1ConfigND->max_pt      = 10.0;
  p1ConfigND->nBins_eta   = 20;
  p1ConfigND->min_eta     = -1;
  p1ConfigND->max_eta     =  1;
  p1ConfigND->nBins_y     = 20;
  p1ConfigND->min_y       = -1;
  p1ConfigND->max_y       = 1;
  p1ConfigND->nBins_phi   = 36;
  p1ConfigND->min_phi     = 0.0;
  p1ConfigND->max_phi     = 2.0*3.1415927;
  p1ConfigND->validate();


  ParticlePairAnalyzerConfiguration * p2ConfigND = new ParticlePairAnalyzerConfiguration();
  p2ConfigND->useEventStream0             = true;
  p2ConfigND->createHistograms            = true;
  p2ConfigND->scaleHistograms             = true;
  p2ConfigND->calculateDerivedHistograms  = true;
  p2ConfigND->calculateCombinedHistograms = false;
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
  p2ConfigND->nBins_eta   = 20;
  p2ConfigND->min_eta     = -1;
  p2ConfigND->max_eta     = 1;
  p2ConfigND->nBins_y     = 20;
  p2ConfigND->min_y       = -1;
  p2ConfigND->max_y       = 1;
  p2ConfigND->nBins_phi   = 36;
  p2ConfigND->min_phi     = 0.0;
  p2ConfigND->max_phi     = 2.0*3.1415927;


  // =========================================
  // Setup all event filters
  // =========================================

  vector<EventFilter*> eventFiltersPythiaGen;
  vector<EventFilter*> eventFiltersPythiaAna;
  vector<EventFilter*> eventFiltersGlobalAna1;
  vector<EventFilter*> eventFiltersGlobalAna2;
  vector<EventFilter*> eventFiltersP1Ana;
  vector<EventFilter*> eventFiltersP2Ana;

  int centralityOptionGeom    = 0;
  int centralityOptionPythia  = 0;
  int centralityOptionGlobal  = 1;
  int centralityOptionP1      = 4;
  int centralityOptionP2      = 4;

  EventFilter * openEventFilter  = new EventFilter(EventFilter::MinBias,0.0,0.0);
  EventFilter * aliceEventFilter = new EventFilterAliceV0(1.0, 1000.0, 1.0, 1000.0);


  switch (centralityOptionPythia)
    {
      default:
      case 0: // Minimum bias only -- open wide
      {
      eventFiltersPythiaGen.push_back( openEventFilter);
      }
      break;
    }

  eventFiltersGlobalAna1.push_back( openEventFilter);

  switch (centralityOptionGlobal)
    {
      default:
      case 0: // Minimum bias only -- open wide
      {
      eventFiltersGlobalAna2.push_back( openEventFilter);
      }
      break;

      case 1: // ALICE V0
      {
      eventFiltersGlobalAna2.push_back( aliceEventFilter);
      }
      break;

      case 2:  // Bins in impact parameter b
      vector<double> limits = { 0.0, 1.0, 2.0, 3.0, 4.0, 6.0, 8.0, 10.0, 12.0, 16.0, 20.0};
      eventFiltersGlobalAna2 = EventFilter::createEventFilterSet(EventFilter::ImpactParameter,limits);
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

      case 1: // ALICE V0 + 1 TPC Hit
      {
      eventFiltersP1Ana.push_back( aliceEventFilter);
      }
      break;

      case 2:  // Bins in impact parameter b
      {
      vector<double> limits = { 0.0, 1.0, 2.0, 3.0, 4.0, 6.0, 8.0, 10.0, 12.0, 16.0, 20.0};
      eventFiltersP1Ana = EventFilter::createEventFilterSet(EventFilter::ImpactParameter,limits);
      }
      break;

      case 3:
      {
      vector<double> limits = { 0.0, 1.0, 2.0, 3.0, 4.0, 6.0, 8.0, 10.0, 12.0, 16.0, 20.0};
      eventFiltersP1Ana = EventFilter::createEventFilterSet(EventFilter::ParticleFilter0,limits);
      }
      break;

      case 4:
      {
      vector<double> limits = { 0.0, 4.0, 7.0, 12.0, 200.0};
      eventFiltersP1Ana = EventFilterAliceV0::createEventFilterAliceV0Set(limits);
      }
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

      case 1: // ALICE V0 + 1 TPC Hit
      {
      eventFiltersP2Ana.push_back( aliceEventFilter);
      }
      break;

      case 2:  // Bins in impact parameter b
      {
      vector<double> limits = { 0.0, 1.0, 2.0, 3.0, 4.0, 6.0, 8.0, 10.0, 12.0, 16.0, 20.0};
      eventFiltersP2Ana = EventFilter::createEventFilterSet(EventFilter::ImpactParameter,limits);
      }
      break;

      case 3:
      {
      vector<double> limits = { 0.0, 1.0, 2.0, 3.0, 4.0, 6.0, 8.0, 10.0, 12.0, 16.0, 20.0};
      eventFiltersP2Ana = EventFilter::createEventFilterSet(EventFilter::ParticleFilter0,limits);
      }
      break;

      case 4:
      {
      vector<double> limits = {  0.0, 4.0, 7.0, 12.0, 200.0 };
      eventFiltersP2Ana = EventFilterAliceV0::createEventFilterAliceV0Set(limits);
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
  vector<double> limits;

  ParticleFilter* openParticleFilter     = new ParticleFilter(0.001, 100.0, -6.0, 6.0, 10.0, -10.0,ParticleFilter::Hadron,ParticleFilter::Charged);
  ParticleFilter* aliceV0ParticleFilter  = new ParticleFilterAliceV0(ParticleFilterAliceV0::V0M,ParticleFilter::Hadron,ParticleFilter::Charged,ParticleFilter::Live,0.001,50.0);
  ParticleFilter* aliceTpcParticleFilter = new ParticleFilter(0.200, 100.0, -1.0, 1.0, 10.0, -10.0,ParticleFilter::Hadron,ParticleFilter::Charged);

  particleFiltersPythiaGen.push_back( openParticleFilter     );
  particleFiltersGlobalAna.push_back( aliceV0ParticleFilter  );
  particleFiltersGlobalAna.push_back( aliceTpcParticleFilter );

  int analysisOptionP1 = 2;
  int analysisOptionP2 = 2;

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

  // ND : no decay
  // WD : with decays
  // BS : with boost

  PythiaEventGenerator * pythiaGenerator         = new PythiaEventGenerator("PythiaEventGenerator",pythiaConfig,eventFiltersPythiaGen,particleFiltersPythiaGen, messageLevel);
  GlobalAnalyzer   * globalAnalyzer1             = new GlobalAnalyzer(  "G1Pre",globalConfig1,eventFiltersGlobalAna1,particleFiltersGlobalAna,messageLevel);
  GlobalAnalyzer   * globalAnalyzer2             = new GlobalAnalyzer(  "G1",   globalConfig2,eventFiltersGlobalAna2,particleFiltersGlobalAna,messageLevel);
  ParticleAnalyzer * particleAnalyzer            = new ParticleAnalyzer("P1",   p1ConfigND, eventFiltersP1Ana, particleFiltersP1Ana, messageLevel);
  ParticlePairAnalyzer * particlePairAnalyzer    = new ParticlePairAnalyzer("P2", p2ConfigND, eventFiltersP2Ana, particleFiltersP2Ana, messageLevel);

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

  TaskIterator * masterTask = new TaskIterator();
  masterTask->setName("IteratorTask");
  masterTask->setNEventRequested(nEventRequested);
  masterTask->setNEventReported(nEventReported);
  masterTask->setReportLevel(messageLevel);
  masterTask->setNEventPartialSave(nEventPartialSave);
  masterTask->setPartialSave(partialSave);
  masterTask->setSubsampleAnalysis(subsampleAnalysis);
  masterTask->addSubtask( pythiaGenerator        );
  masterTask->addSubtask( globalAnalyzer1        );
  masterTask->addSubtask( globalAnalyzer2        );
  masterTask->addSubtask( particleAnalyzer       );
  masterTask->addSubtask( particlePairAnalyzer );
  masterTask->run();

  return 0;
}
