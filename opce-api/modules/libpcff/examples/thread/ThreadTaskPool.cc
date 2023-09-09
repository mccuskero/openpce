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

#include <pcff/BasicExceptionMgr.h>
#include <pcff/Task.h>
#include <pcff/TestException.h>

#include <pcff/thread/ThreadDirector.h>
#include <pcff/thread/ServerPool.h>

#include "ThreadConfig.h"
#include "ThreadTaskPool.h"
#include "TestThreadPool.h"

using namespace std;
using namespace pcff;


//
//
ThreadTaskPool::ThreadTaskPool() 
{
}

//
//
//
ThreadTaskPool::~ThreadTaskPool() 
{
}

//
//
//
bool ThreadTaskPool::run() 
{
   
try 
{
   int c, i, j;
   int iter = 1000;
   int concurency = 100;
   ServerPoolThread *thd = NULL;
   
   cout << "Thread Server Pooling test starting..." << endl;
   
   iter =       ThreadConfig::instance().getIterations();
   concurency = ThreadConfig::instance().getNumThreads();
   
   // setup the thread pool, and populate
   ServerPool::instance().init(concurency);
   ServerPool::instance().populate();

   // TODO: Need to sync startup...
   // thd-startServing will fail, not intialize
   ServerPool::instance().waitTillStart();
   // sleep(1);
   
   for (i = 0; i < iter; i++) 
	{
      cout << endl << i << " --START----------------------" << endl;
      for (j = 0; j < concurency; j++) 
      {
         thd = (ServerPoolThread *)ServerPool::instance().get();
         thd->startServing();
      }
      
      sleep(1);
      cout << endl << i << " --END----------------------" << endl;    
	}
   
   ServerPool::instance().waitTillEnd();
   ServerPool::instance().shutdown();   
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
