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

//
// In order to create your own type of ServerPool you need to:
// 1) create your ServerPoolThread class
// 2) derice your own ServerPool class
//     a. overload the instance (maybe not, must make base class's available)
//     b. overload the initialize
//
// This base class has minimal functionality based in the ServerPoolThread
// type. Review the mainProc function to get an understanding of what it does.
//

#include <stdlib.h>
#include <sstream>
#include <iostream>

#include <pcff/LogLevel.h>

#include <pcff/thread/ThreadExceptionMgr.h>
#include <pcff/thread/ThreadDirector.h>
#include <pcff/thread/ServerPoolThread.h>
#include <pcff/thread/ServerPool.h>

using namespace std;
using namespace pcff;

ServerPool &ServerPool::instance()
{
   if (_instance == NULL)
   {
      _instance = new ServerPool();
   }
   
   return (ServerPool &)(*_instance);
}

void ServerPool::shutdown()
{
   // shutdowns the thread framework
   ThreadDirector::instance().shutdown();
   
   if (_instance != NULL)
   {
      delete _instance;
   }   
}

ServerPool::ServerPool()
{
   _getBlockMode = ServerPool::WaitForAvailableItem;
}

ServerPool::~ServerPool()
{
   //  TODO: move to base class??
   // whatch type issues
   for (int i=0; i<_poolSize; i++)
   {
      delete _pool[i];
   }   
   delete [] _pool;
   _pool = NULL;   
}

//
// this is overloaded based on PoolType
// This initiziation creates ServerPoolThreads, 
// which have a default behavior to output awake,
// and sleep messages.
//
// pool can be a polymorphic container of server
// types based on a ServerPoolThread type.
//
void ServerPool::init(int poolSize) 
{
   // initialize the thread framework
   ThreadDirector::instance().initialize();
      
   // initialize the contained ConnectionPool
   ConnectionPool::init(poolSize);   
}

void ServerPool::populate()
{
   //create the pool, startup up the threads,
   // threads will add themselves to the link list.
   for (int i=0; i<_poolSize; i++)
   {
      _pool[i] = new ServerPoolThread(*this);
      _pool[i]->init();   // adds itself to the available list 
      _pool[i]->start();
   }   
}

ConnectionPoolItem *ServerPool::get()
{
   ConnectionPoolItem *cpi = NULL;
   
   if (_getBlockMode == ServerPool::NoBlock)
   {
//      cerr << "ServerPool::get NoBlock" << endl; cerr.flush();
      cpi = ConnectionPool::get();
   }
   else if (_getBlockMode == ServerPool::WaitForAvailableItem)
   {
//     cerr << "ServerPool::get WaitForAvailableItem" << endl; cerr.flush();
      cpi = ConnectionPool::get();
      
      //
      // TODO: need to implement exit strategy
      // possible infinite loop
      //
      while(cpi == NULL)
      {
         ThreadExceptionMgr::instance()->log("ServerPool::get waiting for next availabile ServerPoolThread", LogLevel::Warn);
         sleep(2);
         cpi = ConnectionPool::get();      
      }      
   }
   else
   {
      cpi = ConnectionPool::get();      
   }
   
   return cpi;
}

void ServerPool::waitTillStart()
{
   stringstream sStr;
   // 
   // Note: potential a infinite loop! 
   // can add timers, and other things on startup
   //
   while ( ThreadDirector::instance().getThreadState() == ThreadDirector::NoThreadsRunning  &&
           ThreadDirector::instance().getThreadState() != ThreadDirector::AllThreadsExited)
   {
      sStr << "ServerPool::waitTillStart: waiting for startup numThreads: " << ThreadDirector::instance().getNumThreads() << " state: " << ThreadDirector::instance().getThreadState();
      ThreadExceptionMgr::instance()->log(sStr.str().c_str(),  LogLevel::Info);
      sStr.str("");
      sleep(2);
   }   
}

void ServerPool::waitTillEnd()
{
   stringstream sStr;

   // 
   // Note: potential a infinite loop! 
   // can add timers,  and other fail saves (SIGINT)
   //
   while (ThreadDirector::instance().getThreadState() == ThreadDirector::ThreadsRunning)
   {
      sStr << "ServerPool::waitTillEnd: monitoring threads num(" << ThreadDirector::instance().getNumThreads() << ")";
      ThreadExceptionMgr::instance()->log( sStr.str().c_str(),  LogLevel::Info );
      sStr.str("");
      sleep(2);
   }   
}

