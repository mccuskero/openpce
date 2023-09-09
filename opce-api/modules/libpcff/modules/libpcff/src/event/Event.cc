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
 * Events originate from devices. Devices can be sensors. If the device is a sensor,
 * then the sensor ID, will be the device id.
 *
 */

#include <sstream>

#include <pcff/BasicExceptionMgr.h>

#include <pcff/event/EventException.h>
#include <pcff/event/EventDirector.h>

#include <pcff/event/Event.h>

using namespace std;
using namespace pcff;

Event::Event()
{
   initMembers();
}

Event::Event(const Event &in)
{
   _eventID = in._eventID;
   _deviceID = in._deviceID;
   _recvTimeSec = in._recvTimeSec;
   _recvTimeUSec = in._recvTimeUSec;
   _rawEventBufSize = in._rawEventBufSize;
   _rawEventBuf = in._rawEventBuf;
}

Event::~Event()
{
   if (_rawEventBuf != NULL)
   {
      delete [] _rawEventBuf;
      _rawEventBuf = NULL;
   }
}

void Event::initMembers()
{
   _eventID = Event::UnknownEvent;
   _deviceID = Event::UnknownDevice;
   _recvTimeSec = 0;
   _recvTimeUSec = 0;
   _rawEventBufSize = 0;
   _rawEventBuf = NULL;
}

//
// Note: serialization of the Observable (_data) is done by parent
// class who knows about the type of the Observable.
//

void Event::serialize(Serializer &s)
{
   s.transfer(this->_eventID);
   s.transfer(this->_deviceID);
   s.transfer(this->_recvTimeSec);
   s.transfer(this->_recvTimeUSec);
   s.transfer(this->_rawEventBufSize);
   s.transfer(this->_rawEventBuf, _rawEventBufSize);
}

void Event::markRecvTime()
{
   struct timeval currtime;

   gettimeofday(&currtime, NULL);

   _recvTimeSec = currtime.tv_sec;
   _recvTimeUSec = currtime.tv_usec;
}

void Event::getRecvTime(long &rsec, long &rusec)
{
   rsec = _recvTimeSec;
   rusec = _recvTimeUSec;
}

void Event::setRecvTime(long rsec,
                        long rusec)
{
   _recvTimeSec = rsec;
   _recvTimeUSec = rusec;
}

string Event::getEventInfo()
{
   ostringstream ss;

   ss << endl;
   ss << "Event Info   		:  " << endl;

   return ss.str();
}

void Event::setTestData()
{
   struct timeval currtime;

   gettimeofday(&currtime, NULL);

   _eventID = Event::UnknownEvent;
   _deviceID = Event::UnknownDevice;
   _recvTimeSec = currtime.tv_sec;
   _recvTimeUSec = currtime.tv_usec;
   _rawEventBufSize = 0;
   _rawEventBuf = NULL;
}

void Event::setRawEvent(unsigned char *buf, unsigned long bufSize)
{
   if (bufSize > 0)
   {
      _rawEventBufSize = bufSize;
      _rawEventBuf = new unsigned char[_rawEventBufSize];
      memcpy(_rawEventBuf, buf, _rawEventBufSize);
   }
   else
   {
      throw EventException(EventException::InvalidBufferSize, "Event::setRawEvent");
   }
}

void Event::getRawEvent(unsigned char *&buf, unsigned long &bufSize)
{
   if (_rawEventBufSize > 0)
   {
      bufSize = _rawEventBufSize;
      buf = _rawEventBuf;
   }
   else
   {
      throw EventException(EventException::InvalidBufferSize, "Event::setRawEvent");
   }
}
