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
#include <pcff/proxy/ProxyServiceFactoryException.h>
#include <pcff/proxy/ProxyService.h>

#include <pcsec/SensorProxyConfig.h>
#include <pcsec/network/SilkFileIngestor.h>
#include <pcsec/network/SilkDatastoreIngestor.h>
#include <pcsec/network/SilkDefaultEventProcessor.h>
#include <pcsec/network/SilkSocketOutputEventProcessor.h>

#include <pcsec/network/SilkProxyServiceFactory.h>


using namespace std;
using namespace pcff;
using namespace pcsec;

SilkProxyServiceFactory::SilkProxyServiceFactory()
{
}

SilkProxyServiceFactory::~SilkProxyServiceFactory()
{
}

ProxyService *SilkProxyServiceFactory::create(InformationList &createInfo)
{
    ProxyService *service = NULL;
    EventIngestor *evtIngestor = NULL;
    EventProcessor *evtProcessor = NULL;
    bool batch = false;

    service = new ProxyService();

    evtIngestor = createIngestor(createInfo);
    evtProcessor = createProcessor(createInfo);

    if (evtIngestor == NULL)
    {
        throw ProxyServiceFactoryException(ProxyServiceFactoryException::IngestorIsNull);
    }
    else if (evtProcessor == NULL)
    {
        throw ProxyServiceFactoryException(ProxyServiceFactoryException::ProcessorIsNull);
    }

    evtIngestor->set(service);
    evtProcessor->set(service);

    if (testMode(createInfo) == true)
    {
        evtIngestor->setTestMode(); // TODO: check to see of continual is default!!!
        service->setTestMode();
    }
    else if (batchMode(createInfo) == true)
    {
        evtIngestor->setBatchMode(); // TODO: check to see of continual is default!!!
        service->setBatchMode();
    }

    service->set(evtIngestor);
    service->set(evtProcessor);

    return service;
}

EventIngestor *SilkProxyServiceFactory::createIngestor(InformationList &createInfo)
{
    EventIngestor *eventIngestor = NULL;
    // TODO: what happens if not in list??? need to get the AttributeValuePair,
    // set Unitialized to -1
    string eIngestorType = createInfo.get("ingestor").getValue();

    if (eIngestorType.compare("file") == 0)
    {
        eventIngestor = createSilkFileIngestor(createInfo);
    }
    else if (eIngestorType.compare("datastore") == 0)
    {
        eventIngestor = createSilkDatastoreIngestor(createInfo);
    }
    else if (eIngestorType.compare("Unknown") == 0)
    {
        throw ProxyServiceFactoryException(ProxyServiceFactoryException::UnknownIngestorType);
    }
    else
    {
        throw ProxyServiceFactoryException(ProxyServiceFactoryException::UnknownIngestorType);
    }

    return eventIngestor;
}

EventProcessor *SilkProxyServiceFactory::createProcessor(InformationList &createInfo)
{
    EventProcessor *eventProcessor = NULL;

    // TODO: what happens if not in list??? need to get the AttributeValuePair,
    // set Unitialized to -1
    string eProcessorType = createInfo.get("processor").getValue();

    if (eProcessorType.compare("default") == 0)
    {
        eventProcessor = new SilkDefaultEventProcessor();
    }
    else if (eProcessorType.compare("socket") == 0)
    {
        eventProcessor = new SilkSocketOutputEventProcessor();
    }
    else if (eProcessorType.compare("Unknown") == 0)
    {
        throw ProxyServiceFactoryException(ProxyServiceFactoryException::UnknownProcessorType);
    }
    else
    {
        throw ProxyServiceFactoryException(ProxyServiceFactoryException::UnknownProcessorType);
    }

    return eventProcessor;
}

bool SilkProxyServiceFactory::batchMode(InformationList &createInfo)
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

bool SilkProxyServiceFactory::testMode(InformationList &createInfo)
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

EventIngestor *SilkProxyServiceFactory::createSilkFileIngestor(InformationList &createInfo)
{
    SilkFileIngestor *ingestor = new SilkFileIngestor();
    string pathname;
    string filename;

    pathname = createInfo.get("data-store").getValue();
    ingestor->setDatastore(pathname.c_str());

    filename = createInfo.get("data-file").getValue();
    ingestor->setFilename(filename.c_str());

    ingestor->initialize();

    return ingestor;
}

EventIngestor *SilkProxyServiceFactory::createSilkDatastoreIngestor(InformationList &createInfo)
{
    SilkDatastoreIngestor *ingestor = new SilkDatastoreIngestor();
    string pathname;
    string logDir;

    pathname = createInfo.get("data-store").getValue();
    ingestor->setDatastorePathname(pathname.c_str());

    return ingestor;
}
