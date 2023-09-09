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
#include <iostream>
#include <sstream>

#include <pcff/BasicExceptionMgr.h>
#include <pcff/Task.h>
#include <pcff/TestException.h>

#include <pcff/thread/ThreadExceptionMgr.h>
#include <pcff/thread/ThreadDirector.h>

#include "ThreadConfig.h"
#include "ThreadTaskContain.h"
#include "TestThreadContain.h"

using namespace std;
using namespace pcff;


//
//
ThreadTaskContain::ThreadTaskContain() 
{
}

//
//
//
ThreadTaskContain::~ThreadTaskContain() 
{
}

//
//
//
bool ThreadTaskContain::run() 
{
   
try 
{
   int c, i, j;
   int iter = 1000;
   int concurency = 100;
   stringstream sStr;
   cout << "Thread Containment test starting..." << endl;
   
   iter =       ThreadConfig::instance().getIterations();
   concurency = ThreadConfig::instance().getNumThreads();
   
   TestThreadContain **ta = new TestThreadContain*[concurency];

   // initialize the thread framework
   ThreadDirector::instance().initialize();

   for (i = 0; i < iter; i++) 
	{
      cout << endl << i << " --START----------------------" << endl;
      for (j = 0; j < concurency; j++) 
      {
	      ta[j] = new TestThreadContain();
	      ta[j]->init();
	      ta[j]->start();
      }
      
      // 
      // Note: potential a infinite loop! 
      // can add timers, and other things on startup
      //
      while ( ThreadDirector::instance().getThreadState() == ThreadDirector::NoThreadsRunning  &&
              ThreadDirector::instance().getThreadState() != ThreadDirector::AllThreadsExited)
      {
         sStr << "main: waiting for startup numThreads: " << ThreadDirector::instance().getNumThreads() << " state: " << ThreadDirector::instance().getThreadState();
         ThreadExceptionMgr::instance()->log(sStr.str().c_str());
         sStr.str("");
         sleep(2);
      }
      
      // 
      // Note: potential a infinite loop! 
      // can add timers,  and other fail saves (SIGINT)
      //
      while (ThreadDirector::instance().getThreadState() == ThreadDirector::ThreadsRunning)
      {
         sStr << "main: monitoring threads num(" << ThreadDirector::instance().getNumThreads() << ")";
         ThreadExceptionMgr::instance()->log( sStr.str().c_str() );
         sleep(2);
      }    
      
      cout << endl << i << " --END----------------------" << endl;    
      for (j = 0; j < concurency; j++) 
      {
	      delete ta[j];
	      ta[j] = NULL;
      }
	}
   
   delete [] ta;
   
   // initialize the thread framework
   ThreadDirector::instance().shutdown();
   
   cout << endl;
}  
catch(TestException& ex)
{
   BasicExceptionMgr::instance()->handle( ex );
}
catch(BasicException& ex)
{
   BasicExceptionMgr::instance()->handle( ex );
}
catch(...)
{
   BasicExceptionMgr::instance()->handle( "Unknown exception" );
}


cout << "Thread test complete." << endl;
return true;
}
