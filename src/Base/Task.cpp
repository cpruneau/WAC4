// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2021, Claude Pruneau.
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
#include "TParameter.h"
#include "Task.hpp"
#include "SubSampleStatCalculator.hpp"

ClassImp(Task);

Task::Task()
:
MessageLogger(Info),
taskName                ( "Task" ),
taskConfiguration       ( TaskConfiguration::getDefaultConfiguration() ),
taskRandomGenerator     ( gRandom),
particleFactory         ( nullptr),
eventStreams            (),
eventFilters            (),
particleFilters         (),
particleSmearers        (),
eventCountHistos        ( nullptr),
inputHistograms         (),
histograms              (),
derivedHistograms       (),
combinedHistograms      (),
nFilteredEventsAccepted (),
nEventProcessed         ( 0 ),
nEventAccepted          ( 0 ),
subSampleIndex          ( 0 ),
subtasks                (),
masterCollection        ( nullptr)
{
  // no ops
}

void Task::initialize()
{
  if (isTaskOk() && taskConfiguration->useParticles)
    {
    particleFactory  = Particle::getFactory();
    if (!particleFactory)
      {
      if (reportFatal("Task",getName(),"initialize()"))
        {
        cout << " Particle factory required but not available. Must abort." << endl;
        }
      postTaskFatal();
      return;
      }
    masterCollection = ParticleTypeCollection::getMasterParticleCollection();
    if (!masterCollection)
      {
      if (reportFatal("Task",getName(),"initialize()"))
        {
        cout << " Particle master collection required but not available. Must abort." << endl;
        }
      postTaskFatal();
      return;
      } 
    }
  if (isTaskOk() && taskConfiguration->useEventStream0)  eventStreams.push_back(Event::getEventStream(0));
  if (isTaskOk() && taskConfiguration->useEventStream1)  eventStreams.push_back(Event::getEventStream(1));
  if (isTaskOk() && taskConfiguration->useEventStream2)  eventStreams.push_back(Event::getEventStream(2));
  if (isTaskOk() && taskConfiguration->useEventStream3)  eventStreams.push_back(Event::getEventStream(3));
  if (isTaskOk() && taskConfiguration->loadHistograms)   loadHistograms();
  if (isTaskOk() && taskConfiguration->createHistograms)
    {
    if (reportInfo("Task",getName(),"initialize()")) cout << "eventCountHistos" << endl;
    eventCountHistos = new EventCountHistos(getName(),getReportLevel());
    if (reportInfo("Task",getName(),"initialize()")) cout << "eventCountHistos" << endl;
    eventCountHistos->createHistograms();
    if (reportInfo("Task",getName(),"initialize()")) cout << "eventCountHistos" << endl;
    createHistograms();
    if (reportInfo("Task",getName(),"initialize()")) cout << "eventCountHistos" << endl;
    }


  nEventProcessed = 0;
  nEventAccepted  = 0;
}


void Task::finalize()
{
  if (nEventAccepted<1 || nEventProcessed<1)
    {
    if (reportWarning("Task",getName(),"finalize()")) printEventCount();
    }
  if (reportInfo("Task",getName(),"finalize()")) printEventCount();
  if (isTaskOk() )
    {
    if (eventCountHistos) eventCountHistos->fill(nFilteredEventsAccepted);
    completeFill();
    }
  if (isTaskOk() && taskConfiguration->scaleHistograms)            scaleHistograms();
  if (isTaskOk() && taskConfiguration->calculateDerivedHistograms) calculateDerivedHistograms();
  if (isTaskOk() && taskConfiguration->saveHistograms)             saveHistograms();
}

void Task::printEventCount() const
{
  cout << endl
  << "    nEventProcessed:" << nEventProcessed << endl
  << "     nEventAccepted:" << nEventAccepted << endl;
  for (unsigned int k=0; k<nFilteredEventsAccepted.size(); k++)
    {
    cout << "      Event filter:" << k << "  nFiltered Events Accepted: " << nFilteredEventsAccepted[k] << endl;
    }
}

void Task::savePartial()
{
  if (nEventAccepted<1 || nEventProcessed<1)
    {
    if (reportWarning("Task",getName(),"savePartial()")) printEventCount();
    }
  if (reportInfo("Task",getName(),"savePartial()")) printEventCount();
  if (isTaskOk() )
    {
    if (eventCountHistos) eventCountHistos->fill(nFilteredEventsAccepted);
    completeFill();
    }
  if (isTaskOk() && taskConfiguration->scaleHistograms)            scaleHistograms();
  if (isTaskOk() && taskConfiguration->calculateDerivedHistograms) calculateDerivedHistograms();
  if (isTaskOk() && taskConfiguration->saveHistograms)             saveHistograms();
}

void Task::subsampleAnalysis()
{
  if (!taskConfiguration->subsampleAnalysis)
    {
    if (reportInfo("Task",getName(),"subsampleAnalysis(...)"))
      {
      cout << "No subsample analysis requested for task:" << getName() << endl;
      }
    return;
    }

  TString inputPathName    = taskConfiguration->outputPath;        // where files were saved
  TString inputFileName    = taskConfiguration->rootOuputFileName; // base name of files in a job
  inputFileName            += getName();                           // name of this task instance
  int numberOfPartialSaves = getSubSampleIndex();                  // number of partial saves
  TString outputPathName   = inputPathName;                        // where sum file will be saved
  TString outputFileName   = inputFileName;                        // base name of sum file
  outputFileName           += "_Sum";

  if (reportInfo("Task",getName(),"subsampleAnalysis(...)"))
    {
    cout << "Subsample analysis requested for task:" << getName() << endl;
    cout << "                        inputPathName:" << inputPathName << endl;
    cout << "                        inputFileName:" << inputFileName << endl;
    cout << "                 numberOfPartialSaves:" << numberOfPartialSaves << endl;
    cout << "                       outputPathName:" << outputPathName << endl;
    cout << "                       outputFileName:" << outputFileName << endl;
    }
  SubSampleStatCalculator * subSampleStatCalculator = new SubSampleStatCalculator(inputPathName,inputFileName,numberOfPartialSaves,
                                                                                  outputPathName,outputFileName,getReportLevel());
  subSampleStatCalculator->execute();
  delete subSampleStatCalculator;
  if (reportEnd("Task",getName(),"subsampleAnalysis(...)"))
    ;
}

void Task::reset()
{
  nEventProcessed = 0;
  nEventAccepted  = 0;
  for (unsigned int k=0; k<nFilteredEventsAccepted.size(); k++) nFilteredEventsAccepted[k] = 0;
  if (isTaskOk())   resetHistograms();
 }

void Task::clear()
{
  reset();
}


void Task::setConfiguration(TaskConfiguration * config)
{
  //cout << "Task::setConfiguration() ptr:" << config << endl;
  if (config!=nullptr)
    {
    taskConfiguration = config;
    if (reportInfo("Task",getName(),"setConfiguration(...)"))
      {
      cout << "Task configuration was changed." << endl;
      }
    }
  else
    {
    if (reportError("Task",getName(),"setConfiguration(...)"))
      {
      cout << "Attempting to set task configuration to a null pointer."  << endl;
      }
    postTaskError();
    }
}

void Task::printConfiguration(ostream & output)
{
  output << "Task Name : " << taskName <<  endl;
  taskConfiguration->printConfiguration(output);
}

void Task::setRandomGenerator(TRandom * randomGenerator)
{
  if (!randomGenerator)
    {
    taskRandomGenerator = randomGenerator;
    if (reportInfo("Task",getName(),"setRandomGenerator(TRandom * randomGenerator)"))
      {
      cout << "Default random generator was changed." << endl;
      }
    }
  else
    {
    postTaskWarning();
    if (reportError("Task",getName(),"setConfiguration(...)"))
      {
      cout << "Null pointer. Random generator will not be set." << endl;
      }
    }
}

void Task::loadHistograms()
{
  if (reportStart("Task",getName(),"loadHistograms()"))
    ;
  TFile * inputFile;
  TString inputFileName = taskConfiguration->getInputRootFileName();
  if (reportInfo("Task",getName(),"loadHistograms()")) cout << "Opening input file: " << inputFileName << endl;
  inputFile = new TFile(inputFileName,"OLD");
  if (!inputFile)
    {
    if (reportError("Task",getName(),"loadHistograms()")) cout << "Could not open input file:" << inputFileName << endl;
    postTaskError();
    return;
    }
  eventCountHistos = new EventCountHistos(getName(),getReportLevel());
  eventCountHistos->loadHistograms(inputFile);
  loadHistograms(inputFile);
  if (reportEnd("Task",getName(),"loadHistograms()"))
    ;
}

void Task::resetHistograms()
{
  eventCountHistos->reset();
  for (unsigned int iHisto=0; iHisto<histograms.size(); iHisto++) histograms[iHisto]->reset();
  for (unsigned int iHisto=0; iHisto<histograms.size(); iHisto++) histograms[iHisto]->reset();
}

void Task::clearHistograms()
{
  eventCountHistos->reset();
  for (unsigned int iHisto=0; iHisto<histograms.size(); iHisto++) histograms[iHisto]->reset();
  for (unsigned int iHisto=0; iHisto<derivedHistograms.size(); iHisto++) derivedHistograms[iHisto]->reset();
}

void Task::completeFill()
{
  if (reportInfo("Task",getName(),"completeFill()"))
    {
    cout << "Accepted number of events: " <<  nEventAccepted << endl;
    }
  for (unsigned int k=0; k<histograms.size(); k++ )
    {
      histograms[k]->completeFill();
    }
  if (reportInfo("Task",getName(),"completeFill()"))
    {
    cout << " Completed." << endl;
    }
}

void Task::scaleHistograms()
{
  unsigned int nEventFilters    = eventFilters.size();
  unsigned int nParticleFilters = particleFilters.size();
  if (reportInfo("Task",getName(),"scaleHistograms()"))
    {
    cout << "Scaling histograms" << endl << "Accumulated statistics:" << endl;
    printEventCount();
    }
  for (unsigned int iEventFilter=0; iEventFilter<nEventFilters; iEventFilter++ )
    {
    if (nFilteredEventsAccepted[iEventFilter]<1)
      {
      if (reportWarning("Task",getName(),"scaleHistograms()"))
        cout << "No events accumulated for EventFilter " << iEventFilter << " -- No scaling will be performed for this set." << endl;
      }
    else
      {
      double factor = 1.0/double(nFilteredEventsAccepted[iEventFilter]);
      for (unsigned int iParticleFilter=0; iParticleFilter<nParticleFilters; iParticleFilter++ )
        {
        histograms[iParticleFilter+iEventFilter*nParticleFilters]->scale(factor);
        }
      }
    }
  if (reportInfo("Task",getName(),"scaleHistograms()"))
    {
    cout << "Completed." << endl;
    }
}


void Task::saveHistogramsAsText()
{
  if (reportNoOps("Task",getName(),"saveHistogramsAsText()"))
    ;
}

void Task::saveHistograms()
{
  if (reportStart("Task",getName(),"saveHistograms()"))
    ;
  TFile * outputFile;
  TString outputFileName = taskConfiguration->outputPath;
  outputFileName += taskConfiguration->rootOuputFileName;
  outputFileName += getName();
  if (taskConfiguration->subsampleAnalysis || taskConfiguration->partialSave)
    {
    outputFileName += "_";
    outputFileName += subSampleIndex++;
    }
  outputFileName += ".root";

  if (taskConfiguration->forceHistogramsRewrite)
    {
    if (reportInfo("Task",getName(),"saveHistograms()")) cout << "Opening output (RECREATE) file  " << outputFileName << endl;
    outputFile = new TFile(outputFileName,"RECREATE"); // obliterate past work...
    if (!outputFile)
      {
      if (reportError("Task",getName(),"saveHistograms()")) cout << "Could not open (RECREATE) file  " << outputFileName << endl;
      postTaskWarning();
      return;
      }
    }
  else
    {
    if (reportInfo("Task",getName(),"saveHistograms()")) cout << "Opening  output (NEW) file  " << outputFileName << endl;
    outputFile = new TFile(outputFileName,"NEW"); // protect past work...
    if (!outputFile)
      {
      if (reportError("Task",getName(),"saveHistograms()")) cout << "Could not open (NEW) file  " << outputFileName << endl;
      postTaskWarning();
      return;
      }
    }
  saveNEventProcessed(outputFile);
  saveNEventAccepted(outputFile);
  if (eventCountHistos) eventCountHistos->saveHistograms(outputFile);
  else
  {
  if (reportWarning("Task",getName(),"saveHistograms()")) cout << "eventCountHistos is not defined and thus not saved"<< endl;
  }
  for (unsigned int iHisto=0; iHisto<histograms.size(); iHisto++)
    {
    histograms[iHisto]->saveHistograms(outputFile);
    }
  for (unsigned int iHisto=0; iHisto<derivedHistograms.size(); iHisto++)
    {
    derivedHistograms[iHisto]->saveHistograms(outputFile);
    }
  for (unsigned int iHisto=0; iHisto<combinedHistograms.size(); iHisto++)
    {
    combinedHistograms[iHisto]->saveHistograms(outputFile);
    }
  outputFile->Close();
  if (reportEnd("Task",getName(),"saveHistograms()"))
    ;
}

void Task::saveNEventProcessed(TFile * outputFile)
{
  outputFile->cd();
  TParameter<Long64_t>("EventProcessed",nEventProcessed,'+').Write();
}

void Task::saveNEventAccepted(TFile * outputFile)
{
  outputFile->cd();
  TParameter<Long64_t>("EventAccepted",nEventAccepted,'+').Write();
}

void Task::saveNFilteredEventsAccepted(TFile * outputFile, vector<unsigned int> & nFilteredEventsAccepted)
{
  outputFile->cd();
  for (unsigned int k=0; k<nFilteredEventsAccepted.size(); k++)
    {
    TString name = "nFilteredEventsAccepted_"; name += k;
    TParameter<Long64_t>(name,nFilteredEventsAccepted[k],'+').Write();
    }
}


long Task::loadNEventProcessed(TFile * inputFile)
{
  TParameter<Long64_t> *par = (TParameter<Long64_t> *) inputFile->Get("nEventProcessed");
  nEventProcessed = par->GetVal();
  delete par;
  return nEventProcessed;
}
long Task::loadNEventAccepted(TFile * inputFile)
{
  TParameter<Long64_t> *par = (TParameter<Long64_t> *) inputFile->Get("nEventAccepted");
  nEventAccepted = par->GetVal();
  delete par;
  return nEventAccepted;
}

void Task::loadNFilteredEventsAccepted(TFile * inputFile, vector<unsigned int> & nFilteredEventsAccepted)
{
  for (unsigned int k=0; k<nFilteredEventsAccepted.size(); k++)
    {
    TString name = "nFilteredEventsAccepted_"; name += k;
    TParameter<Long64_t> *par = (TParameter<Long64_t> *) inputFile->Get(name);
    nFilteredEventsAccepted[k] = par->GetVal();
    delete par;
    }
}

void Task::calculateDerivedHistograms()
{
  if (reportStart("Task",getName(),"calculateDerivedHistograms()"))
    ;
  if (derivedHistograms.size()==0)
    {
    // Derived histograms are in the same class as the basic histograms
    for (unsigned int iHisto=0; iHisto<histograms.size(); iHisto++)
      {
      histograms[iHisto]->calculateDerivedHistograms();
      }
    }
  else
    {
    ;
    // Derived histograms are in a distinct class
    //    for (unsigned int iHisto=0; iHisto<histograms.size(); iHisto++)
    //      {
    //      derivedHistograms[iHisto]->calculateDerivedHistograms(histograms[iHisto]);
    //      }
    }
  if (reportEnd("Task",getName(),"calculateDerivedHistograms()"))
    ;
}


Task::TaskStatus Task::taskStatus = Task::TaskOk;

TString Task::getTaskStatusName()
{
  TString statusName;
  switch (taskStatus)
    {
      case Unknown:     statusName = "Unknown"; break;
      case TaskOk:      statusName = "TaskOk";  break;
      case TaskEof:     statusName = "TaskEof"; break;
      case TaskEod:     statusName = "TaskEod"; break;
      case TaskWarning: statusName = "TaskWarning"; break;
      case TaskError:   statusName = "TaskError";   break;
      case TaskFatal:   statusName = "TaskFatal";   break;
    }
  return statusName;
}


double Task::readParameter(TFile * inputFile, const TString & parameterName)
{
  if (reportDebug ("Task",getName(),"readParameter(...)")) cout << "Reading parameter: " << parameterName << endl;
  TParameter<Long64_t> *par = (TParameter<Long64_t> *) inputFile->Get(parameterName);
  if (!par)
    {
    if (reportError("Task",getName(),"readParameter()")) cout << "Parameter not found." << endl;
    postTaskError();
    return -1;
    }
  double value = par->GetVal();
  delete par;
  if (reportDebug ("Task",getName(),"readParameter(...)")) cout << "Parameter value : " << value << endl;
  return value;
}

TFile *  Task::openRootFile(const TString & inputPath, const TString & fileName, const TString & ioOption)
{
  TString inputFileName = inputPath;
  inputFileName += "/";
  inputFileName += fileName;
  inputFileName += ".root";
  if (reportDebug ("Task",getName(),"openRootFile(...)"))
    cout << "Opening file: " << inputFileName << " with option: " << ioOption << endl;
  TFile * inputFile = new TFile(inputFileName,ioOption);
  if (!inputFile)
    {
    if (reportError("Task",getName(),"openRootFile()")) cout << "file: " << inputFileName << " not found." << endl;
    postTaskError();
    return nullptr;
    }
  else
    {
    if (reportDebug ("Task",getName(),"openRootFile()")) cout << "file opened successfully." << endl;
    }
  return inputFile;
}


void Task::initializeTasks()
{
  if (!isTaskOk()) return;
  initialize();
  unsigned int nSubtasks = subtasks.size();
  if (nSubtasks>0)
    for (unsigned int  iTask=0; iTask<nSubtasks; iTask++) { if (isTaskOk())  subtasks[iTask]->initializeTasks(); }
}

void Task::executeTasks()
{
  if (!isTaskOk()) return;
  execute();
  unsigned int nSubtasks = subtasks.size();
  if (nSubtasks>0)
    for (unsigned int  iTask=0; iTask<nSubtasks; iTask++)  { if (isTaskOk()) subtasks[iTask]->executeTasks(); }
}

void Task::finalizeTasks()
{
  if (!isTaskOk()) return;
  finalize();
  unsigned int nSubtasks = subtasks.size();
  if (nSubtasks>0)
    for (unsigned int  iTask=0; iTask<nSubtasks; iTask++)  { if (isTaskOk()) subtasks[iTask]->finalizeTasks(); }
}

void Task::resetTasks()
{
  if (!isTaskOk()) return;
  reset();
  unsigned int nSubtasks = subtasks.size();
  if (nSubtasks>0)
    for (unsigned int  iTask=0; iTask<nSubtasks; iTask++) if (isTaskOk()) subtasks[iTask]->resetTasks();
}

void Task::clearTasks()
{
  if (!isTaskOk()) return;
  clear();
  unsigned int nSubtasks = subtasks.size();
  if (nSubtasks>0)
    for (unsigned int  iTask=0; iTask<nSubtasks; iTask++) if (isTaskOk()) subtasks[iTask]->clearTasks();
}


void Task::savePartialTasks()
{
  if (!isTaskOk()) return;
  savePartial();
  unsigned int nSubtasks = subtasks.size();
  if (nSubtasks>0)
    for (unsigned int  iTask=0; iTask<nSubtasks; iTask++) if (isTaskOk()) subtasks[iTask]->savePartialTasks();
}

void Task::subsampleAnalysisTasks()
{
  if (!isTaskOk()) return;
  subsampleAnalysis();
  unsigned int nSubtasks = subtasks.size();
  if (nSubtasks>0)
    for (unsigned int  iTask=0; iTask<nSubtasks; iTask++) if (isTaskOk()) subtasks[iTask]->subsampleAnalysis();
}


void Task::printConfigurationTasks(ostream & output)
{
  if (!isTaskOk()) return;
  printConfiguration(output);
  unsigned int nSubtasks = subtasks.size();
  if (nSubtasks>0)
    for (unsigned int  iTask=0; iTask<nSubtasks; iTask++) subtasks[iTask]->printConfigurationTasks(output);
}


Task *  Task::addSubtask(Task * task)
{
  if (!task)
    {
    if (reportFatal("Task",getName(),"addSubtask(Task * task)")) cout << "Given task pointer is null. Abort." << endl;
    postTaskFatal();
    return task;
    }
  subtasks.push_back( task );
  if (reportInfo("Task",getName(),"addSubtask(Task * task)")) cout << "Added task " << task->getName() << " to task " << getName() << endl;
  return task;
}

void Task::addHistogramsToExtList(TList *list, bool all)
{
  if (reportInfo("Task",getName(),"addHistogramsToExtList(...)"))
    ;
  all = false; // silence warning.
  if (!list)
    {
    if (reportError("Task",getName(),"addHistogramsToExtList(...)"))
      {
      cout << "Given list pointer is null." << endl;
      }
    postTaskWarning();
    return;
    }
  list->Add(new TParameter<Long64_t>("nEventProcessed",getNEventProcessed(),'+'));

  for (unsigned int k=0; eventFilters.size(); k++)
    {
    TString name = "nFilteredEventsAccepted"; name += k;
    list->Add(new TParameter<Long64_t>(name,nFilteredEventsAccepted[k],'+'));
    }
  for (unsigned int k=1; k<histograms.size(); k++)
    {
    histograms[k]->addHistogramsToExtList(list, all);
    }
  for (unsigned int k=1; k<histograms.size(); k++)
    {
    derivedHistograms[k]->addHistogramsToExtList(list, all);
    }
}
