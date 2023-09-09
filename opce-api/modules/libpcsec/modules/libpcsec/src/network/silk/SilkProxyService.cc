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

#include <pcsec/network/SilkFileIngestor.h>
#include <pcsec/network/SilkDatastoreIngestor.h>
#include <pcsec/network/SilkDefaultEventProcessor.h>
#include <pcsec/network/SilkSocketOutputEventProcessor.h>
#include <pcsec/network/SilkProxyService.h>

using namespace std;
using namespace pcff;
using namespace pcsec;

// 
// TODO: create the ability to archive data to be used later
//  
//

SilkProxyService::SilkProxyService()
{
	
}

SilkProxyService::~SilkProxyService()
{
}

void SilkProxyService::process(Event &evt)
{
	if (_eventProcessor != NULL)
	{
		_eventProcessor->process(evt);
	}
	else
	{
        BasicExceptionMgr::instance()->log("SilkProxyService::process: _eventProcessor not set", LogLevel::Warn);		
	}
}

void SilkProxyService::testProcess()
{	
	try
	{
	    BasicExceptionMgr::instance()->log("SilkProxyService::testProcess - ingesting...", LogLevel::Info);
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

void SilkProxyService::batchProcess()
{
	try
	{
		BasicExceptionMgr::instance()->log("SilkProxyService::batchProcess ingesting...", LogLevel::Info);
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

void SilkProxyService::archiveProcess()
{
	try
	{
		BasicExceptionMgr::instance()->log("SilkProxyService::archiveProcess - ingesting...", LogLevel::Info);
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

void SilkProxyService::continualProcess()
{
	try
	{
		BasicExceptionMgr::instance()->log("SilkProxyService::continualProcess - ingesting...", LogLevel::Info);
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

stringstream SilkProxyService::getStatus()
{
	
}



