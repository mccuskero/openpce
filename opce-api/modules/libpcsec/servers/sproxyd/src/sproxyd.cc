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
#include <pcff/LogLevel.h>
#include <pcff/SIGTERM_SignalEventHandler.h>
#include <pcff/SIGINT_SignalEventHandler.h>
#include <pcff/SignalHandler.h>
#include <pcff/Daemon.h>
#include <pcff/thread/ThreadDirector.h>
#include <pcff/proxy/SensorProxyException.h>

#include <pcsec/PcsecSensorProxyException.h>
#include <pcsec/SensorProxyConfig.h>
#include <pcsec/PcsecSensorProxy.h>


using namespace std;
using namespace pcff;
using namespace pcsec;

int main( int argc, char* argv[])
{
  LogLevel *myLog = NULL;
  BasicExceptionMgr::instance();

  try
  {
    myLog = new LogLevel(Log::logToFile | Log::logToCrt,
		    1000 );
    myLog->init("test.log");
    myLog->logMessage("Opening log");
    BasicExceptionMgr::instance()->setLog( myLog );

    BasicExceptionMgr::instance()->log("--== Starting sproxyd ==--", LogLevel::Info);
    SensorProxyConfig::instance().init(argc,argv);

    BasicExceptionMgr::instance()->log("---Initial Startup Options---", LogLevel::Info);
    SensorProxyConfig::instance().showInitialValues();

    int logLevel = SensorProxyConfig::instance().getVerbosityLevel();

    BasicExceptionMgr::instance()->setLogLevel(logLevel);

    ThreadDirector::instance().initialize();

  }
  catch(SensorProxyException& ex)
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

  //
  try
  {
	string workingDir = SensorProxyConfig::instance().workingDir();

	SignalHandler *signalHandler = SignalHandler::instance();
    signalHandler->registerEventHandler(SIGTERM);
    signalHandler->registerEventHandler(SIGINT);

    if (SensorProxyConfig::instance().daemonize() == true)
    {
    	// all file descriptors are closed in forked child daemon
    	BasicExceptionMgr::instance()->setLog( NULL );
    	delete myLog;

    	Daemon myDaemon;
    	myDaemon.init();
    	myDaemon.setRunningDir((char *)workingDir.c_str());
    	myDaemon.daemonize();

    	myLog = new LogLevel(Log::logToFile | Log::logToCrt,
    		    1000 );
    	myLog->init("sproxyd.log");
    	myLog->logMessage("Opening log");

    	int logLevel = SensorProxyConfig::instance().getVerbosityLevel();
    	BasicExceptionMgr::instance()->setLog( myLog );
    	BasicExceptionMgr::instance()->setLogLevel(logLevel);
    }
  }
  catch(SensorProxyException& ex)
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


  //
  // run the proxy and it service
  //
  try
  {
	bool bExit = false;
	SignalHandler *signalHandler = SignalHandler::instance();

    PcsecSensorProxy::instance().initialize();
    PcsecSensorProxy::instance().startService();

    while (!bExit)
    {
        cout << "Main sleeping..." << endl;
    	sleep(5); // sleep 5 secs
        BasicExceptionMgr::instance()->log("Main sleeping...");

        cout << "Main monitor service..." << endl;
        PcsecSensorProxy::instance().monitorService();

        if ( signalHandler->signaled(SIGTERM) )
        {
      	  cout << "SIGTERM recieved, exiting..." << endl;
      	  BasicExceptionMgr::instance()->log("SIGTERM recieved, exiting...");
      	  exit(0);
        }

        if ( signalHandler->signaled(SIGINT) )
        {
        	cout << "SIGINT recieved, exiting..." << endl;
        	BasicExceptionMgr::instance()->log("SIGINT recieved, exiting...");
        	bExit = true;
        }
    }

  }
  catch(SensorProxyException& ex)
  {
    BasicExceptionMgr::instance()->handle( ex );
    PcsecSensorProxy::instance().shutdown();
  }
  catch(PcsecSensorProxyException& ex)
  {
    BasicExceptionMgr::instance()->handle( ex );
    PcsecSensorProxy::instance().shutdown();
  }
  catch(BasicException& ex)
  {
    BasicExceptionMgr::instance()->handle( ex );
    PcsecSensorProxy::instance().shutdown();
  }
  catch(...)
  {
    BasicExceptionMgr::instance()->handle( "Unknown exception" );
    PcsecSensorProxy::instance().shutdown();
  }
}
