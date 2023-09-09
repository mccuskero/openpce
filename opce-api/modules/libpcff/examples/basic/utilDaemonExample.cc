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

#include "TestException.h"
#include "BasicExceptionMgr.h"
#include "Log.h"

#include "SIGTERM_SignalEventHandler.h"
#include "SIGINT_SignalEventHandler.h"
#include "SignalHandler.h"
#include "Daemon.h"

using namespace std;
using namespace pcff;

int main( int argc, char* argv) {
  // ======================================================= 
  // This section of code test exception handling and logging
  // 
  Log *myLog = NULL;
  BasicExceptionMgr::instance();
  
  try {
    myLog = new Log(Log::logToFile | Log::logToCrt,
		    1000 );
    myLog->init("test.log");
    myLog->logMessage("Opening log");
    BasicExceptionMgr::instance()->setLog( myLog );
    
    throw TestException( TestException::Test1, "This is a test" );
  }
  catch(TestException& ex) {
    BasicExceptionMgr::instance()->handle( ex );
  }
  catch(BasicException& ex) {
    BasicExceptionMgr::instance()->handle( ex );
  }
  catch(...) {
    BasicExceptionMgr::instance()->handle( "Unknown exception" );
  }
 
  // all file descriptors are closed in forked child daemon  
  BasicExceptionMgr::instance()->setLog( NULL );
  delete myLog;


  // ===============================
  // This section of code test the daemon code out
  //


  // the signal event handlers need to be scoped into the main
  // processing loop of a process, or a daemon.
  // then may be attributes of the daemon, or process main.
  // test signal handlers

  SignalHandler *signalHandler = SignalHandler::instance();  

///////////
// Examples on using or creating speciliazed event
// handlers.
//
//  SIGTERM_SignalEventHandler sigterm;
//  SIGINT_SignalEventHandler sigint;
//  signalHandler->registerEventHandler( SIGTERM, &sigterm );
//  signalHandler->registerEventHandler( SIGINT, &sigint );
///////////////
  
  // example use of default event signal handler
  signalHandler->registerEventHandler(SIGTERM);
  signalHandler->registerEventHandler(SIGINT);

  bool bExit = false;

  Daemon myDaemon;
  myDaemon.init();
  myDaemon.setRunningDir("/tmp");
  myDaemon.daemonize();
  
  try {
    myLog = new Log(Log::logToFile | Log::logToCrt,
		    1000 );
    myLog->init("testDaemon.log");
    myLog->logMessage("Opening log");
    BasicExceptionMgr::instance()->setLog( myLog );

    while(!bExit) {
      sleep(5); // sleep 5 secs
      cout << "sleeping..." << endl;
      myLog->logMessage("sleeping...");
      if ( signalHandler->signaled(SIGTERM) ) {
	cout << "SIGTERM recieved, exiting..." << endl;
        myLog->logMessage("SIGTERM recieved, exiting...");
	exit(0);
      }
      if ( signalHandler->signaled(SIGINT) ) {
	cout << "SIGINT recieved, exiting..." << endl;
        myLog->logMessage("SIGINT recieved, exiting...");
	bExit = true;
      }
    }
  }
  catch(...) {
    BasicExceptionMgr::instance()->handle( "Unknown exception" );
  }
}
