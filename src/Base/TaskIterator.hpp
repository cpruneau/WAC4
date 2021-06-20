// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/

#ifndef WAC_TaskIterator
#define WAC_TaskIterator
#include "Task.hpp"
#include "Timer.hpp"

class TaskIterator : public Task
{
public:
  
  TaskIterator();
  virtual ~TaskIterator() {}
  virtual void run();
  virtual void run(long nEvent, long nReport);
  inline void setNEventRequested(long _nEventsRequested)    { nEventRequested    = _nEventsRequested; }
  inline void setNEventReported(long _nEventsReported)      { nEventReported     = _nEventsReported; }
  inline void setNEventPartialSave(long _nEventsPartialSave){ nEventPartialSave  = _nEventsPartialSave; }
  inline void setPartialSave(bool _partialSave)              { partialSave         = _partialSave; }
  inline void setSubsampleAnalysis(bool _subsampleAnalysis)  { subsampleAnalysis   = _subsampleAnalysis; }

  inline long getNEventRequested() const   { return nEventRequested; }
  inline long getNEventReported() const    { return nEventReported; }
  inline long getNEventPartialSave() const { return nEventPartialSave; }
  inline bool getPartialSave() const       { return partialSave; }
  inline bool getSubsampleAnalysis() const { return subsampleAnalysis; }

protected:

  Timer timer;
  long nEventRequested;
  long nEventReported;
  long nEventPartialSave;
  long nEventProcessed;
  bool partialSave;
  bool subsampleAnalysis;

  ClassDef(TaskIterator,0)
};

#endif /* WAC_TaskIterator */
