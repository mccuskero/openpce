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

#include "SilkSensorProxyConfigCommandLine.h"
#include "SilkSensorProxyConfigFile.h"
#include "SilkSensorProxyConfig.h"

using namespace std;
using namespace pcff;
using namespace pcsec;

SilkSensorProxyConfig *SilkSensorProxyConfig::_instance = NULL;

SilkSensorProxyConfig &SilkSensorProxyConfig::instance()
{
    if( _instance == NULL ) {
        _instance = new SilkSensorProxyConfig();
    }

    return *_instance;
}


SilkSensorProxyConfig::SilkSensorProxyConfig()
{

}

SilkSensorProxyConfig::~SilkSensorProxyConfig()
{

}

void SilkSensorProxyConfig::init( int argc, char* argv[])
{
	SilkSensorProxyConfigCommandLine::instance().init();
	SilkSensorProxyConfigCommandLine::instance().parseCommandLine(argc, argv);

	string configFile = SilkSensorProxyConfigCommandLine::instance().siteConfigFile();

	SilkSensorProxyConfigFile::instance().init();
	SilkSensorProxyConfigFile::instance().readConfigFile(configFile);
}

int SilkSensorProxyConfig::getVerbosityLevel()
{
	return SilkSensorProxyConfigCommandLine::instance().getVerbosityLevel();
}

bool SilkSensorProxyConfig::daemonize()
{
	bool bDeamonize = false;

	if (SilkSensorProxyConfigCommandLine::instance().daemonizeArgv() )
	{
		bDeamonize = true;
	}

	return bDeamonize;
}

string SilkSensorProxyConfig::workingDir()
{
	if (SilkSensorProxyConfigCommandLine::instance().workingDirArgv() )
	{
		return SilkSensorProxyConfigCommandLine::instance().workingDir();
	}
	else
	{
		return SilkSensorProxyConfigFile::instance().workingDir();
	}
}

bool SilkSensorProxyConfig::batch()
{
	return SilkSensorProxyConfigCommandLine::instance().batch();
}

bool SilkSensorProxyConfig::test()
{
	return SilkSensorProxyConfigCommandLine::instance().test();
}

bool SilkSensorProxyConfig::archive()
{
	return SilkSensorProxyConfigCommandLine::instance().archive();
}

long SilkSensorProxyConfig::getMaxLogSize()
{
	return SilkSensorProxyConfigFile::instance().getMaxLogSize();
}

int SilkSensorProxyConfig::service()
{
	int service = 0;

	// if service arg passed in at command line use it
	if (SilkSensorProxyConfigCommandLine::instance().serviceArgv() )
	{
		service = SilkSensorProxyConfigCommandLine::instance().service();
	}
	else
	{
		service = SilkSensorProxyConfigFile::instance().service();
	}

	return service;
}

int SilkSensorProxyConfig::ingestor()
{
	// if service arg passed in at command line use it
	if (SilkSensorProxyConfigCommandLine::instance().ingestorArgv() )
	{
		return SilkSensorProxyConfigCommandLine::instance().ingestor();
	}
	else
	{
		return SilkSensorProxyConfigFile::instance().ingestor();
	}

	return SilkSensorProxyConfigFile::instance().ingestor();
}

int SilkSensorProxyConfig::processor()
{
	// if service arg passed in at command line use it
	if (SilkSensorProxyConfigCommandLine::instance().processorArgv() )
	{
		return SilkSensorProxyConfigCommandLine::instance().processor();
	}
	else
	{
		return SilkSensorProxyConfigFile::instance().processor();
	}
}

string SilkSensorProxyConfig::siteConfigFile()
{
	return SilkSensorProxyConfigCommandLine::instance().siteConfigFile();
}

string SilkSensorProxyConfig::dataFile()
{
	return SilkSensorProxyConfigCommandLine::instance().dataFile();
}

string SilkSensorProxyConfig::datastore()
{
	// if service arg passed in at command line use it
	if (SilkSensorProxyConfigCommandLine::instance().datastoreArgv() )
	{
		return SilkSensorProxyConfigCommandLine::instance().datastore();
	}
	else
	{
		return SilkSensorProxyConfigFile::instance().datastore();
	}
}

string SilkSensorProxyConfigCommandLine::dataBins()
{
	return SilkSensorProxyConfigCommandLine::instance().dataBins();
}

/*
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
*/

void SilkSensorProxyConfig::showInitialValues()
{
    BasicExceptionMgr::instance()->log("---Command Line Options---", LogLevel::Info);
	SilkSensorProxyConfigCommandLine::instance().showInitialValues();

	BasicExceptionMgr::instance()->log("---Config File Options---", LogLevel::Info);
	SilkSensorProxyConfigFile::instance().showInitialValues();
}

void SilkSensorProxyConfig::getCreateInfo(int serviceType, InformationList &createInfo)
{
	switch(serviceType)
	{
	case SilkSensorProxyConfig::SilkService :
		getCreateInfoSilkService(createInfo);
		break;
	case SilkSensorProxyConfig::Unknown :
		cout << "Unknown service" << endl;
		break;
	default:
		cout << "Unknown service" << endl;
		break;
	}
}

void SilkSensorProxyConfig::getCreateInfoSilkService(InformationList &createInfo)
{
	int ingestorType 	= SilkSensorProxyConfig::instance().ingestor();
	int processorType 	= SilkSensorProxyConfig::instance().processor();
	bool batch			= SilkSensorProxyConfig::instance().batch();
	bool test			= SilkSensorProxyConfig::instance().test();
	bool archive		= SilkSensorProxyConfig::instance().archive();

	// set ingestor type
	switch(ingestorType)
	{
	case SilkFileIngestor:
		createInfo.set("ingestor",  AttributeValuePair::STR,  "file" );
		createInfo.set("data-store",   AttributeValuePair::STR,  (char *)SilkSensorProxyConfig::instance().datastore().c_str() );
		createInfo.set("data-file",   AttributeValuePair::STR,  (char *)SilkSensorProxyConfig::instance().dataFile().c_str() );
		break;
	case SilkDatastoreIngestor:
		createInfo.set("ingestor",  AttributeValuePair::STR,  "datastore" );
		createInfo.set("data-store",   AttributeValuePair::STR,  (char *)SilkSensorProxyConfig::instance().datastore().c_str() );
		break;
	default:
		break;
	}

	// test processor type
	switch(processorType)
	{
	case SilkDefaultEventProcessor:
		createInfo.set("processor", AttributeValuePair::STR,  "default");
		break;
	case SilkSocketOutputEventProcessor:
		createInfo.set("processor", AttributeValuePair::STR,  "socket");
		break;
	default:
		break;
	}

	if (batch == true)
	{
		createInfo.set("batch",         AttributeValuePair::STR, "true");
	}
	else
	{
		createInfo.set("batch",         AttributeValuePair::STR, "false");
	}

	if (test == true)
	{
		createInfo.set("test",         AttributeValuePair::STR, "true");
	}
	else
	{
		createInfo.set("test",         AttributeValuePair::STR, "false");
	}

	if (archive == true)
	{
		createInfo.set("archive",         AttributeValuePair::STR, "true");
	}
	else
	{
		createInfo.set("archive",         AttributeValuePair::STR, "false");
	}

}
