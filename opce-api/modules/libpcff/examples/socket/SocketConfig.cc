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

#include <pcff/BasicException.h>

#include "SocketConfig.h"

using namespace std;
using namespace pcff;

SocketConfig *SocketConfig::_instance = NULL;

SocketConfig &SocketConfig::instance() {
    if( _instance == NULL ) {
        _instance = new SocketConfig();
        _instance->initAttributes();
    }
    
    return *_instance;
}

    
SocketConfig::SocketConfig() {
    
}

SocketConfig::~SocketConfig() {
    
}

void SocketConfig::initAttributes() {
    setAttribute("testType",    Config::STR, Config::BOTH, "UDPserver", "specifies the test type to be done\n" \
                                                                        "UDPclient    - echo client test using UDP protocol\n" \
                                                                        "UDPserver    - echo server test using UDP protocol\n" \
                                                                        "TCPclient    - echo client test using TCP protocol\n" \
                                                                        "TCPserver    - echo server test using TCP protocol\n" \
                                                                        "UnixDGRAMclient    - echo client test using Unix Domain Datagram protocol\n" \
                                                                        "UnixDGRAMserver    - echo server test using Unix Domain Datagram protocol\n" \
                                                                        "Unixclient    - echo client test using Unix Domain socket\n" \
                                                                        "Unixserver    - echo server test using Unix Domain socket\n");
    setAttribute("host",        Config::STR, Config::BOTH, "localhost", "<Client Only> hostname to connect to if client");
    setAttribute("port",        Config::INT, Config::BOTH, "7766",      "port to connect to, or listening port if server");
    setAttribute("ipVersion",   Config::STR, Config::BOTH, "IPv4",      "IP version: IPv4 or IPv6");
    setAttribute("testStr",     Config::STR, Config::BOTH, "test",      "<Client Only> string to send to server");
    setAttribute("serverPath",  Config::STR, Config::BOTH, "/tmp/sockServerTest",      "unix domain socket server pathname");
    setAttribute("clientPath",  Config::STR, Config::BOTH, "/tmp/sockClientTest",      "unix domain socket client pathname");
}

void SocketConfig::printVersionHelper() {
    cout << "Socket Test (GCC) 0.0.1 (Linux)" << endl;
    cout << "Copyright (C) 2004 Free Software Foundation, Inc." << endl;
    cout << "This is free software; see the source for copying conditions.  There is NO" << endl;
    cout << "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE." << endl << endl;
}

void SocketConfig::printUsageHelper(char *) {
   cout << " Usage: socketTest [-testType <testType>] [--help]" << endl;
}

int SocketConfig::getTestType() {
    string sType = m_ConfigOptions["testType"].getValue();
    if (sType.compare("UDPclient") == 0) {
       _testType = UDPEchoClient;
    }
    else if (sType.compare("UDPserver") == 0) {
       _testType = UDPEchoServer;
    }
    else if (sType.compare("TCPclient") == 0) {
       _testType = TCPEchoClient;
    }
    else if (sType.compare("TCPserver") == 0) {
       _testType = TCPEchoServer;
    }
    else if (sType.compare("Unixclient") == 0) {
       _testType = UnixEchoClient;
    }
    else if (sType.compare("Unixserver") == 0) {
       _testType = UnixEchoServer;
    }
    else if (sType.compare("UnixDGRAMclient") == 0) {
       _testType = UnixDatagramEchoClient;
    }
    else if (sType.compare("UnixDGRAMserver") == 0) {
       _testType = UnixDatagramEchoServer;
    }
    else {
       _testType = UnknownTest;
    }
   
    return _testType;
} 

string SocketConfig::getHost() {
    return m_ConfigOptions["host"].getValue();
}

int SocketConfig::getPort() {
    return m_ConfigOptions["port"].getValueAsInt();
}

int SocketConfig::getIPVersion() {
   string ipVer = m_ConfigOptions["ipVersion"].getValue();

   _ipVersion = UnknownIPVersion;

   if (ipVer.compare("IPv4") == 0) {
      _ipVersion = IPv4;
   }
   else if (ipVer.compare("IPv6") == 0) {
      _ipVersion = IPv6;
   }
   else {
      _ipVersion = UnknownIPVersion;
   }

   return _ipVersion;
}

string SocketConfig::getTestString() {
    return m_ConfigOptions["testStr"].getValue();
}

string SocketConfig::getServerSocketPath() 
{
    return m_ConfigOptions["serverPath"].getValue();
}

string SocketConfig::getClientSocketPath() 
{
    return m_ConfigOptions["clientPath"].getValue();
}
