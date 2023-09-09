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
#include <string>

#ifndef _SensorProxyConfig_h_
#define _SensorProxyConfig_h_

#include <pcff/InformationList.h>

namespace pcsec
{

class SensorProxyConfig
{
public:
   static SensorProxyConfig &instance();
   virtual void init(int argc, char* argv[]);

private:
   SensorProxyConfig();
   virtual ~SensorProxyConfig();

public:
   bool daemonize();
   bool batch();
   bool test();
   bool archive();
   long getMaxLogSize();
   std::string workingDir();
   std::string unixSocketPath();
   int service();
   int ingestor();
   int processor();
   int getVerbosityLevel();
   std::string getPcapDeviceName();
   int getPcapSnapLen();
   std::string getPcapFilename();
   std::string getPcapFilter();
   bool useCallbackMethod();

public:
   void getFileList();
   void showInitialValues();

public:
   void getCreateInfo(int serviceType, pcff::InformationList &createInfo);


private:
   void getCreateInfoSnortService(pcff::InformationList &createInfo);
   void getCreateInfoPcapService(pcff::InformationList &createInfo);

public:

   enum eServices
   {
      Unknown = -1,
      SnortService,
      SiLKService,
      NepenthesService,
      PcapService
   } ServiceType;

   enum eIngestorTypes
   {
      SnortUnifiedFile,
      SnortUnixSocket,
      PcapLive,
      PcapOffline
   } EventIngestorType;

   enum eProcessorTypes
   {
      SnortDefaultProcessor,
      SnortIDMEFSocketProcessor,
      PcapDefaultProcessor,
   } EventProcessorType;

   enum eMode
   {
      Batch = 1,
      Continual,
      Archive
   };

private:
   static SensorProxyConfig *_instance;
};

};

#endif
