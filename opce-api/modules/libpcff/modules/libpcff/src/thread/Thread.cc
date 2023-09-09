/**
 * Copyright (c) 1994-2007 Owen McCusker <pcave@myeastern.com>
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * All rights reserved.
 */

#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include <assert.h>

#include <pcff/LogLevel.h>

#include <pcff/thread/Condition.h>
#include <pcff/thread/Mutex.h>
#include <pcff/thread/Thread.h>
#include <pcff/thread/ThreadDirector.h>
#include <pcff/thread/ThreadExceptionMgr.h>

using namespace std;
using namespace pcff;

// TODO: Exception handling...


// Two behaviors: containment, inheritance: There are two behaviors supported
// by this Thread class.

// default inheritance behavior
Thread::Thread()
: _state(INIT),
  _bSleep(false)
{
   initPrivateMembers();

   int ret = 0;

   if (ThreadDirector::instance().isInitialized() == false)
   {
      cerr << "Thread::Thread Default Thread framework not initialized (remove log)\n"; cerr.flush();
      ThreadExceptionMgr::instance()->log("Thread::Thread Default Thread framework not initialized", LogLevel::Warn);
      return;
   }

   _threadID = ThreadDirector::instance().getNextID();
   _runType = Thread::INHERITED;
}

//
// use this constructor for inheritence behavior
//
Thread::Thread(unsigned int prio,
	       unsigned int stack_size,
	       const char *name,
	       bool bInheritScheduling )
  : ThreadComponent(name),
  _state(INIT),
  _bSleep(false),
  _bCancel(false),
  _prio(prio),
  _stack_size(stack_size)
  //  stack_addr(NULL)
{
  initPrivateMembers();

  int ret = 0;

  if (ThreadDirector::instance().isInitialized() == false)
  {
     ThreadExceptionMgr::instance()->log("Thread::Thread INHERITED Thread framework not initialized", LogLevel::Warn);
     cerr << "INHERITED Thread framework not initialized (remove)\n"; cerr.flush();
     return;
  }

  _threadID = ThreadDirector::instance().getNextID();

   _runType = Thread::INHERITED;
}

//
// used for encapsulation/containment behavior
//
Thread::Thread( THREAD_ENTRY_POINT entryProc, void *entryObj)
: _state(INIT),
  _bSleep(false)
{
   int ret = 0;

   initPrivateMembers();

   if (ThreadDirector::instance().isInitialized() == false)
   {
      cerr << "CONTAINED Thread framework not initialized (remove)\n"; cerr.flush();
      ThreadExceptionMgr::instance()->log("Thread::Thread Thread framework not initialized", LogLevel::Warn);
      return;
   }

   _threadID = ThreadDirector::instance().getNextID();
   _entryProc = entryProc;
   _entryObj  = entryObj;

   _runType = Thread::CONTAINED;
}

// TODO: copy constructor??? hmmm...
Thread::Thread(const Thread& thd)
{

}

// need to wait until the thread is create
// before calling the delete, or else pthread_join,
// within the delete call will fail, because
// pthread_id is invalid
Thread::~Thread()
{
  // TODO: make thread_return not NULL someday
  // passing in a return address.
   if (_threadHandle != 0)
   {
	   if (_state == Thread::RUNNING)
	   {
		   ThreadExceptionMgr::instance()->log("~Thread: Thread waiting to end: join", LogLevel::Debug);
		   pthread_join( this->_threadHandle, NULL );
		   ThreadExceptionMgr::instance()->log("~Thread: Thread ending: joined back", LogLevel::Debug);
	   }
	   else
	   {
		   ThreadExceptionMgr::instance()->log("~Thread: Thread never created", LogLevel::Debug);
	   }
   }

   if (_mutex != NULL)   delete _mutex;
   if (_condition != NULL) delete _condition;

   initPrivateMembers();
}

void Thread::initPrivateMembers()
{
   _mutex = NULL;
   _condition = NULL;
   _entryProc = NULL;
   _entryObj = NULL;
   _runType = Thread::UNKNOWN;
   _state = Thread::INIT;
   _threadHandle = 0;
}

void Thread::init()
{
   _mutex = new Mutex();
   _mutex->init();
   _condition = new Condition(_mutex);
   _condition->init();
   ThreadExceptionMgr::instance()->log("Thread::init", LogLevel::Debug);
}

void Thread::start()
{
   ThreadExceptionMgr::instance()->log("Thread::start", LogLevel::Debug);

   // add thread to theaddirector to monitor
   ThreadDirector::instance().monitor(_threadID, this);

   pthread_create(&this->_threadHandle,
		 NULL, //&attr,
		 Thread::threadEntryPoint,
		 this);
}

void Thread::wait()
{
   ThreadExceptionMgr::instance()->log("Thread::wait mutex lock", LogLevel::Debug);
   _mutex->lock();
   _condition->wait(_mutex);
   _mutex->unlock();
   ThreadExceptionMgr::instance()->log("Thread::wait mutex unlock", LogLevel::Debug);
}

//
// Warning: the child thread can exit before the parent
// waits. Therefore the parent will wait indefinately.
// The child needs to sleep, or do some work.
//
void Thread::waitTillStart()
{
   cout.flush();
   ThreadExceptionMgr::instance()->log("Thread::waitTillStart mutex lock", LogLevel::Debug);
   _mutex->lock();
   while( _state != Thread::RUNNING )
   {
      ThreadExceptionMgr::instance()->log("Thread::waitTillStart condition wait", LogLevel::Debug);
      _condition->wait(_mutex);
   }
   ThreadExceptionMgr::instance()->log("Thread::waitTillStart mutex unlock", LogLevel::Debug);
   _mutex->unlock();
}


void Thread::signal()
{
   ThreadExceptionMgr::instance()->log("Thread::signal", LogLevel::Debug);
   _condition->signal();
}

void Thread::setRunning()
{
   _state = RUNNING;
}


void Thread::stop()
{
   int status;

   _state = Thread::FINISHED;

   ThreadExceptionMgr::instance()->log("Thread::stop and exiting", LogLevel::Debug);

   pthread_exit(&status);
}

void Thread::sleep( unsigned int msec )
{
}

void Thread::wakeUp()
{
}

void Thread::join( unsigned int msec )
{
}

void Thread::yield()
{
}

void Thread::raise()
{
}

void Thread::cancel()
{
}

unsigned int Thread::setPriority( unsigned int prio )
{
}

unsigned int Thread::getPriority()
{
}

void Thread::setPThreadHandle()
{
   _threadHandle = pthread_self();
}

//
// now running in another thread of execution
//
void* Thread::threadEntryPoint(void *this_thd)
{
   THREAD_ENTRY_POINT  proc = NULL;
   Thread *thd = NULL;
   void   *procArg = NULL;

   assert( this_thd != NULL);

   thd = ((Thread*)this_thd);

   thd->setThreadObject();
   thd->setRunning();

   if (thd->getRunType() == Thread::CONTAINED)
   {
      proc = thd->getEntryProc();
      procArg = thd->getEntryObj();
   }
   else if (thd->getRunType() == Thread::INHERITED)
   {
      return thd->mainProc();
   }
   else if (thd->getRunType() == Thread::UNKNOWN)
   {
      cerr<< "Thread::threadEntryPoint: unknown runType - thread not initialized\n"; cerr.flush();
   }
   else
   {
      cerr<< "Thread::threadEntryPoint: unknown runType \n"; cerr.flush();
   }
   return proc(procArg);
}

//
// management of thread data on stack,
// this is needed for "glabal" management issues.
//
Thread *Thread::getThreadObject()
{
   Thread *thd;
   pthread_key_t   threadKey = 0;

   threadKey = ThreadDirector::instance().getThreadKey();

   thd = (Thread *)pthread_getspecific (threadKey);

   ThreadExceptionMgr::instance()->log("Thread::getThreadObject", LogLevel::Debug);

   assert (thd != NULL);
   return thd;
}

int Thread::getThreadID()
{
   int retID = 0;

   Thread *thd = Thread::getThreadObject();

   if (thd != NULL)
   {
      retID = thd->getThreadID();
   }

   return retID;
}

pthread_t Thread::gGetThreadHandle()
{
   pthread_t retID = pthread_self();

   return retID;
}


void Thread::setThreadObject()
{
   int ret;
   pthread_key_t   threadKey = 0;

   threadKey = ThreadDirector::instance().getThreadKey();
   // set thread context to point to created THREAD object
   ret = pthread_setspecific ( threadKey, (void *)this);
   assert(ret == 0);

   // verify context
   Thread * thd = getThreadObject();
   assert(thd == this);                   // Thread context not stored
}

