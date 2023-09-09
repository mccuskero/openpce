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

#include <pcff/proxy/ProxyService.h>
#include <pcsec/SensorProxyConfigCommandLine.h>

using namespace std;
using namespace pcff;
using namespace pcsec;

SensorProxyConfigCommandLine *SensorProxyConfigCommandLine::_instance = NULL;

SensorProxyConfigCommandLine &SensorProxyConfigCommandLine::instance() {
    if( _instance == NULL ) {
        _instance = new SensorProxyConfigCommandLine();
    }

    return *_instance;
}


SensorProxyConfigCommandLine::SensorProxyConfigCommandLine() {

}

SensorProxyConfigCommandLine::~SensorProxyConfigCommandLine() {

}

void SensorProxyConfigCommandLine::initAttributes() {

}

void SensorProxyConfigCommandLine::init()
{
    setAttribute("D", 	        Config::BOOL, 	Config::CMDLINE, "false",      "-D  	 daemonizes sproxyd");
    setAttribute("b",  		    Config::BOOL,   Config::CMDLINE, "false", 	   "-b       sets the operational mode of proxy to batch mode\n");
    setAttribute("test", 	    Config::BOOL,   Config::CMDLINE, "false", 	   "-test    sets the operational mode of proxy to test mode\n");
    setAttribute("archive", 	Config::BOOL,   Config::CMDLINE, "false", 	   "-archive enables proxy to archive data ingested\n");
    setAttribute("s",           Config::STR,    Config::CMDLINE, "snort",      "-s       sets the proxy service type\n" \
    																		   "	pcap\n" \
    																		   "	snort\n" \
    																		   "	silk\n"  \
    																		   "	nepenthes\n");
    setAttribute("ingestor",  Config::STR,   Config::BOTH, "snortUSock",		"ingestor              sets the event ingestor type\n" \
    																			"	snortUFile  - ingest snort unified file format\n" \
    																			"	snortUSock  - ingests Snort Unix Socket\n" \
    																			"	pcapLive    - pcap live capture\n" \
    																			"	pcapOffline - ingest pcap archive file\n");
    setAttribute("processor",  Config::STR,   Config::BOTH, "snortDefault", 	"processor              sets the event processor type\n" \
    																			"	snortDefault\n" \
    																			"	pcapDefault\n");
    setAttribute("c",           Config::STR,   	Config::CMDLINE, "sproxyd.conf","-c    sets the configuration file pathname\n");

    // pcap related
    setAttribute("dev", 	   Config::STR,   Config::CMDLINE, "en0", 	        "-dev           capture device name (default: en0)\n");
    setAttribute("snaplen",    Config::INT,   Config::CMDLINE, "96",            "-snaplen       capture length (default: 96) Setting to 0 captures all packet\n");
    setAttribute("filename",   Config::STR,   Config::CMDLINE, "not.set",       "-filename      pcap archive filename\n");
    setAttribute("filter",     Config::STR,   Config::CMDLINE, "",              "-filter pcap   filter string\n");
    setAttribute("noCallback", Config::BOOL,  Config::CMDLINE, "false",         "-noCallback    sets the operational mode to use a callback\n");
}

void SensorProxyConfigCommandLine::printVersionHelper()
{
    cout << "sproxyd (GCC) 0.0.1 (Linux)" << endl;
    cout << "Copyright (C) 2007 Free Software Foundation, Inc." << endl;
    cout << "This is free software; see the source for copying conditions.  There is NO" << endl;
    cout << "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE." << endl << endl;
}

//
// TODO: need to working on processing files, or files in directory (after options)
//
void SensorProxyConfigCommandLine::printUsageHelper(char *progName)
{
    cout << "(Continual Mode) Usage: " << progName << " [OPTIONS]" << endl;
    cout << "(Batch Mode)     Usage: " << progName << " -b [OPTIONS] ...FILES... " << endl;
    cout << "(Test Mode)      Usage: " << progName << " -test [OPTIONS]" << endl;
}

bool SensorProxyConfigCommandLine::daemonize()
{
	ConfigTuple tuple;
	tuple	= m_ConfigOptions["D"];
	return tuple.getValueAsBool();
}

bool SensorProxyConfigCommandLine::daemonizeArgv()
{
	return m_ConfigOptions["D"].optionEnabled();
}

bool SensorProxyConfigCommandLine::batch()
{
	ConfigTuple tuple;
	tuple	= m_ConfigOptions["b"];
	return tuple.getValueAsBool();
}

bool SensorProxyConfigCommandLine::test()
{
	ConfigTuple tuple;
	tuple	= m_ConfigOptions["test"];
	return tuple.getValueAsBool();
}

bool SensorProxyConfigCommandLine::archive()
{
	ConfigTuple tuple;
	tuple	= m_ConfigOptions["archive"];
	return tuple.getValueAsBool();
}

bool SensorProxyConfigCommandLine::serviceArgv()
{
	return m_ConfigOptions["s"].optionEnabled();
}

int SensorProxyConfigCommandLine::service()
{
	int type = 0;
    ConfigTuple optionTuple = m_ConfigOptions["s"];

    string sService = optionTuple.getValue();

    if (sService.compare("snort") == 0)
    {
       type = SensorProxyConfig::SnortService;
    }
    else if (sService.compare("pcap") == 0)
    {
       type = SensorProxyConfig::PcapService;
    }
    else if (sService.compare("SilK") == 0)
    {
       type = SensorProxyConfig::SiLKService;
    }
    else if (sService.compare("Nepenthes") == 0)
    {
       type = SensorProxyConfig::NepenthesService;
    }
    else
    {
        type = SensorProxyConfig::Unknown;
    }

    return type;
}

bool SensorProxyConfigCommandLine::ingestorArgv()
{
	return m_ConfigOptions["ingestor"].optionEnabled();
}

int SensorProxyConfigCommandLine::ingestor()
{
	int type = 0;

	ConfigTuple optionTuple = m_ConfigOptions["ingestor"];

    string sService = optionTuple.getValue();

    if (sService.compare("snortUFile") == 0)
    {
       type = SensorProxyConfig::SnortUnifiedFile;
    }
    else if (sService.compare("snortUSock") == 0)
    {
       type = SensorProxyConfig::SnortUnixSocket;
    }
    else if (sService.compare("pcapLive") == 0)
    {
       type = SensorProxyConfig::PcapLive;
    }
    else if (sService.compare("pcapOffline") == 0)
    {
       type = SensorProxyConfig::PcapOffline;
    }
    else
    {
        type = SensorProxyConfig::Unknown;
    }

    return type;
}

bool SensorProxyConfigCommandLine::processorArgv()
{
	return m_ConfigOptions["processor"].optionEnabled();
}

int SensorProxyConfigCommandLine::processor()
{
	int type = 0;

    ConfigTuple optionTuple = m_ConfigOptions["processor"];

    string sService = optionTuple.getValue();

    if (sService.compare("snortDefault") == 0)
    {
       type = SensorProxyConfig::SnortDefaultProcessor;
    }
    else if (sService.compare("pcapDefault") == 0)
    {
       type = SensorProxyConfig::PcapDefaultProcessor;
    }
    else
    {
        type = SensorProxyConfig::Unknown;
    }

    return type;
}

string SensorProxyConfigCommandLine::configFile()
{
    return m_ConfigOptions["c"].getValue();
}

string SensorProxyConfigCommandLine::getPcapDeviceName()
{
    return m_ConfigOptions["dev"].getValue();

}
int SensorProxyConfigCommandLine::getPcapSnapLen()
{
	ConfigTuple tuple;
	tuple	= m_ConfigOptions["snaplen"];
	return tuple.getValueAsInt();
}
string SensorProxyConfigCommandLine::getPcapFilename()
{
    return m_ConfigOptions["filename"].getValue();

}

string SensorProxyConfigCommandLine::getPcapFilter()
{
    return m_ConfigOptions["filter"].getValue();
}

bool SensorProxyConfigCommandLine::useCallbackMethod()
{
   bool retVal = true;

   if (m_ConfigOptions["noCallback"].getValueAsBool() == true)
   {
      retVal = false;
   }

       cout << "SensorProxyConfigCommandLine::useCallbackMethod: bool return " << retVal << endl;


    return retVal;
}
