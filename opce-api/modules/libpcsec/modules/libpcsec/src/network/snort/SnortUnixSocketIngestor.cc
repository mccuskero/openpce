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
 /*
 * \class SnortUnixSocketIngestor
 * \brief Ingests snort alerts from the -A unixsock option
 */

#include <iostream>

#include <pcff/DumpBuffer.h>
#include <pcff/BasicException.h>
#include <pcff/BasicExceptionMgr.h>

#include <pcff/proxy/ProxyService.h>
#include <pcsec/network/SnortEvent.h>

#include <pcsec/network/SnortUnixSocketIngestor.h>

using namespace std;
using namespace pcff;
using namespace pcsec;

SnortUnixSocketIngestor::SnortUnixSocketIngestor()
{
	initMembers();
}

SnortUnixSocketIngestor::~SnortUnixSocketIngestor()
{
}

void SnortUnixSocketIngestor::initMembers()
{
	_bIngest = false;
}

void SnortUnixSocketIngestor::initialize()
{
	switch(_mode)
	{
	case EventIngestor::Test:
		break;
	case EventIngestor::Batch:
		break;
	case EventIngestor::Continuous:
		// TODO: initialize connection to snort
		initializeContinuous();
		break;
	default:
		break;
	}

}

void SnortUnixSocketIngestor::initializeContinuous()
{
	// create the unix domain socket (pathname?)
    BasicExceptionMgr::instance()->log(LogLevel::Info, "SnortUnixSocketIngestor::initializeContinuous - sockPath :%s", _unixSocketpathname.c_str());
	_serverUnixDSocket.init();

	// call unlink() before bind() to remove the socket if it already exists
	unlink(_unixSocketpathname.c_str());

	_serverUnixDSocket.bind(_unixSocketpathname.c_str());
}

void SnortUnixSocketIngestor::ingest()
{
	_bIngest = true;

	cout << "SnortUnixSocketIngestor::initializeContinuous" << endl;

	switch(_mode)
	{
	case EventIngestor::Test:
		testIngest();
		break;
	case EventIngestor::Batch:
		batchIngest();
		break;
	case EventIngestor::Continuous:
		continuousIngest();
		break;
	default:
		break;
	}
}

void SnortUnixSocketIngestor::testIngest()
{
	SnortEvent *evt = NULL;
    while (true)
    {
    	BasicExceptionMgr::instance()->log("SnortUnixSocketIngestor::testIngest: creating test event...", LogLevel::Info);
    	evt = new SnortEvent();
    	evt->setTestData();

    	_proxyService->process(*evt);
    	sleep(5); // sleep 5 secs
    	delete evt;
    }
}

//
// This function works in conjunction with the archive options
// of an eventprocessor. First, the event processor must create
// an archived file, then this can be used to ingest the archived
// file. May need to use  "magic" number to ensure that the file
// data can be parsed. (magic, version snort, version file)
//
void SnortUnixSocketIngestor::batchIngest()
{
	SnortEvent *evt = NULL;

	while (true)
    {
        BasicExceptionMgr::instance()->log("SnortUnixSocketIngestor::batchIngest - sleeping...", LogLevel::Info);
     	sleep(5); // sleep 5 secs
    }
}

void SnortUnixSocketIngestor::continuousIngest()
{
	SnortEvent *evt = NULL;
	int bytesRcvd = 0;
	static char buf[MAX_DG_SIZE];  // TODO:

	memset(buf, '0', MAX_DG_SIZE);

	try
	{
	    DumpBuffer::instance().init();

	    // TODO: capture signal, TERM, then tell proxy service to shutdown, top ingesting
	    while (_bIngest == true)
		{
		    BasicExceptionMgr::instance()->log("SnortUnixSocketIngestor::continuousIngest - starting...", LogLevel::Info);
		    bytesRcvd = 0;
		    bytesRcvd = _serverUnixDSocket.receiveDatagram(buf, MAX_DG_SIZE);
		    BasicExceptionMgr::instance()->log(LogLevel::Info, "SnortUnixSocketIngestor::continuousIngest - received datagram : bytesRcvd: %d", bytesRcvd);

		    if (BasicExceptionMgr::instance()->getLogLevel() >= LogLevel::Info)
		    {
			    DumpBuffer::instance().dump(bytesRcvd, buf);
		    }

			evt = new SnortEvent();
			evt->initialize(bytesRcvd, buf);

			_proxyService->process(*evt);

			delete evt;

			memset(buf, '0', bytesRcvd);
		}

	    BasicExceptionMgr::instance()->log("SnortUnixSocketIngestor::continuousIngest - exiting...", LogLevel::Info);
    }
	catch(BasicException& ex)
	{
	  BasicExceptionMgr::instance()->handle( ex );
	}
	catch(...)
	{
	  BasicExceptionMgr::instance()->handle( "Unknown exception" );
	}
}

