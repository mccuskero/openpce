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

//
// Just create Ingestors and Process, let the ProxyService class
// call the initialization member
//

#include <stdlib.h>

#include <pcff/InformationList.h>
#include <pcff/proxy/ProxyService.h>

#include <pcsec/PcsecSensorProxyException.h>
#include <pcsec/SensorProxyConfig.h>
#include <pcsec/network/SnortUnifiedFileIngestor.h>
#include <pcsec/network/SnortUnixSocketIngestor.h>
#include <pcsec/network/SnortDefaultEventProcessor.h>

#include <pcsec/network/SnortProxyServiceFactory.h>


using namespace std;
using namespace pcff;
using namespace pcsec;

SnortProxyServiceFactory::SnortProxyServiceFactory()
{
}

SnortProxyServiceFactory::~SnortProxyServiceFactory()
{
}


ProxyService *SnortProxyServiceFactory::create(InformationList &createInfo)
{
	ProxyService   *service = NULL;
	EventIngestor  *evtIngestor    	= NULL;
	EventProcessor *evtProcessor	= NULL;
	bool batch 						= false;

	service = new ProxyService();

	evtIngestor    	= createIngestor(createInfo);
	evtProcessor	= createProcessor(createInfo);

	evtIngestor->set(service);
	evtProcessor->set(service);

	if (testMode(createInfo) == true)
	{
		evtIngestor->setTestMode();  // TODO: check to see of continual is default!!!
		service->setTestMode();
	}
	else if (batchMode(createInfo) == true)
	{
		evtIngestor->setBatchMode();  // TODO: check to see of continual is default!!!
		service->setBatchMode();
	}

	service->set(evtIngestor);
	service->set(evtProcessor);

	return service;
}

EventIngestor *SnortProxyServiceFactory::createIngestor(InformationList &createInfo)
{
	EventIngestor *eventIngestor = NULL;
	// TODO: what happens if not in list??? need to get the AttributeValuePair,
	// set Unitialized to -1
	string eIngestorType = createInfo.get("ingestorType").getValue();

	if (eIngestorType.compare("SnortUnifiedFile") == 0)
	{
		eventIngestor = createSnortUnifiedFileIngestor(createInfo);
	}
	else if (eIngestorType.compare("SnortUnixSocket") == 0)
	{
		eventIngestor = createSnortUnixSocketIngestor(createInfo);
	}
	else if (eIngestorType.compare("Unknown") == 0)
	{
		// TODO: throw exception
	}
	else
	{
		// TODO: throw exception
	}

	return eventIngestor;
}

EventProcessor *SnortProxyServiceFactory::createProcessor(InformationList &createInfo)
{
	EventProcessor *eventProcessor = NULL;

	// TODO: what happens if not in list??? need to get the AttributeValuePair,
	// set Unitialized to -1
	string eProcessorType = createInfo.get("processorType").getValue();

	if (eProcessorType.compare("SnortDefaultProcessor") == 0)
	{
		eventProcessor = new SnortDefaultEventProcessor();
	}
	else if (eProcessorType.compare("Unknown") == 0)
	{
		// TODO: throw exception
	}
	else
	{
		// TODO: throw exception
	}

	return eventProcessor;
}

bool SnortProxyServiceFactory::batchMode(InformationList &createInfo)
{
	bool retVal = false;
	// TODO: what happens if not in list??? need to get the AttributeValuePair,
	// set Unitialized to -1

	string batch = createInfo.get("batch").getValue();

	if (batch.compare("true") == 0)
	{
		retVal = true;
	}
	else
	{
		retVal = false;
	}

	return retVal;
}

bool SnortProxyServiceFactory::testMode(InformationList &createInfo)
{
	bool retVal = false;
	// TODO: what happens if not in list??? need to get the AttributeValuePair,
	// set Unitialized to -1
	string test = createInfo.get("test").getValue();

	if (test.compare("true") == 0)
	{
		retVal = true;
	}
	else
	{
		retVal = false;
	}

	return retVal;
}


EventIngestor *SnortProxyServiceFactory::createSnortUnifiedFileIngestor(InformationList &createInfo)
{
	SnortUnifiedFileIngestor *ingestor = new SnortUnifiedFileIngestor();
	string pathname;
	string logDir;

	// TODO: test dir to see if it exists
	pathname = createInfo.get("spool_dir").getValue();
	ingestor->setPathname(pathname.c_str());

	logDir = createInfo.get("snort_log_dir").getValue();
	ingestor->setLogDir(logDir.c_str());

	return ingestor;
}

EventIngestor *SnortProxyServiceFactory::createSnortUnixSocketIngestor(InformationList &createInfo)
{
	SnortUnixSocketIngestor *ingestor = new SnortUnixSocketIngestor();
	string unixSocketPathname;

	unixSocketPathname = createInfo.get("unixSocketPathname").getValue();

	if (unixSocketPathname.length() == 0)
	{
		throw PcsecSensorProxyException(PcsecSensorProxyException::InvalidUnixSocketPathname, "SnortProxyServiceFactory::createSnortUnixSocketIngestor");
	}

	ingestor->setUnixSocketPathname(unixSocketPathname.c_str());

	return ingestor;
}
