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

#include <sstream>

#include <pcff/thread/Thread.h>
#include <pcff/InformationList.h>
#include <pcff/BasicExceptionMgr.h>

#include <pcff/event/Event.h>
#include <pcff/event/EventException.h>
#include <pcff/event/proxy/EventIngestor.h>
#include <pcff/event/proxy/EventProcessor.h>
#include <pcff/proxy/SensorProxyBase.h>

#include <pcff/proxy/ProxyService.h>

using namespace std;
using namespace pcff;

ProxyService::ProxyService()
{
	initMembers();
}


ProxyService::~ProxyService()
{
	if (_eventProcessor != NULL)
	{
		delete _eventProcessor;
		_eventProcessor = NULL;
	}

	if (_thread != NULL)
	{
		delete _thread;
		_thread = NULL;
	}

	if (_eventIngestor !=  NULL)
	{
		delete _eventIngestor;
		_eventIngestor = NULL;
	}

	if (_proxyServiceInfo != NULL)
	{
		delete _proxyServiceInfo;
		_proxyServiceInfo = NULL;
	}
}

void ProxyService::initMembers()
{
	_mode = ProxyService::Continual;
	_eventIngestor = NULL;
	_eventProcessor = NULL;
	_proxyServiceInfo = NULL;
	_thread = NULL;
	_sensorProxy = NULL;
}

void ProxyService::initialize()
{
	_thread = new Thread( (THREAD_ENTRY_POINT)ProxyService::mainProc,
                         (void*)this);
	_thread->init();

	// initialize the _eventIngestor
	if (_eventIngestor != NULL)
	{
		if (_mode == ProxyService::Test)
		{
			_eventIngestor->setTestMode();
		}
		else if (_mode == ProxyService::Batch)
		{
			_eventIngestor->setBatchMode();
		}
		else if (_mode == ProxyService::Continual)
		{
			_eventIngestor->setContinualMode();
		}
		else
		{
			throw EventException(EventException::InvalidProxyMode, "ProxyService::initialize");
		}

		_eventIngestor->initialize();
	}
	else
	{
		throw EventException(EventException::IngestorIsNull, "ProxyService::initialize");
	}

	// initialize _eventProcessor
	if (_eventProcessor != NULL)
	{
		_eventProcessor->initialize();
	}
	else
	{
		throw EventException(EventException::ProcessorIsNull, "ProxyService::initialize");
	}

}

void ProxyService::startup()
{
	_thread->start();
}

void ProxyService::shutdown()
{
	_thread->setFinished();
}

void ProxyService::run()
{
	if (_mode == ProxyService::Test)
	{
		testProcess();
	}
	else if (_mode == ProxyService::Archive)
	{
		archiveProcess();
	}
	else if (_mode == ProxyService::Batch)
	{
		batchProcess();
	}
	else if (_mode == ProxyService::Continual)
	{
		continualProcess();
	}
	else
	{
		throw EventException(EventException::InvalidProxyMode, "ProxyService::run");
	}
}

void ProxyService::mainProc(ProxyService *objIn)
{
	ProxyService *proxySvc = objIn;

	proxySvc->run();
}

void ProxyService::process(Event &evt)
{
	if (_eventProcessor != NULL)
	{
		if (_sensorProxy != NULL)
		{
			evt.setDeviceID( _sensorProxy->getSensorID() );
		}
		else
		{
	        BasicExceptionMgr::instance()->log("ProxyService::process: _sensorProxy not set", LogLevel::Warn);
		}

		_eventProcessor->process(evt);
	}
	else
	{
        BasicExceptionMgr::instance()->log("ProxyService::process: _eventProcessor not set", LogLevel::Warn);
	}
}

void ProxyService::testProcess()
{
	try
	{
	    BasicExceptionMgr::instance()->log("ProxyService::testProcess - ingesting...", LogLevel::Info);
		_eventIngestor->ingest();
		shutdown();
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

void ProxyService::batchProcess()
{
	try
	{
		BasicExceptionMgr::instance()->log("ProxyService::batchProcess ingesting...", LogLevel::Info);
		_eventIngestor->ingest();
		shutdown();                // need to gracefully shutdown the thread
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

void ProxyService::archiveProcess()
{
	try
	{
		BasicExceptionMgr::instance()->log("ProxyService::archiveProcess - ingesting...", LogLevel::Info);
		_eventIngestor->ingest();
		shutdown();            // need to gracefully shutdown the thread
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

void ProxyService::continualProcess()
{
	try
	{
		BasicExceptionMgr::instance()->log("ProxyService::continualProcess - ingesting...", LogLevel::Info);
		_eventIngestor->ingest();
		shutdown();         // need to gracefully shutdown the thread
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

stringstream ProxyService::getStatus()
{

}

