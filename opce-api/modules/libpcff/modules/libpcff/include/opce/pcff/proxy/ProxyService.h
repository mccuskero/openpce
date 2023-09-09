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


#ifndef __ProxyService_h
#define __ProxyService_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sstream>

#include <pcff/thread/Thread.h>
#include <pcff/InformationList.h>

//
// In the future a ProxyService may have more than
// one EventProcessor (derived class)

namespace pcff
{

class Event;
class EventIngestor;
class EventProcessor;
class SensorProxyBase;

class ProxyService
{
public:
    ProxyService();
    virtual ~ProxyService();

private:
    void initMembers();

public:
    virtual void initialize();
    virtual void startup();
    virtual void shutdown();
    virtual void run();

public:

    void setInitialized()
    {
        _state = ProxyService::Initialized;
    }

    void setRunning()
    {
        _state = ProxyService::Running;
    }

    void setPaused()
    {
        _state = ProxyService::Paused;
    }

    void setFinished()
    {
        _state = ProxyService::Finished;
    }

    void setShutdown()
    {
        _state = ProxyService::Shutdown;
    }

    bool isDone()
    {
        return (((_state == ProxyService::Finished) || (_state == ProxyService::Shutdown)) ? true : false);
    }

    bool isFinished()
    {
        return (_state == ProxyService::Finished) ? true : false;
    }

    bool isShutdown()
    {
        return (_state == ProxyService::Shutdown) ? true : false;
    }

    int getServiceState()
    {
        return _state;
    }

public:

    void setTestMode()
    {
        _mode = ProxyService::Test;
    }

    void setBatchMode()
    {
        _mode = ProxyService::Batch;
    }

    void setContinualMode()
    {
        _mode = ProxyService::Continual;
    }

    void setArchiveMode()
    {
        _mode = ProxyService::Archive;
    }

    int getMode()
    {
        return _mode;
    }

public:
    virtual void process(Event &evt);

public:

    virtual void set(EventIngestor *evtIngestor)
    {
        _eventIngestor = evtIngestor;
    }

    virtual void set(EventProcessor *evtProcessor)
    {
        _eventProcessor = evtProcessor;
    }

    virtual void set(SensorProxyBase *sensorProxy)
    {
        _sensorProxy = sensorProxy;
    }

public:
    virtual std::stringstream getStatus(); // TODO: output to log file (verbosity)

public:
    virtual void testProcess();
    virtual void batchProcess();
    virtual void continualProcess();
    virtual void archiveProcess(); // used to create archive event files for testing

public:
    static void mainProc(ProxyService *proxyService);

public:

    enum ProxyMode
    {
        Unknown = -1,
        Test,
        Batch,
        Continual,
        Archive
    };

    enum ProxyState
    {
        Uninitialized = -1,
        Initialized,
        Running,
        Paused,
        Finished,
        Shutdown
    };

protected:
    EventIngestor *_eventIngestor;
    EventProcessor *_eventProcessor;
    int _mode;
    Thread *_thread;
    InformationList *_proxyServiceInfo;
    SensorProxyBase *_sensorProxy;
    int _state;
};

};


#endif
