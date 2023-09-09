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

#include <pcff/LogLevel.h>
#include <pcff/Config.h>
#include <pcff/SIGTERM_SignalEventHandler.h>
#include <pcff/SIGINT_SignalEventHandler.h>
#include <pcff/SignalHandler.h>
#include <pcff/Daemon.h>

#include <pcff/thread/ThreadExceptionMgr.h>


#include "ServerTestException.h"
#include "ServerTestConfig.h"

#include "EchoServerThread.h"
#include "EchoServer.h"

using namespace std;
using namespace pcff;

#define PROXY_VERSION "0.0.1"

int main( int argc, char** argv) {
   bool                 bExit = false;
   LogLevel             *myLog = NULL;

   // initialize the system
   try 
   {  
      ServerTestConfig &config = ServerTestConfig::instance();
      config.init();
      config.parseCommandLine(argc, argv);

      // print out banner
      // move arch, and prog name somewhere in the Config class...
      cout << "eproxyd (GCC) " << PROXY_VERSION << " (Linux)" << endl << endl;
      
      config.showInitialValues();      
      SignalHandler *signalHandler = SignalHandler::instance();
      signalHandler->registerEventHandler(SIGTERM);
      signalHandler->registerEventHandler(SIGINT);
      
      bool bExit = false;
      
      // all file descriptors are closed in forked child daemon
      // TODO: How to get thread IDs in log messages???
      // need logging mechanism from ThreadDirector,
      // use key
      // ThreadExceptionMgr::instance()->setLog( NULL );
      
      myLog = new LogLevel(Log::logToFile | Log::logToCrt, 1000 );
      myLog->init("echod.log");
      myLog->logMessage("Opening log");
      ThreadExceptionMgr::instance()->setLog( myLog );
      
      EchoServer::instance().init( config.getPoolSize(),
                                   config.getAddrFamily(),
                                   config.getListenHost(),  // get const char pointer
                                   config.getListenPort() );
      
      EchoServer::instance().populate();
            
      if ( ServerTestConfig::instance().noDaemon() == false ) 
      {
         Daemon myDaemon;
         myDaemon.init();
         myDaemon.setRunningDir("/tmp");
         myDaemon.daemonize();      
      }
      
      EchoServer::instance().run();
   }
   catch(...) {
      ThreadExceptionMgr::instance()->handle( "Unknown exception" );
   }   
}
