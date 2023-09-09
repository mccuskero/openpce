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

#include <pcff/socket/SocketExceptions.h>

#include <pcff/BasicExceptionMgr.h>
#include <pcff/LogLevel.h>
#include <pcff/SIGTERM_SignalEventHandler.h>
#include <pcff/SIGINT_SignalEventHandler.h>
#include <pcff/SignalHandler.h>
#include <pcff/Daemon.h>

#include "TestEvent.h"
#include "ServicesConfig.h"
#include "ExampleConnector.h"



using namespace std;
using namespace pcff;

int main(int argc, char* argv[]) 
{
	LogLevel *myLog = NULL;
	ExampleConnector *connector = NULL;
	TestEvent *evt = NULL;
	int   evtCount = 0;
	int   iterationsCount = 0;
	int   logLevel = 0;
	
    myLog = new LogLevel(Log::logToFile | Log::logToCrt,
		    1000 );
    
    myLog->init("connector.log");
    myLog->logMessage("Opening log");
    
    // set the log level
	ServicesConfig::instance().init();
	ServicesConfig::instance().parseCommandLine(argc, argv);
	
    logLevel = ServicesConfig::instance().getVerbosityLevel();
    myLog->setLogLevel(logLevel);

    BasicExceptionMgr::instance()->setLog( myLog );


	BasicExceptionMgr::instance()->log("========-- Connector Example --========");
	
	try 
	{		
		string host 		= ServicesConfig::instance().getDfsHost();
		int    port 		= ServicesConfig::instance().getDfsPort();
		int    addrFamily   = ServicesConfig::instance().getAddrFamily();
		int    numEvents 	= ServicesConfig::instance().getNumEvents();
		int    numIterations = ServicesConfig::instance().getNumIterations();
		unsigned long rawTestSize = ServicesConfig::instance().getRawTestSize();
		unsigned char *buf = NULL;
		
		BasicExceptionMgr::instance()->log("Initial startup values", LogLevel::Info);
		
		ServicesConfig::instance().showInitialValues();
				
		connector = new ExampleConnector();
		connector->initialize();
		connector->connect(host.c_str(), port, addrFamily);
		
		if (rawTestSize > 0)
		{
			buf = new unsigned char[rawTestSize];
		}
		
		while( iterationsCount < numIterations)
		{
			evtCount = 0;
			
			while( evtCount < numEvents)
			{
				evt = new TestEvent();
				evt->setRawEvent(buf, rawTestSize);
				connector->process(Message::Test, *evt);
				delete evt;
				evtCount++;
			}

			sleep(1);
			iterationsCount++;
		}		
				
		if (rawTestSize > 0)
		{
			delete [] buf;
		}
		
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
		BasicExceptionMgr::instance()->log("Connector: unknown exception caught", LogLevel::Error);			
	}
}
