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

#include <iostream>

#include <pcff/socket/SocketExceptions.h>

#include <pcff/BasicExceptionMgr.h>
#include <pcff/LogLevel.h>
#include <pcff/SIGTERM_SignalEventHandler.h>
#include <pcff/SIGINT_SignalEventHandler.h>
#include <pcff/SignalHandler.h>
#include <pcff/Daemon.h>

#include <pcsec/network/SnortEvent.h>

#include "ServicesConfig.h"
#include "ExampleAcceptor.h"


using namespace std;
using namespace pcff;
using namespace pcsec;

int main(int argc, char* argv[]) 
{
	LogLevel *myLog = NULL;
	SnortEvent *evt = NULL;
	int   evtCount = 0;
	int   iterationsCount = 0;
	int   logLevel = 0;
	bool  bExit = false;
	
    myLog = new LogLevel(Log::logToFile | Log::logToCrt,
		    1000 );
    
    myLog->init("acceptor.log");
    myLog->logMessage("Opening log");
    
    // set the log level
	ServicesConfig::instance().init();
	ServicesConfig::instance().parseCommandLine(argc, argv);

	logLevel = ServicesConfig::instance().getVerbosityLevel();
    BasicExceptionMgr::instance()->setLog( myLog );
    BasicExceptionMgr::instance()->setLogLevel(logLevel);

	BasicExceptionMgr::instance()->log("========-- Acceptor Example --========");
	
    myLog->logMessage("Initializing Signal Handlers...");
    SignalHandler *signalHandler = SignalHandler::instance();  
    
    // example use of default event signal handler
    signalHandler->registerEventHandler(SIGTERM);
    signalHandler->registerEventHandler(SIGINT);
	
	try 
	{
		int    addrFamily   = ServicesConfig::instance().getAddrFamily();
		int    poolSize     = ServicesConfig::instance().getPoolSize();
		string listenHost 	= ServicesConfig::instance().getDfsHost();
		int    port 		= ServicesConfig::instance().getDfsPort();
		
		BasicExceptionMgr::instance()->log("Initial startup values", LogLevel::Info);
		
		ServicesConfig::instance().showInitialValues();
	
		ExampleAcceptor::instance().init(addrFamily, poolSize, listenHost, port);
		ExampleAcceptor::instance().populate();
		ExampleAcceptor::instance().run();  

		while(!bExit) 
		{
			if ( signalHandler->signaled(SIGTERM) ) 
			{
				cout << "SIGTERM recieved, exiting..." << endl;
				BasicExceptionMgr::instance()->log("SIGTERM recieved, exiting...");
				bExit = true;
			}
			if ( signalHandler->signaled(SIGINT) ) 
			{
				cout << "SIGINT recieved, exiting..." << endl;
				BasicExceptionMgr::instance()->log("SIGINT recieved, exiting...");
				bExit = true;
			}
		}
		  
		BasicExceptionMgr::instance()->log("Shutting down test acceptor example subsystem...", LogLevel::Info);

	}
	catch(SocketException &ex)
	{
		BasicExceptionMgr::instance()->handle(ex);	
	}
	catch(BasicException &ex)
	{
		BasicExceptionMgr::instance()->handle(ex);			
	}
	catch(...)
	{
		BasicExceptionMgr::instance()->log("test: unknown exception caught", LogLevel::Error);			
	}
}
