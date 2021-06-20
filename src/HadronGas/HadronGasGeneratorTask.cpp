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
momentumGenerators()
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

  ParticleTypeCollection *   particleTypes       = ParticleTypeCollection::getMasterParticleCollection();
  ParticleTypeCollection *   stableParticleTypes = particleTypes->extractCollection(1);
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
  momentumGenerators.push_back( new MomentumGenerator() );

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
  incrementEventAccepted();
  particleFactory->reset();
  eventStreams[0]->reset();
  resetParticleCounters();
  Particle * interaction = particleFactory->getNextObject();
  interaction->reset();
  interaction->setType( ParticleType::getInteractionType());
  //interaction->setXYZT(0.0, 0.0, 0.0, 0.0);
  interaction->setRThetaPhiT(3.0, TMath::PiOver2(), TMath::TwoPi()*taskRandomGenerator->Rndm(),0.0);


  eventStreams[0]->add(interaction);
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
  ep.multiplicity      = eventStreams[0]->getNParticles(); // nominal multiplicity in the reference range
  ep.particlesCounted  = getNParticlesCounted();
  ep.particlesAccepted = getNParticlesAccepted();
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
  vector<double> momentumGeneratorParameters;
  momentumGeneratorParameters = hggc.momentumGeneratorParameters;
  int multiplicity = hggc.totalMult;

  for (int iPart=0; iPart< multiplicity; iPart++)
    {
    // implement basic no correlation scenario.
    ParticleType * type = hadronGases[0]->generateRandomHadron();
    double  index = hadronGases[0]->generateRandomHadronByIndex();
    double  rindex = index + 0.01;
    relativeAbundances->Fill(rindex);

    if (type==nullptr)
      {
      if (reportError("HadronGasGeneratorTask",getName(),"execute()"))
        {
        cout << "HadronGas::generateRandomHadron() return null pointer. Post task error." << endl;
        }
      postTaskError();
      return;
      }
    momentumGeneratorParameters[0] = type->getMass();
    TLorentzVector momentum = momentumGenerators[0]->generate(hggc.generatorType,momentumGeneratorParameters,nullptr);
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
