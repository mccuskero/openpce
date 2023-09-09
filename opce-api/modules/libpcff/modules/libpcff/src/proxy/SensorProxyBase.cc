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
#include <pcff/services/ServicesException.h>

#include <pcff/proxy/SensorProxyException.h>
#include <pcff/proxy/ProxyService.h>

#include <pcff/proxy/SensorProxyBase.h>


using namespace std;
using namespace pcff;

SensorProxyBase::SensorProxyBase()
{
   _service = NULL;
   _sensorID = 0;
   _sensorName = "";
}

void SensorProxyBase::startService()
{
   try
   {
      if (_service != NULL)
      {
         _service->set(this);
         _service->startup();
      }
      else
      {
         throw ServicesException(ServicesException::ServiceObjIsNull, "SensorProxyBase::startService");
      }
   }
   catch (SensorProxyException& ex)
   {
      BasicExceptionMgr::instance()->handle(ex);
      _service->shutdown();
      delete _service;
   }
   catch (BasicException& ex)
   {
      BasicExceptionMgr::instance()->handle(ex);
      if (_service != NULL)
      {
         _service->shutdown();
         delete _service;
         _service = NULL;
      }
   }
   catch (...)
   {
      BasicExceptionMgr::instance()->handle("Unknown exception");
      if (_service != NULL)
      {
         _service->shutdown();
         delete _service;
         _service = NULL;
      }
   }
}

int SensorProxyBase::monitorService()
{
   BasicExceptionMgr::instance()->log(LogLevel::Fine, "Service Status... %d", _service->getServiceState());

   // TODO: Maybe do some processing
   return _service->getServiceState();
}
