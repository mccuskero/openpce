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

#include <stdio.h>
#include <string.h>
#include <iostream>


#include <unistd.h>

#include <pcff/BasicException.h>
#include <pcff/BasicExceptionMgr.h>

#include <pcff/socket/SocketExceptions.h>
#include <pcff/socket/INETAddress.h>
#include <pcff/socket/SocketAddress.h>
#include <pcff/socket/Socket.h>
#include <pcff/socket/DatagramSocket.h>

#include "SocketConfig.h"
#include "UDPEchoClient.h"

using namespace std;
using namespace pcff;


//
//
UDPEchoClient::UDPEchoClient() 
{
}

//
//
//
UDPEchoClient::~UDPEchoClient() 
{
}

//
//
//
bool UDPEchoClient::run() {
  int    portNum = 0;
  string echoString;
  string hostname;
  int addrFamily = DatagramSocket::AF_INET_IPv4;

  try { 

    cout << "UDPEchoClient test starting..." << endl;

    portNum =    SocketConfig::instance().getPort();
    echoString = SocketConfig::instance().getTestString();
    hostname =   SocketConfig::instance().getHost();
    
    if (SocketConfig::instance().getIPVersion() == SocketConfig::IPv6) 
       addrFamily = Socket::AF_INET_IPv6;

    INETAddress  echoServAddr(addrFamily);

    echoServAddr.setHostname(hostname.c_str());

    DatagramSocket      clientSocket(addrFamily);
    char                pcRecvBuffer[65535];
    int                 iBytesTransferred;

    sprintf(pcRecvBuffer, "");

    printf("Datagram Echo Client\n\n");

    printf("Using Echo Server\n");
    echoServAddr.print();
    printf(" - Port    : %d\n\n", portNum );

    clientSocket.init();

//    clientSocket.connect( echoServAddr, portNum );
//    printf("socket info...");
//    clientSocket.localINETAddr().print();
//    printf("local port    : %d\n\n", clientSocket.localPort());
//    printf("Socket Details - Server Information");
//    clientSocket.remoteINETAddr().print();
//    printf("remote port    : %d\n\n", clientSocket.remotePort());

    printf("Send Buffer    : [%s]\n", echoString.c_str() );
    printf("Receive Buffer : [%s]\n\n", pcRecvBuffer);

    printf("sending request...\n");
    iBytesTransferred = clientSocket.sendDatagram( (void *)echoString.c_str(), 
						                           strlen(echoString.c_str()),
												   echoServAddr, 
												   portNum );
												   
    printf("sent %d bytes\n\n", iBytesTransferred);

    printf("receiving...\n");
    iBytesTransferred = clientSocket.receiveDatagram(pcRecvBuffer, 
													 65535,
													 echoServAddr, 
													 portNum);

    pcRecvBuffer[iBytesTransferred] = 0;

    printf("send Buffer    : [%s]\n", echoString.c_str());
    printf("receive Buffer : [%s]\n", pcRecvBuffer);
  }
  catch( BasicException& ex) {
    BasicExceptionMgr::instance()->handle( ex );
    return false;
  }
  catch(...) {
    BasicExceptionMgr::instance()->handle( "UnitTest::run: unknown ERROR" );
    return false; 
  }


    cout << "UDPEchoClient test complete." << endl;

  return true;
}
