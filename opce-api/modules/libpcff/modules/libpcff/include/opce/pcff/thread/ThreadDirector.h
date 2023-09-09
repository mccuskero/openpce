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

// TODO: put startup in main while NoThreadsRunning sleep
//           shutdown while threads running sleep
//        put print status, get status from thread
//            start time, work time, num times working
//            add status member to Thread, overload
//        create logNoDate member in Log
//        call watch member in Thread::start, passs in ID, this
//        use global mutex on add, and delete
// finish up map
// 


#ifndef _ThreadDirector_h
#define _ThreadDirector_h

#include <pcff/thread/Mutex.h>

namespace pcff 
{
   class Thread;
   
   class ThreadDirector
   {    
public:
      static ThreadDirector&  instance(int concurrency = 10);
      void                    initialize();
      static void             shutdown();

private:
      ThreadDirector(int conncurrency = 10 );
      virtual ~ThreadDirector();
         
public:
      bool isInitialized()  { return _gInitialized; }
      void monitor(int thdID, Thread *thd);
      int getThreadState() { return _threadsState; }
      int getNumThreads()   { return _numThreadsRunning; }
      void checkThreadsExitState();
      
      enum ThreadState 
      {
         NoThreadsRunning = 1,        // not threads started
         ThreadsRunning,              // at least one thread has started
         AllThreadsExited             // all started and monitored threads have exited
      };
  
public:
      pthread_key_t   getThreadKey() { return _gThreadKey; }
      int             getThreadID();
         
      // global sync functions for the threads in parent thread
public:
      void gLock();
      void gLockOnce();
      void gUnlock();    
      void gUnlockCompletely();
      int  getNextID();
      
public:
      void killAllThreads() { _allThreadsMustDie = true; }
      
private:
      void    initPrivateMembers();
      static void threadExiting (void *threadIn);
      
private:
      static ThreadDirector *_instance;
      
private:
      pthread_key_t       _gThreadKey; // thread key for storing info in thread.
      bool                _gInitialized;           // thread inited or not
      Mutex               *_gThreadMutex;  // mutex for all threads
      int                 _gLockLevel;    // level for locking thread    
      Mutex               *_gNextIDMutex;    
      int                 _gDebug;
      bool                _allThreadsMustDie;
      Thread              *_parent;
      int                 _concurrency;
      
      int                 _gNextID;
      int                 _threadsState;
      int                 _numThreadsRunning;
      
// TODO: put map together for closer monitoring of thread
// add in overridable member for string Thread::getStatus()
//   createTime, upTime, ....
// private:      
//      map<int, Thread&>  _runningThreads;
   };
};


#endif
