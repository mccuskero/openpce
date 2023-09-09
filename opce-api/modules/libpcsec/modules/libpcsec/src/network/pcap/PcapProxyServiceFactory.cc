/* 
 * File:   pcapproxyservicefactory.cc
 * Author: owenm
 * 
 * Created on April 17, 2009, 7:33 PM
 */

#include <stdlib.h>

#include <iostream>

#include <pcff/InformationList.h>
#include <pcff/proxy/ProxyService.h>

#include <pcsec/PcsecSensorProxyException.h>
#include <pcsec/SensorProxyConfig.h>
#include <pcsec/network/PcapException.h>
#include <pcsec/network/PcapLiveIngestor.h>
#include <pcsec/network/PcapOfflineIngestor.h>
#include <pcsec/network/PcapDefaultEventProcessor.h>

#include <pcsec/network/PcapProxyServiceFactory.h>

using namespace std;
using namespace pcff;
using namespace pcsec;

PcapProxyServiceFactory::PcapProxyServiceFactory()
{
}

PcapProxyServiceFactory::~PcapProxyServiceFactory()
{
}

ProxyService *PcapProxyServiceFactory::create(InformationList &createInfo)
{
    ProxyService *service = NULL;
    EventIngestor *evtIngestor = NULL;
    EventProcessor *evtProcessor = NULL;
    bool batch = false;

    service = new ProxyService();

    evtIngestor = createIngestor(createInfo);
    evtProcessor = createProcessor(createInfo);

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

EventIngestor *PcapProxyServiceFactory::createIngestor(InformationList &createInfo)
{
    EventIngestor *eventIngestor = NULL;
    // TODO: what happens if not in list??? need to get the AttributeValuePair,
    // set Unitialized to -1
    string ingestorType = createInfo.get("ingestorType").getValue();

    if (ingestorType.compare("PcapLive") == 0)
    {
        eventIngestor = createPcapLiveIngestor(createInfo);
    }
    else if (ingestorType.compare("PcapOffline") == 0)
    {
        eventIngestor = createPcapOfflineIngestor(createInfo);
    }
    else if (ingestorType.compare("Unknown") == 0)
    {
        // TODO: throw exception
    }
    else
    {
        // TODO: throw exception
    }

    return eventIngestor;
}

EventProcessor *PcapProxyServiceFactory::createProcessor(InformationList &createInfo)
{
    EventProcessor *eventProcessor = NULL;

    // TODO: what happens if not in list??? need to get the AttributeValuePair,
    // set Unitialized to -1
    string eProcessorType = createInfo.get("processorType").getValue();

    if (eProcessorType.compare("PcapDefaultProcessor") == 0)
    {
        eventProcessor = new PcapDefaultEventProcessor();
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

bool PcapProxyServiceFactory::batchMode(InformationList &createInfo)
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

bool PcapProxyServiceFactory::testMode(InformationList &createInfo)
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

EventIngestor *PcapProxyServiceFactory::createPcapLiveIngestor(InformationList &createInfo)
{
    PcapLiveIngestor *ingestor = new PcapLiveIngestor();
    string devName;
    string filterExp;
    bool useCallbackMethod = true;

    devName = createInfo.get("deviceName").getValue();
    ingestor->setDeviceName(devName);

    filterExp = createInfo.get("filter").getValue();
    ingestor->setBPF(filterExp);

    useCallbackMethod = createInfo.get("useCallback").getValueAsBool();

    cout << "PcapProxyServiceFactory::createPcapLiveIngestor: useCallbackMethod " << useCallbackMethod << endl;

    if (useCallbackMethod == true)
    {
       ingestor->setUseCallback();
    }
    else
    {
       ingestor->resetUseCallback();
    }

    return ingestor;
}

EventIngestor *PcapProxyServiceFactory::createPcapOfflineIngestor(InformationList &createInfo)
{
    PcapOfflineIngestor *ingestor = new PcapOfflineIngestor();
    string pcapFilename;
    string filterExp;
    bool useCallbackMethod = true;

    pcapFilename = createInfo.get("filename").getValue();

    if (pcapFilename.length() == 0)
    {
        throw PcapException(PcapException::ErrorPcapFile, "PcapProxyServiceFactory::createPcapOfflineIngestor");
    }

    ingestor->setFilename(pcapFilename);

    filterExp = createInfo.get("filter").getValue();
    ingestor->setBPF(filterExp);

    useCallbackMethod = createInfo.get("useCallback").getValueAsBool();

    cout << "PcapProxyServiceFactory::createPcapLiveIngestor: useCallbackMethod " << useCallbackMethod << endl;

    if (useCallbackMethod == true)
    {
       ingestor->setUseCallback();
    }
    else
    {
       ingestor->resetUseCallback();
    }

    return ingestor;
}
