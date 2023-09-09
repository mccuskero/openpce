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
#include <pcff/proxy/SensorProxyException.h>
#include <pcff/services/ServicesException.h>

#include <pcsec/network/SnortProxyServiceFactory.h>
#include <pcsec/network/PcapProxyServiceFactory.h>
#include <pcsec/SensorProxyConfig.h>

#include <pcsec/PcsecSensorProxy.h>


using namespace std;
using namespace pcff;
using namespace pcsec;

PcsecSensorProxy *PcsecSensorProxy::_instance = NULL;

PcsecSensorProxy &PcsecSensorProxy::instance()
{
	if (_instance == NULL)
	{
		_instance = new PcsecSensorProxy();
	}

	return *_instance;
}

void PcsecSensorProxy::shutdown()
{
	if (_instance == NULL)
	{
		delete _instance;
	}
}

PcsecSensorProxy::PcsecSensorProxy()
{
	_service = NULL;
	_sensorID = 0;
	_sensorName = "";
}

PcsecSensorProxy::~PcsecSensorProxy()
{
}


bool PcsecSensorProxy::initialize()
{
	bool                 retVal = true;
	ProxyServiceFactory *factory = NULL;
	InformationList     createInfo;
	int serviceType 	= SensorProxyConfig::instance().service();

	try
	{
		switch(serviceType)
		{
		case SensorProxyConfig::SnortService :
			factory = new SnortProxyServiceFactory();
			SensorProxyConfig::instance().getCreateInfo(SensorProxyConfig::SnortService, createInfo);
			_service = factory->create(createInfo);
            BasicExceptionMgr::instance()->log(LogLevel::Info, "PcsecSensorProxy::initialize: Creating Snort Service");
			break;
		case SensorProxyConfig::PcapService :
			factory = new PcapProxyServiceFactory();
			SensorProxyConfig::instance().getCreateInfo(SensorProxyConfig::PcapService, createInfo);
			_service = factory->create(createInfo);
            BasicExceptionMgr::instance()->log(LogLevel::Info, "PcsecSensorProxy::initialize: Creating Pcap Service");
			break;
		case SensorProxyConfig::NepenthesService :
			break;
		case SensorProxyConfig::SiLKService :
			break;
		case SensorProxyConfig::Unknown :
			cout << "Unknown service" << endl;
			break;
		default:
			cout << "Unknown service" << endl;
			break;
		}

		if (_service != NULL)
		{
			if (SensorProxyConfig::instance().batch() )
			{
				_service->setBatchMode();
			}
			else if (SensorProxyConfig::instance().archive())
			{
				_service->setArchiveMode();
			}
			else if (SensorProxyConfig::instance().test())
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
	catch(BasicException& ex)
	{
	  BasicExceptionMgr::instance()->handle( ex );
	  _service->shutdown();
	  delete _service;
	  _service = NULL;
	  retVal = false;
	}
	catch(...)
	{
	  BasicExceptionMgr::instance()->handle( "Unknown exception" );
	  _service->shutdown();
	  delete _service;
	  _service = NULL;
	  retVal = false;
	}

	return retVal;
}
