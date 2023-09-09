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
 *
 * Description:
 * ObervableEvents contain observables along with geolcation data.
 * This events can come from mobile sensors.
 *
 */
        
#include <sstream>

#include <pcff/BasicExceptionMgr.h>
#include <pcff/event/EventException.h>

#include <pcff/event/ObservableEvent.h>

using namespace std;
using namespace pcff;

ObservableEvent::ObservableEvent()
{
   initMembers();
}

ObservableEvent::ObservableEvent(const ObservableEvent &in)

{
   _bMobileSensor = in._bMobileSensor; // usually false: when false, the proxy and sensor lat,lon are the same
   _sensorproxy_lat = in._sensorproxy_lat;
   _sensorproxy_lon = in._sensorproxy_lon;
   _sensor_lat = in._sensor_lat;
   _sensor_lon = in._sensor_lon;
   _bHaveData = in._bHaveData;
   _serializedDataType = in._serializedDataType;



}

ObservableEvent::~ObservableEvent()
{
}

void ObservableEvent::initMembers()
{
   _obsType = 0;
   _sensorID = 0;
   _bMobileSensor = false;
   _refTimeSec = 0;;
   _refTimeUSec = 0;
   _srcLat = 0.0;
   _srcLon = 0.0;
   _currentLat = 0.0;
   _currentLon = 0.0;
   _dstLat = 0.0;
   _dstLon = 0.0;
   _altitude = 0.0;
   _magnitude = 0.0; //
   _direction = 0.0;
}

void ObservableEvent::serialize(Serializer &s)
{
   Event::serialize(s);
   s.transfer(this->_bMobileSensor);
   s.transfer(this->_ObservableEventID);
   s.transfer(this->_sensorproxy_lat);
   s.transfer(this->_sensorproxy_lon);
   s.transfer(this->_sensor_lat);
   s.transfer(this->_sensor_lon);
   s.transfer(this->_bHaveData);
   s.transfer(this->_serializedDataType);
}

void ObservableEvent::setSensorLoc(float lat, float lon)
{

   _sensor_lat = lat;
   _sensor_lon = lon;

   if (_bMobileSensor == false)
   {
      BasicExceptionMgr::instance()->log(LogLevel::Warn, "Not a mobile sensor");
   }
}

void ObservableEvent::setSensorProxyLoc(float lat, float lon)
{
   _sensorproxy_lat = lat;
   _sensorproxy_lon = lon;
}

string ObservableEvent::getInfo()
{
   ostringstream ss;

   ss << endl;
   ss << "ObservableEventInfo       		:  " << _ObservableEventID << endl;

   return ss.str();
}

void ObservableEvent::setTestData()
{
   struct timeval currtime;

   gettimeofday(&currtime, NULL);
}

void ObservableEvent::getReferenceTime(long &rsec, long &rusec)
{
}

void ObservableEvent::getReferenceLocation(float &lat, float &lon)
{
}
