// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
/**
 \class HadronGasGeneratorTask
 \ingroup WAC

 Class defining HadronGasGeneratorTask
 */
#include "TParameter.h"
#include "HadronGasGeneratorTask.hpp"

ClassImp(HadronGasGeneratorTask);

HadronGasGeneratorTask::HadronGasGeneratorTask(const TString & _name,
                                               HadronGasGeneratorConfiguration * _configuration,
                                               LogLevel _selectedLevel)
:
NucleonNucleonCollisionGenerator(),
particleTypes(nullptr),
stableParticleTypes(nullptr),
hadronGases(),
momentumGenerators(),
standaloneMode(_configuration->standaloneMode)
{
  setName(_name);
  _configuration->useParticles      = true;
  _configuration->useEventStream0   = true;
  setConfiguration(_configuration);
  setReportLevel(_selectedLevel);
  //  eventFilters    = _eventFilters;
  //  particleFilters = _particleFilters;
}

void HadronGasGeneratorTask::initialize()
{
  Task::initialize();
  HadronGasGeneratorConfiguration & hadronGasConfig = *(HadronGasGeneratorConfiguration*) getConfiguration();

  particleTypes       = ParticleTypeCollection::getMasterParticleCollection();
  stableParticleTypes = particleTypes->extractCollection(1);
  stableParticleTypes->printProperties(std::cout);
  double dT = (hadronGasConfig.maxT - hadronGasConfig.minT)/hadronGasConfig.nT;
  double muB = 0.0;
  double muS = 0.0;
  for (int iT=0;iT<hadronGasConfig.nT;iT++)
    {
    double temperature = hadronGasConfig.minT  + double(iT)*dT;
    TString label = "T";
    label += int(1000*temperature);
    HadronGas * gas = new HadronGas(particleTypes,stableParticleTypes,getLogLevel());
    gas->setName(label);
    gas->calculateAllProperties(temperature,muB,muS);
    gas->printProperties(std::cout);
    hadronGases.push_back(gas);
    }

  unsigned int nSpecies = particleTypes->size();
  for (unsigned int k=0; k<nSpecies; k++)
    {
    // const TString & _partName, double _mass, double _tMin, double _tMax, double _tWidth, int _stat

    ParticleType * type = particleTypes->getParticleType(k);
    TString name = type->getName();
    double mass = type->getMass();
    double stat = type->getStatistics();
    MomentumGenerator * gen = new MomentumGenerator(name,mass, 0.170, 0.172, 0.001, stat);
    momentumGenerators.push_back(gen);
    }

  TString histoName = getName();
  histoName += "_relativeAbundances";
  relativeAbundances = new TH1D(histoName,histoName,400,0.0,400.0);
  histoName = getName();
  histoName += "_relativeAbundancesGas";
  relativeAbundancesGas = new TH1D(histoName,histoName,400,0.0,400.0);

  vector<double>  & d = hadronGases[0]->particleDensities;
  for (unsigned int k=1; k<=d.size();k++)
  {
  relativeAbundancesGas->SetBinContent(k, d[k]);
  relativeAbundancesGas->SetBinError(k, 0.0);
  }

  if (reportEnd("HadronGasGeneratorTask",getName(),"initialize()"))
    ;
}

void HadronGasGeneratorTask::execute()
{
  if (reportNoOps("HadronGasGeneratorTask",getName(),"execute()"))
    ;
  incrementEventProcessed();
  Event & event = *eventStreams[0];
  Particle * interaction;
  resetParticleCounters();

  if (standaloneMode)
    {
    // In this mode, we generate one HG  event per call. One interaction vertex is
    // inserted in the event stream and generate is called to carry out the particle generation.
    particleFactory->reset();
    event.reset();
    resetParticleCounters();
    interaction = particleFactory->getNextObject();
    interaction->reset();
    interaction->setType( ParticleType::getInteractionType());
    interaction->setXYZT(0.0, 0.0, 0.0, 0.0);
    //interaction->setRThetaPhiT(3.0, TMath::PiOver2(), TMath::TwoPi()*taskRandomGenerator->Rndm(),0.0);
    event.add(interaction);
    event.setNucleusA(1.0,1.0);
    event.setNucleusB(1.0,1.0);
    generate(interaction);
    EventProperties & ep = * eventStreams[0]->getEventProperties();
    ep.zProjectile       = 1;     // atomic number projectile
    ep.aProjectile       = 1;     // mass number projectile
    ep.nPartProjectile   = 1;     // number of participants  projectile
    ep.zTarget           = 1;     // atomic number target
    ep.aTarget           = 1;     // mass number target
    ep.nPartTarget       = 1;     // number of participants  target
    ep.nPartTotal        = 2;     // total number of participants
    ep.nBinaryTotal      = 1;     // total number of binary collisions
    ep.impactParameter   = -99999; // nucleus-nucleus center distance in fm
    ep.centrality        = -99999; // fraction cross section value
    ep.multiplicity      = eventStreams[0]->getNParticles();// nominal multiplicity in the reference range
    ep.particlesCounted  = getNParticlesCounted();
    ep.particlesAccepted = getNParticlesAccepted();
    }
  else
    {
    // In this mode, we generate several HG cells per event. Interaction vertices and their locations
    // are assumed to be already loaded in the event stream and we produce as many HG cells as there
    // are nucleon-nucleon interactions in the stream. Objects nucleusA and nucleusB are assumed defined by
    // an earlier task and the structure EventProperties is assumed filled by that earlier task. We only fill the number of
    // particle generated and accepted (in addition to storing the generated particles in the event)

    // May skip this event if it does not satisfy the event cut.
    if (!eventFilters[0]->accept(event)) return;
    if (event.getParticleCount() < 1)
      {
      return;
      }
    vector<Particle*> interactions = event.getNucleonNucleonInteractions();

    unsigned int n = interactions.size();
//    if (reportWarning("PythiaEventGenerator",getName(),"execute()"))
//      cout << "Size of interactions:" <<  n << endl;
    for (unsigned int kInter=0; kInter<n; kInter++)
      {
      generate(interactions[kInter]);
      }
    EventProperties & ep = * event.getEventProperties();
    ep.multiplicity      = getNParticlesAccepted(); // nominal multiplicity in the reference range
    ep.particlesCounted  = getNParticlesCounted();
    ep.particlesAccepted = getNParticlesAccepted();
    }




}

void HadronGasGeneratorTask::generate(Particle * parent)
{
  if (reportStart("HadronGasGeneratorTask",getName(),"generate(Particle * parent)"))
    ;
  HadronGasGeneratorConfiguration & hggc = *(HadronGasGeneratorConfiguration*) getConfiguration();
  Event & event = * eventStreams[0];
  //ParticleFilter & particleFilter = * particleFilters[0];
  Particle * particle;
  TLorentzVector parentPosition = parent->getPosition();
  int    multiplicity = hggc.totalMult;
  double temperature  = hggc.minTkin;
//  if (reportDebug("HadronGasGeneratorTask",getName(),"execute()"))
//    {
//    cout << "         hadronGases.size(): " << hadronGases.size() << endl;
//    cout << "  momentumGenerators.size(): " << momentumGenerators.size() << endl;
//
//    }
  for (int iPart=0; iPart< multiplicity; iPart++)
    {
    // implement basic no correlation scenario.
    //ParticleType * type = hadronGases[0]->generateRandomHadron();
    int     index  = hadronGases[0]->generateRandomHadronByIndex();
    double  rindex = index + 0.01;
    relativeAbundances->Fill(rindex);
    if (index<0)
      {
      if (reportFatal("HadronGasGeneratorTask",getName(),"execute()"))
        {
        cout << "hadron index=" << index << endl;
        }
      exit(1);
      }
    ParticleType * type = particleTypes->getParticleType(index);
    if (type==nullptr)
      {
      if (reportError("HadronGasGeneratorTask",getName(),"execute()"))
        {
        cout << "HadronGas::generateRandomHadron() return null pointer. Post task error." << endl;
        }
      postTaskError();
      exit(1);
      return;
      }
    TLorentzVector momentum = momentumGenerators[index]->generate(temperature);
    particle = particleFactory->getNextObject();
    particle->setParent(parent);
    particle->set(type, momentum, parentPosition,true);
    //particle->printProperties(cout);
    incrementParticlesCounted(); // photons are NOT included in this tally
    //if (!particleFilter.accept(*particle)) continue; // no filter for now...
    event.add(particle);
    incrementParticlesAccepted();
    }
  if (reportDebug("HadronGasGeneratorTask",getName(),"execute()"))
    {
    cout << endl;
    cout << "HadronGasGeneratorTask::execute() No of accepted particles : "<< getNParticlesAccepted() << endl;
    cout << "HadronGasGeneratorTask::execute() No of counted particles : " << getNParticlesCounted()  << endl;
    }
}
