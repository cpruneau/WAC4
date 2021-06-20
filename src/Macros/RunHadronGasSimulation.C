//==============================================================================
//  calculate the thermodynamic quantities of hadron resonance gas
//==============================================================================
#include <iostream>
#include <fstream>
#include <TStyle.h>
#include <TROOT.h>
using namespace std;


int RunHadronGasSimulation()
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
  gSystem->Load(includesPath+"ParticleDecayerTask.hpp");
  gSystem->Load(includesPath+"ParticleAnalyzer.hpp");
  gSystem->Load(includesPath+"ParticlePairAnalyzer.hpp");
  gSystem->Load(includesPath+"RadialBoostTask.hpp");
  gSystem->Load(includesPath+"SubSampleStatCalculator.hpp");
  gSystem->Load("libBase.dylib");

  //  includesPath = "/Users/claudeapruneau/Documents/GitHub/WAC/WacPythia/";
  //  gSystem->Load(includesPath+"PythiaConfiguration.hpp");
  //  gSystem->Load(includesPath+"PythiaEventGenerator.hpp");
  //  gSystem->Load(includesPath+"PythiaEventReader.hpp");
  //  gSystem->Load("libWacPythia.dylib");

  includesPath = "/Users/claudeapruneau/Documents/GitHub/WAC/HadronGas/";
  gSystem->Load(includesPath+"HadronGas.hpp");
  gSystem->Load(includesPath+"HadronGasGeneratorConfiguration.hpp");
  gSystem->Load(includesPath+"HadronGasHistograms.hpp");
  gSystem->Load(includesPath+"HadronGasVsTempHistograms.hpp");
  gSystem->Load("libHadronGas.dylib");

  MessageLogger::LogLevel messageLevel = MessageLogger::Info; //MessageLogger::Debug; //
  unsigned long nEventRequested   = 10000;
  unsigned long nEventReported    = 10000;
  unsigned long nEventPartialSave = 500;
  bool    partialSave             = false;
  bool    subsampleAnalysis       = false;
  double  beamEnergy              = 7000.0; // GeV
  double  minBias                 = true; // alternative is AliceV0
  TString outputFileNameBase      = "HG_200MeV_";
  TString inputPathName           = getenv("WAC_INPUT_PATH");
  TString outputPathName          = getenv("WAC_OUTPUT_PATH");
  inputPathName  += "/HG/";
  outputPathName += "/HG/SmallTest/";
  gSystem->mkdir(outputPathName,1);

  std::cout << "==================================================================================" << std::endl;
  std::cout << "Hadron Gas Model Analysis " << endl;
  std::cout << "==================================================================================" << std::endl;
  TString pdgDataTable = getenv("WAC_SRC");
  pdgDataTable += "/EOS/pdg.dat";
  ParticleTypeCollection * particles = ParticleTypeCollection::getMasterParticleCollection(); //ParticleTypeCollection::getMasterParticleCollection();
  particles->readFromFile(pdgDataTable);
  //particles->printProperties(std::cout);
  ParticleTypeCollection * stableParticles = particles->extractCollection(1);
  //stableParticles->printProperties(std::cout);

//  ofstream decayList;
//  decayList.open ("fullDecayList.tex");
//  particles->printDecayProperties(decayList);
//  decayList.close();


  // =====================================
  // Configure iterator task and run it...
  // =====================================


  HadronGasGeneratorConfiguration * hggc = new HadronGasGeneratorConfiguration();
  hggc->volume    = 1.0; // not used here
  hggc->totalMult = 100; // fixed
  hggc->generatorType = MomentumGenerator::CylindricalMaxwellPtUniformY;
  hggc->momentumGeneratorParameters.push_back(0.0);  // mass set later
  hggc->momentumGeneratorParameters.push_back(-2.0); // min rapidity
  hggc->momentumGeneratorParameters.push_back( 4.0); // max rapidity
  hggc->momentumGeneratorParameters.push_back(0.200);  // pt width
  hggc->nT    = 1; // one temperature value
  hggc->minT  = 0.200;
  hggc->maxT  = 0.200;
  hggc->useEventStream0     = true;
  hggc->dataInputUsed       = false;
  hggc->dataInputFileName   = "";
  hggc->createHistograms    = false;
  hggc->scaleHistograms     = false;
  hggc->saveHistograms      = false;
  hggc->subsampleAnalysis   = false;
  hggc->partialSave         = false;
  hggc->outputPath          = outputPathName;
  hggc->rootOuputFileName   = outputFileNameBase;


  GlobalAnalyzerConfiguration * ga = new GlobalAnalyzerConfiguration();
  ga->useEventStream0             = true;
  ga->loadHistograms              = false;
  ga->createHistograms            = true;
  ga->scaleHistograms             = true;
  ga->calculateDerivedHistograms  = false;
  ga->saveHistograms              = true;
  ga->forceHistogramsRewrite      = true;
  ga->subsampleAnalysis           = subsampleAnalysis;
  ga->partialSave                 = partialSave;
  ga->outputPath                  = outputPathName;
  ga->rootOuputFileName           = outputFileNameBase;
  ga->countParticles              = true;
  ga->setEvent                    = true;
  ga->fillCorrelationHistos       = false;
  ga->nBins_n  = 400;
  ga->nBins_n2 = 40;
  ga->min_n    = 0.0;
  ga->max_n    = 400;
  ga->nBins_e  = 200;
  ga->nBins_e2 = 20;
  ga->min_e    = 0.0;
  ga->max_e    = 1000.0;
  ga->nBins_q  = 200;
  ga->nBins_q2 = 20;
  ga->min_q    = -20.0;
  ga->max_q    = 20.0;
  ga->nBins_b  = 200;
  ga->nBins_b2 = 200;
  ga->min_b    = -20.0;
  ga->max_b    = 20.0;
  ga->validate();
  GlobalAnalyzerConfiguration * gaND = new GlobalAnalyzerConfiguration(*ga);
  gaND->setEvent = false;

  ParticleAnalyzerConfiguration * ac = new ParticleAnalyzerConfiguration();
  ac->useEventStream0             = true;
  ac->loadHistograms              = false;
  ac->createHistograms            = true;
  ac->scaleHistograms             = true;
  ac->calculateDerivedHistograms  = true;
  ac->saveHistograms              = true;
  ac->forceHistogramsRewrite      = true;
  ac->partialSave                 = partialSave;
  ac->subsampleAnalysis           = subsampleAnalysis;
  ac->inputPath                   = inputPathName;
  ac->outputPath                  = outputPathName;
  ac->rootOuputFileName           = outputFileNameBase;
  ac->nBins_pt    = 100;
  ac->min_pt      = 0.00;
  ac->max_pt      = 10.0;
  ac->nBins_eta   = 80;
  ac->min_eta     = -2;
  ac->max_eta     = 2;
  ac->nBins_y     = 80;
  ac->min_y       = -2;
  ac->max_y       = 2;
  ac->nBins_phi   = 36;
  ac->min_phi     = 0.0;
  ac->max_phi     = 2.0*3.1415927;

  RadialBoostConfiguration * rbc = new RadialBoostConfiguration();
  rbc->max_r = 20.0;
  rbc->param_a = 0.2;
  rbc->param_b = 1.0;
  rbc->betaMaximum = 0.999;

  ////  ParticlePairAnalyzerConfiguration * ac2 = new ParticlePairAnalyzerConfiguration();
  ////  ac2->useEventStream0             = true;
  ////  ac2->createHistograms            = true;
  ////  ac2->scaleHistograms             = true;
  ////  ac2->calculateDerivedHistograms  = true;
  ////  ac2->calculateCombinedHistograms = true;
  ////  ac2->saveHistograms              = true;
  ////  ac2->forceHistogramsRewrite      = true;
  ////  ac2->partialSave                 = partialSave;
  ////  ac2->subsampleAnalysis           = subsampleAnalysis;
  ////  ac2->inputPath                   = inputPathName;
  ////  ac2->outputPath                  = outputPathName;
  ////  ac2->rootOuputFileName           = outputFileNameBase;
  ////  ac2->nBins_pt    = 18;
  ////  ac2->min_pt      = 0.200;
  ////  ac2->max_pt      = 2.0;
  ////  ac2->nBins_eta   = 40;
  ////  ac2->min_eta     = -2;
  ////  ac2->max_eta     = 2;
  ////  ac2->nBins_y     = 40;
  ////  ac2->min_y       = -2;
  ////  ac2->max_y       = 2;
  ////  ac2->nBins_phi   = 36;
  ////  ac2->min_phi     = 0.0;
  ////  ac2->max_phi     = 2.0*3.1415927;
  ////
  ////  vector<unsigned int> comb;
  ////  comb.push_back(0);comb.push_back(3); ac2->combinations.push_back( comb ); comb.clear();
  ////  comb.push_back(0);comb.push_back(1); comb.push_back(3);comb.push_back(4); ac2->combinations.push_back( comb );comb.clear();
  ////  comb.push_back(0);comb.push_back(2); comb.push_back(3);comb.push_back(5); ac2->combinations.push_back( comb );comb.clear();
  ////  comb.push_back(1);comb.push_back(0); comb.push_back(4);comb.push_back(3); ac2->combinations.push_back( comb );comb.clear();
  ////  comb.push_back(1);comb.push_back(4); ac2->combinations.push_back( comb );  comb.clear();
  ////  comb.push_back(1);comb.push_back(2); comb.push_back(4);comb.push_back(5); ac2->combinations.push_back( comb );comb.clear();
  ////  comb.push_back(2);comb.push_back(0); comb.push_back(5);comb.push_back(3); ac2->combinations.push_back( comb );comb.clear();
  ////  comb.push_back(2);comb.push_back(1); comb.push_back(5);comb.push_back(4); ac2->combinations.push_back( comb );comb.clear();
  ////  comb.push_back(2);comb.push_back(5); ac2->combinations.push_back( comb );

  cout << "Now, let's setup the pdg list" << endl;

  vector<int> allPdgCodes;     allPdgCodes = particles->getListOfPdgCodes();
  vector<int> stablePdgCodes;  stablePdgCodes = stableParticles->getListOfPdgCodes();

  vector<EventFilter*>    eventFiltersGlob;
  vector<EventFilter*>    eventFiltersP1;
  vector<EventFilter*>    eventFiltersP2;
  vector<ParticleFilter*> particleFiltersGlob;
  vector<ParticleFilter*> particleFiltersP1;
  vector<ParticleFilter*> particleFiltersP2;

  EventFilter * openEventFilter = new EventFilter(EventFilter::MinBias,0.0,0.0);

  cout << "Now, let's setup the event filters" << endl;

  eventFiltersGlob.push_back   ( openEventFilter);
  int eventFilterOption = 0;
  switch (eventFilterOption)
    {
      case 0:
      eventFiltersP1.push_back     ( openEventFilter); break;
      eventFiltersP2.push_back     ( openEventFilter); break;
      case 1:
      vector<double> limits;
      // limits based on all charged particles in V0M
      limits.push_back(0.0);
      limits.push_back(7.0);   // 80-90%
      limits.push_back(12.0);  // 60-70%
      limits.push_back(20.0);  // 40-50%
      limits.push_back(38.0);  // 20-30%
      limits.push_back(400.0); // 0-10%
      vector<EventFilter*>  eventFilterPartitions = EventFilter::createEventFilterSet(EventFilter::ParticleFilter0, limits);
      for (unsigned int k=0; k<eventFilterPartitions.size(); k++)
        {
        eventFiltersP1.push_back( eventFilterPartitions[k] );
        eventFiltersP2.push_back( eventFilterPartitions[k] );
        }
      break;
    }

  cout << "Now, let's setup the particle filters" << endl;


  ParticleFilter * openPartFilter  = new ParticleFilter(0.0000, 100.0, -6.0, 6.0, 10.0, -10.0,ParticleFilter::Hadron,ParticleFilter::Charged,ParticleFilter::Live);
  vector<ParticleFilter*>  particleFiltersAllHG    = ParticleFilter::createMultiplePdgFilters(0.0000, 100.0, -6.0, 6.0, 10.0, -10.0,allPdgCodes);
  vector<ParticleFilter*>  particleFiltersStableHG = ParticleFilter::createMultiplePdgFilters(0.0000, 100.0, -6.0, 6.0, 10.0, -10.0,stablePdgCodes);

  cout << "Now, let's assign the filters" << endl;

  particleFiltersGlob.push_back( openPartFilter );
  int particleFilterOptionP1 = 1;
  switch (particleFilterOptionP1)
    {
      case 0: particleFiltersP1.push_back( openPartFilter ); break;
      case 1: for (unsigned int k=0; k<particleFiltersAllHG.size();k++)    particleFiltersP1.push_back(particleFiltersAllHG[k] );    break;
      case 2: for (unsigned int k=0; k<particleFiltersStableHG.size();k++) particleFiltersP1.push_back(particleFiltersStableHG[k] ); break;
      case 3: particleFiltersP1 = ParticleFilter::createChargedHadronFilters(0.000, 10.0, -2.0, 2.0, 10.0, -10.0);  break;
      case 4: particleFiltersP1 = ParticleFilter::createPlusMinusHadronFilters(0.000, 10.0, -2.0, 2.0, 10.0, -10.0);  break;
    }

  cout << "Now, instantiating tasks..." << endl;

  HadronGasGeneratorTask * hadronGasGeneratorTask = new HadronGasGeneratorTask("HadronGasGenerator",hggc,messageLevel);
  ParticleDecayerTask    * decayer = new ParticleDecayerTask();
  RadialBoostTask  * radialBoostTask     = new RadialBoostTask (rbc, messageLevel);
  GlobalAnalyzer   * globalAnalyzerND    = new GlobalAnalyzer  ("GlobalND",gaND,eventFiltersGlob, particleFiltersGlob, messageLevel);  // generated
  GlobalAnalyzer   * globalAnalyzerBs    = new GlobalAnalyzer  ("GlobalBs",ga,  eventFiltersGlob, particleFiltersGlob, messageLevel);  // boosted
  GlobalAnalyzer   * globalAnalyzer      = new GlobalAnalyzer  ("Global",  ga,  eventFiltersGlob, particleFiltersGlob, messageLevel);  // with decays
  ParticleAnalyzer * particleAnalyzerND  = new ParticleAnalyzer("P1ND",    ac,  eventFiltersP1,   particleFiltersP1,   messageLevel);  // analysis w/o decays
  ParticleAnalyzer * particleAnalyzerBs  = new ParticleAnalyzer("P1BS",    ac,  eventFiltersP1,   particleFiltersP1,   messageLevel);
  ParticleAnalyzer * particleAnalyzer    = new ParticleAnalyzer("P1",      ac,  eventFiltersP1,   particleFiltersP1,   messageLevel);
  //  ParticlePairAnalyzer * particlePairAnalyzerND  = new ParticlePairAnalyzer("P2", ac2, eventFiltersP2, particleFiltersP2, messageLevel);
  //  ParticlePairAnalyzer * particlePairAnalyzer    = new ParticlePairAnalyzer("P2", ac2, eventFiltersP2, particleFiltersP2, messageLevel);

  TaskIterator * masterTask = new TaskIterator();
  masterTask->setNEventRequested(nEventRequested);
  masterTask->setNEventReported(nEventReported);
  masterTask->setReportLevel(messageLevel);
  masterTask->setNEventPartialSave(nEventPartialSave);
  masterTask->setPartialSave(partialSave);
  masterTask->setSubsampleAnalysis(subsampleAnalysis);
  masterTask->addSubtask( hadronGasGeneratorTask );
  masterTask->addSubtask( globalAnalyzerND       );
  masterTask->addSubtask( particleAnalyzerND     );
  masterTask->addSubtask( radialBoostTask        );
  masterTask->addSubtask( globalAnalyzerBs       );
  masterTask->addSubtask( particleAnalyzerBs     );
  masterTask->addSubtask( decayer                );
  masterTask->addSubtask( globalAnalyzer         );
  masterTask->addSubtask( particleAnalyzer       );
  // masterTask->addSubtask( particlePairAnalyzer );
  masterTask->run();

  ////
  ////  SubSampleStatCalculator * subSampleStatCalculator;
  ////  if (subsampleAnalysis && ga->subsampleAnalysis)
  ////    {
  ////    TString inputFileName   = outputFileNameBase+"Global";
  ////    TString outputFileName  = outputFileNameBase+"Global_Sum";
  ////    subSampleStatCalculator = new SubSampleStatCalculator(outputPathName,inputFileName,globalAnalyzer->getSubSampleIndex(),outputPathName,outputFileName,MessageLogger::Info);
  ////    subSampleStatCalculator->execute();
  ////    }
  ////
  ////  if (subsampleAnalysis && ac->subsampleAnalysis)
  ////    {
  ////    TString inputFileName   = outputFileNameBase+"P1";
  ////    TString outputFileName  = outputFileNameBase+"P1_Sum";
  ////    subSampleStatCalculator = new SubSampleStatCalculator(outputPathName,inputFileName,particleAnalyzer->getSubSampleIndex(),outputPathName,outputFileName,MessageLogger::Info);
  ////    subSampleStatCalculator->execute();
  ////    }
  ////
  ////  if (subsampleAnalysis && ac2->subsampleAnalysis)
  ////    {
  ////    TString inputFileName   = outputFileNameBase+"P2";
  ////    TString outputFileName  = outputFileNameBase+"P2_Sum";
  ////    subSampleStatCalculator = new SubSampleStatCalculator(outputPathName,inputFileName,particlePairAnalyzer->getSubSampleIndex(),outputPathName,outputFileName,MessageLogger::Info);
  ////    subSampleStatCalculator->execute();
  ////    }

  return 0;
}


//
