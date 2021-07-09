// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
/**
 \class Task
 \ingroup WAC

 Class defining Task
 */
#include "CollisionGeometryGenerator.hpp"
ClassImp(CollisionGeometryGenerator);

//!
//! Constructor.
//! @param _name Name given to this task instance
//! @param _configuration Configuration to be used by this task instance
//! @param _eventFilters  Vector of event filters to be used by this task instance
//! @param _requiredLevel Debug/report level to be used by this task instance
CollisionGeometryGenerator::CollisionGeometryGenerator(const TString &                  _name,
                                                       CollisionGeometryConfiguration * _configuration,
                                                       vector<EventFilter*>             _eventFilters,
                                                       LogLevel                         _requiredLevel)
:
Task(),
nucleusGeneratorA(new NucleusGenerator()),
nucleusGeneratorB(new NucleusGenerator()),
minB(0), minBSq(0.0), maxB(10.0), maxBSq(100.0),
nnCrossSection(0.0),
maxNNDistanceSq(0.)
{
  setName(_name);
  _configuration->useParticles      = true;
  setConfiguration(_configuration);
  setReportLevel  (_requiredLevel);
  setEventFilters (_eventFilters);
}

CollisionGeometryGenerator::~CollisionGeometryGenerator()
{
  clear();
  delete nucleusGeneratorA;
  delete nucleusGeneratorB;
}

void CollisionGeometryGenerator::initialize()
{
  if (reportStart("CollisionGeometryGenerator",getName(),"initialize()"))
    ;
  Task::initialize();
  CollisionGeometryConfiguration & config = * (CollisionGeometryConfiguration*)getConfiguration();
  Event & event = * eventStreams[0];
  event.setNucleusA(config.aNucleusZ,config.aNucleusA);
  event.setNucleusB(config.bNucleusZ,config.bNucleusA);
  nucleusGeneratorA->initialize("NGA",
                                config.aGeneratorType,
                                config.aParA, config.aParB, config.aParC,
                                config.aNR,   config.aMinR, config.aMaxR,
                                config.useRecentering,
                                config.useNucleonExclusion,
                                config.exclusionRadius);
  nucleusGeneratorB->initialize("NGB",
                                config.bGeneratorType,
                                config.bParA, config.bParB, config.bParC,
                                config.bNR,   config.bMinR, config.bMaxR,
                                config.useRecentering,
                                config.useNucleonExclusion,
                                config.exclusionRadius);
  minB   = config.minB; minBSq = minB*minB;
  maxB   = config.maxB; maxBSq = maxB*maxB;
  nnCrossSection  = config.nnCrossSection;
  maxNNDistanceSq = nnCrossSection/3.1415927;
  if (reportInfo("CollisionGeometryGenerator",getName(),"initialize()"))
    {
    cout << endl;
    cout << "================================================================" << endl;
    cout << "================================================================" << endl;
    cout << "      nnCrossSection:"  << nnCrossSection << endl;
    cout << "     maxNNDistanceSq:" << maxNNDistanceSq << endl;
    cout << "        max distance:" << sqrt(maxNNDistanceSq) << endl;
    cout << "================================================================" << endl;
    cout << "================================================================" << endl;
    }
  if (reportEnd("CollisionGeometryGenerator",getName(),"initialize()"))
    ;
}

void CollisionGeometryGenerator::clear()
{
  eventStreams[0]->clear();
}

void CollisionGeometryGenerator::reset()
{
  eventStreams[0]->reset();
}

void CollisionGeometryGenerator::execute()
{
  if (reportStart("CollisionGeometryGenerator",getName(),"execute()"))
    ;
  incrementEventProcessed();
  Event & event = *eventStreams[0];
  event.reset();
  Nucleus & nucleusA = event.getNucleusA();
  Nucleus & nucleusB = event.getNucleusB();
  double rr = gRandom->Rndm();
  double b  = sqrt(minBSq + rr*(maxBSq-minBSq));
  event.setImpactParameter(b);
  nucleusGeneratorA->generate(nucleusA, -b/2.0);
  nucleusGeneratorB->generate(nucleusB,  b/2.0);
  Particle* interaction;
  for (unsigned int i1=0; i1<nucleusA.getNNucleons(); i1++)
    {
    Particle * nucleonA = nucleusA.getNucleonAt(i1);
    for (unsigned int i2=0; i2<nucleusB.getNNucleons(); i2++)
      {
      Particle * nucleonB = nucleusB.getNucleonAt(i2);
      double dSq = nucleonA->distanceXYSq(nucleonB);
      if (dSq<maxNNDistanceSq)
        {
        //cout << " A:  x= " << nucleonA->getPosition().X() << " B:  x= " << nucleonB->getPosition().X() << " I:  x= " << interaction.getPosition().X() << endl;
        interaction = event.addInteraction(nucleonA,nucleonB);
        if (!nucleonA->isWounded())
          {
          TLorentzVector & positionA = nucleonA->getPosition();
          event.getParticipantMoments().fill(positionA.X() ,positionA.Y());
          nucleonA->setWounded(true);
          }
        if (!nucleonB->isWounded())
          {
          TLorentzVector & positionB = nucleonA->getPosition();
          event.getParticipantMoments().fill(positionB.X() ,positionB.Y());
          nucleonB->setWounded(true);
          }
        TLorentzVector & positionInt = interaction->getPosition();
        event.getBinaryMoments().fill(positionInt.X(),positionInt.Y());
        }
      }
    }
  event.getBinaryMoments().calculate();
  event.getBinaryMoments().calculate();
//  if (reportInfo("CollisionGeometryGenerator",getName(),"execute()"))
//   {
//   cout << "Nuclei after generation" << endl;
//   cout << "             b:" << b << endl;
//   cout << "A:  n  protons:" << nucleusA.getNProtons() << endl;
//   cout << "A:  n neutrons:" << nucleusA.getNNeutrons() << endl;
//   cout << "A:  n nucleons:" << nucleusA.getNNucleons() << endl;
//   cout << "A:  n  wounded:" << nucleusA.countWounded() << endl;
//   cout << "B:  n  protons:" << nucleusB.getNProtons() << endl;
//   cout << "B:  n neutrons:" << nucleusB.getNNeutrons() << endl;
//   cout << "B:  n nucleons:" << nucleusB.getNNucleons() << endl;
//   cout << "B:  n  wounded:" << nucleusB.countWounded() << endl;
//   }
  double nWoundedA = nucleusA.countWounded();
  double nWoundedB = nucleusB.countWounded();
  EventProperties & ep = *event.getEventProperties();
  ep.zProjectile       = nucleusA.getNProtons();     // atomic number projectile
  ep.aProjectile       = nucleusA.getNNucleons();    // mass number projectile
  ep.nPartProjectile   = nWoundedA;                  // number of participants  projectile
  ep.zTarget           = nucleusB.getNProtons();     // atomic number target
  ep.aTarget           = nucleusB.getNNucleons();    // mass number target
  ep.nPartTarget       = nWoundedB;                  // number of participants  target
  ep.nPartTotal        = nWoundedA+nWoundedB;        // total number of participants
  ep.nBinaryTotal      = event.getNBinaryCollisions();  // total number of binary collisions
  ep.impactParameter   = b;    // nucleus-nucleus center distance in fm
  ep.centrality        = -99999; // fraction cross section value
  ep.multiplicity      = ep.nBinaryTotal; // number of binary collisions
  ep.particlesCounted  = ep.nBinaryTotal;
  ep.particlesAccepted = ep.nBinaryTotal;
  if (reportDebug("CollisionGeometryGenerator",getName(),"execute()"))
    {
    event.printProperties(cout);
    }
  if (reportEnd("CollisionGeometryGenerator",getName(),"execute()"))
    ;
}

