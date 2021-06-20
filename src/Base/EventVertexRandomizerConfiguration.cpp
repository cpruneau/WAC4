// Author: Claude Pruneau   09/25/2019

/*************************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 *************************************************************************/
/**
 \class Property
 \ingroup WAC

 Utility class used to defineEventVertexRandomizerConfiguration
 */

#include "EventVertexRandomizerConfiguration.hpp"
#include "TMath.h"

ClassImp(EventVertexRandomizerConfiguration);

EventVertexRandomizerConfiguration::EventVertexRandomizerConfiguration()
:
TaskConfiguration(),
 rConversion(1.0E9),  // micrometer to femtometer
 tConversion(3.0E14), // nanosecond to fm/c
 xAvg(0.0), xRms(10.0),
 yAvg(0.0), yRms(10.0),
 zAvg(0.0), zRms(10.0),
 tAvg(0.0), tRms(10.0)
{
//  no ops
}

EventVertexRandomizerConfiguration::EventVertexRandomizerConfiguration(const EventVertexRandomizerConfiguration & source)
:
TaskConfiguration( source ),
rConversion(source.rConversion),
tConversion(source.tConversion),
xAvg(source.xAvg), xRms(source.xRms),
yAvg(source.yAvg), yRms(source.yRms),
zAvg(source.zAvg), zRms(source.zRms),
tAvg(source.tAvg), tRms(source.tRms)
{
}

EventVertexRandomizerConfiguration & EventVertexRandomizerConfiguration::operator=(const EventVertexRandomizerConfiguration & source)
{
  if (this!=&source)
    {
    TaskConfiguration::operator=( source );
    rConversion = source.rConversion;
    tConversion = source.tConversion;
    xAvg = source.xAvg; xRms = source.xRms;
    yAvg = source.yAvg; yRms = source.yRms;
    zAvg = source.zAvg; zRms = source.zRms;
    tAvg = source.tAvg; tRms = source.tRms;
    }
  return *this;
}

void EventVertexRandomizerConfiguration::printConfiguration(ostream & os)
{
  TaskConfiguration::printConfiguration(os);
  os
  << "    Analysis   Parameters: " << endl
  << " ------------------------------------------------------------------------------------------" << endl
  << "  rConversion = " << rConversion    << endl
  << "  tConversion = " << tConversion    << endl
  << "         xAvg = " << xAvg    << endl
  << "         xRms = " << xRms    << endl
  << "         yAvg = " << yAvg    << endl
  << "         yRms = " << yRms    << endl
  << "         zAvg = " << zAvg    << endl
  << "         zRms = " << zRms    << endl
  << "         tAvg = " << tAvg    << endl
  << "         tRms = " << tRms    << endl;
}
