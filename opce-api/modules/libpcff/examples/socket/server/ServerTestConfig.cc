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

#include "ServerTestConfig.h"

using namespace std;
using namespace pcff;

ServerTestConfig *ServerTestConfig::_instance = NULL;

ServerTestConfig &ServerTestConfig::instance() {
    if( _instance == NULL ) {
        _instance = new ServerTestConfig();
    }
    
    return *_instance;
}

    
ServerTestConfig::ServerTestConfig() {
    
}

ServerTestConfig::~ServerTestConfig() {
    
}

void ServerTestConfig::initAttributes() {
	
}

// TODO: what about no thread option?
// different types of proxies to enable: echo, randomize data, ...
void ServerTestConfig::init() {
    setAttribute("lhost",      Config::STR,  Config::CMDLINE,    "localhost",   "sets forward hostname");
    setAttribute("lport",      Config::INT,  Config::BOTH,    "5555",           "sets the receive listen port for proxy" );
    setAttribute("nodaemon",   Config::BOOL, Config::FILE,    "true",          "used to disable daemoninzation" );
    setAttribute("poolsize",   Config::INT,  Config::BOTH,    "32",             "sets the initial pool size" );
    setAttribute("IPVersion",  Config::STR,  Config::BOTH,    "IPv4",           "sets IP version" );
}

void ServerTestConfig::printVersionHelper() {
    cout << "echod (GCC) 0.0.1 (Linux)" << endl;
    cout << "Copyright (C) 2004 Free Software Foundation, Inc." << endl;
    cout << "This is free software; see the source for copying conditions.  There is NO" << endl;
    cout << "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE." << endl << endl;
}

string ServerTestConfig::getListenHost() {
   return m_ConfigOptions["fhost"].getValue();
}

int  ServerTestConfig::getListenPort() {
	ConfigTuple tuple;
	tuple	= m_ConfigOptions["lport"];
   
	return tuple.getValueAsInt();	
}

bool ServerTestConfig::noDaemon() {
	ConfigTuple tuple;
	tuple	= m_ConfigOptions["nodaemon"];
	return tuple.getValueAsBool();
}

int  ServerTestConfig::getPoolSize() {
	ConfigTuple tuple;
	tuple	= m_ConfigOptions["poolsize"];
   
	return tuple.getValueAsInt();	
}

int  ServerTestConfig::getAddrFamily() {
   string  verStr;
	ConfigTuple tuple;
	tuple	= m_ConfigOptions["IPVersion"];
   int retVal = BaseSocket::AF_INET_IPv4;
   
   verStr = tuple.getValue();
   
   if (verStr.compare("AF_INT_IPv4") == 0) 
   {
      retVal = BaseSocket::AF_INET_IPv4;
   }
   // TODO: not supported message, until tested
   else if (verStr.compare("AF_INET_IPv6") == 0)
   {
      retVal = BaseSocket::AF_INET_IPv6;
   }
   
	return retVal;	
}
