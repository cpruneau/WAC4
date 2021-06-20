// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
#ifndef WAC_EventVertexRandomizerConfiguration
#define WAC_EventVertexRandomizerConfiguration
#include "TaskConfiguration.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Configuration of a given analysis
////////////////////////////////////////////////////////////////////////////////////////////////////////
class EventVertexRandomizerConfiguration : public TaskConfiguration
{
public:
  
  EventVertexRandomizerConfiguration();
  EventVertexRandomizerConfiguration(const EventVertexRandomizerConfiguration & source);
  virtual ~EventVertexRandomizerConfiguration(){}
  EventVertexRandomizerConfiguration & operator=(const EventVertexRandomizerConfiguration & source);
  virtual void printConfiguration(ostream & os);

  double rConversion;
  double tConversion;
  double xAvg, xRms;
  double yAvg, yRms;
  double zAvg, zRms;
  double tAvg, tRms;

  ClassDef(EventVertexRandomizerConfiguration,0)
};

#endif /* WAC_EventVertexRandomizerConfiguration */
