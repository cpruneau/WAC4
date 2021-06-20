// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
/**
 \class Task
 \ingroup WAC

 Class defining Task
 */

#include "TaskIterator.hpp"
ClassImp(TaskIterator);

TaskIterator::TaskIterator()
:
Task(),
timer(),
nEventRequested(1),
nEventReported(1),
nEventPartialSave(10000),
nEventProcessed(0),
partialSave(0),
subsampleAnalysis(0)
{
  setName("TaskIterator");
}

void TaskIterator::run()
{
  run(nEventRequested,nEventReported);
}

void TaskIterator::run(long nEvent, long nReport)
{
  timer.start();

  if (reportInfo("TaskIterator",getName(),"run(...)")) cout << "Running for nEvent: " << nEvent << endl;
  postTaskOk();
  initializeTasks();
  if (!isTaskOk())
    {
    if (reportWarning("TaskIterator",getName(),"run(...)")) cout << "Initialization failed. Abort." << endl;
    return;
    }
  nEventProcessed = 0;
  if (reportInfo("TaskIterator",getName(),"run(...)")) cout << "Starting with nEventRequested:" << nEventRequested << endl;
  for (long iEvent=1; iEvent<=nEvent; iEvent++)
    {
    executeTasks();
    if (!isTaskOk())
      {
      if (reportWarning("TaskIterator",getName(),"run(...)")) cout << "Error detected after event:" << iEvent << endl;
      break;
      }
    nEventProcessed++;
    if (nEventProcessed%nReport==0 )
      {
      if (reportInfo("TaskIterator",getName(),"run(...)")) cout << "Completed event # " << iEvent << endl;
      }
    if ( (subsampleAnalysis||partialSave) && nEventProcessed%nEventPartialSave==0)
      {
      savePartialTasks();
      if (subsampleAnalysis) resetTasks();
      if (!isTaskOk()) continue;
      }
    }
  if (isTaskOk()) finalizeTasks();

  if (isTaskOk() && subsampleAnalysis)
    {
    subsampleAnalysisTasks();
    }

  timer.stop();
  if (reportInfo("TaskIterator",getName(),"run(...)"))
    {
    cout << endl;
    cout << "  Completed with status : " << getTaskStatusName() << endl;
    cout << "       Completed Events : " << nEventProcessed << endl;
    cout << "       Requested Events : " << nEvent << endl;
    cout << "            "; timer.print(cout);
    cout << endl << endl<< endl << endl;
    }
}
