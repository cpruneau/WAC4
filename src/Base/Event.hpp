// Author: Claude Pruneau   09/25/2019

/*!
Copyright (C) 2019-2021, Claude Pruneau, Victor Gonzalez, Sumit Basu.
All rights reserved.
Based on the ROOT package and environment
For the licensing terms see LICENSE.
*/

#ifndef Event_hpp
#define Event_hpp
#include <vector>
#include "Particle.hpp"
#include "Nucleus.hpp"
#include "EventProperties.hpp"
#include "CollisionGeometryMoments.hpp"

//!
//! Class encupsalting all components of events (real data or monte carlo)
//!
class Event
{
protected:

  //!
  //! Constructor is protected. Use the static function getEventStream() to obtain instances of this class.
  //!
  Event();

  public:

  virtual ~Event();

  //!
  //! Clear the event as an empty event
  //!
  virtual void clear();

  //!
  //! Reset the event as an empty event
  //!
  virtual void reset();

  //!
  //! Add the given particle to this  event
  //!
  virtual void add(Particle * particle);

  //!
  //! Return to the particle at position "index" in this event. A null pointer is returned is index exceeds the
  //!  number of particles in this event.
  //!
  Particle * getParticleAt(unsigned long index)
  {
    if (index<particles.size() )
      {
      return particles[index];
      }
    else
      return nullptr;
  }

  //!
  //! Return the number of particles stored in this event.
  //!
  unsigned long  getParticleCount() { return particles.size();}

  //!
  //! Return the number of particles stored in this event.
  //!
  unsigned long  getNParticles() { return particles.size();}

  //!
  //! Return a vector of all the particles stored in this event.
  //!
  vector<Particle*> & getParticles() { return particles;}

  //!
  //! Return a constant vector of all the particles stored in this event.
  //!
  const vector<Particle*> & getParticles() const { return particles;}

  //!
  //! Return the index of this event. The event index might correspond to the position of the event
  //! in the production or input stream.
  //!
  unsigned long getEventIndex() const      { return eventIndex;  }

  //!
  //! Return the event number of this event. The event number might correspond to a tag or index given
  //! to this event externally by some other  production or input stream.
  //!
  unsigned long getEventNumber() const     { return eventNumber; }

  //!
  //! Increment the index of this event..
  //!
  void incrementEventIndex()      { eventIndex++;       }

  //!
  //! Set the index of this event..
  //!
  void setEventIndex(unsigned long index)  { eventIndex  = index;}

  //!
  //! Set the event number of this event.
  //!
  void setEventNumber(unsigned long number){ eventNumber = number;}

  //!
  //! Replace (set) the current event property object held by this event to that given. This is a pointer assignment not a deep copy/assignment.
  //!
  void setEventProperties(EventProperties * properties) { eventProperties = properties; }

  //!
  //! Print  the properties of this event on the given output stream.
  //!
  virtual void printProperties(ostream & output);

  //!
  //! Add an interaction between the two given nucleons to the current event. The function does NOT change the wounded status of the two
  //! nucleons being added. Changing their wounded status must be done explicitly by calling the setWounded(true) on instances nucleonA and nucleonB.
  //!
  Particle* addInteraction(Particle* nucleonA, Particle* nucleonB);

  //!
  //! Set the impact parameter of this event to the given value.
  //! @param _b impact paramter of this event.
  void setImpactParameter(double _b)  { b = _b;   }

  //!
  //! Set nucleus A to have the given atomic number (z) and mass number (a).
  //! @param z atomic number.
  //! @param a mass number.
  void setNucleusA(int z, int a);
  void setNucleusB(int z, int a);

  double    getImpactParameter() const { return b; }
  double    getCrossSection() const { return b*b/2.0; }
  Nucleus & getNucleusA() { return *nucleusA; }
  Nucleus & getNucleusB() { return *nucleusB; }

  unsigned int getNParticipants() const;
  unsigned int getNBinaryCollisions() const;

  vector<Particle*> getNucleonNucleonInteractions();

//  unsigned int getNProtonProtonCollisions()   const;
//  unsigned int getNProtonNeutronCollisions()  const;
//  unsigned int getNNeutronNeutronCollisions() const;
//  unsigned int getNBinaryCollisions() const           { return nnInteractions.size(); }

  EventProperties * getEventProperties() { return eventProperties; }

  static Event * getEventStream(unsigned int index);
  static unsigned int getNEventStreams();

  //!
  //! Return the participant moments of the current event
  //!
  CollisionGeometryMoments & getParticipantMoments()
  {
  return *participantMoments;
  }

  //!
  //! Return the participant moments of the current event
  //!
  const CollisionGeometryMoments & getParticipantMoments() const
  {
  return *participantMoments;
  }

  //!
  //! Return the binary moments of the current event
  //!
  CollisionGeometryMoments & getBinaryMoments()
  {
  return *binaryMoments;
  }

  //!
  //! Return the binary moments of the current event
  //!
  const CollisionGeometryMoments & getBinaryMoments() const
  {
  return *binaryMoments;
  }
protected:

  unsigned long eventIndex;
  unsigned long eventNumber;
  vector<Particle*> particles;
  EventProperties * eventProperties;
  static vector<Event*> eventStreams;

  double b;
  Nucleus * nucleusA;
  Nucleus * nucleusB;
  CollisionGeometryMoments * binaryMoments;
  CollisionGeometryMoments * participantMoments;

  ClassDef(Event,0)

};




#endif /* Event_hpp */