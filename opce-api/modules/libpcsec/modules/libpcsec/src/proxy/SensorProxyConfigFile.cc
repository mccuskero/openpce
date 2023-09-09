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

#include <pcsec/SensorProxyConfig.h>
#include <pcsec/SensorProxyConfigFile.h>

using namespace std;
using namespace pcff;
using namespace pcsec;

SensorProxyConfigFile *SensorProxyConfigFile::_instance = NULL;

SensorProxyConfigFile &SensorProxyConfigFile::instance()
{
    if( _instance == NULL )
    {
        _instance = new SensorProxyConfigFile();
    }

    return *_instance;
}


SensorProxyConfigFile::SensorProxyConfigFile()
{

}

SensorProxyConfigFile::~SensorProxyConfigFile()
{

}

void SensorProxyConfigFile::initAttributes()
{

}

void SensorProxyConfigFile::init()
{
    setAttribute("pid_file",  Config::STR,   	Config::FILE, "/var/run/sproxyd.pid",       	"sproxyd pid file" );
    setAttribute("workingDir",  Config::STR,   	Config::FILE, "/tmp",       					"workingDir      sets the working directory of daemon" );
    setAttribute("maxLogSize",  Config::LONG,  	Config::FILE, "350000",     					"maxLogSize      sets the max log size" );
    setAttribute("service",     Config::STR,   Config::FILE,  "snort",      					"service               sets the data stream name\n" \
    																		   					"	snort\n" \
    																		   					"	silk\n"  \
    																		   					"	nepenthes\n");
    setAttribute("ingestor",  Config::STR,   Config::FILE, "snortUSock",     					"ingestor              sets the event ingestor type\n" \
    																							"	snortUFile - ingest snort unified file format\n" \
    																							"	snortUSock - ingests Snort Unix Socket\n");
    setAttribute("processor",  Config::STR,   Config::FILE, "snortDefault", 					"processor              sets the event processor type\n" \
    																							"	snortDefault\n");
    setAttribute("spool_dir", 			Config::STR,   	Config::FILE, "/var/log/snort",      	"spool directory" );
    setAttribute("archive_dir", 	    Config::STR,   	Config::FILE, "/var/log/snort/archive", "archive directory" );

    // snort related config options
    setAttribute("sid_msg_file",  		Config::STR,   	Config::FILE, "sid-msg.map",       		"Snort SID message map file" );
    setAttribute("gen_msg_file",  		Config::STR,   	Config::FILE, "gen-msg.map",       		"Snort GEN message map file" );
    setAttribute("classification_file", Config::STR,   	Config::FILE, "classification.config",  "Snort classification configuration file" );
    setAttribute("snort_log_dir", 	 	Config::STR,   	Config::FILE, "/var/log/snort",       	"Snort log directory" );
    setAttribute("unix_sock_path", 	 	Config::STR,   	Config::FILE, "/var/log/snort/snort_alert", "Snort Unix socket pathname" );

    // SiLK related config options

    // Nepenthes related config options

    // Ourmon related config options

}

long SensorProxyConfigFile::getMaxLogSize()
{
    return m_ConfigOptions["maxLogSize"].getValueAsLong();
}

string SensorProxyConfigFile::workingDir()
{
    return m_ConfigOptions["workingDir"].getValue();
}

string SensorProxyConfigFile::unixSocketPath()
{
    return m_ConfigOptions["unix_sock_path"].getValue();
}


int SensorProxyConfigFile::service()
{
	int type = 0;
    ConfigTuple optionTuple = m_ConfigOptions["service"];

    string sService = optionTuple.getValue();

    if (sService.compare("snort") == 0)
    {
       type = SensorProxyConfig::SnortService;
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

int SensorProxyConfigFile::ingestor()
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
    else
    {
        type = SensorProxyConfig::Unknown;
    }

    return type;
}

int SensorProxyConfigFile::processor()
{
	int type = 0;

    ConfigTuple optionTuple = m_ConfigOptions["processor"];

    string sService = optionTuple.getValue();

    if (sService.compare("snortDefault") == 0)
    {
       type = SensorProxyConfig::SnortDefaultProcessor;
    }
    else
    {
        type = SensorProxyConfig::Unknown;
    }

    return type;
}

