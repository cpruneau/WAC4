// Author: Claude Pruneau   09/25/2019

/*************************************************************************
 * Copyright (C) 2019, Claude Pruneau.                                   *
 * All rights reserved.                                                  *
 * Based on the ROOT package and environment                             *
 *                                                                       *
 * For the licensing terms see LICENSE.                                  *
 *************************************************************************/
/**
 \class Task
 \ingroup WAC

 Class defining Two Particle Correlation Analyzer Task
 */

#include "ParticleDecayerTask.hpp"

ClassImp(ParticleDecayerTask);

ParticleDecayerTask::ParticleDecayerTask(LogLevel _selectedLevel)
:
Task(),
decayer()
{
  setName("ParticleDecayerTask");
  TaskConfiguration & config = * getConfiguration();
  config.useParticles    = true;
  config.useEventStream0 = true;
  setReportLevel(_selectedLevel);
 }

// ====================================================================
// Decay unstable particles in the event
// Decayed particles are retained in the event but their live
// flag is set to false.
// Children particles are added at the tail end of the event
// with a live flag  set to true.
// ====================================================================
void ParticleDecayerTask::execute()
{
  if (reportDebug("ParticleDecayerTask",getName(),"execute()"))
    cout << "---1---" << endl;
  incrementEventProcessed();
  incrementEventAccepted();
  if (reportDebug("ParticleDecayerTask",getName(),"execute()"))
    cout << "---2---" << endl;
  Event & event = * eventStreams[0];
  if (reportDebug("ParticleDecayerTask",getName(),"execute()"))
    cout << "---3---" << endl;
  int nParticles = event.getNParticles();

  if (nParticles< 1)
    {
    if (reportWarning("ParticleDecayerTask",getName(),"execute()"))
      cout << "Skipping empty event." << endl;
    return;
    }

  unsigned int iParticle = 0;
  bool done = false;
  while (!done)
    {
    if (reportDebug("ParticleDecayerTask",getName(),"execute()"))
      cout << "iParticle:" << iParticle << endl;
    Particle & parent = * event.getParticleAt(iParticle);
    if (parent.isLive() &&  !parent.isStable() && !parent.isInteraction() )
      {
      if (reportDebug("ParticleDecayerTask",getName(),"execute()"))
        cout << "isLive()" <<  endl;
      ParticleType   &    parentType      = parent.getType();
      TLorentzVector &    parentMomentum  = parent.getMomentum();
      TLorentzVector &    parentPosition  = parent.getPosition();
      ParticleDecayMode & decayMode       = parentType.generateDecayMode();
      int nChildren = decayMode.getNChildren();
      switch (nChildren)
        {
          case 1:
          break;
          case 2:
          {
          Particle * child1 = particleFactory->getNextObject();
          Particle * child2 = particleFactory->getNextObject();
          ParticleType   & childType1 = decayMode.getChildType(0); child1->setType(&childType1); child1->setLive(true);
          ParticleType   & childType2 = decayMode.getChildType(1); child2->setType(&childType2); child2->setLive(true);
          TLorentzVector & p1 = child1->getMomentum();
          TLorentzVector & r1 = child1->getPosition();
          TLorentzVector & p2 = child2->getMomentum();
          TLorentzVector & r2 = child2->getPosition();
          decayer.decay2(parentType,
                         parentMomentum,
                         parentPosition,
                         childType1,p1,r1,
                         childType2,p2,r2);
          event.add(child1);
          event.add(child2);
          parent.setDecayed(true);
          nParticles += 2;
          }
          break;


          case 3:
          {
          Particle * child1 = particleFactory->getNextObject();
          Particle * child2 = particleFactory->getNextObject();
          Particle * child3 = particleFactory->getNextObject();
          ParticleType   & childType1 = decayMode.getChildType(0); child1->setType(&childType1); child1->setLive(true);
          ParticleType   & childType2 = decayMode.getChildType(1); child2->setType(&childType2); child2->setLive(true);
          ParticleType   & childType3 = decayMode.getChildType(2); child3->setType(&childType3); child3->setLive(true);
          TLorentzVector & p1 = child1->getMomentum();
          TLorentzVector & r1 = child1->getPosition();
          TLorentzVector & p2 = child2->getMomentum();
          TLorentzVector & r2 = child2->getPosition();
          TLorentzVector & p3 = child3->getMomentum();
          TLorentzVector & r3 = child3->getPosition();
          decayer.decay3(parentType,
                         parentMomentum,
                         parentPosition,
                         childType1,p1,r1,
                         childType2,p2,r2,
                         childType3,p3,r3);
          event.add(child1);
          event.add(child2);
          event.add(child3);
          parent.setDecayed(true);
          nParticles += 3;
          }
          break;
          case 4:
          {
          //cout << "ParticleDecayerTask::execute() skipping 4" << endl;

          break;
  //        Particle * child1 = particleFactory->getNextObject();
  //        Particle * child2 = particleFactory->getNextObject();
  //        Particle * child3 = particleFactory->getNextObject();
  //        Particle * child4 = particleFactory->getNextObject();
  //        ParticleType   & childType1 = decayMode.getChildType(0); child1->setType(&childType1); child1->setLive(true);
  //        ParticleType   & childType2 = decayMode.getChildType(1); child2->setType(&childType2); child2->setLive(true);
  //        ParticleType   & childType3 = decayMode.getChildType(2); child3->setType(&childType3); child3->setLive(true);
  //        ParticleType   & childType4 = decayMode.getChildType(3); child4->setType(&childType4); child4->setLive(true);
  //        TLorentzVector & p1 = child1->getMomentum();
  //        TLorentzVector & r1 = child1->getPosition();
  //        TLorentzVector & p2 = child2->getMomentum();
  //        TLorentzVector & r2 = child2->getPosition();
  //        TLorentzVector & p3 = child3->getMomentum();
  //        TLorentzVector & r3 = child3->getPosition();
  //        TLorentzVector & p4 = child4->getMomentum();
  //        TLorentzVector & r4 = child4->getPosition();
  //        decayer.decay4(parentType,
  //                       parentMomentum,
  //                       parentPosition,
  //                       childType1,p1,r1,
  //                       childType2,p2,r2,
  //                       childType3,p3,r3,
  //                       childType4,p4,r4);
  //        event.add(child1);
  //        event.add(child2);
  //        event.add(child3);
  //        event.add(child4);
  //        parent.setDecayed(true);
  //        nParticles += 4;
          }
          break;
          case 5:
          break;
        }
     // cout << "ParticleDecayerTask::execute() iParticle" << iParticle << " nParticles:" << nParticles << " event.getNParticles():" << event.getNParticles()<< " Done:" << done << endl;
      }
    iParticle++;
    done = (iParticle >= event.getNParticles());
    }
 // cout << "ParticleDecayerTask::execute() DONE" << endl;
}





