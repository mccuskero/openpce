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

#ifndef _ObservableEvent_h_
#define _ObservableEvent_h_

#include <string.h>

#include <pcff/serialize/Serializeable.h>

namespace pcff
{
   class Observable;

   class ObservableEvent : public Event
   {
   public:
      ObservableEvent();
      ObservableEvent(const ObservableEvent &in);
      virtual ~ObservableEvent();

   private:
      void initMembers();

   public:

      void setObservableType(int type)
      {
         _obsType = type;
      }

      int getObservableType()
      {
         return _obsType;
      }

   public:
      void getInfo(std::string &info);

   public:
      virtual void serialize(pcff::Serializer &s);
      virtual void setTestData();

   public:

      void setSensorID(int sensorID)
      {
         _deviceID = _sensorID = sensorID;
      }

      int getSensorID()
      {
         return _sensorID;
      }

      void enableMobileSensor()
      {
         _bMobileSensor = true;
      }

      void disableMobileSensor()
      {
         _bMobileSensor = false;
      }

      void setAltitude(float alt)
      {
         _altitude = alt;
      }

      void setMagnitude(float mag)
      {
         _magnitude = mag;
      }

      void setDirection(float d)
      {
         _direction = d;
      }

      void setSourceLocation(float lat, float lon);
      void getSourceLocation(float &lat, float &lon);
      void setCurrentLocationLocation(float lat, float lon);
      void getCurrentLocation(float &lat, float &lon);
      void setDestinationLocation(float lat, float lon);
      void getDestinationLocation(float &lat, float &lon);
      void setReferenceTime(long refsec,
                            long refusec);
      void getReferenceTime(long &refsec,
                            long &refusec);

      enum
      {
         UnknownType
      };

      enum
      {
         CyberSecurity,
         PhysicalSecurity
      };


   public:
      virtual std::string getInfo();

   public:
      virtual void serialize(pcff::Serializer &s);
      virtual void setTestData();

      typedef enum
      {
         UnknownID = -1
      };

   protected:
      int _obsType;
      int _sensorID; // TODO: UUID???
      bool _bMobileSensor; // usually false: when false, the proxy and sensor lat,lon are the same
      long _refTimeSec; // event reference time, detect time
      long _refTimeUSec;
      float _srcLat;
      float _srcLon;
      float _currentLat;
      float _currentLon;
      float _dstLat;
      float _dstLon;
      float _altitude;
      float _magnitude; //
      float _direction;
   };
};

#endif /*_ObservableEvent_h_*/
