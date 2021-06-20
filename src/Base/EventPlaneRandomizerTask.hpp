// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
#ifndef WAC_EventPlaneRandomizerTask
#define WAC_EventPlaneRandomizerTask
#include "Task.hpp"

class EventPlaneRandomizerTask : public Task
{
public:


  EventPlaneRandomizerTask();
  virtual ~EventPlaneRandomizerTask() {}
  virtual void execute();

  ClassDef(EventPlaneRandomizerTask,0)
};

#endif /* WAC_EventPlaneRandomizerTask */
