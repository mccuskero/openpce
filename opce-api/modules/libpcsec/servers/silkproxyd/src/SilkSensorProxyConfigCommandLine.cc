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

#include "SilkSensorProxyConfigCommandLine.h"

using namespace std;
using namespace pcff;
using namespace pcsec;

SilkSensorProxyConfigCommandLine *SilkSensorProxyConfigCommandLine::_instance = NULL;

SilkSensorProxyConfigCommandLine &SilkSensorProxyConfigCommandLine::instance() {
    if( _instance == NULL ) {
        _instance = new SilkSensorProxyConfigCommandLine();
    }

    return *_instance;
}


SilkSensorProxyConfigCommandLine::SilkSensorProxyConfigCommandLine() {

}

SilkSensorProxyConfigCommandLine::~SilkSensorProxyConfigCommandLine() {

}

void SilkSensorProxyConfigCommandLine::initAttributes() {

}

void SilkSensorProxyConfigCommandLine::init()
{
    setAttribute("D", 	        		Config::BOOL, 	Config::CMDLINE, "false",      		"-D  	 daemonizes sproxyd");
    setAttribute("b",  		    		Config::BOOL,   Config::CMDLINE, "false", 	   		"-b       sets the operational mode of proxy to batch mode\n");
    setAttribute("s",           		Config::STR,    Config::CMDLINE, "silk",      		"-s       sets the proxy service type\n" \
    																		          		"	silk\n");
    setAttribute("-ingestor",   		Config::STR,   	Config::CMDLINE, "file", 		    "--ingestor    sets the ingestor type: of the file to be ingested\n" \
    																						"   file      - ingests a single file" \
    																						"	datastore - ingests files in a datastore based on ingest parameters\n");
    setAttribute("-processor",   		Config::STR,   	Config::CMDLINE, "default", 		"--processor   sets the filename of the file to be ingested\n" \
    		    																			"   default - logs flows to screen" \
    		    																			"	socket  - serializes flows and sends data over socket\n");
    setAttribute("-site-config-file",   Config::STR,   	Config::CMDLINE, "silkproxyd.conf", "-site-config-file - sets the sites configuration file pathname\n");
    setAttribute("-data-file",   		Config::STR,   	Config::CMDLINE, "not.set", 		"--data-file    - sets the filename of the file to be ingested\n");
    setAttribute("-data-store",   		Config::STR,   	Config::CMDLINE, "/data", 			"--data-dir     - sets the sites root Silk data store pathname\n");
    setAttribute("-start-time",   		Config::STR,   	Config::CMDLINE, "1/1/1", 			"--start-time   - sets start time for ingest on datastore\n");
    setAttribute("-end-time",   		Config::STR,   	Config::CMDLINE, "1/1/1", 			"--end-time     - sets end time for ingest on datastore\n");
    setAttribute("-current-time",    	Config::BOOL,  	Config::CMDLINE, "false", 			"--current-time - sets tells ingestor to ingest current time only\n");
    setAttribute("-test", 	    		Config::BOOL,   Config::CMDLINE, "false", 	   		"-test    - sets the operational mode of proxy to test mode\n");
    setAttribute("-archive", 			Config::BOOL,   Config::CMDLINE, "false", 	   		"-archive - enables proxy to archive data ingested\n");
}

void SilkSensorProxyConfigCommandLine::printVersionHelper()
{
    cout << "silkproxyd (GCC) 0.0.1 (Linux)" << endl;
    cout << "Copyright (C) 2008 Free Software Foundation, Inc." << endl;
    cout << "This is free software; see the source for copying conditions.  There is NO" << endl;
    cout << "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE." << endl << endl;
}

//
// TODO: need to working on processing files, or files in directory (after options)
//
void SilkSensorProxyConfigCommandLine::printUsageHelper(char *progName)
{
    cout << "(Continual Mode) Usage: " << progName << " [OPTIONS]" << endl;
    cout << "(Batch Mode)     Usage: " << progName << " -b [OPTIONS] ...FILES... " << endl;
    cout << "(Test Mode)      Usage: " << progName << " -test [OPTIONS]" << endl;
}

bool SilkSensorProxyConfigCommandLine::daemonize()
{
	ConfigTuple tuple;
	tuple	= m_ConfigOptions["D"];
	return tuple.getValueAsBool();
}

bool SilkSensorProxyConfigCommandLine::daemonizeArgv()
{
	return m_ConfigOptions["D"].optionEnabled();
}

string SilkSensorProxyConfigCommandLine::workingDir()
{
    return m_ConfigOptions["workingDir"].getValue();
}

bool SilkSensorProxyConfigCommandLine::workingDirArgv()
{
    return m_ConfigOptions["workingDir"].optionEnabled();
}

bool SilkSensorProxyConfigCommandLine::batch()
{
	ConfigTuple tuple;
	tuple	= m_ConfigOptions["b"];
	return tuple.getValueAsBool();
}

bool SilkSensorProxyConfigCommandLine::test()
{
	ConfigTuple tuple;
	tuple	= m_ConfigOptions["-test"];
	return tuple.getValueAsBool();
}

bool SilkSensorProxyConfigCommandLine::archive()
{
	ConfigTuple tuple;
	tuple	= m_ConfigOptions["-archive"];
	return tuple.getValueAsBool();
}

bool SilkSensorProxyConfigCommandLine::serviceArgv()
{
	return m_ConfigOptions["s"].optionEnabled();
}

int SilkSensorProxyConfigCommandLine::service()
{
	int type = 0;
    ConfigTuple optionTuple = m_ConfigOptions["s"];

    string sService = optionTuple.getValue();

    if (sService.compare("silk") == 0)
    {
       type = SilkSensorProxyConfig::SilkService;
    }
    else
    {
        type = SilkSensorProxyConfig::Unknown;
    }

    return type;
}

string SilkSensorProxyConfigCommandLine::siteConfigFile()
{
    return m_ConfigOptions["-site-config-file"].getValue();
}

bool SilkSensorProxyConfigCommandLine::ingestorArgv()
{
	return m_ConfigOptions["-ingestor"].optionEnabled();
}

int SilkSensorProxyConfigCommandLine::ingestor()
{
	int type = 0;

	ConfigTuple optionTuple = m_ConfigOptions["-ingestor"];

    string sService = optionTuple.getValue();

    if (sService.compare("file") == 0)
    {
       type = SilkSensorProxyConfig::SilkFileIngestor;
    }
    else if (sService.compare("datastore") == 0)
    {
       type = SilkSensorProxyConfig::SilkDatastoreIngestor;
    }
    else
    {
        type = SilkSensorProxyConfig::Unknown;
    }

    return type;
}

bool SilkSensorProxyConfigCommandLine::processorArgv()
{
	return m_ConfigOptions["-processor"].optionEnabled();
}

int SilkSensorProxyConfigCommandLine::processor()
{
	int type = 0;

    ConfigTuple optionTuple = m_ConfigOptions["-processor"];

    string sService = optionTuple.getValue();

    if (sService.compare("default") == 0)
    {
       type = SilkSensorProxyConfig::SilkDefaultEventProcessor;
    }
    else if (sService.compare("socket") == 0)
    {
       type = SilkSensorProxyConfig::SilkSocketOutputEventProcessor;
    }
    else
    {
        type = SilkSensorProxyConfig::Unknown;
    }

    return type;
}

string SilkSensorProxyConfigCommandLine::dataFile()
{
    return m_ConfigOptions["-data-file"].getValue();
}

bool SilkSensorProxyConfigCommandLine::datastoreArgv()
{
	return m_ConfigOptions["-data-store"].optionEnabled();
}

string SilkSensorProxyConfigCommandLine::datastore()
{
    return m_ConfigOptions["-data-store"].getValue();
}

string SilkSensorProxyConfigCommandLine::startTime()
{
    return m_ConfigOptions["-start-time"].getValue();
}

string SilkSensorProxyConfigCommandLine::endTime()
{
    return m_ConfigOptions["-end-time"].getValue();
}

bool SilkSensorProxyConfigCommandLine::currentTime()
{
	ConfigTuple tuple;
	tuple	= m_ConfigOptions["-current-time"];
	return tuple.getValueAsBool();
}
