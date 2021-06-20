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

#include "ParticlePairAnalyzer.hpp"

ClassImp(ParticlePairAnalyzer);

ParticlePairAnalyzer::ParticlePairAnalyzer(const TString &                 _name,
                                           ParticleAnalyzerConfiguration * _configuration,
                                           vector<EventFilter*>            _eventFilters,
                                           vector<ParticleFilter*>         _particleFilters,
                                           LogLevel                        _selectedLevel)
:
Task(),
nHistos(0),
nDerivedHistos(0)
{
  setName(_name);
  _configuration->useParticles      = true;
  _configuration->createHistograms  = true;
  setConfiguration(_configuration);
  setReportLevel(_selectedLevel);
  eventFilters    = _eventFilters;
  particleFilters = _particleFilters;
}

const TString ParticlePairAnalyzer::makeHistoName(const TString & baseName,
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


const TString ParticlePairAnalyzer::makePairHistoName(const TString & baseName,
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


void ParticlePairAnalyzer::createHistograms()
{
  if (reportStart("ParticlePairAnalyzer",getName(),"createHistograms()"))
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
  nFilteredEventsAccepted.assign(nEventFilters,0.0);
  Histograms * histos;
  if (reportInfo("ParticlePairAnalyzer",getName(),"createHistograms()"))
    {
    cout << "Creating Histograms for..."  << endl;
    cout << "       nEventFilters: " << nEventFilters << endl;
    cout << "    nParticleFilters: " << nParticleFilters << endl;
    }
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
     TString efn = eventFilters[iEventFilter]->getName();
    // singles
    for (unsigned int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
      {
      TString pfn = particleFilters[iParticleFilter]->getName();
      histos = new ParticleHistos(makeHistoName(bn,efn,pfn,""),ac,debugLevel);
      histos->createHistograms();
      histograms.push_back(histos);
      }
    // pairs
    if (ac->fillPairs)
      {
      for (unsigned int iParticleFilter1=0; iParticleFilter1<nParticleFilters; iParticleFilter1++ )
        {
        TString pfn1 = particleFilters[iParticleFilter1]->getName();
        for (unsigned int iParticleFilter2=0; iParticleFilter2<nParticleFilters; iParticleFilter2++ )
          {
          TString pfn2 = particleFilters[iParticleFilter2]->getName();
          histos = new ParticlePairHistos(makePairHistoName(bn,efn,pfn1,pfn2,""),ac,debugLevel);
          histos->createHistograms();
          histograms.push_back(histos);
          }
        }
      }
    // derived pairs
    if (ac->fillPairs &&
        ac->calculateDerivedHistograms)
      {
      for (unsigned int iParticleFilter1=0; iParticleFilter1<nParticleFilters; iParticleFilter1++ )
        {
        TString pfn1 = particleFilters[iParticleFilter1]->getName();
        for (unsigned int iParticleFilter2=0; iParticleFilter2<nParticleFilters; iParticleFilter2++ )
          {
          TString pfn2 = particleFilters[iParticleFilter2]->getName();
          histos = new ParticlePairDerivedHistos(makePairHistoName(bn,efn,pfn1,pfn2,""),ac,debugLevel);
          histos->createHistograms();
          derivedHistograms.push_back(histos);
          }
        }
      }
    // combined pair histograms such as CI and CD
    if (ac->fillPairs &&
        ac->calculateDerivedHistograms &&
        ac->calculateCombinedHistograms)
      {
      unsigned int nCombinations = ac->combinations.size();
      for (unsigned int k=0;k<nCombinations;k++)
        {
        vector<unsigned int> combination = ac->combinations[k]; // f1, f2 or f1,f2, f3, f4
        unsigned int iParticleFilter1 = combination[0]; TString pfn1 = particleFilters[iParticleFilter1]->getName();
        unsigned int iParticleFilter2 = combination[1]; TString pfn2 = particleFilters[iParticleFilter2]->getName();
        histos = new ParticlePairCombinedHistos(makePairHistoName(bn,efn,pfn1,pfn2,"_CI"),ac,debugLevel);
        histos->createHistograms();
        combinedHistograms.push_back(histos);
        histos = new ParticlePairCombinedHistos(makePairHistoName(bn,efn,pfn1,pfn2,"_CD"),ac,debugLevel);
        histos->createHistograms();
        combinedHistograms.push_back(histos);
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
  if (reportEnd("ParticlePairAnalyzer",getName(),"createHistograms()"))
    ;
}

void ParticlePairAnalyzer::loadHistograms(TFile * inputFile)
{
  if (reportStart("ParticlePairAnalyzer",getName(),"loadHistograms(TFile * inputFile)"))
    ;
  if (reportEnd("ParticlePairAnalyzer",getName(),"loadHistograms(TFile * inputFile)"))
    ;
}


void ParticlePairAnalyzer::execute()
{
  incrementEventProcessed();
  ParticlePairAnalyzerConfiguration & ac = * (ParticlePairAnalyzerConfiguration *) getConfiguration();
  Event & event = *eventStreams[0];
  unsigned int nEventFilters    = eventFilters.size();
  unsigned int nParticleFilters = particleFilters.size();
  unsigned int nParticles       = event.getNParticles();

  /* before filtering let's build the particle indexes to speed up the process */
  for (unsigned int iParticle = 0; iParticle < nParticles; iParticle++)
    {
    Particle & particle = *event.getParticleAt(iParticle);
    particle.ixEtaPhi   = ac.getIxEtaPhi(particle.getMomentum().Eta(),particle.getMomentum().Phi());
    particle.ixYPhi     = ac.getIxYPhi(particle.getMomentum().Rapidity(),particle.getMomentum().Phi());
    }

  unsigned int index;
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    if (!eventFilters[iEventFilter]->accept(event)) continue;
    incrementEventAccepted(); // count eventStreams used to fill histograms and for scaling at the end...
    nFilteredEventsAccepted[iEventFilter]++;
    vector<double> nAccepted(nParticleFilters,0.0);
    vector<double> totalEnergy(nParticleFilters,0.0);

    unsigned int baseSingle = iEventFilter*nHistos;
    unsigned int basePair   = iEventFilter*nHistos+nParticleFilters;

    // Fill singles

    //cout << "ParticlePairAnalyzer::execute() ----------------------------------------- " << endl;
    //cout << "ParticlePairAnalyzer::execute() nParticleFilters:" << nParticleFilters << endl;
    for (unsigned int iParticle1=0; iParticle1<nParticles; iParticle1++)
      {
      Particle & particle1 = * event.getParticleAt(iParticle1);
      //cout << "ParticlePairAnalyzer::execute() iParticle1:" << iParticle1 << endl;
      for (unsigned int iParticleFilter1=0; iParticleFilter1<nParticleFilters; iParticleFilter1++)
        {
        //cout << "ParticlePairAnalyzer::execute() iParticleFilter1:" << iParticleFilter1 << endl;

        if (particleFilters[iParticleFilter1]->accept(particle1))
          {
          //cout << "ParticlePairAnalyzer::execute() accepted:" << endl;
          nAccepted[iParticleFilter1]++;
          totalEnergy[iParticleFilter1] += particle1.getMomentum().E();
          index = baseSingle+iParticleFilter1;
          ParticleHistos * histos = (ParticleHistos *) histograms[index];
          histos->fill(particle1, 1.0);
          }
        }
      }

    for (unsigned int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
      {
      index = baseSingle+iParticleFilter;
      ParticleHistos * histos = (ParticleHistos *) histograms[index];
      histos->fillMultiplicity(nAccepted[iParticleFilter],totalEnergy[iParticleFilter],1.0);
      }

    //ParticleHistos * histos = (ParticleHistos *) histograms[iEventFilter*nHistos+iParticleFilter1];
    //histos->fill(nAccepted,totalEnergy, 1.0);

    // Fill pairs
    if (ac.fillPairs)
      {

      for (unsigned int iParticle1=0; iParticle1<nParticles; iParticle1++)
        {
        Particle & particle1 = * event.getParticleAt(iParticle1);
        for (unsigned int iParticle2=0; iParticle2<nParticles; iParticle2++)
          {
          if (iParticle1==iParticle2) continue;
          Particle & particle2 = * event.getParticleAt(iParticle2);
          for (unsigned int iParticleFilter1=0; iParticleFilter1<nParticleFilters; iParticleFilter1++)
            {
            if (!particleFilters[iParticleFilter1]->accept(particle1)) continue;
            for (unsigned int iParticleFilter2=0; iParticleFilter2<nParticleFilters; iParticleFilter2++)
              {
              if (!particleFilters[iParticleFilter2]->accept(particle2))  continue;
              index = basePair+iParticleFilter1*nParticleFilters+iParticleFilter2;
              ParticlePairHistos * histos = (ParticlePairHistos *) histograms[index];
              histos->fill(particle1,particle2, 1.0);
              } // iParticleFilter2
            } // iParticleFilter1
          } //iParticle2
        } // iParticle1
      } // pairs
    } // eventFilters
}


// ====================================
// calculate Derived Histograms
// ====================================
void ParticlePairAnalyzer::calculateDerivedHistograms()
{
  unsigned int nEventFilters    = eventFilters.size();
  unsigned int nParticleFilters = particleFilters.size();
  if (reportInfo("ParticlePairAnalyzer",getName(),"calculateDerivedHistograms()"))
    {
    cout << "    nEventFilters:" << nEventFilters << endl;
    cout << " nParticleFilters:" << nParticleFilters << endl;
    cout << "          nHistos:" << nHistos << endl;
    cout << "   nDerivedHistos:" << nDerivedHistos << endl;
    cout << "  nCombinedHistos:" << nCombinedHistos << endl;
    }
  ParticlePairAnalyzerConfiguration & ac = *(ParticlePairAnalyzerConfiguration *) getConfiguration();
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    if (reportInfo("ParticlePairAnalyzer",getName(),"calculateDerivedHistograms()"))
      {
      cout << "     iEventFilter:" << iEventFilter << endl;
      cout << "       Do Singles:" << endl;
      }
    unsigned int baseSingle        = iEventFilter*nHistos;
    unsigned int basePair          = iEventFilter*nHistos+nParticleFilters;
    unsigned int baseDerivedPair   = iEventFilter*nDerivedHistos;
    unsigned int baseCombinedPair  = iEventFilter*nCombinedHistos;

    // singles
    for (unsigned int iParticleFilter1=0; iParticleFilter1<nParticleFilters; iParticleFilter1++)
      {
      ParticleHistos * histos = (ParticleHistos *) histograms[baseSingle+iParticleFilter1];
      if (reportInfo("ParticlePairAnalyzer",getName(),"calculateDerivedHistograms()"))
        {
        cout << " iParticleFilter1:" << iParticleFilter1 << endl;
        cout << "   completeFill():" <<  endl;
        }
      histos->completeFill();
      if (reportInfo("ParticlePairAnalyzer",getName(),"calculateDerivedHistograms()"))
        {
        cout << " calculateAverages():" <<  endl;
        }
      histos->calculateAverages();
      
      }
    if (!ac.fillPairs) continue;
    // pairs
    if (reportInfo("ParticlePairAnalyzer",getName(),"calculateDerivedHistograms()"))
      {
      cout << "         Do Pairs:" << endl;
      }
    for (unsigned int iParticleFilter1=0; iParticleFilter1<nParticleFilters; iParticleFilter1++)
      {
      ParticleHistos * histos1 = (ParticleHistos *) histograms[baseSingle+iParticleFilter1];
      for (unsigned int iParticleFilter2=0; iParticleFilter2<nParticleFilters; iParticleFilter2++)
        {
        ParticleHistos * histos2 = (ParticleHistos *) histograms[baseSingle+iParticleFilter2];
        ParticlePairHistos * pairHistos = (ParticlePairHistos *) histograms[basePair+iParticleFilter1*nParticleFilters+iParticleFilter2];
        pairHistos->completeFill();
        ParticlePairDerivedHistos * derivedPairHistos = (ParticlePairDerivedHistos *) derivedHistograms[baseDerivedPair+iParticleFilter1*nParticleFilters+iParticleFilter2];
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
    if (reportInfo("ParticlePairAnalyzer",getName(),"calculateDerivedHistograms()"))
      {
      cout << "Computing nCombinations:" << nCombinations << endl;
      }
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
      if (reportInfo("ParticlePairAnalyzer",getName(),"calculateDerivedHistograms()"))
        {
        cout << "Computing combination k:" << k << endl;
        }
      vector<unsigned int> combination = ac.combinations[k]; // f1, f2 or f1,f2, f3, f4
      if (reportInfo("ParticlePairAnalyzer",getName(),"calculateDerivedHistograms()"))
        {
        cout << "combination.size():" << combination.size() << endl;
        }
      if (combination.size()==2)
        {
        iParticleFilter1 = combination[0];
        iParticleFilter2 = combination[1];
        cout << "iParticleFilter1:" <<  iParticleFilter1 << " iParticleFilter2:" << iParticleFilter2 << endl;
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
        cout << "iParticleFilter1:" <<  iParticleFilter1 << " iParticleFilter2:" << iParticleFilter2 << endl;
        cout << "iParticleFilter3:" <<  iParticleFilter3 << " iParticleFilter4:" << iParticleFilter4 << endl;
        index1  = baseDerivedPair+iParticleFilter1*nParticleFilters+iParticleFilter4;
        index2  = baseDerivedPair+iParticleFilter3*nParticleFilters+iParticleFilter2;
        index3  = baseDerivedPair+iParticleFilter1*nParticleFilters+iParticleFilter2;
        index4  = baseDerivedPair+iParticleFilter3*nParticleFilters+iParticleFilter4;
        }
      else
        {
        if (reportError("ParticlePairAnalyzer",getName(),"calculateDerivedHistograms()"))
          {
          cout << "Invalid number of combinatinos encounted. Abort." << endl;
          }
        exit(0);
        }
      indexCI = baseCombinedPair + 2*k;
      indexCD = baseCombinedPair + 2*k + 1;
      ParticlePairDerivedHistos  * derivedHistos1  = (ParticlePairDerivedHistos *)  derivedHistograms[index1];
      ParticlePairDerivedHistos  * derivedHistos2  = (ParticlePairDerivedHistos *)  derivedHistograms[index2];
      ParticlePairDerivedHistos  * derivedHistos3  = (ParticlePairDerivedHistos *)  derivedHistograms[index3];
      ParticlePairDerivedHistos  * derivedHistos4  = (ParticlePairDerivedHistos *)  derivedHistograms[index4];
      ParticlePairCombinedHistos * combinedHistosCI = (ParticlePairCombinedHistos *) combinedHistograms[indexCI];
      ParticlePairCombinedHistos * combinedHistosCD = (ParticlePairCombinedHistos *) combinedHistograms[indexCD];
      combinedHistosCI->calculate(derivedHistos1,derivedHistos2,derivedHistos3, derivedHistos4,  0.25, 0.25,  0.25, 0.25);
      combinedHistosCD->calculate(derivedHistos1,derivedHistos2,derivedHistos3, derivedHistos4,  0.25, 0.25, -0.25,-0.25);
      }
    }
  if (reportEnd("ParticlePairAnalyzer",getName(),"calculateDerivedHistograms()"))
    ;
}


void ParticlePairAnalyzer::scaleHistograms()
{
  unsigned int nEventFilters    = eventFilters.size();
  unsigned int nParticleFilters = particleFilters.size();
  if (reportInfo("Task",getName(),"scaleHistograms()"))
    ;
  unsigned int index = 0;
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    // Scale the singles....
    double factor = 1.0/double(nFilteredEventsAccepted[iEventFilter]);
    for (unsigned int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
      {
      index = iEventFilter*nHistos + iParticleFilter;
      histograms[index]->scale(factor);
      }
    // Scale the pairs...
    for (unsigned int iParticleFilter1=0; iParticleFilter1<nParticleFilters; iParticleFilter1++ )
      {
      for (unsigned int iParticleFilter2=0; iParticleFilter2<nParticleFilters; iParticleFilter2++ )
        {
        index = iEventFilter*nHistos + nParticleFilters + iParticleFilter1*nParticleFilters + iParticleFilter2;
        histograms[index]->scale(factor);
        }
      }
    }
  if (reportInfo("Task",getName(),"scaleHistograms()"))
    {
    cout << " Completed." << endl;
    }
}

