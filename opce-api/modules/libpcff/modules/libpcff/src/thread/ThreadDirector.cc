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

// There are two behaviors supported
// by this Thread class: containment, inheritance:
// They are initialized in the corresponding Constructor

#include <stdlib.h>
#include <pthread.h>
#include <string>
#include <sstream>
#include <iostream>
#include <assert.h>


#include <pcff/thread/Mutex.h>
#include <pcff/thread/Thread.h>
#include <pcff/thread/ThreadDirector.h>
#include <pcff/thread/ThreadExceptionMgr.h>

using namespace std;
using namespace pcff;

ThreadDirector *ThreadDirector::_instance = NULL;

ThreadDirector &ThreadDirector::instance(int concurrency)
{
   if (_instance == NULL)
   {
      _instance = new ThreadDirector(concurrency);
   }
   
   return *_instance;
}

void ThreadDirector::shutdown()
{
   // check for mutex?
   ThreadExceptionMgr::instance()->log("Thread framework is shutting down");      
   
   delete _instance;
}

ThreadDirector::ThreadDirector(int concurrency)
{
   initPrivateMembers();
   _concurrency = concurrency;
}

// need to wait until the thread is create 
// before calling the delete, or else pthread_join,
// within the delete call will fail, because
// pthread_id is invalid 
ThreadDirector::~ThreadDirector()
{
   if (_gThreadMutex != NULL)
   {
      delete _gThreadMutex;
   }
   
   if (_gNextIDMutex != NULL)
   {
      delete _gNextIDMutex;
   }
}

void ThreadDirector::initPrivateMembers()
{
   _gNextID       = 1; // thread 0 is the parent process
   _gInitialized  = false;
   _gThreadMutex  = NULL;
   _gNextIDMutex  = NULL;
   _gLockLevel    = 0;
   _gDebug        = 0;
   _concurrency   = 10;
   _threadsState  = ThreadDirector::NoThreadsRunning;
   _numThreadsRunning = 0;
}

void ThreadDirector::monitor(int thdID, Thread *thd)
{
   _gThreadMutex->lock();
   _numThreadsRunning++;
   if (_numThreadsRunning > 0)
   {
      _threadsState = ThreadDirector::ThreadsRunning;
   }
   // TODO:
//   _runningThreads[thdID] = thd;
   _gThreadMutex->unlock();
}

//
// Used to initialize the thread framework
//
void ThreadDirector::initialize()
{
   int ret = 0;
   
   ret = pthread_key_create (&_gThreadKey, ThreadDirector::threadExiting);
   assert (ret == 0);
   
   _gThreadMutex = new Mutex();
   ret = _gThreadMutex->init();
   assert (ret == 0);
   
   _gNextIDMutex = new Mutex();
   ret = _gNextIDMutex->init();
   assert (ret == 0);
   
   // hint to the OS about the number of threads that will be run
   pthread_setconcurrency(_concurrency);

   ThreadExceptionMgr::instance()->log("Thread framework is initialized");         
   _gInitialized = true;

   // parent thread initialization   
   _parent = new Thread();
   _parent->init();
   _parent->setPThreadHandle();  // only called for parent, not needed for created threads
   _parent->setThreadObject();   
}

int ThreadDirector::getThreadID()
{
   int retID = 0;
   
   Thread *thd = Thread::getThreadObject();
   
   if (thd != NULL)
   {
      retID = thd->getThreadID();
   }
   
   return retID;
}

void ThreadDirector::gLock () 
{ 
   _gThreadMutex->lock();
   
   _gLockLevel++;
   
   if (_gDebug)
   {
      Thread *thd = Thread::getThreadObject();
      assert (thd!=0);
      cerr << "( " << thd->getHandle() <<") locked = " << _gLockLevel << endl; 
      cerr.flush();
   }
}

void ThreadDirector::gUnlock ()  
{
   if (_gDebug)
   {
      Thread *thd = Thread::getThreadObject();
      assert(thd!=0);
      
      cerr << "(  " << thd->getHandle() <<") unlocked = " << _gLockLevel << endl; 
      cerr.flush();
   }
   _gLockLevel-- ;
   
   _gThreadMutex->unlock();
} 


void ThreadDirector::gLockOnce ()
{
   _gThreadMutex->lock();
   _gLockLevel++;
   
   while (_gLockLevel>1)
   {
      gUnlock();
   }
   
   if (_gDebug)
   {
      cerr << "(  " << Thread::getThreadObject()->getHandle() <<") locked = " << _gLockLevel << "\n"; cerr.flush();      
   }
}

void ThreadDirector::gUnlockCompletely()
{ 
   while (_gLockLevel > 0) gUnlock();
}

//
// used to track thread IDs within the pcave
// framework
//
int ThreadDirector::getNextID()
{
   int ret = 0;
   int retID = 0;
   
   ret = _gNextIDMutex->lock();
   assert(ret == 0);
   retID = _gNextID;
   _gNextID++;
   ret = _gNextIDMutex->unlock();
   assert(ret == 0);

   return retID;
}

void ThreadDirector::checkThreadsExitState()
{
   stringstream sStr;
   
   _gThreadMutex->lock();
   _numThreadsRunning--;
   if (_numThreadsRunning <= 0)
   {
      _threadsState = ThreadDirector::AllThreadsExited;
   }
   _gThreadMutex->unlock();   
   
   if (_threadsState == ThreadDirector::AllThreadsExited)
   {
      sStr << "ThreadDirector::checkThreadsExitState AllThreadsExited state: " << _threadsState;
      ThreadExceptionMgr::instance()->log( sStr.str().c_str(), LogLevel::Debug );      
   }
}

void ThreadDirector::threadExiting ( void * threadIn )
{
   Thread *thdIn = (Thread *)threadIn;
   stringstream sStr;
   
   ThreadDirector::instance().checkThreadsExitState();
   
   sStr << "ThreadDirector::threadExiting " << thdIn->getID() \
      << " runningThreads: " \
      << ThreadDirector::instance().getNumThreads() \
      << " state: " \
      << ThreadDirector::instance().getThreadState();
   
   ThreadExceptionMgr::instance()->log( sStr.str().c_str(), LogLevel::Debug );
   
   if (thdIn == 0)
   {
      return;
   }
   
   if ( thdIn->isExitingNormally() == false)
   {
      cerr << "Thread ("<< thdIn->getID() <<") suffered an untimely death\n";
      cerr.flush ();
      
      delete thdIn;
   }
}
