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
#include <iostream>

#include <pcff/BasicExceptionMgr.h>
#include <pcff/SignalHandler.h>
#include <pcff/InformationList.h>
#include <pcff/proxy/ProxyService.h>
#include <pcff/proxy/ProxyServiceFactory.h>
#include <pcff/services/ServicesException.h>

#include <pcsec/network/SilkProxyServiceFactory.h>

#include "SilkSensorProxyException.h"
#include "SilkSensorProxyConfig.h"

#include "SilkSensorProxy.h"

using namespace std;
using namespace pcff;
using namespace pcsec;

SilkSensorProxy *SilkSensorProxy::_instance = NULL;

SilkSensorProxy &SilkSensorProxy::instance()
{
    if (_instance == NULL)
    {
        _instance = new SilkSensorProxy();
    }

    return *_instance;
}

void SilkSensorProxy::shutdown()
{
    if (_instance == NULL)
    {
        delete _instance;
    }
}

SilkSensorProxy::SilkSensorProxy()
{
}

SilkSensorProxy::~SilkSensorProxy()
{
}

bool SilkSensorProxy::initialize()
{
    bool retVal = true;
    ProxyServiceFactory *factory = NULL;
    InformationList createInfo;
    int serviceType = SilkSensorProxyConfig::instance().service();

    try
    {
        switch (serviceType)
        {
        case SilkSensorProxyConfig::SilkService :
                    factory = new SilkProxyServiceFactory();
            SilkSensorProxyConfig::instance().getCreateInfo(SilkSensorProxyConfig::SilkService, createInfo);
            _service = factory->create(createInfo);
            break;
        case SilkSensorProxyConfig::Unknown :
                    cout << "Unknown service" << endl;
            break;
        default:
            cout << "Unknown service" << endl;
            break;
        }

        if (_service != NULL)
        {
            if (SilkSensorProxyConfig::instance().batch())
            {
                _service->setBatchMode();
            }
            else if (SilkSensorProxyConfig::instance().archive())
            {
                _service->setArchiveMode();
            }
            else if (SilkSensorProxyConfig::instance().test())
            {
                _service->setTestMode();
            }
            else
            {
                _service->setContinualMode();
            }

            _service->initialize();
        }
    }
    catch (BasicException& ex)
    {
        BasicExceptionMgr::instance()->handle(ex);
        _service->shutdown();
        delete _service;
        _service = NULL;
        retVal = false;
    }
    catch (...)
    {
        BasicExceptionMgr::instance()->handle("Unknown exception");
        _service->shutdown();
        delete _service;
        _service = NULL;
        retVal = false;
    }

    return retVal;
}

int SilkSensorProxy::monitorService()
{
    int retVal = 0;

    BasicExceptionMgr::instance()->log("SilkSensorProxy::monitorService - Service Status...", LogLevel::Debug);

    if (_service != NULL)
    {
        retVal = _service->getServiceState();
    }

    return retVal;
}
