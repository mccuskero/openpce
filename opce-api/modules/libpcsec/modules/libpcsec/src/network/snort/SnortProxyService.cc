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

#include <pcff/BasicException.h>
#include <pcff/BasicExceptionMgr.h>

#include <pcsec/network/SnortUnifiedFileIngestor.h>
#include <pcsec/network/SnortUnixSocketIngestor.h>
#include <pcsec/network/SnortDefaultEventProcessor.h>
#include <pcsec/network/SnortProxyService.h>

using namespace std;
using namespace pcff;
using namespace pcsec;

// 
// TODO: create the ability to archive data to be used later
//  
//

SnortProxyService::SnortProxyService()
{
	
}

SnortProxyService::~SnortProxyService()
{
}

void SnortProxyService::process(Event &evt)
{
	if (_eventProcessor != NULL)
	{
		_eventProcessor->process(evt);
	}
	else
	{
        BasicExceptionMgr::instance()->log("SnortProxyService::process: _eventProcessor not set", LogLevel::Warn);		
	}
}

void SnortProxyService::testProcess()
{	
	try
	{
	    BasicExceptionMgr::instance()->log("SnortProxyService::testProcess - ingesting...", LogLevel::Info);
		_eventIngestor->ingest();			
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

void SnortProxyService::batchProcess()
{
	try
	{
		BasicExceptionMgr::instance()->log("SnortProxyService::batchProcess ingesting...", LogLevel::Info);
		_eventIngestor->ingest();
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

void SnortProxyService::archiveProcess()
{
	try
	{
		BasicExceptionMgr::instance()->log("SnortProxyService::archiveProcess - ingesting...", LogLevel::Info);
		_eventIngestor->ingest();
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

void SnortProxyService::continualProcess()
{
	try
	{
		BasicExceptionMgr::instance()->log("SnortProxyService::continualProcess - ingesting...", LogLevel::Info);
		_eventIngestor->ingest();	
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

stringstream SnortProxyService::getStatus()
{
	
}



