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

#include "SilkSensorProxyConfig.h"
#include "SilkSensorProxyConfigFile.h"

using namespace std;
using namespace pcff;
using namespace pcsec;

SilkSensorProxyConfigFile *SilkSensorProxyConfigFile::_instance = NULL;

SilkSensorProxyConfigFile &SilkSensorProxyConfigFile::instance()
{
    if( _instance == NULL )
    {
        _instance = new SilkSensorProxyConfigFile();
    }

    return *_instance;
}


SilkSensorProxyConfigFile::SilkSensorProxyConfigFile()
{

}

SilkSensorProxyConfigFile::~SilkSensorProxyConfigFile()
{

}

void SilkSensorProxyConfigFile::initAttributes()
{

}

void SilkSensorProxyConfigFile::init()
{
    setAttribute("-pid_file",  	Config::STR,   	Config::FILE, "/var/run/silkproxyd.pid",    "--pid_file        sproxyd pid file" );
    setAttribute("-workingDir", Config::STR,   	Config::FILE, "/tmp",       				"--workingDir      sets the working directory of daemon" );
    setAttribute("-maxLogSize", Config::LONG,  	Config::FILE, "350000",     			 	"--maxLogSize      sets the max log size" );
    setAttribute("-service",    Config::STR,   Config::FILE,  "silk",      					"--service         sets the data stream name\n" \
    																		   					"	silk\n");
    setAttribute("-ingestor",  	Config::STR,   Config::FILE, "file",     					"--ingestor         sets the event ingestor type\n" \
    																							"	file      - ingest silk binary file\n" \
    																							"	datastore - ingests silk datastore\n");
    setAttribute("-processor",  Config::STR,   Config::FILE, "default", 					 "--processor              sets the event processor type\n" \
    																							"	default\n" \
    																							"   socket\n");
    setAttribute("-spool_dir", 	Config::STR,   	Config::FILE, "/var/log/silk",      	     "--spool_dir - sets spool directory" );
    setAttribute("-archive_dir", Config::STR,   	Config::FILE, "/var/log/silk/archive",   "--archive_dir - sets archive directory" );

    // silk related config options
    setAttribute("-data-store", Config::STR,   	Config::FILE, "/data", 			"--data-store     - sets the sites root Silk data store pathname\n");

    // SiLK related config options

    // Nepenthes related config options

    // Ourmon related config options

}

long SilkSensorProxyConfigFile::getMaxLogSize()
{
    return m_ConfigOptions["-maxLogSize"].getValueAsLong();
}

string SilkSensorProxyConfigFile::workingDir()
{
    return m_ConfigOptions["-workingDir"].getValue();
}

int SilkSensorProxyConfigFile::service()
{
	int type = 0;
    ConfigTuple optionTuple = m_ConfigOptions["-service"];

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

int SilkSensorProxyConfigFile::ingestor()
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

int SilkSensorProxyConfigFile::processor()
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

string SilkSensorProxyConfigFile::datastore()
{
    return m_ConfigOptions["-data-store"].getValue();
}
