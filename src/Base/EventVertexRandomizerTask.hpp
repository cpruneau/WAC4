// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
#ifndef WAC_EventVertexRandomizerTask
#define WAC_EventVertexRandomizerTask
#include "Task.hpp"
#include "EventVertexRandomizerConfiguration.hpp"

class EventVertexRandomizerTask : public Task
{
public:


  EventVertexRandomizerTask(EventVertexRandomizerConfiguration * configuration);
  virtual ~EventVertexRandomizerTask() {}
  virtual void execute();

  ClassDef(EventVertexRandomizerTask,0)
};

#endif /* WAC_EventVertexRandomizerTask */
