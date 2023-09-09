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

#include <pcff/socket/Socket.h>
#include <pcff/event/ProxyService.h>

#include "ServicesConfig.h"

using namespace std;
using namespace pcff;

ServicesConfig *ServicesConfig::_instance = NULL;

ServicesConfig &ServicesConfig::instance() {
    if( _instance == NULL ) {
        _instance = new ServicesConfig();
    }
    
    return *_instance;
}

    
ServicesConfig::ServicesConfig() {
    
}

ServicesConfig::~ServicesConfig() {
    
}

void ServicesConfig::initAttributes() {
	
}

void ServicesConfig::init() 
{
    setAttribute("i", 	        Config::INT, 	Config::CMDLINE, "2",              "-i 1  	    	sets the number of interactions");
    setAttribute("e", 	        Config::INT, 	Config::CMDLINE, "2",             "-e 30  	    	sets the number of events per interaction");
    setAttribute("h",  		    Config::BOOL,   Config::CMDLINE, "localhost", 	   "-h 10.1.0.1 	sets the dfs hostname  \n");
    setAttribute("p", 	        Config::INT,    Config::CMDLINE, "5555", 	       "-p 5555        	sets the dfs port\n");
    setAttribute("poolsize",   Config::INT,  Config::BOTH,     "32",             "sets the initial pool size" );
    setAttribute("rawSize",     Config::LONG,    Config::CMDLINE, "0",   	       "-rawSize 1400   sets raw event size\n");
    setAttribute("IPVersion",  Config::STR,  Config::BOTH,     "IPv4",           "sets IP version" \
    																			"IPv4 - IP version 4" \
    																			"IPv6 - IP version 6" );    

}

void ServicesConfig::printVersionHelper() 
{
    cout << "utilTest (GCC) 0.0.1 (Linux)" << endl;
    cout << "Copyright (C) 2004 Free Software Foundation, Inc." << endl;
    cout << "This is free software; see the source for copying conditions.  There is NO" << endl;
    cout << "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE." << endl << endl;
}

void ServicesConfig::printUsageHelper(char *progName) 
{
    cout << "Usage: " << progName << " [OPTIONS]" << endl;
}

int ServicesConfig::getNumIterations() 
{
	ConfigTuple tuple;
	tuple	= m_ConfigOptions["i"];
	return tuple.getValueAsInt();
}

int ServicesConfig::getNumEvents() 
{
	ConfigTuple tuple;
	tuple	= m_ConfigOptions["e"];
	return tuple.getValueAsInt();
}

int ServicesConfig::getDfsPort() 
{
	ConfigTuple tuple;
	tuple	= m_ConfigOptions["p"];
	return tuple.getValueAsInt();
}

string ServicesConfig::getDfsHost()
{
    return m_ConfigOptions["h"].getValue();
}

unsigned long ServicesConfig::getRawTestSize()
{
    return m_ConfigOptions["rawSize"].getValueAsLong();	
}

int  ServicesConfig::getAddrFamily() {
   string  verStr;
	ConfigTuple tuple;
	tuple	= m_ConfigOptions["IPVersion"];
   int retVal = BaseSocket::AF_INET_IPv4;
   
   verStr = tuple.getValue();
   
   if (verStr.compare("IPv4") == 0) 
   {
      retVal = BaseSocket::AF_INET_IPv4;
   }
   // TODO: not supported, until tested
   else if (verStr.compare("IPv6") == 0)
   {
      retVal = BaseSocket::AF_INET_IPv6;
   }
   
	return retVal;	
}

int  ServicesConfig::getPoolSize() {
	ConfigTuple tuple;
	tuple	= m_ConfigOptions["poolsize"];
   
	return tuple.getValueAsInt();	
}
