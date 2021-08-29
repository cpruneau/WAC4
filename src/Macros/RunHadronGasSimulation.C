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
  gSystem->Load(includesPath+"ParticleAnalyzerV2.hpp");
  gSystem->Load(includesPath+"ParticlePairAnalyzerV2.hpp");
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

  MessageLogger::LogLevel messageLevel =  MessageLogger::Info; // MessageLogger::Debug; //
  unsigned long nEventRequested   = 200000;
  unsigned long nEventReported    = 10000;
  unsigned long nEventPartialSave = 10000;
  bool    partialSave             = true;
  bool    subsampleAnalysis       = true;
  double  beamEnergy              = 7000.0; // GeV
  double  minBias                 = true; // alternative is AliceV0
  TString outputFileNameBase      = "HG_Isotropic_200MeV_";
  TString inputPathName           = getenv("WAC_INPUT_PATH");
  TString outputPathName          = getenv("WAC_OUTPUT_PATH");
  inputPathName  += "/HG/";
  outputPathName += "/HG/CorrectStatIsotropic/SpeedTestG1P1/";
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

  CollisionGeometryConfiguration * cggConfig = new CollisionGeometryConfiguration();
  cggConfig->forceHistogramsRewrite = true;
  cggConfig->nnCrossSection         = 6.4; // cross section must be adjusted for beam energy.
  cggConfig->outputPath             = outputPathName;
  cggConfig->rootOuputFileName      = outputFileNameBase;


  HadronGasGeneratorConfiguration * hggc = new HadronGasGeneratorConfiguration();
  hggc->volume    = 1.0; // not used here
  hggc->totalMult = 20; // fixed
  hggc->nT    = 1; // chemical  temperature values
  hggc->minT  = 0.200;
  hggc->maxT  = 0.200;
  hggc->nTkin    = 1;     // kinetic  temperature value
  hggc->minTkin  = 0.170;
  hggc->maxTkin  = 0.170;

  hggc->standaloneMode      = true;
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

  ParticleAnalyzerConfiguration * p1C = new ParticleAnalyzerConfiguration();
  p1C->useEventStream0             = true;
  p1C->loadHistograms              = false;
  p1C->createHistograms            = true;
  p1C->scaleHistograms             = true;
  p1C->calculateDerivedHistograms  = true;
  p1C->saveHistograms              = true;
  p1C->forceHistogramsRewrite      = true;
  p1C->partialSave                 = partialSave;
  p1C->subsampleAnalysis           = subsampleAnalysis;
  p1C->inputPath                   = inputPathName;
  p1C->outputPath                  = outputPathName;
  p1C->rootOuputFileName           = outputFileNameBase;
  p1C->nBins_n1    = 200;
  p1C->min_n1      = 0.0;
  p1C->max_n1      = 200.0;
  p1C->nBins_eTot  = 200;
  p1C->min_eTot    = 0.0;
  p1C->max_eTot    = 400.0;
  p1C->nBins_pt    = 400;
  p1C->min_pt      = 0.00;
  p1C->max_pt      = 4.0;
  p1C->nBins_eta   = 80;
  p1C->min_eta     = -2;
  p1C->max_eta     = 2;
  p1C->nBins_y     = 80;
  p1C->min_y       = -2;
  p1C->max_y       = 2;
  p1C->nBins_phi   = 36;
  p1C->min_phi     = 0.0;
  p1C->max_phi     = 2.0*3.1415927;
//  nBins_n1(100),          min_n1(0.0),    max_n1(10000.0), range_n1(10000.0),
//  nBins_eTot(100),        min_eTot(0.0),  max_eTot(1.0E6), range_eTot(1.0E6),
  RadialBoostConfiguration * rbc = new RadialBoostConfiguration();
  rbc->useEventStream0   = true;
  rbc->loadHistograms    = true;
  rbc->createHistograms  = true;
  rbc->scaleHistograms   = true;
  rbc->saveHistograms         = true;
  rbc->forceHistogramsRewrite = true;
  rbc->param_a     = 0.4;
  rbc->param_b     = 1.0;
  rbc->betaMaximum = 0.999;
  rbc->baseName    = "CGGA";// CG/PbPb/PbPb_Geom_Gradients.root";
  rbc->inputPath   = "/Users/claudeapruneau/Documents/GitHub/WAC4/data//OutputFiles/CG/PbPb/"; //  PbPb_Geom_Gradients.root;
  rbc->rootInputFileName = "Geom_CGGA_Gradients.root";
  rbc->outputPath        = outputPathName;
  rbc->rootOuputFileName = outputFileNameBase;

    ParticlePairAnalyzerConfiguration * p2C = new ParticlePairAnalyzerConfiguration();
    p2C->useEventStream0             = true;
    p2C->createHistograms            = true;
    p2C->scaleHistograms             = true;
    p2C->calculateDerivedHistograms  = true;
    p2C->calculateCombinedHistograms = false;
    p2C->saveHistograms              = false;
    p2C->forceHistogramsRewrite      = true;
    p2C->partialSave                 = partialSave;
    p2C->subsampleAnalysis           = subsampleAnalysis;
    p2C->inputPath                   = inputPathName;
    p2C->outputPath                  = outputPathName;
    p2C->rootOuputFileName           = outputFileNameBase;
  p2C->fillEta     = true;
  p2C->fillP2      = false;
  p2C->nBins_n2    = 100;
  p2C->min_n2      = 0.0;
  p2C->max_n2      = 400.0;
    p2C->nBins_pt    = 20;
    p2C->min_pt      = 0.001;
    p2C->max_pt      = 2.0;
    p2C->nBins_eta   = 40;
    p2C->min_eta     = -2;
    p2C->max_eta     = 2;
    p2C->nBins_y     = 40;
    p2C->min_y       = -2;
    p2C->max_y       = 2;
    p2C->nBins_phi   = 36;
    p2C->min_phi     = 0.0;
    p2C->max_phi     = 2.0*3.1415927;
    p2C->validate();

  ////
  ////  vector<unsigned int> comb;
  ////  comb.push_back(0);comb.push_back(3); p2C->combinations.push_back( comb ); comb.clear();
  ////  comb.push_back(0);comb.push_back(1); comb.push_back(3);comb.push_back(4); p2C->combinations.push_back( comb );comb.clear();
  ////  comb.push_back(0);comb.push_back(2); comb.push_back(3);comb.push_back(5); p2C->combinations.push_back( comb );comb.clear();
  ////  comb.push_back(1);comb.push_back(0); comb.push_back(4);comb.push_back(3); p2C->combinations.push_back( comb );comb.clear();
  ////  comb.push_back(1);comb.push_back(4); p2C->combinations.push_back( comb );  comb.clear();
  ////  comb.push_back(1);comb.push_back(2); comb.push_back(4);comb.push_back(5); p2C->combinations.push_back( comb );comb.clear();
  ////  comb.push_back(2);comb.push_back(0); comb.push_back(5);comb.push_back(3); p2C->combinations.push_back( comb );comb.clear();
  ////  comb.push_back(2);comb.push_back(1); comb.push_back(5);comb.push_back(4); p2C->combinations.push_back( comb );comb.clear();
  ////  comb.push_back(2);comb.push_back(5); p2C->combinations.push_back( comb );

  cout << "Now, let's setup the pdg list" << endl;

  vector<int> allPdgCodes;     allPdgCodes = particles->getListOfPdgCodes();
  vector<int> stablePdgCodes;  stablePdgCodes = stableParticles->getListOfPdgCodes();

  vector<EventFilter*>    eventFiltersGeomGen;
  vector<EventFilter*>    eventFiltersG1;
  vector<EventFilter*>    eventFiltersP1;
  vector<EventFilter*>    eventFiltersP2;
  vector<ParticleFilter*> particleFiltersG1;
  vector<ParticleFilter*> particleFiltersP1ND; // no decays
  vector<ParticleFilter*> particleFiltersP1WD; // with decays
  vector<ParticleFilter*> particleFiltersP2ND;
  vector<ParticleFilter*> particleFiltersP2WD;

  EventFilter * openEventFilter = new EventFilter(EventFilter::MinBias,0.0,0.0);

  cout << "Now, let's setup the event filters" << endl;

///
  int centralityOptionGeom    = 1;
  switch (centralityOptionGeom)
    {
      default:
      case 0: // Minimum bias only -- open wide
      {
      eventFiltersGeomGen.push_back( openEventFilter);
      //eventFiltersGeomAna.push_back( openEventFilter);
      }
      break;

      case 1:  // Bins in impact parameter b
      {
      eventFiltersGeomGen.push_back( openEventFilter);
      vector<double> limits = { 0.0, 1.0, 2.0, 3.0, 4.0, 6.0, 8.0, 10.0, 12.0, 16.0, 20.0};
      //eventFiltersGeomAna = EventFilter::createEventFilterSet(EventFilter::ImpactParameter,limits);
      }
      break;
    }



  ///




  eventFiltersG1.push_back   ( openEventFilter);
  int eventFilterOption = 0;
  switch (eventFilterOption)
    {
      case 0:
      eventFiltersP1.push_back     ( openEventFilter);
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


  //ParticleFilter * openPartFilter  = new ParticleFilter(0.0000, 100.0, -6.0, 6.0, 10.0, -10.0,ParticleFilter::Hadron,ParticleFilter::Charged,ParticleFilter::Live);
  ParticleFilter * openPartFilter  = new ParticleFilter(0.0000, 100.0, -6.0, 6.0, 10.0, -10.0,ParticleFilter::Hadron,ParticleFilter::AllCharges,ParticleFilter::Live);
  vector<ParticleFilter*>  particleFiltersAllHG    = ParticleFilter::createMultiplePdgFilters(0.0000, 100.0, -6.0, 6.0, 10.0, -10.0,allPdgCodes);
  vector<ParticleFilter*>  particleFiltersStableHG = ParticleFilter::createMultiplePdgFilters(0.0000, 100.0, -6.0, 6.0, 10.0, -10.0,stablePdgCodes);

  cout << "Now, let's assign the filters" << endl;

  particleFiltersG1.push_back( openPartFilter );
  int particleFilterOptionP1 = 1;
  switch (particleFilterOptionP1)
    {
      case 0:
      particleFiltersP1ND.push_back( openPartFilter );
      particleFiltersP1WD.push_back( openPartFilter ); break;

      case 1:
      for (unsigned int k=0; k<particleFiltersAllHG.size();k++)    particleFiltersP1ND.push_back(particleFiltersAllHG[k] );
      for (unsigned int k=0; k<particleFiltersStableHG.size();k++) particleFiltersP1WD.push_back(particleFiltersStableHG[k] ); break;

      case 3:
      particleFiltersP1ND.push_back( openPartFilter );
      particleFiltersP1WD = ParticleFilter::createChargedHadronFilters(0.000, 10.0, -2.0, 2.0, 10.0, -10.0);  break;

      case 4:
      particleFiltersP1ND.push_back( openPartFilter );
      particleFiltersP1WD = ParticleFilter::createPlusMinusHadronFilters(0.000, 10.0, -2.0, 2.0, 10.0, -10.0);  break;
    }

  vector<EventFilter*> eventFiltersRadialBoost;
  vector<double> limits = { 0.0, 0.5, 1.0, 2.0, 3.0, 4.0, 6.0, 8.0, 10.0, 12.0, 16.0, 20.0};
  eventFiltersRadialBoost = EventFilter::createEventFilterSet(EventFilter::ImpactParameter,limits);

  cout << "Now, instantiating tasks..." << endl;

  CollisionGeometryGenerator * collisionGeomGenerator = new CollisionGeometryGenerator("CGG", cggConfig,eventFiltersGeomGen,messageLevel);

  HadronGasGeneratorTask * hadronGasGeneratorTask = new HadronGasGeneratorTask("HadronGasGenerator",hggc,messageLevel);
  ParticleDecayerTask    * decayer = new ParticleDecayerTask();

  RadialBoostTask  * r1     = new RadialBoostTask ("R1",  rbc, eventFiltersRadialBoost, messageLevel);
  GlobalAnalyzer   * g1ND    = new GlobalAnalyzer  ("G1ND",gaND,eventFiltersG1, particleFiltersG1, messageLevel);  // generated
  GlobalAnalyzer   * g1WD    = new GlobalAnalyzer  ("G1WD",ga,  eventFiltersG1, particleFiltersG1, messageLevel);  // boosted
  GlobalAnalyzer   * g1Bs    = new GlobalAnalyzer  ("G1Bs",ga,  eventFiltersG1, particleFiltersG1, messageLevel);  // boosted
  ParticleAnalyzerV2 * p1ND  = new ParticleAnalyzerV2("P1ND",p1C,  eventFiltersP1, particleFiltersP1ND,   messageLevel);  // analysis w/o decays
  ParticleAnalyzerV2 * p1WD  = new ParticleAnalyzerV2("P1WD",p1C,  eventFiltersP1, particleFiltersP1WD,   messageLevel);  // analysis w/o decays
  ParticleAnalyzerV2 * p1Bs  = new ParticleAnalyzerV2("P1BS",p1C,  eventFiltersP1, particleFiltersP1ND,   messageLevel);
  ParticlePairAnalyzerV2 * p2WD  = new ParticlePairAnalyzerV2("P2WD", p2C, eventFiltersP2, particleFiltersP1WD, messageLevel);

  TaskIterator * masterTask = new TaskIterator();
  masterTask->setName("IteratorTask");
  masterTask->setNEventRequested(nEventRequested);
  masterTask->setNEventReported(nEventReported);
  masterTask->setReportLevel(messageLevel);
  masterTask->setNEventPartialSave(nEventPartialSave);
  masterTask->setPartialSave(partialSave);
  masterTask->setSubsampleAnalysis(subsampleAnalysis);
  //masterTask->addSubtask( collisionGeomGenerator );
  masterTask->addSubtask( hadronGasGeneratorTask );
  masterTask->addSubtask( g1ND       );
  //masterTask->addSubtask( p1ND     );
  //masterTask->addSubtask( r1        );
  //masterTask->addSubtask( g1Bs       );
  //masterTask->addSubtask( p1Bs     );
  masterTask->addSubtask( decayer  );
  masterTask->addSubtask( g1WD     );
  masterTask->addSubtask( p1WD     );
  //masterTask->addSubtask( p2WD );
  masterTask->run();

  return 0;
}


//
