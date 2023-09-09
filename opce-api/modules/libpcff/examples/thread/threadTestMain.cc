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

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <iostream>

#include <pcff/BasicException.h>
#include <pcff/LogLevelTS.h>
#include <pcff/Task.h>

#include <pcff/thread/ThreadExceptionMgr.h>

#include "ThreadConfig.h"
#include "ThreadTestDirector.h"

using namespace std;
using namespace pcff;

void printBanner();
void printEndMessage();
void testMain(int argc, char** argv);


int main(int argc, char** argv) 
{
  cout << "Thread Test Suite - v0.0.1  01-01-99" << endl;
  testMain(argc, argv);
  cout << "Test Complete with ";
  cout << ThreadExceptionMgr::instance()->getNumErrors() << " errors;" << endl;
}


//
//
//
void testMain(int argc, char** argv) 
{
  LogLevel *myLog = NULL;
  ThreadExceptionMgr::instance();
  ThreadTestDirector* testDirector = NULL;

  cout << endl;
  cout << "starting test..." << endl;

  try 
  {
    myLog = new LogLevelTS(Log::logToFile | Log::logToCrt, 1000 );
    myLog->init("test.log");
    myLog->setLogLevel(LogLevel::Debug);
    myLog->logMessage("Opening log");
    
    ThreadExceptionMgr::instance()->setLog( myLog );
    
    ThreadExceptionMgr::instance()->setLogLevel( EXverbose );

    ThreadConfig::instance().parseCommandLine(argc, argv);

    testDirector = new ThreadTestDirector();
    testDirector->run(argc, argv);
    delete testDirector;
  }
  catch( BasicException& ex ) {
    ThreadExceptionMgr::instance()->handle( ex );
    delete testDirector;
  }
  catch(...) {
    ThreadExceptionMgr::instance()->handle();
    ThreadExceptionMgr::instance()->handle(  "testMain: Unknown Exception" );
    delete testDirector;
  }
}
