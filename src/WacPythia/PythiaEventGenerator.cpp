// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
/**
 \class   PythiaEventGenerator
 \ingroup WAC

 Class defining Pythia Generator Wrapper for use in WAC
 */
#include "PythiaEventGenerator.hpp"

ClassImp(PythiaEventGenerator);

PythiaEventGenerator::PythiaEventGenerator(const TString &         _name,
                                           PythiaConfiguration *   _configuration,
                                           vector<EventFilter*>    _eventFilters,
                                           vector<ParticleFilter*> _particleFilters,
                                           LogLevel                _selectedLevel)
:
NucleonNucleonCollisionGenerator(),
pythia8(nullptr),
nMax(10000),
particles(nullptr),
outputFile(nullptr),
outputEvent(nullptr),
outputTree(nullptr),
standaloneMode(_configuration->standaloneMode)
{
  setName(_name);
  _configuration->useParticles    = true;
  _configuration->useEventStream0 = true;

  setConfiguration(_configuration);
  setReportLevel(_selectedLevel);
  eventFilters    = _eventFilters;
  particleFilters = _particleFilters;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Initialize generator
//pythia8->Initialize(2212 /* p */, 2212 /* p */, 14000. /* GeV */);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PythiaEventGenerator::initialize()
{
  Task::initialize();
  PythiaConfiguration * pc = (PythiaConfiguration*) getConfiguration();
  unsigned int nEventFilters    = eventFilters.size();
  unsigned int nParticleFilters = particleFilters.size();
  nFilteredEventsAccepted.assign(nEventFilters,0.0);
  pythia8 = new TPythia8();
  for (unsigned int iOption=0; iOption<pc->options.size(); iOption++)
    {
    pythia8->ReadString( *pc->options[iOption]);
    }
  pythia8->Initialize(pc->beam,pc->target,pc->energy);
  if (reportStart("PythiaEventGenerator",getName(),"initialize()"))
    {
    pythia8->ListAll();
    }
  if (pc->dataOutputUsed)
    {
    TString outputFileName = pc->dataOutputPath;
    outputFileName += "/";
    outputFileName += pc->dataOutputFileName;
    outputFile = TFile::Open(outputFileName,"recreate");
    //outputEvent = &pythia8->Pythia8()->event;
    outputTree  = new TTree(pc->dataOutputTreeName,"PythiaEventTree");
    particles = (TClonesArray*) pythia8->GetListOfParticles();
    outputTree->Branch("particles", &particles);
    //outputTree->Branch("event",&outputEvent);
    }
  if (pc->dataConversionToWac)
    {
    nMax = 10000;
    particles = new TClonesArray("TParticle", nMax);
    }
  if (reportEnd("PythiaEventGenerator",getName(),"initialize()"))
    ;
}

void PythiaEventGenerator::execute()
{
  if (reportStart("PythiaEventGenerator",getName(),"execute()"))
    ;
  incrementEventProcessed();
  Event & event = *eventStreams[0];
  Particle * interaction;
  resetParticleCounters();
  if (standaloneMode)
    {
    // In this mode, we generate one PYTHIA (pp) collision per event. One interaction vertex is
    // inserted in the event stream and PYTHIA is called to carry out the particle generation.
    particleFactory->reset();
    event.reset();
    resetParticleCounters();
    interaction = particleFactory->getNextObject();
    interaction->reset();
    interaction->setType( ParticleType::getInteractionType());
    interaction->setXYZT(0.0, 0.0, 0.0, 0.0);
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
    ep.multiplicity      = getNParticlesAccepted(); // nominal multiplicity in the reference range
    ep.particlesCounted  = getNParticlesCounted();
    ep.particlesAccepted = getNParticlesAccepted();
    }
  else
    {
    // In this mode, we generate several PYTHIA collisions per event. Interaction vertices and their locations
    // are assumed to be already loaded in the event stream and we produce as many PYTHIA events as there
    // are nucleon-nucleon interactions in the stream. Objects nucleusA and nucleusB are assumed defined by
    // an earlier task and the structure EventProperties is assumed filled by that earlier task.He we only fill the number of
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

void PythiaEventGenerator::generate(Particle * parentInteraction)
{
  PythiaConfiguration & pc = *(PythiaConfiguration*) getConfiguration();
  Particle * particle;

  int nparts           = 0;
  double zero          = 0.0;

  bool seekingEvent = true;
  while (seekingEvent)
    {
    pythia8->GenerateEvent();
    if (reportDebug()) pythia8->EventListing();
    pythia8->ImportParticles(particles,"Final");
    nparts = particles->GetEntriesFast();
    if (nparts>2) seekingEvent = false;
    }
  if (nparts>nMax)
    {
    if (reportError()) cout << " ARRAY TOO SMALL np>nMax. nparts:" << nparts << " nMax:" << nMax << endl;
    postTaskFatal();
    exit(1);
    }
  if (pc.dataOutputUsed)
    {
    outputTree->Fill();
    }
  if (pc.dataConversionToWac)
    {
    TLorentzVector sourcePosition = parentInteraction->getPosition();
    double sourceX = sourcePosition.X();
    double sourceY = sourcePosition.Y();
    double sourceZ = sourcePosition.Z();
    double sourceT = sourcePosition.T();

    Event & event = * eventStreams[0];
    ParticleFilter & particleFilter = * particleFilters[0];
    // load particles from TClone storage and copy into event.
    //if (reportDebug()) cout << "PythiaEventGenerator::execute() starting copy loop into event..." << endl;
    for (int iParticle = 0; iParticle < nparts; iParticle++)
      {
      TParticle & part = * (TParticle*) particles->At(iParticle);
      int ist = part.GetStatusCode();
      if (ist <= 0) continue;
      int pdg = part.GetPdgCode();
      if (pdg==22) continue;
//      if (pdg==311 || pdg==-311 || pdg==3122|| pdg==-3122)
//        {
//        cout << "K SHORT are not decaed by PYTHIA" << endl;
//        cout << "K SHORT are not decaed by PYTHIA" << endl;
//        cout << "K SHORT are not decaed by PYTHIA" << endl;
//        cout << "K SHORT are not decaed by PYTHIA" << endl;
//        exit(0);
//        }
//      if (pdg==2212 || pdg==-2212 )
//        {
//        if (part.GetMother(1)==3122 || part.GetMother(1)==-3122)
//          {
//          cout << "K SHORT are decaed by PYTHIA" << endl;
//          cout << "K SHORT are decaed by PYTHIA" << endl;
//          exit(0);
//          }
//        }

      ParticleType * type = masterCollection->findPdgCode(part.GetPdgCode());
      if (type==nullptr) continue;
      particle = particleFactory->getNextObject();
      particle->set(type,part.Px(),part.Py(),part.Pz(),part.Energy(),sourceX,sourceY,sourceZ,sourceT,true);
      incrementParticlesCounted(); // photons are NOT included in this tally
      if (!particleFilter.accept(*particle)) continue;
      event.add(particle);
      incrementParticlesAccepted();
      }
    if (reportDebug("PythiaEventGenerator",getName(),"execute()"))
      {
      cout << endl;
      cout << "PythiaEventGenerator::execute() No of accepted particles : "<< getNParticlesAccepted() << endl;
      cout << "PythiaEventGenerator::execute() No of counted particles : " << getNParticlesCounted()  << endl;
      }
    }
}

void PythiaEventGenerator::finalize()
{
  if (reportDebug()) cout << "PythiaEventGenerator::finalize() started" << endl;
  if (reportInfo()) pythia8->PrintStatistics();
  PythiaConfiguration * pc = (PythiaConfiguration*) getConfiguration();
  if (pc->dataOutputUsed)
    {
    outputTree->Print();
    outputTree->Write();
    delete outputFile;
    }
  if (reportDebug()) cout << "PythiaEventGenerator::finalize() completed" << endl;
}




//pythia8->ReadString("Init:showChangedSettings = on");      // list changed settings
//pythia8->ReadString("Init:showChangedParticleData = off"); // list changed particle data
//pythia8->ReadString("Next:numberCount = 100000");            // print message every n events
//pythia8->ReadString("Next:numberShowInfo = 1");            // print event information n times
//pythia8->ReadString("Next:numberShowProcess = 0");         // print process record n times
//pythia8->ReadString("Next:numberShowEvent = 0");
//
//pythia8->ReadString("SoftQCD:all = off");                   // Allow total sigma = elastic/SD/DD/ND
//                                                           // Optionally only study one or a few processes at a time.
//                                                           //  pythia8->ReadString("SoftQCD:elastic = on");               // Elastic
//                                                           //  pythia8->ReadString("SoftQCD:singleDiffractive = on");     // Single diffractive
//                                                           //  pythia8->ReadString("SoftQCD:doubleDiffractive = on");     // Double diffractive
//                                                           //  pythia8->ReadString("SoftQCD:centralDiffractive = on");    // Central diffractive
//                                                           //  pythia8->ReadString("SoftQCD:nonDiffractive = on");        // Nondiffractive (inelastic)
//                                                           //  pythia8->ReadString("SoftQCD:inelastic = on");             // All inelastic
//                                                           // Optionally switch on hand-set cross section and Coulomb term.
//                                                           // Note: these values are illustrative only, not to be taken seriously.
//                                                           //  pythia8->ReadString("SigmaTotal:setOwn = on");              // set your own total/elastic/diffr. sigma
//                                                           //  pythia8->ReadString("SigmaTotal:sigmaTot = 106.0");         // total cross section (excluding Coulomb)
//                                                           //  pythia8->ReadString("SigmaTotal:sigmaXX = 5.0");        // reduce double diffractive cross section
//                                                           //  pythia8->ReadString("SigmaElastic:setOwn = on");        // suboption for Coulomb term in elastic
//                                                           //  pythia8->ReadString("SigmaElastic:tAbsMin = 4e-5");        // divergence requires lower cutoff
//
//pythia8->ReadString("HardQCD:all = on");
//
//
////  Optionally select diffractive model.
////  pythia8->ReadString("SigmaDiffractive:mode = 2");           // the MBR model for diffraction
//
//// Multiparton interactions and impact parameter picture.
//// Note: these values are illustrative only, not to be taken seriously.
////  pythia8->ReadString("MultipartonInteractions:pT0Ref = 2.");
////  pythia8->ReadString("MultipartonInteractions:ecmRef = 1960.");
////  pythia8->ReadString("MultipartonInteractions:ecmPow = 0.16");
////  pythia8->ReadString("MultipartonInteractions:pTmin = 0.2");
////  pythia8->ReadString("MultipartonInteractions:bProfile = 2");
////  pythia8->ReadString("MultipartonInteractions:coreRadius = 0.4");
////  pythia8->ReadString("MultipartonInteractions:coreFraction = 0.5");
////  pythia8->ReadString("ultipartonInteractions:expPow = 1.");
//
//// use a reproducible seed: always the same results for the tutorial.
////pythia8->ReadString("Random:setSeed = on");
////pythia8->ReadString("Random:seed = 42");


//for (int iParticle = 0; iParticle < nparts; iParticle++)
//  {
//  TParticle & part = * (TParticle*) particles->At(iParticle);
//  int ist = part.GetStatusCode();
//  //if (reportDebug()) cout << "PythiaEventGenerator::execute() ist: " << ist << endl;
//  if (ist <= 0) continue;
//  int pdg = part.GetPdgCode();
//  mass = TDatabasePDG::Instance()->GetParticle(pdg)->Mass();
//  if (mass<0.002) continue;  // no photons, electrons...
//  charge = TDatabasePDG::Instance()->GetParticle(pdg)->Charge();
//  p_x  = cosPhi*part.Px() - sinPhi*part.Py();
//  p_y  = sinPhi*part.Px() + cosPhi*part.Py();
//  p_z  = part.Pz();
//  p_e  = part.Energy();
//  aParticle.setPidPxPyPzE(pdg,p_x,p_y,p_z,p_e);
//  //aParticle.printProperties(cout);
//  //if (reportDebug()) cout << "PythiaEventGenerator::execute() calling filter " << endl;
//  particleCounted++;
//  if (!particleFilter->accept(aParticle)) continue;
//  particle = particleFactory->getNextObject();
//  *particle = aParticle;
//  event->add(particle);
//  particleAccepted++;
//  //    if (true)
//  //      {
//  //      cout << "PythiaEventGenerator::execute() particle: " << iParticle << " / " << particleAccepted << endl;
//  //      particle->printProperties(cout);
//  //      }
//  }
