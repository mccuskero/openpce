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

#include <iostream>
#include <sstream>

#include <pcff/BasicExceptionMgr.h>
#include <pcff/InformationList.h>

//#include <pcsec/ProxyService.h>
#include <pcsec/SensorProxyConfigCommandLine.h>
#include <pcsec/SensorProxyConfigFile.h>
#include <pcsec/SensorProxyConfig.h>

using namespace std;
using namespace pcff;
using namespace pcsec;

SensorProxyConfig *SensorProxyConfig::_instance = NULL;

SensorProxyConfig &SensorProxyConfig::instance()
{
   if (_instance == NULL)
   {
      _instance = new SensorProxyConfig();
   }

   return *_instance;
}

SensorProxyConfig::SensorProxyConfig()
{

}

SensorProxyConfig::~SensorProxyConfig()
{

}

void SensorProxyConfig::init(int argc, char* argv[])
{
   SensorProxyConfigCommandLine::instance().init();
   SensorProxyConfigCommandLine::instance().parseCommandLine(argc, argv);

   string configFile = SensorProxyConfigCommandLine::instance().configFile();

   SensorProxyConfigFile::instance().init();
   SensorProxyConfigFile::instance().readConfigFile(configFile);
}

int SensorProxyConfig::getVerbosityLevel()
{
   return SensorProxyConfigCommandLine::instance().getVerbosityLevel();
}

bool SensorProxyConfig::daemonize()
{
   bool bDeamonize = false;

   if (SensorProxyConfigCommandLine::instance().daemonizeArgv())
   {
      bDeamonize = true;
   }

   return bDeamonize;
}

bool SensorProxyConfig::batch()
{
   return SensorProxyConfigCommandLine::instance().batch();
}

bool SensorProxyConfig::test()
{
   return SensorProxyConfigCommandLine::instance().test();
}

bool SensorProxyConfig::archive()
{
   return SensorProxyConfigCommandLine::instance().archive();
}

long SensorProxyConfig::getMaxLogSize()
{
   return SensorProxyConfigFile::instance().getMaxLogSize();
}

string SensorProxyConfig::workingDir()
{
   return SensorProxyConfigFile::instance().workingDir();
}

string SensorProxyConfig::unixSocketPath()
{
   return SensorProxyConfigFile::instance().unixSocketPath();
}

int SensorProxyConfig::service()
{
   int service = 0;

   // if service arg passed in at command line use it
   if (SensorProxyConfigCommandLine::instance().serviceArgv())
   {
      service = SensorProxyConfigCommandLine::instance().service();
   }
   else
   {
      service = SensorProxyConfigFile::instance().service();
   }

   return service;
}

int SensorProxyConfig::ingestor()
{
   int ingestor = 0;

   // if service arg passed in at command line use it
   if (SensorProxyConfigCommandLine::instance().ingestorArgv())
   {
      ingestor = SensorProxyConfigCommandLine::instance().ingestor();
   }
   else
   {
      ingestor = SensorProxyConfigFile::instance().ingestor();
   }

   return ingestor;
}

int SensorProxyConfig::processor()
{
   int processor = 0;

   // if service arg passed in at command line use it
   if (SensorProxyConfigCommandLine::instance().processorArgv())
   {
      processor = SensorProxyConfigCommandLine::instance().processor();
   }
   else
   {
      processor = SensorProxyConfigFile::instance().processor();
   }

   return processor;
}

void SensorProxyConfig::getFileList()
{
   // TODO: file list class, with numFiles attribute
}


string SensorProxyConfig::getPcapDeviceName()
{
   return SensorProxyConfigCommandLine::instance().getPcapDeviceName();
}

int SensorProxyConfig::getPcapSnapLen()
{
   return SensorProxyConfigCommandLine::instance().getPcapSnapLen();
}

string SensorProxyConfig::getPcapFilename()
{
   return SensorProxyConfigCommandLine::instance().getPcapFilename();
}

string SensorProxyConfig::getPcapFilter()
{
   return SensorProxyConfigCommandLine::instance().getPcapFilter();

}

bool SensorProxyConfig::useCallbackMethod()
{
   return SensorProxyConfigCommandLine::instance().useCallbackMethod();
}

void SensorProxyConfig::showInitialValues()
{
   BasicExceptionMgr::instance()->log("---Command Line Options---", LogLevel::Info);
   SensorProxyConfigCommandLine::instance().showInitialValues();

   BasicExceptionMgr::instance()->log("---Config File Options---", LogLevel::Info);
   SensorProxyConfigFile::instance().showInitialValues();
}

void SensorProxyConfig::getCreateInfo(int serviceType, InformationList &createInfo)
{
   switch (serviceType)
   {
   case SensorProxyConfig::SnortService :
              getCreateInfoSnortService(createInfo);
      break;
   case SensorProxyConfig::PcapService :
              getCreateInfoPcapService(createInfo);
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
}

void SensorProxyConfig::getCreateInfoSnortService(InformationList &createInfo)
{
   int ingestorType = SensorProxyConfig::instance().ingestor();
   int processorType = SensorProxyConfig::instance().processor();
   bool batch = SensorProxyConfig::instance().batch();
   bool test = SensorProxyConfig::instance().test();
   bool archive = SensorProxyConfig::instance().archive();

   // set ingestor type
   switch (ingestorType)
   {
   case SnortUnifiedFile:
      createInfo.set("ingestorType", AttributeValuePair::STR, "SnortUnifiedFile");
      createInfo.set("workingDir", AttributeValuePair::STR, (char *) SensorProxyConfig::instance().workingDir().c_str());
      break;
   case SnortUnixSocket:
      createInfo.set("ingestorType", AttributeValuePair::STR, "SnortUnixSocket");
      createInfo.set("unixSocketPathname", AttributeValuePair::STR, (char *) SensorProxyConfig::instance().unixSocketPath().c_str());
      break;
   default:
      break;
   }

   // test processor type
   switch (processorType)
   {
   case SnortDefaultProcessor:
      createInfo.set("processorType", AttributeValuePair::STR, "SnortDefaultProcessor");
      break;
   default:
      break;
   }

   if (batch == true)
   {
      createInfo.set("batch", AttributeValuePair::STR, "true");
   }
   else
   {
      createInfo.set("batch", AttributeValuePair::STR, "false");
   }

   if (test == true)
   {
      createInfo.set("test", AttributeValuePair::STR, "true");
   }
   else
   {
      createInfo.set("test", AttributeValuePair::STR, "false");
   }

   if (archive == true)
   {
      createInfo.set("archive", AttributeValuePair::STR, "true");
   }
   else
   {
      createInfo.set("archive", AttributeValuePair::STR, "false");
   }

}

void SensorProxyConfig::getCreateInfoPcapService(InformationList &createInfo)
{
   int ingestorType = SensorProxyConfig::instance().ingestor();
   int processorType = SensorProxyConfig::instance().processor();
   bool batch = SensorProxyConfig::instance().batch();
   bool test = SensorProxyConfig::instance().test();
   bool archive = SensorProxyConfig::instance().archive();

   // set ingestor type
   switch (ingestorType)
   {
   case PcapLive:
      createInfo.set("ingestorType", AttributeValuePair::STR, "PcapLive");
      createInfo.set("workingDir", AttributeValuePair::STR, (char *) SensorProxyConfig::instance().workingDir().c_str());
      createInfo.set("deviceName", AttributeValuePair::STR, (char *) SensorProxyConfig::instance().getPcapDeviceName().c_str());
      createInfo.set("snapLen", AttributeValuePair::INT, SensorProxyConfig::instance().getPcapSnapLen());
      createInfo.set("filter", AttributeValuePair::STR, (char *) SensorProxyConfig::instance().getPcapFilter().c_str());
      createInfo.set("useCallback", AttributeValuePair::STR, (char *)((SensorProxyConfig::instance().useCallbackMethod() == true) ? "true" : "false") );
      break;
   case PcapOffline:
      createInfo.set("ingestorType", AttributeValuePair::STR, "PcapOffline");
      createInfo.set("workingDir", AttributeValuePair::STR, (char *) SensorProxyConfig::instance().workingDir().c_str());
      createInfo.set("filename", AttributeValuePair::STR, (char *) SensorProxyConfig::instance().getPcapFilename().c_str());
      createInfo.set("snapLen", AttributeValuePair::INT, SensorProxyConfig::instance().getPcapSnapLen());
      createInfo.set("filter", AttributeValuePair::STR, (char *) SensorProxyConfig::instance().getPcapFilter().c_str());
      createInfo.set("useCallback", AttributeValuePair::STR, (char *)((SensorProxyConfig::instance().useCallbackMethod() == true) ? "true" : "false") );
      break;
   default:
      break;
   }

   // test processor type
   switch (processorType)
   {
   case PcapDefaultProcessor:
      createInfo.set("processorType", AttributeValuePair::STR, "PcapDefaultProcessor");
      break;
   default:
      break;
   }

   if (batch == true)
   {
      createInfo.set("batch", AttributeValuePair::STR, "true");
   }
   else
   {
      createInfo.set("batch", AttributeValuePair::STR, "false");
   }

   if (test == true)
   {
      createInfo.set("test", AttributeValuePair::STR, "true");
   }
   else
   {
      createInfo.set("test", AttributeValuePair::STR, "false");
   }

   if (archive == true)
   {
      createInfo.set("archive", AttributeValuePair::STR, "true");
   }
   else
   {
      createInfo.set("archive", AttributeValuePair::STR, "false");
   }

}
