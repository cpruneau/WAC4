// Author: Claude Pruneau   12/12/16

/*************************************************************************
 * Copyright (C) 2019, Claude Pruneau.                                   *
 * All rights reserved.                                                  *
 * Based on the ROOT package and environment                             *
 *                                                                       *
 * For the licensing terms see LICENSE.                                  *
 *************************************************************************/
/**
 \class CanvasConfiguration
 \ingroup WAC

 Utility class used to define the parameters of a root canvas
 */
#include "Event.hpp"
ClassImp(Event);

Event::Event()
 :
eventIndex(0),
eventNumber(0),
particles(),
eventProperties(new EventProperties() ),
b(-9999.0),
nucleusA(new Nucleus()),
nucleusB(new Nucleus()),
binaryMoments(new CollisionGeometryMoments()),
participantMoments(new CollisionGeometryMoments())
{
 // no ops
}

// ====================================================
// DTOR
// ====================================================
Event::~Event()
{
  particles.clear();
  eventProperties->clear();
  delete eventProperties;
  delete nucleusA;
  delete nucleusB;
  delete binaryMoments;
  delete participantMoments;
}

// ====================================================
// Call before (re)starting simulation
// ====================================================
void Event::clear()
{
  eventIndex      = 0;
  eventNumber     = 0;
  b               = -99999;
  particles.clear();
  if (nucleusA) nucleusA->clear();
  if (nucleusB) nucleusB->clear();
  if (binaryMoments) binaryMoments->reset();
  if (participantMoments) participantMoments->reset();
  if (eventProperties) eventProperties->reset();
  Particle::getFactory()->reset();
}

// ====================================================
// Call before generating new event
// ====================================================
void Event::reset()
{
  eventIndex++;
  eventNumber   = 0;
  b             = -99999;
  particles.clear();
  if (nucleusA) nucleusA->reset();
  if (nucleusB) nucleusB->reset();
  if (binaryMoments) binaryMoments->reset();
  if (participantMoments) participantMoments->reset();
  if (eventProperties) eventProperties->reset();
  Particle::getFactory()->reset();
}


// ====================================================
// Get the particle at the given index
// ====================================================
void Event::add(Particle * particle)
{
  particles.push_back(particle);
}


Particle* Event::addInteraction(Particle* particleA,
                                Particle* particleB)
{

  Particle * interaction = Particle::getFactory()->getNextObject();
  interaction->setParents(particleA,particleB);
  interaction->setType(ParticleType::getInteractionType());
  interaction->setLive(true);
  add(interaction);
  return interaction;
}

unsigned int Event::getNParticipants() const
{
  if (nucleusA && nucleusB)
    return  nucleusA->countWounded() + nucleusB->countWounded();
  else
    return 0;
}

unsigned int Event::getNBinaryCollisions() const
{
  unsigned int  nBinary = 0;
  for (unsigned int iPart=0; iPart<particles.size(); iPart++)
    {
    if (particles[iPart]->isNucleonNucleonInteraction()) nBinary++;
    }
  return nBinary;
}

vector<Particle*> Event::getNucleonNucleonInteractions()
{
  vector<Particle*>  interactions;
  for (unsigned int iPart=0; iPart<particles.size(); iPart++)
    {
    if (particles[iPart]->isNucleonNucleonInteraction()) interactions.push_back(particles[iPart]);
    }
  return interactions;
}



//unsigned int CollisionGeometry::getNProtonProtonCollisions()   const
//{
//  unsigned int n = 0;
//  for (unsigned int k=0; k<nnInteractions.size(); k++)
//    {
//    if (nnInteractions[k].isProtonProton()) n++;
//    }
//  return n;
//}
//
//unsigned int CollisionGeometry::getNProtonNeutronCollisions()  const
//{
//  unsigned int n = 0;
//  for (unsigned int k=0; k<nnInteractions.size(); k++)
//    {
//    if (nnInteractions[k].isProtonNeutron()) n++;
//    }
//  return n;
//}
//
//unsigned int CollisionGeometry::getNNeutronNeutronCollisions() const
//{
//  unsigned int n = 0;
//  for (unsigned int k=0; k<nnInteractions.size(); k++)
//    {
//    if (nnInteractions[k].isNeutronNeutron()) n++;
//    }
//  return n;
//}


void Event::setNucleusA(int z, int a)
{
  if (!nucleusA) nucleusA = new Nucleus();
  nucleusA->defineAs(z,a);
}

void Event::setNucleusB(int z, int a)
{
  if (!nucleusB) nucleusB = new Nucleus();
  nucleusB->defineAs(z,a);
}


 ///////////////////////////////////////////////////////
 // Print properties of this event at the given output
 ///////////////////////////////////////////////////////
void Event::printProperties(ostream & output)
 {
 output << "----------------------------------------------------------------" << endl;
 output << "       event index : " << eventIndex << endl;
 output << "      event number : " << eventNumber << endl;
 output << " number of particle: " << particles.size() << endl;
 eventProperties->printProperties(output);
 for (unsigned long iParticle=0; iParticle<particles.size(); iParticle++)
   {
   particles[iParticle]->printProperties(output);
   }
 }

vector<Event*> Event::eventStreams;

Event * Event::getEventStream(unsigned int index)
{
  while (index>=eventStreams.size())
    {
    eventStreams.push_back(new Event());
    }
  return eventStreams[index];
}

unsigned int Event::getNEventStreams()
{
return eventStreams.size();
}
