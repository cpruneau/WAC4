// ======================================================================
// Author: Claude Pruneau, Victor Gonzalez, Sumit Basu
// Copyright (C) 2019-2021, Claude Pruneau.
// All rights reserved.
// Based on the ROOT package and environment
// For the licensing terms see LICENSE.
// ======================================================================

#ifndef WAC_Task
#define WAC_Task
#include <iostream>
#include "TClass.h"
#include "TH1D.h"
#include "TH1F.h"
#include "TH2D.h"
#include "TH2F.h"
#include "TH3D.h"
#include "TH3F.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TFile.h"
#include "TList.h"
#include "TAxis.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TString.h"
#include "TMath.h"
#include "TPad.h"
#include "TF1.h"
#include "TF2.h"
#include "TRandom.h"
#include "TString.h"
#include "TLatex.h"
#include "TLine.h"
#include "TArrow.h"
#include "TLegend.h"
#include "TFile.h"
#include "TaskConfiguration.hpp"
#include "MessageLogger.hpp"
#include "Event.hpp"
#include "EventFilter.hpp"
#include "ParticleFilter.hpp"
#include "ParticleSmearer.hpp"
#include "Histograms.hpp"
#include "ParticleTypeCollection.hpp"
#include "EventCountHistos.hpp"

using namespace std;

//!
//! Class defining generic tasks.
//! blabla bla vaa
//! blabla bla vaa
//! blabla bla vaa
//! blabla bla vaa
//! blabla bla vaa
//! blabla bla vaa
//! blabla bla vaa
class Task : public MessageLogger
{
public:
  enum TaskStatus   { Unknown, TaskOk, TaskEof, TaskEod, TaskWarning, TaskError, TaskFatal};

protected:

  //!
  //! Name given to this task instance
  //!
  TString                  taskName;
  //!
  //! Pointer to a TaskConfiguation class which defines the behavior of this instance of the class Task and/or its derived class instances.
  //!
  TaskConfiguration *      taskConfiguration;
  //!
  //! Pointer to a random number generator of the class TRandom. This variable is set to "gRandom" by default and thus does not need
  //!   to be explictly by the user/developer.
  //!
  TRandom *                taskRandomGenerator;
  //!
  //! Pointer to a factory of entities of type Particle.
  //!
  Factory<Particle> *      particleFactory;
  //!
  //! Array of pointers to streams (potentially) used by this task.
  //!
  vector<Event*>           eventStreams;
  //!
  //! Array of pointers to event filters (class EventFilter) used by this task instance.
  //!
  vector<EventFilter*>     eventFilters;
  //!
  //! Array of pointers to particle filters (class ParticleFilter)  used by this task.
  //!
  vector<ParticleFilter*>  particleFilters;
  //!
  //! Array of pointers to particle smearers (class ParticleSmearer)  used by this task.
  //!
  vector<ParticleSmearer*> particleSmearers;
  //!
  //! Pointer to an object of the class EventCountHistos. An instance of the EventCountHistos is used to keep track of the number of events
  //!  accepted by this task for each of its event filters.
  //!
  EventCountHistos *       eventCountHistos;
  //!
  //! Array of pointers to Histograms objects used on input for initialization or calibration of the activity of this task instance.
  //!
  vector<Histograms*>      inputHistograms;
  //!
  //! Array of pointers to Histograms objects produced on output by this task instance.
  //!
  vector<Histograms*>      histograms;
  //!
  //! Array of pointers to Histograms objects produced on output by this task instance as "derivatives" of those contained in the array "histograms".
  //!
  vector<Histograms*>      derivedHistograms;
  //!
  //! Array of pointers to Histograms objects produced on output by this task instance as "combinations" of other histogram groups.
  //!
  vector<Histograms*>      combinedHistograms;
  //!
  //! Array of  number of events accepted by each of the event filters used by this task instance.
  //!
  vector<double>           nFilteredEventsAccepted;
  //!
  //! Number of events processed by this task instance.
  //!
  long nEventProcessed;
  //!
  //! Number of events accepted for analysis by this task instance. Note that this number is actually the sum of the number of events accepted
  //! by the event filters used by this task.
  //!
  long nEventAccepted;

  //!
  //! Number of particles counted by this task instance in a given event.
  //!
  unsigned int particlesCounted;    // read or generated multiplicity (before filter cut)

  //!
  //! Number of particles accepted by particle filters associated with this task instance in a given event.
  //!
  unsigned int particlesAccepted;   // accepted multiplicity by one or many particle filters.




  //!
  //! Index to be tagged to the name of histogram files produced by this task on output. These tags are used to keep track of the sub-samples of events used
  //! in a particular data analysis. The files can then be read in by SubSampleStatCalculator task to compute estimate of the statitical errors of all histograms
  //! of all histogram group owned/managed by this task.
  //!
  int  subSampleIndex;
  //!
  //! Array of pointers to subtasks called by this task instance, once per event analyzed (or iteration generated by TaskIterator task). If this instance carries out
  //! initialize, finalize, execute type operations, these are performed BEFORE the corresponding operations by the subtasks.
  //!
  vector<Task*> subtasks;
  //!
  //! Pointer to a singleton of the class ParticleTypeCollection possibly used by this task instance for particle type identification and classification purposes.
  //!
  ParticleTypeCollection * masterCollection;
  //!
  //! Status of the ongoing tasks. Since this is a static variable, it reflects a global state or status of an ongoing analysis. The task status may be set or read by
  //! all task instances.
  //!
  static TaskStatus taskStatus;
  
public:

  //!
  //! Default constructor. It allocates resources but DOES NOT initialize the task. Task initialization can be performed by a call to the initializationTask() and/or
  //!  initialization() methods.
  //!
  Task();
  virtual ~Task() {}
  //!
  //! Method called for the initialization of this task instance and  its subtasks. This method first calls the iniatilize() method implemented by this class or  one of its derived classes.
  //! It next calls the initializeTasks() method of each  of the  subtasks of this instance, if any.
  //!
  void initializeTasks();
  //!
  //! Method called to execute this task instance and  its subtasks. This method first calls the execute() method implemented by this class or  one of its derived classes.
  //! It next calls the executeTasks() method of each  of the  subtasks of this instance, if any.
  //!
  void executeTasks();
  //!
  //! Method called to finalize this task instance and  its subtasks. This method first calls the finalize() method implemented by this class or  one of its derived classes.
  //! It next calls the finalizeTasks() method of each  of the  subtasks of this instance, if any.
  //!
  void finalizeTasks();
  //!
  //! Method called to rest this task instance and its subtasks. This method first calls the reset() method implemented by this class or  one of its derived classes.
  //! It next calls the resetTasks() method of each  of the  subtasks of this instance, if any. See the class documentation for the definition of task "reset".
  //!
  void resetTasks();
  //!
  //! Method called to clear  this task instance and its subtasks. This method first calls the clear() method implemented by this class or  one of its derived classes.
  //! It next calls the resetTasks() method of each  of the  subtasks of this instance, if any. See the class documentation for the definition of task "clear".
  //!
  void clearTasks();

  //!
  //! Method called to save the histogram objects handled  this task instance and its subtasks. This method first calls the savePartial() method implemented by this class or  one of its derived classes.
  //! It next calls the savePartialTasks() method of each  of the  subtasks of this instance, if any. See the class documentation to figure out how to generate partial saves.
  //!
  void savePartialTasks();


  //!
  //! Carry out a sub-sample analysis to compute the statistical uncertainties of the histograms of this task and its subtasks.
  //!
  void subsampleAnalysisTasks();


  //!
  //! Method called to print  the configuration of  this task instance and its subtasks. This method first calls the printConfiguration() method implemented by this class or  one of its derived classes.
  //! It next calls the printConfigurationTasks() method of each  of the  subtasks of this instance, if any. The output is sent to the ostream identified by the argument "output".
  //!
  void printConfigurationTasks(ostream & output);
  //!
  //! Returns a boolean (true) is this task instance has subtasks.
  //!
  bool hasSubtasks()  const
  {
  return subtasks.size()>0;
  }
  //!
  //! Returns the number of subtasks held by this task instance.
  //!
  unsigned int getNSubtasks() const
  {
  return subtasks.size();
  }
  //!
  //! Returns a pointer to the subtask at the given index. The current implementation of the code does not checl whether the index might be larger
  //! than the actual number of subtasks held by this instance. As such, a call with a value of index in excess of the number of subtasks will yield
  //!  an exception (segmentation fault).
  //!
  Task * getSubtaskAt(unsigned int index)
  {
  return subtasks[index];
  }
  //!
  //! Adds the given taks a subtask of this task instance.
  //!
  Task *  addSubtask(Task * task);
  //!
  //! Initialize this task instance. Implement this method in a derived class if the functionality provided in this base class is insufficient.
  //!
  virtual void initialize();
  //!
  //! Execute this task instance. This base class implementation only increments the counters nEventProcessed and nEventAccepted. Implement this method in
  //! derived class to achieve the actual task of interest. Note: You can increase the counters nEventProcessed and nEventAccepted indiscrimately
  //! by a call to Task::execute(). However, in general, you may wish to count all events looked and only count as accepted those that passed one of the event
  //! filters used by this task. To achive this, use calls to the methods incrementEventProcessed() and incrementEventAccepted() rather than explicitly incrementing
  //! the variable nEventProcessed and nEventAccepted. These are currently protected  but may defined as private in a future release.
  //!
  virtual void execute()
  {
    ++nEventProcessed;
    ++nEventAccepted;
  }
  //!
  //! Finalize this task instance. Implement this method in a derived class if the functionality provided in this base class is insufficient.
  //!
  virtual void finalize();
  //!
  //! Reset this task instance. Implement this method in a derived class if the functionality provided in this base class is insufficient.
  //!
  virtual void reset();
  //!
  //! Clear this task instance. Implement this method in a derived class if the functionality provided in this base class is insufficient.
  //!
  virtual void clear();
  //!
  //! Save histogram groups owned by this task instance. Implement this method in a derived class if the functionality provided in this base class is insufficient.
  //!
  virtual void savePartial();
  //!
  //! Print event statistics on standard output.
  //!
  virtual void printEventCount() const;
  //!
  //! Print this class instance configuration. This is typically achieved by a clas to the printProperties of the task configuration  (TaskConfiguration) instance
  //! used to control the behavior of this class.
  //!
  virtual void printConfiguration(ostream & output);
  //!
  //! Nominally creates histogram groups used by this task. The method of this based class does not perform any histogram instantiation and must be
  //! overiden in a derived class to achieve the desited behavior.
  //!
  virtual void createHistograms() {}
  //!
  //! Loads histogram groups used by this task. This method opens the root file identified in the TaskConfiguration object owned by this task. If the file exists,
  //! it calls the  loadHistograms(TFile * inputFile) to actually load the relevant groups.
  //!
  virtual void loadHistograms();
  //!
  //! Implement this method in a derived class to load from a root file the histogram groups that are need for the execution of this task.
  //!
  virtual void loadHistograms(TFile * inputFile) { inputFile = nullptr;}
  //!
  //! Calls the reset method of all the histogram groups owned by this task.
  //!
  virtual void resetHistograms();
  //!
  //! Calls the clear method of all the histogram groups owned by this task. This effectively delete the histogram groups. Only call this method if you know
  //! what you are doing...
  //!
  virtual void clearHistograms();
  //!
  //! Method used to complete the filling of histogram groups owned by this instance.
  //!
  virtual void completeFill();
  //!
  //! Calculate and fill the derived histogram  groups owned by this instance. This is used in particular to carry out the calculation of correlators and
  //! correlation functions. If this task is execute in parallel on the grid (or train), this method should not be called by the distributed  instances of the
  //! task but only once all or sufficiently many data files (events) have been accumulated and analyzed.
  //!
  virtual void calculateDerivedHistograms();
  //!
  //! Scale (all) the histograms held in all the histogram groups owned by this task instance. This operation is executed automatically within the finalize() method call if
  //! and only if the scaleHistograms parameter of the TaskConfiguration instance controlling this task is set to "true".
  //!
  virtual void scaleHistograms();
  //!
  //! Save this histogram in text format.
  //!
  virtual void saveHistogramsAsText();
  //!
  //! Save this histogram groups owned by this task instance. This method calls opens the root file specified in the configuration object and calls the saveHistogram
  //! methods of the histogram groups owned by this instance.
  //!
  virtual void saveHistograms();

  //!
  //! Carry out a sub-sample analysis to compute the statistical uncertainties of the histograms of this task and its subtasks.
  //!
  virtual void subsampleAnalysis();



  //!
  //! Add the histograms of this task to the given TList.
  //!
  virtual void addHistogramsToExtList(TList *list, bool all=false);

  //!
  //! Read the given named parameter from the given input file.
  //!
  virtual double readParameter(TFile * inputFile, const TString & parameterName);

  //!
  //! Open the root file named "fileName" located on the path "inputPath", using options specified by "ioOption".
  //! @param inputPath path where to find or create the file.
  //! @param fileName name of the root file to open for i/o.
  //! @param ioOption i/o options.
  //! @return Pointer to the file if successfully open or a null pointer if the file could not be opened.
  //!
  TFile * openRootFile(const TString & inputPath, const TString & fileName, const TString & ioOption);

  //!
  //! Returns a pointer to the event stream at the given index. No sanity check is performed in order to keep this call as efficient as possible.
  //! @param index index of the event stream to produce on output.
  //! @return pointer to the selected event stream.
  //!
  Event * getEventStream(unsigned int index)
  {
    return eventStreams[index];
  }

  //!
  //! Add the given event as a stream used by this task instance.
  //! @param event stream to be added to this task.
  //!
  void addEventStream(Event * event)
  {
  eventStreams.push_back(event);
  }

  //!
  //! Returns the number of event streams used  by this task instance.
  //! @return number of streams associated with this task instance.
  //!
  unsigned int getNEventStreams() const
  {
  return eventStreams.size();
  }

  //!
  //! Returns a pointer to the TaskConfiguration object controlling the behavior of this task instance.
  //! @return pointer to the TaskConfiguration instance controlling this task instance.
  //!
  TaskConfiguration * getConfiguration()
  {
    return taskConfiguration;
  }

  //!
  //! Set the TaskConfiguration object controlling the behavior of this task instance.
  //! @param config pointer to a TaskConfiguration instance to be used by this task instance.
  //!
  void setConfiguration(TaskConfiguration * config);

  //!
  //! Return the name of this task instance.
  //! @return name of this task instance.
  //!
  inline TString getName() const
  {
    return taskName;
  }

  //!
  //! Set the name of this task instance.
  //!
  inline void setName(const TString & name)
  {
    taskName = name;
  }

  //!
  //! Return a pointer to the random generator used by this task instance.
  //!
  inline TRandom * getRandomGenerator()
  {
  return taskRandomGenerator;
  }

  //!
  //! Increment the number of event processed by this task instance.
  //!
  inline void incrementEventProcessed()
  {
    ++nEventProcessed;
  }

  //!
  //! Increment the number of event accepted (and used, say, to increment/fill histograms) by this task instance.
  //!
  inline void incrementEventAccepted()
  {
    ++nEventAccepted;
  }

  //!
  //! Return the number of event processed by this task instance.
  //!
  inline long getNEventProcessed() const
  {
  return nEventProcessed;
  }

  //!
  //! Return the number of event accepted (and used, say, to increment/fill histograms) by this task instance.
  //!
  inline long getNEventAccepted() const
  {
  return nEventAccepted;
  }

  //!
  //! Return the current value of the file (and event subset) index.
  //!
  inline long getSubSampleIndex() const
  {
  return subSampleIndex;
  }

  //!
  //! Save the number of event processed by this task instance to the given TFile output.
  //!
  void saveNEventProcessed(TFile * outputFile);
  //!
  //! Save the number of event accepted by this task instance to the given TFile output.
  //!
  void saveNEventAccepted(TFile * outputFile);
  //!
  //! Save the number of event accepted by each event filter own by this task instance to the given TFile output.
  //!
  void saveNFilteredEventsAccepted(TFile * outputFile, vector<unsigned int> & nFilteredEventsAccepted);
  //!
  //! Load the number of event processed by a previous  task instance from the given TFile.
  //!
  long loadNEventProcessed(TFile * inputFile);
  //!
  //! Load the number of event accepted by a previous   task instance from the given TFile.
  //!
  long loadNEventAccepted(TFile * inputFile);
  //!
  //! Load the number of event accepted for each event filter by a previous   task instance from the given TFile.
  //!
  void loadNFilteredEventsAccepted(TFile * inputFile, vector<unsigned int> & nFilteredEventsAccepted);

  //!
  //! Reset to zero the particle counters associated with this task instance
  //!
  inline void resetParticleCounters()
  {
  particlesCounted  = 0;
  particlesAccepted = 0;
  }

  //!
  //! Increment by one unit the number of particles "seen" by this task in a given event.
  //!
  inline void incrementParticlesCounted()      { particlesCounted++; }

  //!
  //! Increment by one unit the number of particles accepted by this task in a given event.
  //!
  inline void incrementParticlesAccepted()     { particlesAccepted++; }

  //!
  //! Return the number of particles "seen" by this task in a given event.
  //!
  inline unsigned int getNParticlesCounted() const   { return particlesCounted; }

  //!
  //! Return the number of particles accepted by this task in a given event.
  //!
  inline unsigned int getNParticlesAccepted() const  { return particlesAccepted; }

  //!
  //! Set the random number generator used by this task instance.
  //!
  void setRandomGenerator(TRandom * randomGenerator);

  //!
  //! Clear the event filters used by this task instance.
  //!
  void clearEventFilters()
  {
  eventFilters.clear();
  }

  //!
  //! Set the event filters used by this task instance.
  //!
  void setEventFilters(vector<EventFilter*> _eventFilters)
  {
  eventFilters = _eventFilters;
  }

  //!
  //! Add an event filter to the event filters used by this task instance.
  //!
  void addEventFilter(EventFilter* _eventFilter)
  {
  eventFilters.push_back(_eventFilter);
  }

  //!
  //! Return event filters used by this task instance.
  //!
  vector<EventFilter*> getEventFilters()
  {
  return eventFilters;
  }

  //!
  //! Return event filters used by this task instance.
  //!
  const vector<EventFilter*> getEventFilters() const
  {
  return eventFilters;
  }

  //!
  //! Clear the particle filters used by this task instance.
  //!
  void clearParticleFilters()
  {
  eventFilters.clear();
  }

  //!
  //! Set particle filters  used by this task instance.
  //!
  void setParticleFilters(vector<ParticleFilter*> _particleFilters)
  {
  particleFilters = _particleFilters;
  }

  //!
  //! Add a particle filter to the particle filters used by this task instance.
  //!
  void addParticleFilter(ParticleFilter* _particleFilter)
  {
  particleFilters.push_back(_particleFilter);
  }

  //!
  //! Return particle filters used by this task instance.
  //!
  vector<ParticleFilter*> getParticleFilters()
  {
  return particleFilters;
  }

  //!
  //! Clear the particle smearer used by this task instance.
  //!
  void clearParticleSmearer()
  {
  particleSmearers.clear();
  }

  //!
  //! Set particle  smearers  used by this task instance.
  //!
  void setParticleSmearers(vector<ParticleSmearer*> _particleSmearers)
  {
  particleSmearers = _particleSmearers;
  }

  //!
  //! Add a particle  smearer to those  used by this task instance.
  //!
  void addParticleSmearer(ParticleSmearer*  _particleSmearer)
  {
  particleSmearers.push_back(_particleSmearer);
  }

  //!
  //! Return particle  smearers   used by this task instance.
  //!
  vector<ParticleSmearer*> getParticleSmearer()
  {
   return particleSmearers;
  }

  //!
  //! Return particle  smearers   used by this task instance.
  //!
  const vector<ParticleSmearer*> getParticleSmearer() const
  {
   return particleSmearers;
  }

  //!
  //! Return the current task status of all ongoing tasks in this job.
  //!
  inline static TaskStatus getTaskStatus()                {    return taskStatus;       }
  //!
  //! Set the current  status of all ongoing tasks in this job.
  //!
  inline static void setTaskStatus(TaskStatus newStatus)  {    taskStatus = newStatus;  }
  //!
  //! Declare the current  status of all ongoing tasks in this job as OK.
  //!
  inline static void postTaskOk()      {    taskStatus = TaskOk;  }
  //!
  //! Declare the current  status of all ongoing tasks in this job as EOF (reached end of file, no more events available in the current file)
  //!
  inline static void postTaskEof()     {    taskStatus = TaskEof; }
  //!
  //! Declare the current  status of all ongoing tasks in this job as EOD (reached end of data, no more events available in the current file
  //! and no more files registered for analysis by this job)
  //!
  inline static void postTaskEod()     {    taskStatus = TaskEod; }
  //!
  //! Declare the current  status of all ongoing tasks in this job as WARNING, thereby indicating that some abnormal but benign
  //! condition has occured.
  //!
  inline static void postTaskWarning() {    taskStatus = TaskWarning; }
  //!
  //! Declare the current  status of all ongoing tasks in this job as ERROR, thereby indicating that some serious error
  //! condition has occured. The tasks will be able to continue until graceful completion of the job is accomplished.
  //!
  inline static void postTaskError()   {    taskStatus = TaskError;   }
  //!
  //! Declare the current  status of all ongoing tasks in this job as FATAL, thereby indicating that some fatal error
  //! condition has occured. This job is immediately aborted with a call to exit(1).
  //!
  inline static void postTaskFatal()   {    taskStatus = TaskFatal;   }
  //!
  //! Returns "true" is the current status of all ongoing tasks in this job is OK.
  //!
  inline static bool isTaskOk()        {    return (taskStatus == TaskOk);  }
  //!
  //! Returns a string corresponding to the current status of all ongoing tasks in this job.
  //!
  static TString getTaskStatusName();

  ClassDef(Task,0)
};

#endif /* WAC_Task */
