// Author: Claude Pruneau   09/25/2019

//!
//! Copyright (C) 2019, Claude Pruneau.                                   *
//! All rights reserved.                                                  *
//! Based on the ROOT package and environment                             *
//!                                                                       *
//! For the licensing terms see LICENSE.                                  *
//!
//!  \class Task
//!  \ingroup WAC
//!
//!  Class defining Two Particle Correlation Analyzer Task.
//!

#include "ParticlePairAnalyzerV2.hpp"

ClassImp(ParticlePairAnalyzerV2);

ParticlePairAnalyzerV2::ParticlePairAnalyzerV2(const TString &                 _name,
                                               ParticleAnalyzerConfiguration * _configuration,
                                               vector<EventFilter*>            _eventFilters,
                                               vector<ParticleFilter*>         _particleFilters,
                                               LogLevel                        _selectedLevel)
:
Task(),
nHistos(0),
nDerivedHistos(0),
nCombinedHistos(0),
filteredParticles()
{
  setName(_name);
  _configuration->useParticles      = true;
  _configuration->createHistograms  = true;
  setConfiguration(_configuration);
  setReportLevel(_selectedLevel);
  eventFilters    = _eventFilters;
  particleFilters = _particleFilters;
  appendClassName("ParticlePairAnalyzerV2");
  setInstanceName(_name);
  for (unsigned int k=0; k<particleFilters.size(); k++)
    {
    vector<ParticleDigit*> list;
    filteredParticles.push_back(list);
    }
}

const TString ParticlePairAnalyzerV2::makeHistoName(const TString & baseName,
                                                    const TString & evtFilterName,
                                                    const TString & partFilterName1,
                                                    const TString & suffix)
{
  TString histoName  = baseName;
  histoName += evtFilterName;
  histoName += "_";
  histoName += partFilterName1;
  histoName += suffix;
  return histoName;
}

const TString ParticlePairAnalyzerV2::makePairHistoName(const TString & baseName,
                                                        const TString & evtFilterName,
                                                        const TString & partFilterName1,
                                                        const TString & partFilterName2,
                                                        const TString & suffix)
{
  TString histoName  = baseName;
  histoName += evtFilterName;
  histoName += "_";
  histoName += partFilterName1;
  histoName += partFilterName2;
  histoName += suffix;
  return histoName;
}

void ParticlePairAnalyzerV2::createHistograms()
{
  if (reportStart("ParticlePairAnalyzerV2",getName(),"createHistograms()"))
    ;
  histograms.clear();
  derivedHistograms.clear();
  combinedHistograms.clear();
  ParticlePairAnalyzerConfiguration * ac = (ParticlePairAnalyzerConfiguration *) getConfiguration();
  LogLevel debugLevel = getReportLevel();
  TString bn  = getName();
  bn += "_";
  unsigned int nEventFilters    = eventFilters.size();
  unsigned int nParticleFilters = particleFilters.size();

  Histograms * histos;
  if (reportDebug("ParticlePairAnalyzerV2",getName(),"createHistograms()"))
    {
    cout << endl;
    cout << "       Creating Histograms: " << bn << endl;
    cout << "             nEventFilters: " << nEventFilters << endl;
    cout << "          nParticleFilters: " << nParticleFilters << endl;
    cout << endl;
    }
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    TString efn = eventFilters[iEventFilter]->getName();
    if (reportDebug("ParticlePairAnalyzerV2",getName(),"createHistograms()"))
      {
      cout << "Event filter:" << efn << endl;
      }
    // singles
    for (unsigned int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
      {
      TString pfn = particleFilters[iParticleFilter]->getName();
      if (reportDebug("ParticlePairAnalyzerV2",getName(),"createHistograms()"))
        {
        cout << "Particle filter (Singles):" << pfn << endl;
        }
      histos = new ParticleHistosV2(makeHistoName(bn,efn,pfn,""),ac,debugLevel);
      histos->createHistograms();
      histograms.push_back(histos);
      }

    for (unsigned int iParticleFilter1=0; iParticleFilter1<nParticleFilters; iParticleFilter1++ )
      {
      TString pfn1 = particleFilters[iParticleFilter1]->getName();
      for (unsigned int iParticleFilter2=0; iParticleFilter2<nParticleFilters; iParticleFilter2++ )
        {
        TString pfn2 = particleFilters[iParticleFilter2]->getName();
        if (reportDebug("ParticlePairAnalyzerV2",getName(),"createHistograms()"))
          {
          cout << "Particle pairs with filter: " << pfn1 << " & " << pfn2 << endl;
          }
        histos = new ParticlePairHistosV2(makePairHistoName(bn,efn,pfn1,pfn2,""),ac,debugLevel);
        histos->createHistograms();
        histograms.push_back(histos);

        if (ac->calculateDerivedHistograms)
          {
          histos = new ParticlePairDerivedHistosV2(makePairHistoName(bn,efn,pfn1,pfn2,""),ac,debugLevel);
          histos->createHistograms();
          derivedHistograms.push_back(histos);
          }
        if (ac->calculateDerivedHistograms && ac->calculateCombinedHistograms)
          {
          histos = new ParticlePairCombinedHistosV2(makePairHistoName(bn,efn,pfn1,pfn2,"_CI"),ac,debugLevel);
          histos->createHistograms();
          combinedHistograms.push_back(histos);
          histos = new ParticlePairCombinedHistosV2(makePairHistoName(bn,efn,pfn1,pfn2,"_CD"),ac,debugLevel);
          histos->createHistograms();
          combinedHistograms.push_back(histos);
          }
        }
      }

    // keep track of the number of histo per event filter class
    // only need to do this once.
    if (iEventFilter==0)
      {
      nHistos         = histograms.size();
      nDerivedHistos  = derivedHistograms.size();
      nCombinedHistos = combinedHistograms.size();
      }
    }
  if (reportEnd("ParticlePairAnalyzerV2",getName(),"createHistograms()"))
    ;
}

void ParticlePairAnalyzerV2::loadHistograms(TFile * inputFile)
{
  TString fct = "loadHistograms(TFile * inputFile)";
  if (reportStart(fct))
    ;
  if (!ptrFileExist(fct,inputFile)) return;
}


void ParticlePairAnalyzerV2::execute()
{
  incrementEventProcessed();
  Event & event = *eventStreams[0];

  //Is this event accepted by this task's event filters?
  bool analyzeThisEvent = false;
  unsigned int nEventFilters = eventFilters.size();
  vector<unsigned int> eventFilterPassed;
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    if (!eventFilters[iEventFilter]->accept(event)) continue;
    incrementEventAccepted(iEventFilter);
    eventFilterPassed.push_back(iEventFilter);
    analyzeThisEvent = true;
    }
  if (!analyzeThisEvent) return;

  // The event is accepted at least one event filter
  // but it may not have particle pairs. If so skip out.
  // Doing the checks in this order guarantees the accepted
  // event count is correct for normalization purposes.
  unsigned int nParticles = event.getNParticles();
  if (nParticles<2) return;

  Factory<ParticleDigit> * factory = ParticleDigit::getFactory();
  factory->reset();

  // produce sublists with ParticleDigits so we do not have to digitize too many
  // times...
  // The histo instance fetched here is used for digitization only. So
  // we use instance [0];
  ParticleHistosV2 * histos = (ParticleHistosV2 *) histograms[0];
  unsigned int nParticleFilters = particleFilters.size();
  for (unsigned int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ ) filteredParticles[iParticleFilter].clear();

  for (unsigned int iParticle=0; iParticle<nParticles; iParticle++)
    {
    Particle & particle = * event.getParticleAt(iParticle);
    float pt, e;
    int iPt, iPhi, iEta, iY;
    ParticleDigit * pd;
    bool digitized = false;
    for (unsigned int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
      {
      if (particleFilters[iParticleFilter]->accept(particle))
        {
        if (!digitized)
          {
          TLorentzVector & momentum = particle.getMomentum();
          pt     = momentum.Pt();
          e      = momentum.E();
          iPt    = histos->getPtBinFor(pt);
          iPhi   = histos->getPhiBinFor(momentum.Phi());
          iEta   = histos->getEtaBinFor(momentum.Eta());
          iY     = histos->getYBinFor(momentum.Rapidity());
          pd     = factory->getNextObject();
          pd->iY   = iY;
          pd->iEta = iEta;
          pd->iPt  = iPt;
          pd->iPhi = iPhi;
          pd->pt   = pt;
          pd->e    = e;
          digitized = true; // so no need to digitize this particle again...
          }
        if (digitized && iPt>=0 && iPhi>=0 && iEta>=0 && iY>=0)
          {
          filteredParticles[iParticleFilter].push_back(pd);
          }
        } // particle accepted by filter
      } //particle loop
    } // particle filter loop


  // nHistos = nParticleFilters * (nParticleFilters+1)
  // use the filtered particles to fill the histos for the accepted event filters
  for (unsigned int jEventFilter=0; jEventFilter<eventFilterPassed.size(); jEventFilter++ )
    {
    unsigned int  iEventFilter = eventFilterPassed[jEventFilter];
    unsigned int  singlePair   = iEventFilter*nHistos;
    unsigned int  basePair     = iEventFilter*nHistos+nParticleFilters;
    unsigned int  index;
    for (unsigned int iParticleFilter1=0; iParticleFilter1<nParticleFilters; iParticleFilter1++ )
      {
      index = singlePair + iParticleFilter1;
      ParticleHistosV2 * histos = (ParticleHistosV2 *) histograms[index];
      histos->fill(filteredParticles[iParticleFilter1],1.0);
      for (unsigned int iParticleFilter2=0; iParticleFilter2<nParticleFilters; iParticleFilter2++ )
        {
        index = basePair + iParticleFilter1*nParticleFilters + iParticleFilter2;
        ParticlePairHistosV2 * histos = (ParticlePairHistosV2 *) histograms[index];
        histos->fill(filteredParticles[iParticleFilter1],filteredParticles[iParticleFilter2],iParticleFilter1==iParticleFilter2,1.0);
        }
      }
    }
}


// ====================================
// calculate Derived Histograms
// ====================================
void ParticlePairAnalyzerV2::calculateDerivedHistograms()
{
  TString fct = "calculateDerivedHistograms()";
  unsigned int nEventFilters    = eventFilters.size();
  unsigned int nParticleFilters = particleFilters.size();

  if (reportInfo(fct))
    {
    cout << endl;
    cout << "           nEventFilters:" << nEventFilters << endl;
    cout << "        nParticleFilters:" << nParticleFilters << endl;
    cout << "                 nHistos:" << nHistos << endl;
    cout << "          nDerivedHistos:" << nDerivedHistos << endl;
    cout << "         nCombinedHistos:" << nCombinedHistos << endl;
    }
  ParticlePairAnalyzerConfiguration & ac = *(ParticlePairAnalyzerConfiguration *) getConfiguration();
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    if (reportDebug(fct)) cout << "  iEventFilter:" << iEventFilter << endl;

    unsigned int baseSingle        = iEventFilter*nHistos;
    unsigned int basePair          = iEventFilter*nHistos+nParticleFilters;
    unsigned int baseDerivedPair   = iEventFilter*nDerivedHistos;
    unsigned int baseCombinedPair  = iEventFilter*nCombinedHistos;

    //! Calculate derived spectra of singles
    for (unsigned int iParticleFilter1=0; iParticleFilter1<nParticleFilters; iParticleFilter1++)
      {
      TString pfn1 = particleFilters[iParticleFilter1]->getName();
      if (reportDebug(fct))   cout << "  iParticleFilter1:" << iParticleFilter1 << " named " << pfn1 << endl;
      ParticleHistosV2 * histos1 = (ParticleHistosV2 *) histograms[baseSingle+iParticleFilter1];
      histos1->calculateDerivedHistograms();
      }

    //! Calculate derived spectra of pairs
    for (unsigned int iParticleFilter1=0; iParticleFilter1<nParticleFilters; iParticleFilter1++)
      {
      TString pfn1 = particleFilters[iParticleFilter1]->getName();
      if (reportDebug(fct))   cout << "  iParticleFilter1:" << iParticleFilter1 << " named " << pfn1 << endl;
      ParticleHistosV2 * histos1 = (ParticleHistosV2 *) histograms[baseSingle+iParticleFilter1];

      for (unsigned int iParticleFilter2=0; iParticleFilter2<nParticleFilters; iParticleFilter2++)
        {
        TString pfn2 = particleFilters[iParticleFilter2]->getName();
        if (reportDebug(fct)) cout << "  iParticleFilter2:" << iParticleFilter2<< " named " << pfn2  << endl;
        ParticleHistosV2 * histos2 = (ParticleHistosV2 *) histograms[baseSingle+iParticleFilter2];
        ParticlePairHistosV2 * pairHistos = (ParticlePairHistosV2 *) histograms[basePair+iParticleFilter1*nParticleFilters+iParticleFilter2];
        ParticlePairDerivedHistosV2 * derivedPairHistos = (ParticlePairDerivedHistosV2 *) derivedHistograms[baseDerivedPair+iParticleFilter1*nParticleFilters+iParticleFilter2];
        derivedPairHistos->calculatePairDerivedHistograms(*histos1,*histos2,*pairHistos,ac.binCorrPP);
        }
      }
    if (!ac.calculateCombinedHistograms) continue;


    // ======================================================================
    // CI and CD combined correlators
    // In the following f# stands for a particle filter index, e.g., f1, f2
    // Two possible scenarios:
    // (1) a la nu-dyn: two species without respect to charge:
    //        CI = (f1f2) + (f2f1) + (f1f1) + (f2f2)
    //        CD = (f1f2) + (f2f1) - (f1f1) - (f2f2)
    // (2) for charge general balance functions
    //      f1: species 1, +ve
    //      f2: species 2, +ve
    //      f3: species 1, -ve
    //      f4: species 2, -ve
    //        CI = (f1f4) + (f3f2) + (f1f2) + (f3f4)
    //        CD = (f1f4) + (f3f2) - (f1f2) - (f3f4)
    //
    // Internally, there is no way to distinguish cases (1) and (2)
    // given there can be an arbitrary number of filters on input, so we need
    // to supply on input the relevant combinations of filters
    // ======================================================================
    unsigned int nCombinations = ac.combinations.size();

    if (reportInfo(fct)) cout << ">>>>>>>>>>>       Computing nCombinations:" << nCombinations << endl;
    for (unsigned int k=0; k<nCombinations; k++)
      {
      unsigned int iParticleFilter1;
      unsigned int iParticleFilter2;
      unsigned int iParticleFilter3;
      unsigned int iParticleFilter4;

      unsigned int index1 = 0;
      unsigned int index2 = 0;
      unsigned int index3 = 0;
      unsigned int index4 = 0;
      unsigned int indexCI = 0;
      unsigned int indexCD = 0;
      if (reportDebug(fct)) cout << "   Computing combination k:" << k << endl;

      vector<unsigned int> combination = ac.combinations[k]; // f1, f2 or f1,f2, f3, f4
      if (reportInfo(fct)) cout << "combination.size():" << combination.size() << endl;
      if (combination.size()==2)
        {
        iParticleFilter1 = combination[0];
        iParticleFilter2 = combination[1];
        if (reportDebug(fct)) cout << "     iParticleFilter1:" <<  iParticleFilter1 << " iParticleFilter2:" << iParticleFilter2 << endl;
        index1  = baseDerivedPair+iParticleFilter1*nParticleFilters+iParticleFilter2;
        index2  = baseDerivedPair+iParticleFilter2*nParticleFilters+iParticleFilter1;
        index3  = baseDerivedPair+iParticleFilter1*nParticleFilters+iParticleFilter1;
        index4  = baseDerivedPair+iParticleFilter2*nParticleFilters+iParticleFilter2;

        }
      else if (combination.size()==4)
        {
        iParticleFilter1 = combination[0];
        iParticleFilter2 = combination[1];
        iParticleFilter3 = combination[2];
        iParticleFilter4 = combination[3];
        if (reportDebug(fct)) cout << "     iParticleFilter1:" <<  iParticleFilter1 << " iParticleFilter2:" << iParticleFilter2 << endl;
        if (reportDebug(fct)) cout << "     iParticleFilter3:" <<  iParticleFilter3 << " iParticleFilter4:" << iParticleFilter4 << endl;
        index1  = baseDerivedPair+iParticleFilter1*nParticleFilters+iParticleFilter4;
        index2  = baseDerivedPair+iParticleFilter3*nParticleFilters+iParticleFilter2;
        index3  = baseDerivedPair+iParticleFilter1*nParticleFilters+iParticleFilter2;
        index4  = baseDerivedPair+iParticleFilter3*nParticleFilters+iParticleFilter4;
        }
      else
        {
        if (reportError(fct)) cout << endl << "    Invalid number of combinatinos encounted. Abort." << endl;
        exit(0);
        }
      indexCI = baseCombinedPair + 2*k;
      indexCD = baseCombinedPair + 2*k + 1;
      ParticlePairDerivedHistosV2  * derivedHistos1  = (ParticlePairDerivedHistosV2 *)  derivedHistograms[index1];
      ParticlePairDerivedHistosV2  * derivedHistos2  = (ParticlePairDerivedHistosV2 *)  derivedHistograms[index2];
      ParticlePairDerivedHistosV2  * derivedHistos3  = (ParticlePairDerivedHistosV2 *)  derivedHistograms[index3];
      ParticlePairDerivedHistosV2  * derivedHistos4  = (ParticlePairDerivedHistosV2 *)  derivedHistograms[index4];
      ParticlePairCombinedHistosV2 * combinedHistosCI = (ParticlePairCombinedHistosV2 *) combinedHistograms[indexCI];
      ParticlePairCombinedHistosV2 * combinedHistosCD = (ParticlePairCombinedHistosV2 *) combinedHistograms[indexCD];
      combinedHistosCI->calculate(derivedHistos1,derivedHistos2,derivedHistos3, derivedHistos4,  0.25, 0.25,  0.25, 0.25);
      combinedHistosCD->calculate(derivedHistos1,derivedHistos2,derivedHistos3, derivedHistos4,  0.25, 0.25, -0.25,-0.25);
      }
    }
  if (reportEnd(fct))
    ;
}


void ParticlePairAnalyzerV2::scaleHistograms()
{
  TString fct = "calculateDerivedHistograms()";
  double scalingFactor;
  unsigned int nEventFilters    = eventFilters.size();
  unsigned int nParticleFilters = particleFilters.size();
  if (reportDebug(fct))
    {
    cout << endl;
    cout << "              Accepted number of events: " <<  nEventAccepted << endl;
    cout << "                          nEventFilters: " <<  nEventFilters << endl;
    cout << "                       nParticleFilters: " <<  nParticleFilters << endl;
    cout << "                      histograms.size(): " <<  histograms.size() << endl;
    cout << "               derivedHistograms.size(): " <<  derivedHistograms.size() << endl;
    cout << "              combinedHistograms.size(): " <<  combinedHistograms.size() << endl;
    cout << "--------------   Accumulated statistics: ----------------------------------" << endl;
    }

  unsigned int index = 0;
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    if (nFilteredEventsAccepted[iEventFilter]>1)
      {
      scalingFactor = 1.0/double(nFilteredEventsAccepted[iEventFilter]);
      if (reportDebug(fct))
        {
        cout << endl;
        cout << "                                    iEventFilter: " <<  iEventFilter<< endl;
        cout << "           nFilteredEventsAccepted[iEventFilter]: " <<  nFilteredEventsAccepted[iEventFilter]<< endl;
        cout << "                                   scalingFactor: " <<  scalingFactor << endl;
        }
      for (unsigned int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
        {
        index = iEventFilter*nHistos + iParticleFilter;
        histograms[index]->scale(scalingFactor);
        }
      for (unsigned int iParticleFilter1=0; iParticleFilter1<nParticleFilters; iParticleFilter1++ )
        {
        for (unsigned int iParticleFilter2=0; iParticleFilter2<nParticleFilters; iParticleFilter2++ )
          {
          index = iEventFilter*nHistos + nParticleFilters + iParticleFilter1*nParticleFilters + iParticleFilter2;
          histograms[index]->scale(scalingFactor);
          }
        }
      }
    else
      {
      if (reportWarning(fct))
        {
        cout << endl;
        cout << "                                    iEventFilter: " <<  iEventFilter<< endl;
        cout << "           nFilteredEventsAccepted[iEventFilter]: " <<  nFilteredEventsAccepted[iEventFilter]<< endl;
        cout << "                            no scaling performed: " <<  endl;
        }
      }
    }
}

