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

// TODO: put map together for closer monitoring of thread
// add in overridable member for string Thread::getStatus()
//   createTime, upTime, ....

#ifndef Thread_h
#define Thread_h

#include <pthread.h>

#include <pcff/thread/Mutex.h>
#include <pcff/thread/Condition.h>
#include <pcff/thread/ThreadComponent.h>

namespace pcff 
{
   
  typedef void* (*THREAD_ENTRY_POINT)(void*);
   
  class Thread : public ThreadComponent {
  public:
    Thread();
    Thread( unsigned int priority, 
	    unsigned int stackSize, 
	    const char *name, 
	    bool bInheritScheduling);
    Thread( THREAD_ENTRY_POINT entryProc, void *entryArg );
    virtual ~Thread();

    virtual void* mainProc() { return NULL; }

  private:
    Thread( const Thread& );
    Thread& operator = (const Thread&);

  public:
    void    init();
    void    setPThreadHandle();
    int     getRunType()          { return _runType; }

    // used for containment behavior
  public:
    THREAD_ENTRY_POINT    getEntryProc()            { return _entryProc; }
    void                 *getEntryObj()             { return _entryObj; } 
    
    // some helper functions putting thread info in
    // thread storage area.
  public:
   static Thread     *getThreadObject();
   static int        getThreadID();
   static pthread_t  gGetThreadHandle();  // return current thread
   pthread_t         getThreadHandle()    { return _threadHandle; }         // return member
   void              setThreadObject();
   
  public:
    void          start();
    void          stop();
    void          wait();
    void          waitTillStart();
    void          signal();
    void          sleep( unsigned int msec );   // what about time resolution?
    void          wakeUp();
    void          join( unsigned int msec );
    void          yield();
    void          raise();
    void          cancel();
    
    void          setRunning();
    void          setFinished()       { _state = FINISHED; }
    bool          isAlive()           { return (_state == RUNNING) ? true : false; }
    bool          isExitingNormally() { return (_state == FINISHED) ? true : false; }
    
    unsigned int  setPriority( unsigned int prio );
    unsigned int  getPriority();
    pthread_t     getHandle()       { return _threadHandle; }
    int           getID()           { return _threadID; }

public:
    void lock()              { _mutex->lock();      }
    void unlock()            { _mutex->unlock();    }
    
  public:
       typedef enum 
    {
       SCOPE_SYSTEM = 0,
       SCOPE_PROCESS = 1
    } SCHED_SCOPE;
    
    typedef enum  
    {
       PRI_MIN,   // oem: 2007.03.22 was PRIO_MIN, and PRIO_MAX, caused conflict on OS X 4, darwin 8.9.0
       PRI_AVG,
       PRI_MAX
    } THD_PRIORITY;
    
    typedef enum 
    {
       INIT,
       RUNNING,
       FINISHED,
       ERR
    } THD_STATE;
    
    typedef enum 
    {
       UNKNOWN = 0,
       CONTAINED,
       INHERITED
    } THD_TYPE;
           
private:
   void    initPrivateMembers();
    
  protected:
    bool             _inheritScheduling;
    int              _threadID; // managed outside pthread
    pthread_t        _threadHandle;
    Condition        *_condition;
    Mutex            *_mutex;
    int              _runType;
    THD_STATE        _state;
    bool             _bSleep;
    bool             _bCancel;
    unsigned int     _prio;
    unsigned int     _stack_size;
    void             *_t_stack_addr;

    // members used for containment behavior
    THREAD_ENTRY_POINT _entryProc;       // used in containment behavior use
    void               *_entryObj;             // used to pass into entry procedure
    
  private:
    static void* threadEntryPoint(void *);
  };
};


#endif
