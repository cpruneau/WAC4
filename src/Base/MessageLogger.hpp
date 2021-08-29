// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/

#ifndef WAC_MessageLogger
#define WAC_MessageLogger
#include "TClass.h"
#include "TString.h"
#include "TFile.h"
#include "TH1.h"
#include <iostream>
using namespace std;
using std::cout;
using std::endl;

class MessageLogger
{
public:

  enum TaskStatus { TaskUnknown, TaskOk, TaskEof, TaskEod, TaskWarning, TaskError, TaskFatal};
  enum LogLevel   { Unknown, Debug, Info, Warning, Error, Fatal};

  //!
  //! Status of the ongoing tasks. Since this is a static variable, it reflects a global state or status of an ongoing analysis. The task status may be set or read by
  //! all objects of class deriving from this class.
  static TaskStatus taskStatus;

  LogLevel  reportLevel;
  LogLevel  holdReportLevel;
  TString className;
  TString instanceName;
  TString fctName;


  MessageLogger(LogLevel selectedLevel=Info) :
  reportLevel( selectedLevel ),
  holdReportLevel(selectedLevel),
  className("MessageLogger"),
  instanceName("MessageLogger"),
  fctName("CTOR")
  {   }

  virtual ~MessageLogger()
  { }

  void setReportLevel(LogLevel requiredLevel)
  {
    reportLevel = requiredLevel;
  }

  void setLogLevel(LogLevel requiredLevel)
  {
    reportLevel = requiredLevel;
  }

  LogLevel getLogLevel() const
  {
    return reportLevel;
  }

  LogLevel getReportLevel() const
  {
    return reportLevel;
  }

  void setHoldLogLevel(LogLevel requiredLevel)
  {
  holdReportLevel = reportLevel;
  reportLevel     = requiredLevel;
  }

  void restoreLogLevel()
  {
  reportLevel = holdReportLevel;
  }

  inline void appendClassName(const TString & _className)
  {
  className += "/";
  className += _className;
  }

  inline void setClassName(const TString & _className)
  {
  className = _className;
  }

  inline void setInstanceName(const TString & _instanceName) { instanceName = _instanceName;  }
  inline void setFunctionName(const TString & _fctName) { fctName = _fctName;   }
  inline const TString & getClassName()     const    { return className; }
  inline const TString & getInstanceName()  const    { return instanceName; }
  inline const TString & getFunctionName() const    { return fctName; }

  bool reportStart  (ostream & output=cout) const;
  bool reportEnd    (ostream & output=cout) const;
  bool reportDebug(ostream & output=cout) const;
  bool reportInfo(ostream & output=cout) const;
  bool reportWarning(ostream & output=cout) const;
  bool reportError(ostream & output=cout) const;
  bool reportFatal(ostream & output=cout) const;

  bool reportDebug  (const TString & fctName, ostream & output=cout) const;
  bool reportNoOps  (const TString & fctName, ostream & output=cout) const;
  bool reportStart  (const TString & fctName, ostream & output=cout) const;
  bool reportEnd    (const TString & fctName, ostream & output=cout) const;
  bool reportInfo   (const TString & fctName, ostream & output=cout) const;
  bool reportWarning(const TString & fctName, ostream & output=cout) const;
  bool reportError  (const TString & fctName, ostream & output=cout) const;
  bool reportFatal  (const TString & fctName, ostream & output=cout) const;

  bool reportDebug  (const TString & className, const TString & instanceName, const TString & fctName, ostream & output=cout) const;
  bool reportNoOps  (const TString & className, const TString & instanceName, const TString & fctName, ostream & output=cout) const;
  bool reportStart  (const TString & className, const TString & instanceName, const TString & fctName, ostream & output=cout) const;
  bool reportEnd    (const TString & className, const TString & instanceName, const TString & fctName, ostream & output=cout) const;
  bool reportInfo   (const TString & className, const TString & instanceName, const TString & fctName, ostream & output=cout) const;
  bool reportWarning(const TString & className, const TString & instanceName, const TString & fctName, ostream & output=cout) const;
  bool reportError  (const TString & className, const TString & instanceName, const TString & fctName, ostream & output=cout) const;
  bool reportFatal  (const TString & className, const TString & instanceName, const TString & fctName, ostream & output=cout) const;

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
  //! Returns "true" is the current status of all ongoing tasks in this job is OK or WARNING.
  //!
  inline static bool isTaskOk()
  {
  return (taskStatus == TaskOk) || (taskStatus == TaskWarning);
  }

  //!
  //! Returns a string corresponding to the current status of all ongoing tasks in this job.
  //!
  static TString getTaskStatusName();

  static TString startString;
  static TString endString;
  static TString debugString;
  static TString infoString;
  static TString warningString;
  static TString errorString;
  static TString fatalString;

  bool ptrFileExist(const TString & caller, const TFile * f) const;

  bool ptrExist(const TString & caller, const TH1 * h1)  const;
  bool ptrExist(const TString & caller, const TH1 * h1, const TH1 * h2) const;
  bool ptrExist(const TString & caller, const TH1 * h1, const TH1 * h2, const TH1 * h3) const;
  bool ptrExist(const TString & caller, const TH1 * h1, const TH1 * h2, const TH1 * h3, const TH1 * h4) const;
  bool ptrExist(const TString & caller, const TH1 * h1, const TH1 * h2, const TH1 * h3, const TH1 * h4, const TH1 * h5) const;
  bool ptrExist(const TString & caller, const TH1 * h1, const TH1 * h2, const TH1 * h3, const TH1 * h4, const TH1 * h5, const TH1 * h6) const;
  bool ptrExist(const TString & caller, const TH1 * h1, const TH1 * h2, const TH1 * h3, const TH1 * h4, const TH1 * h5, const TH1 * h6, const TH1 * h7) const;
  bool ptrExist(const TString & caller, const TH1 * h1, const TH1 * h2, const TH1 * h3, const TH1 * h4, const TH1 * h5, const TH1 * h6, const TH1 * h7, const TH1 * h8) const;
  bool ptrExist(const TString & caller, const TH1 * h1, const TH1 * h2, const TH1 * h3, const TH1 * h4, const TH1 * h5, const TH1 * h6, const TH1 * h7, const TH1 * h8, const TH1 * h9) const;
  bool ptrExist(const TString & caller, const TH1 * h1, const TH1 * h2, const TH1 * h3, const TH1 * h4, const TH1 * h5, const TH1 * h6, const TH1 * h7, const TH1 * h8, const TH1 * h9, const TH1 * h10) const;
  bool ptrExist(const TString & caller, const TH1 * h1, const TH1 * h2, const TH1 * h3, const TH1 * h4, const TH1 * h5, const TH1 * h6, const TH1 * h7, const TH1 * h8, const TH1 * h9, const TH1 * h10, const TH1 * h11) const;
  bool ptrExist(const TString & caller, const TH1 * h1, const TH1 * h2, const TH1 * h3, const TH1 * h4, const TH1 * h5, const TH1 * h6, const TH1 * h7, const TH1 * h8, const TH1 * h9, const TH1 * h10, const TH1 * h11, const TH1 * h12) const;



  ClassDef(MessageLogger,0)
};

#endif /* WAC_MessageLogger */
