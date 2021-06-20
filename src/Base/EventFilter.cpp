//
//  EventFilter.cpp
//  MyMC
//
//  Created by Claude Pruneau on 12/12/16.
//  Copyright © 2016 Claude Pruneau. All rights reserved.
//

#include "EventFilter.hpp"

ClassImp(EventFilter);

//////////////////////////////////////////////////////////////////////////////////////////
// CTOR
//////////////////////////////////////////////////////////////////////////////////////////
EventFilter::EventFilter(EventSelection selection,
                         double min,
                         double max,
                         const TString& _userName)
:
eventSelectionRequested(selection),
minimum(min),
maximum(max),
userName(_userName)
{
  // no ops
}

//////////////////////////////////////////////////////////////////////////////////////////
// DTOR
//////////////////////////////////////////////////////////////////////////////////////////
EventFilter::~EventFilter()
{
  // no ops
}

//////////////////////////////////////////////////////////////////////////////////////////
// accept/reject the given particle based on filter parameter
//////////////////////////////////////////////////////////////////////////////////////////
bool EventFilter::accept(Event & event)
{
  if (eventSelectionRequested==MinBias) return true;
  double condition = 0.0;
  EventProperties & ep = * event.getEventProperties();
  switch (eventSelectionRequested)
    {
      case MinBias: break;
      case Centrality:      condition = ep.centrality; break;
      case Multiplicity:    condition = ep.multiplicity; break;
      case ImpactParameter: condition = ep.impactParameter; break;
      case ParticleFilter0:
      if (ep.nFiltered.size()>=1) condition = ep.nFiltered[0];
      else condition = -99999;
      break;
      case ParticleFilter1:
      if (ep.nFiltered.size()>=2) condition = ep.nFiltered[1];
      else condition = -99999;
      break;
      case ParticleFilter2:
      if (ep.nFiltered.size()>=3) condition = ep.nFiltered[2];
      else condition = -99999;
      break;
    }

  return (condition >= minimum) && (condition < maximum);
}

//enum EventSelection   { MinBias, Centrality, Multiplicity, ImpactParameter, Other };

TString EventFilter::getName()
{
  TString name;
  switch (eventSelectionRequested)
  {
    case MinBias:         name = "MB"; break;
    case Centrality:      name = Form("CGeq%gLt%g",minimum,maximum); break;
    case Multiplicity:    name = Form("MGeq%gLt%g",minimum,maximum); break;
    case ImpactParameter: name = Form("bGeq%gLt%g",minimum,maximum); break;
    case ParticleFilter0: name = Form("PF0nGeq%gLt%g",minimum,maximum); break;
    case ParticleFilter1: name = Form("PF1nGeq%gLt%g",minimum,maximum); ; break;
    case ParticleFilter2: name = Form("PF2nGeq%gLt%g",minimum,maximum); ; break;
  }
  return name;
}



TString EventFilter::getTitle()
{
  TString name;
  switch (eventSelectionRequested)
  {
    case MinBias:         name = "Min-Bias"; break;
    case Centrality:      name = Form("%g < C < %g",minimum,maximum); break;
    case Multiplicity:    name = Form("%g < M < %g",minimum,maximum); break;
    case ImpactParameter: name = Form("%g < b < %g",minimum,maximum); break;
    case ParticleFilter0: name = Form("%g < PF0n < %g",minimum,maximum); break;
    case ParticleFilter1: name = Form("%g < PF0n < %g",minimum,maximum); ; break;
    case ParticleFilter2: name = Form("%g < PF0n < %g",minimum,maximum); ; break;

  }
  return name;
}

vector<EventFilter*> EventFilter::createEventFilterSet(EventSelection  evtSelection, vector<double> limits)
{
  vector<EventFilter*> filters;
  unsigned int n = limits.size() - 1;
  for (unsigned int k=0; k<n; k++)
    {
    filters.push_back( new EventFilter(evtSelection, limits[k], limits[k+1]) );
    }
  return filters;
}
