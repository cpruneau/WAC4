//==============================================================================
//  calculate the thermodynamic quantities of hadron resonance gas
//==============================================================================
#include <iostream>
#include <fstream>
#include <TStyle.h>
#include <TROOT.h>
using namespace std;


int RunCollisionGeometrySimulation()
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
  gSystem->Load(includesPath+"CollisionGeometryGenerator.hpp");
  gSystem->Load(includesPath+"CollisionGeometryConfiguration.hpp");
  gSystem->Load(includesPath+"CollisionGeometryAnalyzer.hpp");
  gSystem->Load(includesPath+"CollisionGeometryHistograms.hpp");
  gSystem->Load("libBase.dylib");

  //  includesPath = "/Users/claudeapruneau/Documents/GitHub/WAC/WacPythia/";
  //  gSystem->Load(includesPath+"PythiaConfiguration.hpp");
  //  gSystem->Load(includesPath+"PythiaEventGenerator.hpp");
  //  gSystem->Load(includesPath+"PythiaEventReader.hpp");
  //  gSystem->Load("libWacPythia.dylib");

//  includesPath = "/Users/claudeapruneau/Documents/GitHub/WAC/HadronGas/";
//  gSystem->Load(includesPath+"HadronGas.hpp");
//  gSystem->Load(includesPath+"HadronGasGeneratorConfiguration.hpp");
//  gSystem->Load(includesPath+"HadronGasHistograms.hpp");
//  gSystem->Load(includesPath+"HadronGasVsTempHistograms.hpp");
//  gSystem->Load("libHadronGas.dylib");

  std::cout << "==================================================================================" << std::endl;
  std::cout << "==================================================================================" << std::endl;
  TString pdgDataTable = getenv("WAC_SRC");
  pdgDataTable += "/EOS/pdg.dat";
  ParticleTypeCollection * particles = ParticleTypeCollection::getMasterParticleCollection(); //ParticleTypeCollection::getMasterParticleCollection();
  particles->readFromFile(pdgDataTable);
  particles->printProperties(std::cout);

  MessageLogger::LogLevel messageLevel = MessageLogger::Info; //MessageLogger::Debug; //
  unsigned long nEventRequested   = 10000000;
  unsigned long nEventReported    = 1000;
  unsigned long nEventPartialSave = 500;
  bool    partialSave             = false;
  bool    subsampleAnalysis       = false;
  double  beamEnergy              = 7000.0; // GeV
  double  minBias                 = true; // alternative is AliceV0
  TString outputFileNameBase      = "Geom_";
  TString inputPathName           = getenv("WAC_INPUT_PATH");
  TString outputPathName          = getenv("WAC_OUTPUT_PATH");
  inputPathName  += "/CG/";
  outputPathName += "/CG/10Million/";
  gSystem->mkdir(outputPathName,1);

  vector<EventFilter*> eventFiltersGen;
  vector<EventFilter*> eventFiltersAna;
  int centralityOption = 1;
  EventFilter * openEventFilter = new EventFilter(EventFilter::MinBias,0.0,0.0);
  eventFiltersGen.push_back( openEventFilter);
  switch (centralityOption)
    {
      default:
      case 0:
      {
      eventFiltersAna.push_back( openEventFilter);
      }
      break;

      case 1:
      vector<double> limits = { 0.0, 0.5, 1.0, 2.0, 3.0, 4.0, 6.0, 8.0, 10.0, 12.0, 16.0, 20.0};
      eventFiltersAna = EventFilter::createEventFilterSet(EventFilter::ImpactParameter,limits);
      break;
    }

  CollisionGeometryConfiguration * cggConfiguration = new CollisionGeometryConfiguration();
  cggConfiguration->forceHistogramsRewrite = true;
  cggConfiguration->nnCrossSection = 6.4;

  cggConfiguration->outputPath                  = outputPathName;
  cggConfiguration->rootOuputFileName           = outputFileNameBase;


  CollisionGeometryGenerator * cgg  = new CollisionGeometryGenerator("CGG", cggConfiguration,eventFiltersGen,MessageLogger::Info);
  CollisionGeometryAnalyzer  * cgga = new CollisionGeometryAnalyzer ("CGGA",cggConfiguration,eventFiltersAna,MessageLogger::Info);

  TaskIterator * masterTask = new TaskIterator();
  masterTask->setNEventRequested(nEventRequested);
  masterTask->setNEventReported(nEventReported);
  masterTask->setReportLevel(MessageLogger::Info);
  masterTask->setNEventPartialSave(nEventPartialSave);
  masterTask->setPartialSave(partialSave);
  masterTask->setSubsampleAnalysis(subsampleAnalysis);
  masterTask->addSubtask( cgg );
  masterTask->addSubtask( cgga );
  masterTask->run();

  return 0;
}


//
