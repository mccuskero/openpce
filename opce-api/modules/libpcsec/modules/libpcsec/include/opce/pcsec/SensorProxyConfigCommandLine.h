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

#ifndef _SensorProxyConfigCommandLine_h
#define _SensorProxyConfigCommandLine_h


#include <pcff/Config.h>
#include <pcsec/SensorProxyConfig.h>

namespace pcsec
{

class SensorProxyConfigCommandLine : public pcff::Config
{
public:
   static SensorProxyConfigCommandLine &instance();
   virtual void init();
   virtual void initAttributes();

private:
   SensorProxyConfigCommandLine();
   virtual ~SensorProxyConfigCommandLine();

public:
   bool daemonize();
   bool daemonizeArgv(); // was -D opion passed in?
   bool batch();
   bool test();
   bool archive();
   int mode();
   int service();
   int ingestor();
   int processor();
   bool serviceArgv(); // was the -s option passed in?
   bool ingestorArgv(); // was the -s option passed in?
   bool processorArgv(); // was the -s option passed in?
   std::string configFile();
   std::string getPcapDeviceName();
   int getPcapSnapLen();
   std::string getPcapFilename();
   std::string getPcapFilter();
   bool useCallbackMethod();

private:
   virtual void printVersionHelper();
   virtual void printUsageHelper(char *progName);

private:
   static SensorProxyConfigCommandLine *_instance;
};

};
#endif
