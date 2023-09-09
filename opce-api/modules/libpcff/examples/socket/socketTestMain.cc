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
#include <pcff/BasicExceptionMgr.h>
#include <pcff/LogLevel.h>

#include "SocketConfig.h"
#include "SocketTestDirector.h"

//------------------------------------------
// Notes: create a signal manager!
//
//
//

using namespace std;
using namespace pcff;

void printBanner();
void printEndMessage();
void testMain(int argv, char** argc);


int main(int argv, char** argc) 
{
  cout << endl;
  cout << endl;
  cout << "Socket Test Suite - v0.0.1  01-01-03" << endl;
  cout << endl;
  cout << endl;

  testMain(argv, argc);

  cout << endl;
  cout << "Test Complete with ";
  cout << BasicExceptionMgr::instance()->getNumErrors() << " errors;" << endl;
  cout << endl;
  cout << endl;
}


//
//
//
void testMain(int argc, char** argv) 
{
  LogLevel *myLog = NULL;
  BasicExceptionMgr::instance();
  SocketTestDirector* testDirector = NULL;

  cout << endl;
  cout << "starting test..." << endl;

  try 
  {
    myLog = new LogLevel(Log::logToFile | Log::logToCrt,
		    1000 );
    myLog->init("test.log");
    myLog->logMessage("Opening log");
    BasicExceptionMgr::instance()->setLog( myLog );
    BasicExceptionMgr::instance()->setLogLevel( EXverbose );

    SocketConfig::instance().parseCommandLine(argc, argv);

    testDirector = new SocketTestDirector();
    testDirector->run(argc, argv);
    delete testDirector;
  }
  catch( BasicException& ex ) {
    BasicExceptionMgr::instance()->handle( ex );
    delete testDirector;
  }
  catch(...) {
    BasicExceptionMgr::instance()->handle();
    BasicExceptionMgr::instance()->handle(  "testMain: Unknown Exception" );
    delete testDirector;
  }
}
